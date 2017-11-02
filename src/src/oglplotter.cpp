/*
    Speccer
    Copyright (C) 2017  VeniVidiCodici @ github.com

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/



#include "oglplotter.h"
#include "../styleframe.h"

#ifdef __WXMAC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#ifndef WIN32
#include <unistd.h>

#endif

#define GL_GLEXT_PROTOTYPES

#ifdef __WXMAC__
#include "OpenGL/glu.h"
#include "OpenGL/gl.h"
#else
#include <GL/glu.h>
#include <GL/gl.h>
#endif

#include <wx/glcanvas.h>
#include <math.h>
#include "uiapi.h"
#include <iostream>

BEGIN_EVENT_TABLE(OGLplotter, wxGLCanvas)
    EVT_PAINT    (OGLplotter::Paintit)
    EVT_KEY_DOWN(OGLplotter::OnKeyDown)
    EVT_MOUSEWHEEL(OGLplotter::mouseWheelMoved)
    EVT_LEFT_DOWN(OGLplotter::mouseDown)
    EVT_RIGHT_DOWN(OGLplotter::mouseDown)
    EVT_LEFT_UP(OGLplotter::mouseReleased)
    EVT_MOTION(OGLplotter::mouseMoved)
END_EVENT_TABLE()

// /// in tree editor there's no segfault at window close with only event paint
// or not, that seems irrelevant


OGLplotter::OGLplotter(wxWindow *parent):wxGLCanvas(parent,wxID_ANY, NULL,
                 wxDefaultPosition, wxDefaultSize,0, "GLCanvas", wxNullPalette)
{
    wMode = editor;
    FTInit = true;
    FTFaceInit = true;
    drawnComposite = nullptr;


    if (FT_Init_FreeType(&FTLib))
        {
            FTInit = false;
            wxMessageDialog info2(parent,"Could not init FreeType.","Error");
            info2.ShowModal();
        }
    if (FT_New_Face(FTLib, "DejaVuSans.ttf", 0, &FTFace))
        {
            FTFaceInit = false;
            wxMessageDialog info2(parent,"Could not load default font.","Error");
            info2.ShowModal();
        }

    //FTGLPixmapFont FTFace("DejaVuSans.ttf"), currentFace("DejaVuSans.ttf"), initFace("DejaVuSans.ttf");
    /*FTFace = FTGLPixmapFont("DejaVuSans.ttf");
    if (FTFace.Error())
        {
            FTFaceInit = false;
            wxMessageDialog info2(parent,"Could not load default font.","Error");
            info2.ShowModal();
        }*/

    x=y=0;
    int a =5;
    drawsPrimaryBranch = false;
    drawsSecondaryBranch = false;
    w=h=0;
    scale=1.0;
    centx=centy=0;
    debug="";
    itIsDragged=false;
    refreshFields=false;
    f=nullptr;

    glgenflag = false;
    glgentexflag = false;
    pvflag = false;
    ivflag = false;
    tvflag = false;

    initARBs();
    wglc=nullptr;
    clientStatesAreSet = false;
}


OGLplotter::~OGLplotter()
{
    if (clientStatesAreSet)
        {
        glDisableClientState(GL_COLOR_ARRAY);
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        }
    if (f!=nullptr)
        delete f;
    if (wglc!=nullptr)
        delete wglc;
    for (std::map<std::string, texture>::iterator it=textures.begin(); it!=textures.end(); ++it)
    {
        SOIL_free_image_data(it->second.i);
        glDeleteTextures(1, &(it->second.t));
    }
    //for (std::map<std::string, FTGLPixmapFont*>::iterator it=ftfaces.begin(); it!=ftfaces.end(); ++it)
    //{
    //    delete it->second;
    //}
}


void OGLplotter::initARBs()
{

    glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC) glXGetProcAddress(
                            reinterpret_cast<const GLubyte*>("glBindVertexArray"));
    glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC) glXGetProcAddress(
                            reinterpret_cast<const GLubyte*>("glGenVertexArrays"));
    glBindVertexArrayAPPLE = (PFNGLBINDVERTEXARRAYAPPLEPROC) glXGetProcAddress(
                            reinterpret_cast<const GLubyte*>("glBindVertexArrayAPPLE"));
    glGenVertexArraysAPPLE = (PFNGLGENVERTEXARRAYSAPPLEPROC) glXGetProcAddress(
                            reinterpret_cast<const GLubyte*>("glGenVertexArraysAPPLE"));
    glGenBuffersARB = (PFNGLGENBUFFERSARBPROC) glXGetProcAddress(
                                reinterpret_cast<const GLubyte*>("glGenBuffersARB"));
    glBindBufferARB = (PFNGLBINDBUFFERARBPROC) glXGetProcAddress(
                                reinterpret_cast<const GLubyte*>("glBindBufferARB"));
    glBufferDataARB = (PFNGLBUFFERDATAARBPROC) glXGetProcAddress(
                                reinterpret_cast<const GLubyte*>("glBufferDataARB"));
    glEnableVertexAttribArrayARB = (PFNGLENABLEVERTEXATTRIBARRAYARBPROC) glXGetProcAddress(
                                reinterpret_cast<const GLubyte*>("glEnableVertexAttribArrayARB"));
    glVertexAttribPointerARB = (PFNGLVERTEXATTRIBPOINTERARBPROC) glXGetProcAddress(
                                reinterpret_cast<const GLubyte*>("glVertexAttribPointerARB"));
    glDrawArraysEXT = (PFNGLDRAWARRAYSEXTPROC) glXGetProcAddress(
                                reinterpret_cast<const GLubyte*>("glDrawArraysEXT"));
    glVertexPointerEXT = (PFNGLVERTEXPOINTEREXTPROC) glXGetProcAddress(
                                reinterpret_cast<const GLubyte*>("glVertexPointerEXT"));
    glEnableClientStateiEXT = (PFNGLENABLECLIENTSTATEIEXTPROC) glXGetProcAddress(
                                reinterpret_cast<const GLubyte*>("glEnableClientStateiEXT"));

}
void OGLplotter::enableClientStates()
{
    if (clientStatesAreSet)
        return;
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    clientStatesAreSet = true;
}
void OGLplotter::redraw()
{
    wxPaintDC(this);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    identity();
    glColorMaterial(GL_FRONT, GL_DIFFUSE);
    glEnable(GL_TEXTURE_2D);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glEnable(GL_COLOR_MATERIAL);
    glDisable(GL_CULL_FACE);
    glClearColor(1.0,1.0,1.0,0.0);
    if (wMode == editor)
    {
        float interval=100.0;
        int kount=10;
        glLineWidth(1.0);
        std::vector<vertex> p;
        colorconv col(200,200,200,255);
        for(int i=-kount;i<=kount;i++)
        {
            p.push_back(vertex(i*interval,-interval*kount,col));
            p.push_back(vertex(i*interval,interval*kount,col));
            p.push_back(vertex(-interval*kount,i*interval,col));
            p.push_back(vertex(interval*kount,i*interval,col));
        }
        col.setColor(128,128,128);
        p.push_back(vertex(0,-interval*kount,col));
        p.push_back(vertex(0,interval*kount,col));
        p.push_back(vertex(-interval*kount,0,col));
        p.push_back(vertex(interval*kount,0,col));
        ogldraw(GL_LINES,p);
    }

    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    if (wMode == editor)
    {
    f->setBranch(nullptr);
    brdata = nullptr;
    if (refreshFields)
        {
            owner->removeAllCustomFields();
            refreshFields=false;
        }
    for(drawable * obj: owner->getAllDrawables())
        {
            glPushMatrix();
            identity();
            Matrix tfm = transformationToMatrix(0.0,0.0,0.0,1.0,1.0,0.0,0.0);
            drawnComposite = nullptr;
            f->setBranch(nullptr);
            drawDrawable(obj, tfm);
            glPopMatrix();
        }
    }
    else if (nodes->getRootBranch()!=nullptr)
    {
        shape * bboxShape = nullptr;
        f->setDataHolder(nodes->getCurrentStyle()->getNodeByName("Leaf"));

        ttx = tty = 0;

        if (nodes->getCurrentStyle()->getNodeByName("Leaf")!=nullptr)
        for(drawable * obj: nodes->getCurrentStyle()->getNodeByName("Leaf")->getAllDrawables())
                {
                    if (obj->isSimplex())
                    {
                        shape * sh = (shape *)obj;
                        if (sh->getType()==shape::bbox)
                        {
                            bboxShape = sh;
                            break;
                        }
                    }
                }
        if (bboxShape!=nullptr)
            nodes->getRootBranch()->calculateMetrics(1,bboxShape, f, this);
        nodes->getRootBranch()->callDraw(this);
    }

    glFlush();
    glDisable(GL_LINE_SMOOTH);
    glDisable(GL_BLEND);
    SwapBuffers();
}
void OGLplotter::drawForBranch(branch * br)
{
    calcCoordsForBranch(br);
    f->setBranch(br);
    if (nodes->getCurrentStyle()->getNodeByName(br->getNode())==nullptr)
        return;
    drawable * brp = nodes->getCurrentStyle()->getNodeByName(br->getNode())->getShapeByType(shape::anchor);
    if (brp!=nullptr)
    {
        glPushMatrix();
        identity();
        Matrix tfm = transformationToMatrix(br->getrootx(),br->getrooty(),0.0,1.0,1.0,0.0,0.0);
        drawnComposite = nullptr;
        f->setDrawnComposite(nullptr);
        drawDrawable(brp, tfm);
        glPopMatrix();
    }

    for(drawable * obj: nodes->getCurrentStyle()->getNodeByName(br->getNode())->getAllDrawables())
    {
        if (obj->isSimplex())
        {
            shape * sh = (shape *)obj;
            brdata = br;
            shape::shapeType st = sh->getType();
        }
        glPushMatrix();
        identity();
        Matrix tfm = transformationToMatrix(br->getrootx(),br->getrooty(),0.0,1.0,1.0,0.0,0.0);
        drawnComposite = nullptr;
        f->setDrawnComposite(nullptr);
        drawDrawable(obj, tfm);
        glPopMatrix();
    }

    brp = nodes->getCurrentStyle()->getNodeByName(br->getNode())->getShapeByType(shape::bbox);
    if (brp!=nullptr)
    if (owner->getUIAPI()->getPrimaryBranch()==br)
    {
        drawsPrimaryBranch = true;
        glPushMatrix();
        identity();
        Matrix tfm = transformationToMatrix(br->getrootx(),br->getrooty(),0.0,1.0,1.0,0.0,0.0);
        drawnComposite = nullptr;
        f->setDrawnComposite(nullptr);
        drawDrawable(brp, tfm);
        glPopMatrix();
    }

    if (brp!=nullptr)
    if (owner->getUIAPI()->getSecondaryBranch()==br)
    {
        drawsSecondaryBranch = true;
        glPushMatrix();
        identity();
        Matrix tfm = transformationToMatrix(br->getrootx(),br->getrooty(),0.0,1.0,1.0,0.0,0.0);
        drawnComposite = nullptr;
        f->setDrawnComposite(nullptr);
        drawDrawable(brp, tfm);
        glPopMatrix();
    }

    drawsPrimaryBranch = false;
    drawsSecondaryBranch = false;

    brp = nodes->getCurrentStyle()->getNodeByName(br->getNode())->getShapeByType(shape::brpoint);
    if (brp!=nullptr)
    {
        glPushMatrix();
        identity();
        Matrix tfm = transformationToMatrix(br->getrootx(),br->getrooty(),0.0,1.0,1.0,0.0,0.0);
        drawnComposite = nullptr;
        f->setDrawnComposite(nullptr);
        drawDrawable(brp, tfm);
        glPopMatrix();
    }
}

void OGLplotter::calcCoordsForBranch(branch * br)
{
    f->setBranch(br);
    if (nodes->getCurrentStyle()->getNodeByName(br->getNode())==nullptr)
        return;
    drawable * brp = nodes->getCurrentStyle()->getNodeByName(br->getNode())->getShapeByType(shape::anchor);
    if (brp!=nullptr)
    {
        //glPushMatrix();
        //identity();
        Matrix tfm;// = transformationToMatrix(br->getrootx(),br->getrooty(),0.0,1.0,1.0,0.0,0.0);
        drawnComposite = nullptr;
        f->setDrawnComposite(nullptr);
        setDrawableCoordinates(brp, tfm);
        //glPopMatrix();
    }

    for(drawable * obj: nodes->getCurrentStyle()->getNodeByName(br->getNode())->getAllDrawables())
    {
        if (obj->isSimplex())
        {
            shape * sh = (shape *)obj;
            brdata = br;
            shape::shapeType st = sh->getType();
        }
        //glPushMatrix();
        //identity();
        Matrix tfm;// = transformationToMatrix(br->getrootx(),br->getrooty(),0.0,1.0,1.0,0.0,0.0);
        drawnComposite = nullptr;
        f->setDrawnComposite(nullptr);
        setDrawableCoordinates(obj, tfm);
        //glPopMatrix();
    }
}

void OGLplotter::refresh()
{
    redraw();
}

void OGLplotter::Paintit(wxPaintEvent& WXUNUSED(event))
{
    wxPaintDC dc(this);
    const wxSize ClientSize = GetClientSize();
    if (wglc==nullptr)
    {
        wglc = new wxGLContext(this);
        wglc->SetCurrent(*this);
    }
    run = (glGetString(GL_VERSION)!=NULL);
    glViewport(0, 0, ClientSize.x, ClientSize.y);
    glClearColor(1.0, 1.0, 1.0, 1.0);

    if (run)
    {
        try
        {
            redraw();
        }
        catch(std::exception& e)
        {
            std::cout << "OGLplotter::Paintit exception \n";
            std::cout << "   of type: " << e.what() << '\n';
            owner->getUIAPI()->setStatusBarString(std::string("OGLplotter::Paintit exception of type: ")+
                                                  e.what(),3);
        }
        catch(int e)
        {
            std::cout << "OGLplotter::Paintit exception: " << e << "\n";
            owner->getUIAPI()->setStatusBarString("OGLplotter::Paintit exception: "+std::to_string(e),3);
        }
        catch(...)
        {
            std::cout << "OGLplotter::Paintit exception neither int nor standard\n";
            owner->getUIAPI()->setStatusBarString("OGLplotter::Paintit exception neither int nor standard",3);
        }
    }
    else
    {
        owner->getUIAPI()->setStatusBarString("openGL loading error !",3);
    }
}

void OGLplotter::createFormulaParser(bool actualGlobalVariables)
{
    f = new formulaParser(actualGlobalVariables, owner);
}
void OGLplotter::updateFormulaParserDataHolder()
{
    //if (f!=nullptr)///temporary solution!
        f->setDataHolder(owner);
}
void OGLplotter::updatesize()
{

}
void OGLplotter::checksize()
{

}
void OGLplotter::identity()
{
    glLoadIdentity();
    setView();
}
void OGLplotter::setView()
{
    GLint w, h;

    w = (GLint)GetSize().x;
    h = (GLint)GetSize().y;

    glOrtho( centx-scale*w/2, centx+scale*w/2,
             centy+scale*h/2, centy-scale*h/2, 0.f, 1.f );
}
dataPoint * OGLplotter::getSurfaceDataPoint(dataPoint * ofThisOne)
{
    if (ofThisOne==nullptr)
        return nullptr;
    if (ofThisOne->getSurfaceName()=="")
        return ofThisOne;
    if (drawnComposite!=nullptr)
    {
        dataPoint * res = drawnComposite->getPropertyByNameAndType(
                                        ofThisOne->getSurfaceName(),
                                        ofThisOne->getType());
        if (res==nullptr)
            return ofThisOne;
        else
            return res;
    }
    else if (f!=nullptr)
    {
        if (f->getBranch()!=nullptr)
        {
            dataPoint * res = f->getBranch()->getPropertyDontAdd(
                                        ofThisOne->getSurfaceName(),
                                        ofThisOne->getType());
            if (res==nullptr)
                return ofThisOne;
            else
                return res;
        }
        else return ofThisOne;
    }
    else return ofThisOne;
}

void OGLplotter::setDrawableCoordinates(drawable * drawn,Matrix Transform, bool globvar)
{
    if (!drawn->isSimplex())
    {
        //applyTransformationHierarchy((matrixData *)((composite *)drawn)->getPropertyByName("tfmatrix"));
        for(drawable * obj:((composite *)drawn)->getShapes())
        {
            f->setDrawnComposite((composite *)drawn);
            drawnComposite = (composite *)drawn;
            //applyTransformationHierarchyToMatrix(Transform,
              //              (matrixData *)((composite *)drawn)->getPropertyByName("tfmatrix"));
            setDrawableCoordinates(obj,Transform,false);
        }
    }
    else
    {
        //identity();
        //oglMtransform(Transform);
        shape * drawnShape = (shape*)drawn;
        formulaData * xpos = (formulaData *) getSurfaceDataPoint(drawnShape->getPropertyByName("xpos"));
        formulaData * ypos = (formulaData *) getSurfaceDataPoint(drawnShape->getPropertyByName("ypos"));
        numberData * outlinethickness = (numberData *)getSurfaceDataPoint(drawnShape->getPropertyByName("outlinethickness"));
        formulaData * xorigin = (formulaData *)getSurfaceDataPoint(drawnShape->getPropertyByName("xorigin"));
        formulaData * yorigin = (formulaData *)getSurfaceDataPoint(drawnShape->getPropertyByName("yorigin"));
        formulaData * rotation = (formulaData *)getSurfaceDataPoint(drawnShape->getPropertyByName("rotation"));
        formulaData * xscale = (formulaData *)getSurfaceDataPoint(drawnShape->getPropertyByName("xscale"));
        formulaData * yscale = (formulaData *)getSurfaceDataPoint(drawnShape->getPropertyByName("yscale"));

        double x=0.0,y=0.0,fr=0.0,fg=0.0,fb=0.0,fa=0.0,ro=0.0,og=0.0,ob=0.0,oa=0.0,ot=0.0,
                xo=0.0,yo=0.0,xs=1.0,ys=1.0,r=0.0;
        colorconv olc(0,0,0,0),fc(0,0,0,255);
        if(xpos!=nullptr&&ypos!=nullptr)
            {
                x=f->parse(xpos,globvar);
                y=f->parse(ypos,globvar);
            }
        if(outlinethickness!=nullptr)
            ot=outlinethickness->getData();
        if(xorigin!=nullptr&&yorigin!=nullptr)
            {
            xo=f->parse(xorigin,globvar);
            yo=f->parse(yorigin,globvar);
            }
        if(rotation!=nullptr)
            r=f->parse(rotation,globvar);
        if(xscale!=nullptr&&yscale!=nullptr)
            {
            xs=f->parse(xscale,globvar);
            ys=f->parse(yscale,globvar);
            }


        //wxPaintDC(this);
        //glEnable(GL_LINE_SMOOTH);

        switch(drawnShape->getType())
            {
            case shape::root:
                {
                drawnShape->setx(0.0);
                drawnShape->sety(0.0);
                drawnShape->setW(0.0);
                drawnShape->seth(0.0);
                }
            break;
            case shape::brpoint:
                {
                if(xpos!=nullptr&&ypos!=nullptr)
                    {
                    drawnShape->setx(x);
                    drawnShape->sety(y);
                    drawnShape->setW(0.0);
                    drawnShape->seth(0.0);
                    }
                if (brdata!=nullptr&& false)
                    {
                    vertex v = {brdata->getrootx()+x, brdata->getrooty()+y, 0.0, 0.0, 0.0, 0.0};
                    v = applyTransformationHierarchyToVertex(v,(matrixData *)
                                                             drawnShape->getPropertyByName("tfmatrix"));
                    brdata->setChildrenRootXY((int)v.x,(int)v.y);
                    break;
                    }
                }
            break;
            case shape::anchor:
                {
                if(xpos!=nullptr&&ypos!=nullptr)
                {
                    drawnShape->setx(x);
                    drawnShape->sety(y);
                    drawnShape->setW(0.0);
                    drawnShape->seth(0.0);
                }
                }
            break;
            case shape::bbox:
                {
                formulaData * width = (formulaData *)getSurfaceDataPoint(drawnShape->getPropertyByName("width"));
                formulaData * height = (formulaData *)getSurfaceDataPoint(drawnShape->getPropertyByName("height"));
                if(xpos==nullptr||ypos==nullptr)
                    break;
                if (width==nullptr||height==nullptr)
                    break;
                //ogltransform(x,y,r,xs,ys,xo,yo);
                //applyTransformationHierarchy((matrixData *)drawnShape->getPropertyByName("tfmatrix"));
                double w=0.0, h=0.0;
                w=f->parse(width,globvar);
                h=f->parse(height,globvar);

                if (brdata!=nullptr)  /// transform the coordinates!
                {
                    brdata->setx((int)x);
                    brdata->sety((int)y);
                    brdata->setW((int)w);
                    brdata->seth((int)h);
                }
                drawnShape->setx(x);
                drawnShape->sety(y);
                drawnShape->setW(w);
                drawnShape->seth(h);
                }
            break;
            case shape::circle:
                {
                int m; double c;
                numberData * sides = (numberData *)getSurfaceDataPoint(drawnShape->getPropertyByName("sides"));
                formulaData * radius = (formulaData *)getSurfaceDataPoint(drawnShape->getPropertyByName("radius"));
                if (sides==nullptr||radius==nullptr)
                    break;
                double rad = f->parse(radius,globvar);
                drawnShape->setx(x-r/2);
                drawnShape->sety(y-r/2);
                drawnShape->setW(r);
                drawnShape->seth(r);
                }
            break;
            case shape::arc:
                {

                int m; double c;
                numberData * sides = (numberData *)getSurfaceDataPoint(drawnShape->getPropertyByName("sides"));
                formulaData * radius = (formulaData *)getSurfaceDataPoint(drawnShape->getPropertyByName("radius"));
                formulaData * angle = (formulaData *)getSurfaceDataPoint(drawnShape->getPropertyByName("angle"));
                boolData * iol = (boolData *)drawnShape->getPropertyByName("iol");
                boolData * ool = (boolData *)drawnShape->getPropertyByName("ool");
                if (sides==nullptr||radius==nullptr||angle==nullptr||iol==nullptr||ool==nullptr)
                    break;
                double rad = f->parse(radius,globvar);
                double angl = f->parse(angle,globvar);
                bool hasiol = iol->getData();
                bool hasool = ool->getData();

                m = sides->getData();
                if (m<2)
                    break;
                //ogltransform(x,y,r,xs,ys,xo,yo);
                //applyTransformationHierarchy((matrixData *)drawnShape->getPropertyByName("tfmatrix"));
                c = m / (2*3.1415926535*angl/360.0);
                std::vector<vertex> p;
                double minx=0.0, maxx=0.0, miny=0.0, maxy=0.0;
                double ix, iy;
                p.push_back(vertex(0,0,fr,fg,fb,fa));
                double maxa = 0.0;
                for(int i=0;i<=m;i++)
                {
                    maxa = i/c;
                    //if (i/c>3.1415926535*angl/180)
                      //  break;
                    ix = rad*cos(i/c);
                    iy = rad*sin(i/c);
                    p.push_back(vertex(ix,iy,fr,fg,fb,fa));
                    if (i==0 || ix < minx)
                        minx = ix;
                    if (i==0 || ix > maxx)
                        maxx = ix;
                    if (i==0 || iy < miny)
                        miny = iy;
                    if (i==0 || iy > maxy)
                        maxy = iy;
                }
                drawnShape->setx(x+minx);
                drawnShape->sety(y+miny);
                drawnShape->setW(maxx-minx);
                drawnShape->seth(maxy-miny);
                }
            break;
            case shape::rectangle:
                {
                formulaData * width = (formulaData *)getSurfaceDataPoint(drawnShape->getPropertyByName("width"));
                formulaData * height = (formulaData *)getSurfaceDataPoint(drawnShape->getPropertyByName("height"));
                if (width==nullptr||height==nullptr)
                    break;
                double w,h;
                w=f->parse(width,globvar);
                h=f->parse(height,globvar);
                drawnShape->setx(x);
                drawnShape->sety(y);
                drawnShape->setW(w);
                drawnShape->seth(h);
                }
            break;
            case shape::roundrect:
                {
                formulaData * width = (formulaData *)getSurfaceDataPoint(drawnShape->getPropertyByName("width"));
                formulaData * height = (formulaData *)getSurfaceDataPoint(drawnShape->getPropertyByName("height"));
                formulaData * radius = (formulaData *)getSurfaceDataPoint(drawnShape->getPropertyByName("cradius"));
                numberData * sides = (numberData *)getSurfaceDataPoint(drawnShape->getPropertyByName("sides"));
                if (width==nullptr||height==nullptr)
                    break;
                if (sides==nullptr||radius==nullptr)
                    break;
                double w,h;
                w=f->parse(width,globvar);
                h=f->parse(height,globvar);
                double rad = f->parse(radius,globvar);
                if (w<rad*2+1)
                    w = rad*2+1;
                if (h<rad*2+1)
                    h = rad*2+1;
                drawnShape->setx(x);
                drawnShape->sety(y);
                drawnShape->setW(w);
                drawnShape->seth(h);
                }
            break;
            case shape::convex:
                {
                collectionData * points = (collectionData *)drawnShape->getPropertyByName("points");
                if (points==nullptr)
                    break;
                //ogltransform(x,y,r,xs,ys,xo,yo);
                //applyTransformationHierarchy((matrixData *)drawnShape->getPropertyByName("tfmatrix"));
                std::vector<vertex> p,op;
                std::vector<vertInfo> vertices=points->getData();
                double minx=0.0, maxx=0.0, miny=0.0, maxy=0.0;
                double ix, iy;
                for(int i=0;i<vertices.size();++i)
                {
                    ix = f->parsestring(vertices[i].x,globvar);
                    iy = f->parsestring(vertices[i].y,globvar);
                    p.push_back(vertex(ix,iy,fr, fg, fb, fa));
                    op.push_back(vertex(ix,iy,ro, og, ob, oa));
                    if (i==0 || ix < minx)
                        minx = ix;
                    if (i==0 || ix > maxx)
                        maxx = ix;
                    if (i==0 || iy < miny)
                        miny = iy;
                    if (i==0 || iy > maxy)
                        maxy = iy;
                }

                drawnShape->setx(x+minx);
                drawnShape->sety(y+miny);
                drawnShape->setW(maxx-minx);
                drawnShape->seth(maxy-miny);
                }
            break;
            case shape::text:
                {
                fontData * fd = (fontData *)drawnShape->getPropertyByName("font");
                if (fd==nullptr)
                    break;

                //break;/*
                std::string fontpath = fd->getData();
                std::string dbg = fontpath;
                if (ftfaces.find(fontpath)==ftfaces.end())
                {
                    dbg+="1";
                    ftfacesinit[fontpath] = true;
                    if (FT_New_Face(FTLib, fontpath.c_str(), 0, &initFace))
                        {ftfacesinit[fontpath] = false;dbg+=" x.x ";}
                    ftfaces[fontpath] = initFace;
                }
                if (ftfacesinit[fontpath])
                    currentFace = ftfaces[fontpath];
                else
                {
                    dbg+=" x_x";
                    if (FTFaceInit)
                        currentFace = FTFace;
                    else
                        break;
                }
                dbg+=";";

                numberData * syze = (numberData *)getSurfaceDataPoint(drawnShape->getPropertyByName("size"));
                if (syze == nullptr)
                    FT_Set_Pixel_Sizes(currentFace, 0, 12);
                else
                    FT_Set_Pixel_Sizes(currentFace, 0, syze->getData());
                textData * tex = (textData *)getSurfaceDataPoint(drawnShape->getPropertyByName("text"));
                std::string label;
                if(tex!=nullptr)
                    label = tex->getData();
                else
                    label = "(text)";
                //ogltransform(x,y,r,xs,ys,xo,yo);
                //applyTransformationHierarchy((matrixData *)drawnShape->getPropertyByName("tfmatrix"));
                if (brdata!=nullptr&&tex!=nullptr)
                {
                    /*std::string txt = tex->getData();
                    std::string toBeSet = f->getAPropertyFromText(txt);
                    if (toBeSet!="")
                    {
                        dataPoint * tds = brdata->getProperty(toBeSet,dataPoint::text);
                        if (tds!=nullptr)
                        if (tds->getType()==dataPoint::text)
                        {
                            std::string newstr = ((textData *) tds)->getData();
                            label = f->applyPropertyToText(label, toBeSet, newstr);
                        }
                    }*/
                    std::string txt = tex->getData();
                    std::vector<std::string> toBeSet = f->getTagsFromText(txt);
                    if (toBeSet.size()>0)
                    {
                        std::vector<std::string> reps;
                        for (int rr=0;rr<toBeSet.size();rr++)
                        {
                            dataPoint * tds = brdata->getProperty(toBeSet[rr],dataPoint::text);
                            if (tds!=nullptr)
                            if (tds->getType()==dataPoint::text)
                            {
                                reps.push_back(((textData *) tds)->getData());
                            }
                        }
                        label = f->applyTagsToText(label, reps);
                    }
                }
                textw = texth = 0;
                oglTextSetWH(label,fc);
                drawnShape->setx(x);
                drawnShape->sety(y);
                drawnShape->setW(textw);
                drawnShape->seth(texth);
                }
            break;
            case shape::image:
                {
                imageData * id = (imageData *)getSurfaceDataPoint(drawnShape->getPropertyByName("image"));
                if (id==nullptr)
                    break;
                drawnShape->setx(x);
                drawnShape->sety(y);
                drawnShape->setW(0);
                drawnShape->seth(0);
                if (id->getData().size()<5)
                    break;
                std::string imagepath = id->getData();

                //glEnable(GL_TEXTURE_2D);

                if (textures.find(imagepath)==textures.end())
                    {
                        glEnable(GL_TEXTURE_2D);
                        glGenTextures(1, &(textures[imagepath].t));

                        glActiveTexture(GL_TEXTURE0);
                        glBindTexture(GL_TEXTURE_2D, textures[imagepath].t);

                        textures[imagepath].i = SOIL_load_image(imagepath.c_str(),
                                                                &(textures[imagepath].w),
                                                                &(textures[imagepath].h),
                                                                0, SOIL_LOAD_RGBA);
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                                     textures[imagepath].w,
                                     textures[imagepath].h, 0, GL_RGBA,
                                     GL_UNSIGNED_BYTE, textures[imagepath].i);

                        //SOIL_free_image_data(textures[imagepath].i);
                        glDisable(GL_TEXTURE_2D);
                    }
                if (textures.find(imagepath)!=textures.end())
                    if(textures[imagepath].t>0)
                    {


                        /*glEnable(GL_TEXTURE_2D);
                        glActiveTexture(GL_TEXTURE0);
                        glBindTexture(GL_TEXTURE_2D, textures[imagepath].t);
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                                     textures[imagepath].w,
                                     textures[imagepath].h, 0, GL_RGBA,
                                     GL_UNSIGNED_BYTE, textures[imagepath].i);
                        glDisable(GL_TEXTURE_2D);
                        ogltransform(x,y,r,xs,ys,xo,yo);
                        applyTransformationHierarchy((matrixData *)drawnShape->getPropertyByName("tfmatrix"));*/

                        drawnShape->setx(x);
                        drawnShape->sety(y);
                        drawnShape->setW(textures[imagepath].w);
                        drawnShape->seth(textures[imagepath].h);
                    }
                else
                    {
             //       owner->setdbgstr("Error loading image: " + std::string(SOIL_last_result(),0,3));
                    }
                }
            break;
            case shape::points:
                {
                choiceData * cd = (choiceData *)drawnShape->getPropertyByName("type");
                if (cd==nullptr)
                    break;
                collectionData * points = (collectionData *)drawnShape->getPropertyByName("points");
                if (points==nullptr)
                    break;
                //ogltransform(x,y,r,xs,ys,xo,yo);
                //applyTransformationHierarchy((matrixData *)drawnShape->getPropertyByName("tfmatrix"));
                std::vector<vertex> p;
                std::vector<vertInfo> vertices=points->getData();
                double minx=0.0, maxx=0.0, miny=0.0, maxy=0.0;
                double ix, iy;
                for(int i=0;i<vertices.size();++i)
                {
                    //globvar = false;
                    ix = f->parsestring(vertices[i].x,globvar);
                    iy = f->parsestring(vertices[i].y,globvar);
                    p.push_back(vertex(ix,iy,
                                       vertices[i].col.oglr(),
                                       vertices[i].col.oglg(),
                                       vertices[i].col.oglb(),
                                       vertices[i].col.ogla()));
                    if (i==0 || ix < minx)
                        minx = ix;
                    if (i==0 || ix > maxx)
                        maxx = ix;
                    if (i==0 || iy < miny)
                        miny = iy;
                    if (i==0 || iy > maxy)
                        maxy = iy;
                    //hair_line(ix-3,iy,ix+3,iy,true);
                    //hair_line(ix,iy-3,ix,iy+3,true);

                    //hair_line((i+1)*10,-5,(i+1)*10,3,true);
                }
                drawnShape->setx(x+minx);
                drawnShape->sety(y+miny);
                drawnShape->setW(maxx-minx);
                drawnShape->seth(maxy-miny);
                }
            break;
            case shape::matrix:
                {
                drawnShape->setx(0.0);
                drawnShape->sety(0.0);
                drawnShape->setW(0.0);
                drawnShape->seth(0.0);
                }
            break;
            }

    }
}



void OGLplotter::drawDrawable(drawable * drawn,Matrix Transform, bool globvar)
{
    if (!drawn->isSimplex())
    {
        for(drawable * obj:((composite *)drawn)->getShapes())
        {
            f->setDrawnComposite((composite *)drawn);
            drawnComposite = (composite *)drawn;
            applyTransformationHierarchyToMatrix(Transform,
                            (matrixData *)((composite *)drawn)->getPropertyByName("tfmatrix"));
            drawDrawable(obj,Transform,false);
        }
    }
    else
    {
        shape * drawnShape = (shape*)drawn;
        vertex testV1(drawnShape->getx(),drawnShape->gety(),0,0,0,0),
               testV2(drawnShape->getx()+drawnShape->getw(),drawnShape->gety()+drawnShape->geth(),0,0,0,0);


        identity();
        //ogltransform(t_x,t_y,t_r,t_xs,t_ys,t_ox,t_oy);
        //oglMtransform(Transform);
        //testV1 = vtxMtransform(Transform,testV1);
        //testV2 = vtxMtransform(Transform,testV2);



        std::vector<dataPoint *> props=drawnShape->getProperties();
        formulaData * xpos = (formulaData *) getSurfaceDataPoint(drawnShape->getPropertyByName("xpos"));
        formulaData * ypos = (formulaData *) getSurfaceDataPoint(drawnShape->getPropertyByName("ypos"));
        colorData * fillcolor = (colorData *)getSurfaceDataPoint(drawnShape->getPropertyByName("fillcolor"));
        colorData * outlinecolor = (colorData *)getSurfaceDataPoint(drawnShape->getPropertyByName("outlinecolor"));
        numberData * outlinethickness = (numberData *)getSurfaceDataPoint(drawnShape->getPropertyByName("outlinethickness"));
        formulaData * xorigin = (formulaData *)getSurfaceDataPoint(drawnShape->getPropertyByName("xorigin"));
        formulaData * yorigin = (formulaData *)getSurfaceDataPoint(drawnShape->getPropertyByName("yorigin"));
        formulaData * rotation = (formulaData *)getSurfaceDataPoint(drawnShape->getPropertyByName("rotation"));
        formulaData * xscale = (formulaData *)getSurfaceDataPoint(drawnShape->getPropertyByName("xscale"));
        formulaData * yscale = (formulaData *)getSurfaceDataPoint(drawnShape->getPropertyByName("yscale"));

        double x=0.0,y=0.0,fr=0.0,fg=0.0,fb=0.0,fa=0.0,ro=0.0,og=0.0,ob=0.0,oa=0.0,ot=0.0,
                xo=0.0,yo=0.0,xs=1.0,ys=1.0,r=0.0;
        colorconv olc(0,0,0,0),fc(0,0,0,255);
        if(xpos!=nullptr&&ypos!=nullptr)
            {
                x=f->parse(xpos,globvar);
                y=f->parse(ypos,globvar);
            }
        if(fillcolor!=nullptr)
            {
                if (drawnComposite!=nullptr)
                fc=fillcolor->getData();
                fr=fillcolor->getData().oglr();
                fg=fillcolor->getData().oglg();
                fb=fillcolor->getData().oglb();
                fa=fillcolor->getData().ogla();
            }
        if(outlinecolor!=nullptr)
            {
                olc = outlinecolor->getData();
                ro=outlinecolor->getData().oglr();
                og=outlinecolor->getData().oglg();
                ob=outlinecolor->getData().oglb();
                oa=outlinecolor->getData().ogla();
            }
        if(outlinethickness!=nullptr)
            ot=outlinethickness->getData();
        if(xorigin!=nullptr&&yorigin!=nullptr)
            {
            xo=f->parse(xorigin,globvar);
            yo=f->parse(yorigin,globvar);
            }
        if(rotation!=nullptr)
            r=f->parse(rotation,globvar);
        if(xscale!=nullptr&&yscale!=nullptr)
            {
            xs=f->parse(xscale,globvar);
            ys=f->parse(yscale,globvar);
            }


        wxPaintDC(this);
 /*       glLoadIdentity();
        GLint wi, he;
        wi = (GLint)GetSize().x;
        he = (GLint)GetSize().y;
        glOrtho( -wi/2, wi/2, he/2, -he/2, 0.0, 1.0 ); /// needed to make one OGL unit match one pixel
*/
        glEnable(GL_LINE_SMOOTH);/*

        testV1 = vtxMtransform(Transform,testV1);
        testV2 = vtxMtransform(Transform,testV2);
        //testV1 = vtxtransform(testV1,x,y,r,xs,ys,xo,yo);
        //testV2 = vtxtransform(testV2,x,y,r,xs,ys,xo,yo);
        //testV1 = vtxtransform(testV1,0,0,r,xs,ys,xo,yo);
        //testV2 = vtxtransform(testV2,0,0,r,xs,ys,xo,yo);

        testV1 = applyTransformationHierarchyToVertexNoFirst(testV1,(matrixData *)drawnShape->getPropertyByName("tfmatrix"));
        testV2 = applyTransformationHierarchyToVertexNoFirst(testV2,(matrixData *)drawnShape->getPropertyByName("tfmatrix"));

        double testXmin, testXmax, testYmin, testYmax;

        testXmin = std::min(testV1.x, testV2.x);
        testXmax = std::max(testV1.x, testV2.x);

        testYmin = std::min(testV1.y, testV2.y);
        testYmax = std::max(testV1.y, testV2.y);

        if (false)
            {
            std::vector<vertex> _p;
            _p.push_back(vertex(testXmin,testYmin,0, 1, 0, 1));
            _p.push_back(vertex(testXmax,testYmin,0, 1, 0, 1));
            _p.push_back(vertex(testXmax,testYmax,0, 1, 0, 1));
            _p.push_back(vertex(testXmin,testYmax,0, 1, 0, 1));
            oglLine(_p, 5.0, true);
            }*/
        oglMtransform(Transform);
        ogltransform(x,y,r,xs,ys,xo,yo);
        applyTransformationHierarchy((matrixData *)drawnShape->getPropertyByName("tfmatrix"));


        //bool OGLplotter::isItOnScreen(double x, double y, double w, double h)
        //if (!isItOnScreenMinMax(testXmin, testYmin, testXmax, testYmax)&&drawnShape->getType()!=shape::brpoint)
          //  return ; /// don't draw things outside the screen, save time

        switch(drawnShape->getType())
            {

            case shape::root:
                {
                drawnShape->setx(0.0);
                drawnShape->sety(0.0);
                drawnShape->setW(0.0);
                drawnShape->seth(0.0);
                //applyTransformationHierarchy((matrixData *)drawnShape->getPropertyByName("tfmatrix"));
                if (brdata!=nullptr)
                    break;
                line_raw(-100,0,100,0);
                line_raw(0,-100,0,100);
                }
            break;

            case shape::brpoint:
                {
                int m; double c;
                if(xpos!=nullptr&&ypos!=nullptr)
                    {
                    drawnShape->setx(x);
                    drawnShape->sety(y);
                    drawnShape->setW(0.0);
                    drawnShape->seth(0.0);
                    m = 20;
                    c = m / (2*3.1415926535);
                    //applyTransformationHierarchy((matrixData *)drawnShape->getPropertyByName("tfmatrix"));

                    double lx, ly, lxp, lyp;
                    lxp = 30*cos(0);
                    lyp = 30*sin(0);
                    if (brdata!=nullptr)
                        {
                        vertex v = {brdata->getrootx()+x, brdata->getrooty()+y, 0.0, 0.0, 0.0, 0.0};
                        v = applyTransformationHierarchyToVertex(v,(matrixData *)
                                                                 drawnShape->getPropertyByName("tfmatrix"));
                        brdata->setChildrenRootXY((int)v.x,(int)v.y);
                        break;
                        }
                    for(int i=1;i<=m;i++)
                        {
                            lx = 30*cos(i/c);
                            ly = 30*sin(i/c);
                            line_raw(lxp, lyp, lx, ly);
                            lxp = lx;
                            lyp = ly;
                        }
                    line_raw(0,0,0+50,0);
                    line_raw(0,0,0+47,0-20);
                    line_raw(0,0,0+47,0+20);
                    }
                }
            break;

            case shape::anchor:
                {
                int m; double c;
                if(xpos!=nullptr&&ypos!=nullptr)
                {
                    drawnShape->setx(x);
                    drawnShape->sety(y);
                    drawnShape->setW(0.0);
                    drawnShape->seth(0.0);
                    //applyTransformationHierarchy((matrixData *)drawnShape->getPropertyByName("tfmatrix"));
                    m = 20;
                    c = m / (2*3.1415926535);
                    double lx, ly, lxp, lyp;
                    lxp = 0+30*cos(0);
                    lyp = 0+30*sin(0);


                    if (brdata!=nullptr)
                        break;
                    for(int i=1;i<=m;i++)
                        {
                            lx = 0+30*cos(i/c);
                            ly = 0+30*sin(i/c);
                            line_raw(lxp, lyp, lx, ly);
                            lxp = lx;
                            lyp = ly;
                        }
                    int om = 40;
                    line_raw(0,0-40+om,0,0-80+om);
                    line_raw(0,0-40+om,0-10,0-60+om);
                    line_raw(0,0-40+om,0+10,0-60+om);
                    line_raw(0-10,0-70+om,0+10,0-70+om);
                }
                }
            break;

            case shape::bbox:
                {
                formulaData * width = (formulaData *)getSurfaceDataPoint(drawnShape->getPropertyByName("width"));
                formulaData * height = (formulaData *)getSurfaceDataPoint(drawnShape->getPropertyByName("height"));
                if(xpos==nullptr||ypos==nullptr)
                    break;
                if (width==nullptr||height==nullptr)
                    break;
                double w=0.0, h=0.0;
                w=f->parse(width,globvar);
                h=f->parse(height,globvar);

                if (brdata!=nullptr)  /// transform the coordinates!
                {
                    brdata->setx((int)x);
                    brdata->sety((int)y);
                    brdata->setW((int)w);
                    brdata->seth((int)h);
                }
                drawnShape->setx(x);
                drawnShape->sety(y);
                drawnShape->setW(w);
                drawnShape->seth(h);
                if (brdata!=nullptr&&drawsPrimaryBranch == false && drawsSecondaryBranch == false)
                        break;
                std::vector<vertex> p;fr=fa=1.0;fg=fb=0.0;

                if (drawsSecondaryBranch)
                {
                    fg = 1.0;
                    fr = 0.0;
                }
                double _dx=0.0, _dy=0.0;
                if (drawsPrimaryBranch || drawsSecondaryBranch)
                {
                    w+=10;
                    h+=10;
                    _dx=-5.0;
                    _dy=-5.0;
                }
                p.push_back(vertex(_dx,_dy,fr,fg,fb,fa));
                p.push_back(vertex(_dx+w,_dy,fr,fg,fb,fa));
                p.push_back(vertex(_dx+w,_dy+h,fr,fg,fb,fa));
                p.push_back(vertex(_dx,_dy+h,fr,fg,fb,fa));

                /*
                int ww, hh;
                this->GetClientSize(&ww, &hh);

                if (x> centx + (ww/2)*scale) return false;
                if (y> centy + (hh/2)*scale) return false;
                if (x+w< centx - (ww/2)*scale) return false;
                if (y+h< centy - (hh/2)*scale) return false;
                if (w < 4 && h < 4) return false;
                */
                /*if (drawsPrimaryBranch)
                {
                    identity();
                    p.erase(p.begin(), p.end());
                    int ww, hh;
                    this->GetClientSize(&ww, &hh);

                    p.push_back(vertex(centx + (ww/2)*scale - 5,centy + (hh/2)*scale - 5,fr,fg,fb,fa));
                    p.push_back(vertex(centx + (ww/2)*scale - 5,centy - (hh/2)*scale + 5,fr,fg,fb,fa));
                    p.push_back(vertex(centx - (ww/2)*scale + 5,centy - (hh/2)*scale + 5,fr,fg,fb,fa));
                    p.push_back(vertex(centx + (ww/2)*scale - 5,centy - (hh/2)*scale + 5,fr,fg,fb,fa));
                    p.push_back(vertex(centx + (ww/2)*scale - 5,centy + (hh/2)*scale - 5,fr,fg,fb,fa));
                    p.push_back(vertex(centx - (ww/2)*scale + 5,centy - (hh/2)*scale + 5,fr,fg,fb,fa));
                    ogldraw(GL_POLYGON,p);
                }
                else*/
                oglLine(p, 1.0, true);
                }
            break;

            case shape::circle: // oglt + ath
                {
                int m; double c;
                numberData * sides = (numberData *)getSurfaceDataPoint(drawnShape->getPropertyByName("sides"));
                formulaData * radius = (formulaData *)getSurfaceDataPoint(drawnShape->getPropertyByName("radius"));
                if (sides==nullptr||radius==nullptr)
                    break;
                double rad = f->parse(radius,globvar);
                drawnShape->setx(x-r/2);
                drawnShape->sety(y-r/2);
                drawnShape->setW(r);
                drawnShape->seth(r);

                m = sides->getData();
                if (m<2)
                    break;
                //ogltransform(x,y,r,xs,ys,xo,yo);
                //applyTransformationHierarchy((matrixData *)drawnShape->getPropertyByName("tfmatrix"));
                c = m / (2*3.1415926535);
                std::vector<vertex> p;
                for(int i=0;i<m;i++)
                {
                    p.push_back(vertex(rad*cos(i/c),
                                       rad*sin(i/c),
                                       fr,fg,fb,fa));
                }
                ogldraw(GL_POLYGON,p);

                if (ot>0.0)
                    {
                    std::vector<vertex> p;
                    m = sides->getData();
                    c = m / (2*3.1415926535);
                    double lx, ly, lxp, lyp;
                    lxp = rad*cos(0);
                    lyp = rad*sin(0);

                    for(int i=0;i<m;i++)
                        {
                            lx = rad*cos(i/c);
                            ly = rad*sin(i/c);
                            //line(lxp, lyp, lx, ly, ot, ro, og, ob, oa, 0.0, 0.0, true);
                            //line_raw(lxp, lyp, lx, ly, ot, ro, og, ob);
                            //hair_line(lxp, lyp, lx, ly, true);
                            lxp = lx;
                            lyp = ly;
                            p.push_back(vertex(lx,ly,ro,og,ob,oa));
                        }
                    oglLine(p, ot, true);
                    }
                }
            break;

            case shape::arc:
                {
                int m; double c;
                numberData * sides = (numberData *)getSurfaceDataPoint(drawnShape->getPropertyByName("sides"));
                formulaData * radius = (formulaData *)getSurfaceDataPoint(drawnShape->getPropertyByName("radius"));
                formulaData * angle = (formulaData *)getSurfaceDataPoint(drawnShape->getPropertyByName("angle"));
                boolData * iol = (boolData *)drawnShape->getPropertyByName("iol");
                boolData * ool = (boolData *)drawnShape->getPropertyByName("ool");
                if (sides==nullptr||radius==nullptr||angle==nullptr||iol==nullptr||ool==nullptr)
                    break;
                double rad = f->parse(radius,globvar);
                double angl = f->parse(angle,globvar);
                bool hasiol = iol->getData();
                bool hasool = ool->getData();
                m = sides->getData();
                if (m<2)
                    break;
                c = m / (2*3.1415926535*angl/360.0);
                std::vector<vertex> p;
                double minx=0.0, maxx=0.0, miny=0.0, maxy=0.0;
                double ix, iy;
                p.push_back(vertex(0,0,fr,fg,fb,fa));
                double maxa = 0.0;
                for(int i=0;i<=m;i++)
                {
                    maxa = i/c;
                    ix = rad*cos(i/c);
                    iy = rad*sin(i/c);
                    p.push_back(vertex(ix,iy,fr,fg,fb,fa));
                    if (i==0 || ix < minx)
                        minx = ix;
                    if (i==0 || ix > maxx)
                        maxx = ix;
                    if (i==0 || iy < miny)
                        miny = iy;
                    if (i==0 || iy > maxy)
                        maxy = iy;
                }
                drawnShape->setx(x+minx);
                drawnShape->sety(y+miny);
                drawnShape->setW(maxx-minx);
                drawnShape->seth(maxy-miny);
                ogldraw(GL_POLYGON,p);

                /*if (ot>0.0&&hasiol&&hasool)  /// code for unified drawing of both outlines, doesn't work
                {
                    std::vector<vertex> ip;
                    ix = rad*cos(2*3.1415926535*angl/360.0);
                    iy = rad*sin(2*3.1415926535*angl/360.0);
                    ip.push_back(vertex(ix,iy,ro,og,ob,oa));
                    ip.push_back(vertex(0.0,0.0,ro,og,ob,oa));
                    ix = rad*cos(0.0);
                    iy = rad*sin(0.0);
                    ip.push_back(vertex(ix,iy,ro,og,ob,oa));
                    m = sides->getData();
                    c = m / (2*3.1415926535*angl/360.0);
                    double lx, ly;
                    for(int i=0;i<=m;i++)
                        {
                        lx = rad*cos(i/c);
                        ly = rad*sin(i/c);
                        ip.push_back(vertex(lx,ly,ro,og,ob,oa));
                        }
                    oglLine(ip, ot, false);
                }else*/
                if (ot>0.0&&hasiol)
                {
                    std::vector<vertex> ip;
                    ix = rad*cos(0.0);
                    iy = rad*sin(0.0);
                    ip.push_back(vertex(ix,iy,ro,og,ob,oa));
                    ip.push_back(vertex(0.0,0.0,ro,og,ob,oa));
                    ix = rad*cos(2*3.1415926535*angl/360.0);
                    iy = rad*sin(2*3.1415926535*angl/360.0);
                    ip.push_back(vertex(ix,iy,ro,og,ob,oa));
                    oglLine(ip, ot, false);
                }//else
                if (ot>0.0&&hasool)
                    {
                    std::vector<vertex> p;
                    m = sides->getData();
                    c = m / (2*3.1415926535*angl/360.0);
                    double lx, ly;
                    for(int i=0;i<=m;i++)
                        {
                        //if (i/c>3.1415926535*angl/180)
                          //  break;
                        lx = rad*cos(i/c);
                        ly = rad*sin(i/c);
                        p.push_back(vertex(lx,ly,ro,og,ob,oa));
                        }
                    oglLine(p, ot, false);
                    }
                }
            break;

            case shape::rectangle:
                {
                formulaData * width = (formulaData *)getSurfaceDataPoint(drawnShape->getPropertyByName("width"));
                formulaData * height = (formulaData *)getSurfaceDataPoint(drawnShape->getPropertyByName("height"));
                if (width==nullptr||height==nullptr)
                    break;
                double w,h;
                w=f->parse(width,globvar);
                h=f->parse(height,globvar);
                drawnShape->setx(x);
                drawnShape->sety(y);
                drawnShape->setW(w);
                drawnShape->seth(h);

                std::vector<vertex> p;
                p.push_back(vertex(0,0,fr,fg,fb,fa));
                p.push_back(vertex(0+w,0,fr,fg,fb,fa));
                p.push_back(vertex(0+w,0+h,fr,fg,fb,fa));
                p.push_back(vertex(0,0+h,fr,fg,fb,fa));
                ogldraw(GL_POLYGON,p);
                if (ot>0.0)
                    {
                    std::vector<vertex> p2;
                    p2.push_back(vertex(0,0,ro, og, ob, oa));
                    p2.push_back(vertex(0+w,0,ro, og, ob, oa));
                    p2.push_back(vertex(0+w,0+h,ro, og, ob, oa));
                    p2.push_back(vertex(0,0+h,ro, og, ob, oa));
                    oglLine(p2, ot, true);
                    }
                }
            break;

            case shape::roundrect:
                {
                formulaData * width = (formulaData *)getSurfaceDataPoint(drawnShape->getPropertyByName("width"));
                formulaData * height = (formulaData *)getSurfaceDataPoint(drawnShape->getPropertyByName("height"));
                formulaData * radius = (formulaData *)getSurfaceDataPoint(drawnShape->getPropertyByName("cradius"));
                numberData * sides = (numberData *)getSurfaceDataPoint(drawnShape->getPropertyByName("sides"));
                if (width==nullptr||height==nullptr)
                    break;
                if (sides==nullptr||radius==nullptr)
                    break;
                double w,h;
                w=f->parse(width,globvar);
                h=f->parse(height,globvar);
                double rad = f->parse(radius,globvar);
                if (w<rad*2+1)
                    w = rad*2+1;
                if (h<rad*2+1)
                    h = rad*2+1;
                drawnShape->setx(x);
                drawnShape->sety(y);
                drawnShape->setW(w);
                drawnShape->seth(h);

                int m = sides->getData();


                std::vector<vertex> p;
                double c = m / (2*3.1415926535/4);
                double p4 = 3.1415926535/2;
                for(int i=0;i<=m;i++)
                    p.push_back(vertex(w-rad+rad*cos(i/c),h-rad+rad*sin(i/c),fr,fg,fb,fa));
                for(int i=0;i<=m;i++)
                    p.push_back(vertex(rad+rad*cos(i/c+p4),h-rad+rad*sin(i/c+p4),fr,fg,fb,fa));
                for(int i=0;i<=m;i++)
                    p.push_back(vertex(rad+rad*cos(i/c+p4*2),rad+rad*sin(i/c+p4*2),fr,fg,fb,fa));
                for(int i=0;i<=m;i++)
                    p.push_back(vertex(w-rad+rad*cos(i/c+p4*3),rad+rad*sin(i/c+p4*3),fr,fg,fb,fa));
                ogldraw(GL_POLYGON,p);
                p.clear();
                if (ot>0.0)
                    {
                    std::vector<vertex> p2;
                    for(int i=0;i<=m;i++)
                        p2.push_back(vertex(w-rad+rad*cos(i/c),h-rad+rad*sin(i/c),ro,og,ob,oa));
                    for(int i=0;i<=m;i++)
                        p2.push_back(vertex(rad+rad*cos(i/c+p4),h-rad+rad*sin(i/c+p4),ro,og,ob,oa));
                    for(int i=0;i<=m;i++)
                        p2.push_back(vertex(rad+rad*cos(i/c+p4*2),rad+rad*sin(i/c+p4*2),ro,og,ob,oa));
                    for(int i=0;i<=m;i++)
                        p2.push_back(vertex(w-rad+rad*cos(i/c+p4*3),rad+rad*sin(i/c+p4*3),ro,og,ob,oa));
                    oglLine(p2, ot, true);
                    }
                }
            break;

            case shape::convex:
                {
                collectionData * points = (collectionData *)drawnShape->getPropertyByName("points");
                if (points==nullptr)
                    break;
                std::vector<vertex> p,op;
                std::vector<vertInfo> vertices=points->getData();
                double minx=0.0, maxx=0.0, miny=0.0, maxy=0.0;
                double ix, iy;
                for(int i=0;i<vertices.size();++i)
                {
                    ix = f->parsestring(vertices[i].x,globvar);
                    iy = f->parsestring(vertices[i].y,globvar);
                    p.push_back(vertex(ix,iy,fr, fg, fb, fa));
                    op.push_back(vertex(ix,iy,ro, og, ob, oa));
                    if (i==0 || ix < minx)
                        minx = ix;
                    if (i==0 || ix > maxx)
                        maxx = ix;
                    if (i==0 || iy < miny)
                        miny = iy;
                    if (i==0 || iy > maxy)
                        maxy = iy;
                }

                drawnShape->setx(x+minx);
                drawnShape->sety(y+miny);
                drawnShape->setW(maxx-minx);
                drawnShape->seth(maxy-miny);

                if (p.size()>2)
                    {
                    ogldraw(GL_POLYGON,p);
                    oglLine(op, ot, true);
                    }
                }
            break;

            case shape::text:
                {
                fontData * fd = (fontData *)drawnShape->getPropertyByName("font");
                if (fd==nullptr)
                    break;
                std::string fontpath = fd->getData();
                std::string dbg = fontpath;
                if (ftfaces.find(fontpath)==ftfaces.end())
                {
                    //FTFace = FTGLPixmapFont("DejaVuSans.ttf");
                    //if (FTFace.Error())
                    //dbg+="1";
                    ftfacesinit[fontpath] = true;
                    //ftfaces[fontpath] = FTGLPixmapFont{fontpath.c_str()};
                    //ftfaces[fontpath] = new FTGLPixmapFont{fontpath.c_str()};
                    //if (ftfaces[fontpath].Error())
                    //if (ftfaces[fontpath]->Error())
                    //    ftfacesinit[fontpath] = false;
                    if (FT_New_Face(FTLib, fontpath.c_str(), 0, &initFace))
                        {ftfacesinit[fontpath] = false;dbg+=" x.x ";}
                    ftfaces[fontpath] = initFace; //only temporary
                   // ftfaces[fontpath] = &FTFace; //only temporary
                }
                if (ftfacesinit[fontpath])
                    currentFace = ftfaces[fontpath];
                else
                {
                    dbg+=" x_x";
                    if (FTFaceInit)
                        currentFace = FTFace;
                    else
                        break;
                }
                dbg+=";";

                //currentFace = FTFace;

                numberData * syze = (numberData *)getSurfaceDataPoint(drawnShape->getPropertyByName("size"));
                if (syze == nullptr) //currentFace.FaceSize(12);//
                    FT_Set_Pixel_Sizes(currentFace, 0, 12);
                else //currentFace.FaceSize(syze->getData());//
                    FT_Set_Pixel_Sizes(currentFace, 0, syze->getData());
                textData * tex = (textData *)getSurfaceDataPoint(drawnShape->getPropertyByName("text"));
                std::string label;
                if(tex!=nullptr)
                    label = tex->getData();
                else
                    label = "(text)";
                if (brdata!=nullptr&&tex!=nullptr)
                {
                    std::string txt = tex->getData();
                    std::vector<std::string> toBeSet = f->getTagsFromText(txt);
                    if (toBeSet.size()>0)
                    {
                        std::vector<std::string> reps;
                        for (int rr=0;rr<toBeSet.size();rr++)
                        {
                            dataPoint * tds = brdata->getProperty(toBeSet[rr],dataPoint::text);
                            if (tds!=nullptr)
                            if (tds->getType()==dataPoint::text)
                            {
                                reps.push_back(((textData *) tds)->getData());
                            }
                        }
                        label = f->applyTagsToText(label, reps);
                    }
                }
                textw = texth = 0;
                oglText(label,fc);
                //currentFace.Render(label.c_str());
                //textw = 150;
                //texth = 30;
                drawnShape->setx(x);
                drawnShape->sety(y);
                drawnShape->setW(textw);
                drawnShape->seth(texth);
                }
            break;

            case shape::image:
                {
                imageData * id = (imageData *)getSurfaceDataPoint(drawnShape->getPropertyByName("image"));
                if (id==nullptr)
                    break;
                drawnShape->setx(x);
                drawnShape->sety(y);
                drawnShape->setW(0);
                drawnShape->seth(0);
                if (id->getData().size()<5)
                    break;
                std::string imagepath = id->getData();

                if (textures.find(imagepath)==textures.end())
                    {
                        glEnable(GL_TEXTURE_2D);
                        glGenTextures(1, &(textures[imagepath].t));

                        glActiveTexture(GL_TEXTURE0);
                        glBindTexture(GL_TEXTURE_2D, textures[imagepath].t);



                        textures[imagepath].i = SOIL_load_image(imagepath.c_str(),
                                                                &(textures[imagepath].w),
                                                                &(textures[imagepath].h),
                                                                0, SOIL_LOAD_RGBA);
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                                     textures[imagepath].w,
                                     textures[imagepath].h, 0, GL_RGBA,
                                     GL_UNSIGNED_BYTE, textures[imagepath].i);

                        glDisable(GL_TEXTURE_2D);
                    }
                if (textures.find(imagepath)!=textures.end())
                    if(textures[imagepath].t>0)
                    {


                        glEnable(GL_TEXTURE_2D);
                        glActiveTexture(GL_TEXTURE0);
                        glBindTexture(GL_TEXTURE_2D, textures[imagepath].t);
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                                     textures[imagepath].w,
                                     textures[imagepath].h, 0, GL_RGBA,
                                     GL_UNSIGNED_BYTE, textures[imagepath].i);
                        glDisable(GL_TEXTURE_2D);


                        std::string errr = "toll!";
                        GLenum errLast = GL_NO_ERROR;
                        while(true)
                        {
                            GLenum err = glGetError();
                            if ( err == GL_NO_ERROR )
                                break;
                            if ( err == errLast )
                            {
                                errr = "OpenGL error state couldn't be reset.";
                                break;
                            }
                            errLast = err;
                            errr = "OpenGL error "+std::to_string(err);
                        }
                        colorconv c(255,255,255,255);
                        colorData * blend = (colorData *)getSurfaceDataPoint(drawnShape->getPropertyByName("blend"));
                        if(blend!=nullptr)
                            c = blend->getData();
                        oglImage(textures[imagepath],c);
                        drawnShape->setx(x);
                        drawnShape->sety(y);
                        drawnShape->setW(textures[imagepath].w);
                        drawnShape->seth(textures[imagepath].h);
                        errr = "toll!"+std::to_string(textures[imagepath].t);
                        errLast = GL_NO_ERROR;
                        while(true)
                        {
                            GLenum err = glGetError();
                            if ( err == GL_NO_ERROR )
                                break;
                            if ( err == errLast )
                            {
                                errr = "OpenGL error state couldn't be reset.";
                                break;
                            }
                            errLast = err;
                            errr = "OpenGL error "+std::to_string(err);
                        }
                    }
                else
                    {
                    }
                }
            break;

            case shape::points:
                {
                choiceData * cd = (choiceData *)drawnShape->getPropertyByName("type");
                if (cd==nullptr)
                    break;
                collectionData * points = (collectionData *)drawnShape->getPropertyByName("points");
                if (points==nullptr)
                    break;
                std::vector<vertex> p;
                std::vector<vertInfo> vertices=points->getData();
                double minx=0.0, maxx=0.0, miny=0.0, maxy=0.0;
                double ix, iy;
                for(int i=0;i<vertices.size();++i)
                {
                    //globvar = false;
                    ix = f->parsestring(vertices[i].x,globvar);
                    iy = f->parsestring(vertices[i].y,globvar);
                    p.push_back(vertex(ix,iy,
                                       vertices[i].col.oglr(),
                                       vertices[i].col.oglg(),
                                       vertices[i].col.oglb(),
                                       vertices[i].col.ogla()));
                    if (i==0 || ix < minx)
                        minx = ix;
                    if (i==0 || ix > maxx)
                        maxx = ix;
                    if (i==0 || iy < miny)
                        miny = iy;
                    if (i==0 || iy > maxy)
                        maxy = iy;
                    //hair_line(ix-3,iy,ix+3,iy,true);
                    //hair_line(ix,iy-3,ix,iy+3,true);

                    //hair_line((i+1)*10,-5,(i+1)*10,3,true);
                }
                drawnShape->setx(x+minx);
                drawnShape->sety(y+miny);
                drawnShape->setW(maxx-minx);
                drawnShape->seth(maxy-miny);


                if (p.size()>1)
                switch(cd->getData())
                {
                    case 1: oglLine(p, 1.0);break;
                    case 2: oglLine(p, 1.0, false);break;
                    case 3:
                        ogldraw(GL_TRIANGLES,p);
                        //oglLine(p,1.0, false);
                        break;
                    case 4: ogldraw(GL_TRIANGLE_STRIP,p);break;
                    case 5: ogldraw(GL_TRIANGLE_FAN,p);break;
                    case 6: ogldraw(GL_QUADS,p);break;
                }
                }
            break;

            case shape::matrix:
                {
                drawnShape->setx(0.0);
                drawnShape->sety(0.0);
                drawnShape->setW(0.0);
                drawnShape->seth(0.0);
                if (brdata!=nullptr)
                        break;
                boolData * grid = (boolData *)drawnShape->getPropertyByName("grid");
                colorData * gridcolor = (colorData *)drawnShape->getPropertyByName("gridcolor");
                boolData * tfm = (boolData *)drawnShape->getPropertyByName("tfm");
                colorData * tfmcolor = (colorData *)drawnShape->getPropertyByName("tfmcolor");
                if (grid==nullptr||gridcolor==nullptr||tfm==nullptr||tfmcolor==nullptr)
                    break;
                if (grid->getData())
                {
                    identity();
                    ogltransform(x,y,r,xs,ys,xo,yo);
                    applyTransformationHierarchy((matrixData *)drawnShape->getPropertyByName("tfmatrix"));
                    colorconv col = gridcolor->getData();
                    std::vector<vertex> p;
                    for(int i=-10;i<=10;i++)
                    {
                        p.push_back(vertex(i*100,-1000,col.oglr(),col.oglg(),col.oglb(),col.ogla()));
                        p.push_back(vertex(i*100,1000,col.oglr(),col.oglg(),col.oglb(),col.ogla()));
                        p.push_back(vertex(-1000,i*100,col.oglr(),col.oglg(),col.oglb(),col.ogla()));
                        p.push_back(vertex(1000,i*100,col.oglr(),col.oglg(),col.oglb(),col.ogla()));
                    }
                    oglLine(p, 1.0);
                }
                if (tfm->getData())
                {
                    identity();
                    applyTransformationHierarchy((matrixData *)drawnShape->getPropertyByName("tfmatrix"));
                    colorconv col = tfmcolor->getData();
                    std::vector<vertex> p;
                    for(int i=-10;i<=10;i++)for(int j=-10;j<=10;j++)
                    {
                        p.push_back(vertex(i*100,j*100,col.oglr(),col.oglg(),col.oglb(),col.ogla()));
                        p.push_back(vtxtransform(
                                    vertex(i*100,j*100,col.oglr(),col.oglg(),col.oglb(),col.ogla()),
                                                 x,y,r,xs,ys,xo,yo));
                    }
                    oglLine(p, 1.0);
                }
                }
            break;
            }

         /*if (false)
            {
            std::vector<vertex> _p;
            _p.push_back(vertex(testXmin,testYmin,0, 1, 0, 1));
            _p.push_back(vertex(testXmax,testYmin,0, 1, 0, 1));
            _p.push_back(vertex(testXmax,testYmax,0, 1, 0, 1));
            _p.push_back(vertex(testXmin,testYmax,0, 1, 0, 1));
            oglLine(_p, 1.0, true);
            }*/


    }
}


void OGLplotter::ogldraw(GLenum mode, std::vector<vertex> points)
{
    //return ;
    //GLuint vbo1;
    if (!pvflag)
    {
        glGenBuffersARB(1, &plainvbo);
        pvflag = true;
    }


    glBindBufferARB(GL_ARRAY_BUFFER_ARB, plainvbo);
    ///////^^??
    //http://duriansoftware.com/joe/An-intro-to-modern-OpenGL.-Chapter-2.1:-Buffers-and-Textures.html
    //// + dynamic draw \/
    glBufferDataARB(GL_ARRAY_BUFFER_ARB,points.size() * sizeof(vertex),&points[0],GL_STATIC_DRAW_ARB);
    glVertexPointer(2, GL_FLOAT, sizeof(vertex), NULL);
    glColorPointer(4, GL_FLOAT, sizeof(vertex), (void*)(sizeof(float)*2));
    glEnable(GL_TEXTURE_2D);
    glDisable(GL_TEXTURE_2D); /// these two lines solve the transparency leak issue

///    glEnableClientState(GL_VERTEX_ARRAY);
///    glEnableClientState(GL_COLOR_ARRAY);
    enableClientStates();
    glDrawArrays(mode, 0, points.size());
///    glDisableClientState(GL_COLOR_ARRAY);
///    glDisableClientState(GL_VERTEX_ARRAY);
}

void OGLplotter::oglImage(texture gluimage, colorconv blend)
{
    float ww, hh;
    ww=(float)gluimage.w;
    hh=(float)gluimage.h;
    float r = blend.oglr(), g = blend.oglg(), b = blend.oglg(), a = blend.ogla();
    float points[] =
    {
        0.0, 0.0,   r, g, b, a, 0.0, 0.0,
        0.0,  hh,   r, g, b, a, 0.0, 1.0,
         ww,  hh,   r, g, b, a, 1.0, 1.0,
         ww, 0.0,   r, g, b, a, 1.0, 0.0
    };
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, gluimage.t);
    //GLuint vbo1;
    //glGenBuffersARB(1, &vbo1);
    if (!ivflag)
    {
        glGenBuffersARB(1, &imvbo);
        ivflag = true;
    }
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, imvbo);
    glBufferDataARB(GL_ARRAY_BUFFER_ARB,4 * sizeof(float) * 8,&points[0],GL_STATIC_DRAW_ARB);
    glVertexPointer(2, GL_FLOAT, sizeof(float) * 8, NULL);
    glColorPointer(4, GL_FLOAT, sizeof(float) * 8, (void*)(sizeof(float)*2));
    glTexCoordPointer(2, GL_FLOAT, sizeof(float) * 8, (void*)(sizeof(float)*6));
///    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
///    glEnableClientState(GL_VERTEX_ARRAY);
///    glEnableClientState(GL_COLOR_ARRAY);
    enableClientStates();
    glDrawArrays(GL_QUADS, 0, 4);
///    glDisableClientState(GL_COLOR_ARRAY);
///    glDisableClientState(GL_VERTEX_ARRAY);
///    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisable(GL_TEXTURE_2D);
}



void OGLplotter::oglText(std::string text, colorconv blend)
{
   if (!FTFaceInit||!FTInit)
        return;
    const char *p;
	FT_GlyphSlot g = currentFace->glyph;
	GLuint tex;
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	//glGenTextures(1, &tex);
	if (!glgentexflag)
	{
	    glGenTextures(1, &tex);
	    glgentexflag = true;
	}
	glBindTexture(GL_TEXTURE_2D, tex);
	//glUniform1i(uniform_tex, 0);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//GLuint vbo;

	if (!tvflag)
    {
        glGenBuffersARB(1, &textvbo);
        tvflag = true;
    }
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, textvbo);


	double x=0.0, y=0.0;//currentFace->size->metrics.height;
    glVertexPointer(2, GL_FLOAT, sizeof(float) * 8, NULL);
    glColorPointer(4, GL_FLOAT, sizeof(float) * 8, (void*)(sizeof(float)*2));
    glTexCoordPointer(2, GL_FLOAT, sizeof(float) * 8, (void*)(sizeof(float)*6));
///    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
///    glEnableClientState(GL_VERTEX_ARRAY);
///    glEnableClientState(GL_COLOR_ARRAY);

	enableClientStates();

	int i = 0;
	size_t sp = 0;
    while((sp = text.find("\n\r", sp)) != std::string::npos)
    {
        text.replace(sp, 2, "\n");
        sp += 1;
    }
	float r = blend.oglr(), gr = blend.oglg(), b = blend.oglg(), a = blend.ogla();
	for (p = (text.c_str()); *p; p++, i++)
	{
        if (text[i] == '\n' || text[i] == '\r')
            {
            y += ((double)FTFace->size->metrics.height)/80.0 + 2.0;
            if (y > texth)
                texth = y;
            x=0.0;
            continue;
            }
         else if (i == 0)
            {
            y += ((double)FTFace->size->metrics.height)/80.0 + 2.0;
            if (y > texth)
                texth = y;
            }
		if (FT_Load_Char(currentFace, *p, FT_LOAD_RENDER))
			continue;
		glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, g->bitmap.width, g->bitmap.rows, 0,
               GL_ALPHA, GL_UNSIGNED_BYTE, g->bitmap.buffer);

		float x2 = x + g->bitmap_left;
		float y2 = y - g->bitmap_top;
		float w = g->bitmap.width;
		float h = g->bitmap.rows;

		float points[32] = {
			x2,     y2,     r, gr, b, a, 0, 0,
			x2 + w, y2,     r, gr, b, a, 1, 0,
			x2,     y2 + h, r, gr, b, a, 0, 1,
			x2 + w, y2 + h, r, gr, b, a, 1, 1
		};

		glBufferDataARB(GL_ARRAY_BUFFER_ARB, 4*sizeof(float)*8, &points[0], GL_STATIC_DRAW_ARB);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		if (text[i] != '\n' && text[i] != '\r')
            {
            x += g->advance.x >> 6;
            y += g->advance.y >> 6;
            if (x > textw)
                textw = x;
            if (y > texth)
                texth = y;
            }
	}
///    glDisableClientState(GL_COLOR_ARRAY);
///    glDisableClientState(GL_VERTEX_ARRAY);
///    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisable(GL_TEXTURE_2D);

    //GLfloat black[4] = {0, 0, 0, 1};
  //  glUniform4fv(uniform_color, 1, black);

	//glDeleteTextures(1, &tex);
}

void OGLplotter::oglTextSetWH(std::string text, colorconv blend)
{
    if (!FTFaceInit||!FTInit)
        return;
    const char *p;
	FT_GlyphSlot g = currentFace->glyph;

	double x=0.0, y=0.0;//currentFace->size->metrics.height;
	int i = 0;
	size_t sp = 0;
    while((sp = text.find("\n\r", sp)) != std::string::npos)
    {
        text.replace(sp, 2, "\n");
        sp += 1;
    }
	for (p = (text.c_str()); *p; p++, i++)
	{
        if (text[i] == '\n' || text[i] == '\r')
            {
            y += ((double)FTFace->size->metrics.height)/80.0 + 2.0;
            if (y > texth)
                texth = y;
            x=0.0;
            continue;
            }
         else if (i == 0)
            {
            y += ((double)FTFace->size->metrics.height)/80.0 + 2.0;
            if (y > texth)
                texth = y;
            }
		if (FT_Load_Char(currentFace, *p, FT_LOAD_RENDER))
			continue;


		if (text[i] != '\n' && text[i] != '\r')
            {
            x += g->advance.x >> 6;
            y += g->advance.y >> 6;
            if (x > textw)
                textw = x;
            if (y > texth)
                texth = y;
            }
	}
}



void OGLplotter::OnKeyDown(wxKeyEvent& event)
{
    switch ( event.GetKeyCode() )
    {
        case WXK_RIGHT:
            centx+=10*scale;
            refresh();
            break;
        case WXK_LEFT:
            centx-=10*scale;
            refresh();
            break;
        case WXK_DOWN:
            centy+=10*scale;
            refresh();
            break;
        case WXK_UP:
            centy-=10*scale;
            refresh();
            break;
        case WXK_SPACE:
            resetView(true);
            break;
        case WXK_PAGEDOWN:
            scale*=1.1;
            refresh();
        break;
        case WXK_PAGEUP:
            scale*=0.9;
            refresh();
        break;

        default:
            event.Skip();
            return;
    }
    owner->getUIAPI()->setStatusBarString("Zoom: "+roundto2(scale*100)+"%", 0);
    owner->getUIAPI()->setStatusBarString("View x: "+roundto2(centx)+"", 1);
    owner->getUIAPI()->setStatusBarString("View y: "+roundto2(centy)+"", 2);
    //statusBar->SetStatusText(sk,3);
    //owner->getUIAPI()->setStatusBarString("Mouse x: "+roundto2(xx)+"", 4);
    //owner->getUIAPI()->setStatusBarString("Mouse y: "+roundto2(yy)+"", 5);
}

wxString glGetwxString(GLenum name)
{
    const GLubyte *v = glGetString(name);
    if ( v == 0 )
    {
        glGetError();

        return wxString();
    }

    return wxString((const char*)v);
}
void OGLplotter::applyTransformationHierarchy(matrixData * md)
{
    double rot=0.0, xs=1.0, ys=1.0, xo=0.0, yo=0.0,x=0.0,y=0.0;
    shape * mat;
    formulaData * xpos;
    formulaData * ypos;
    formulaData * xorigin;
    formulaData * yorigin;
    formulaData * rotation;
    formulaData * xscale;
    formulaData * yscale;
    while (md!=nullptr)
    {
        mat = md->getData();
        if (mat==nullptr)
            break;
        if (mat->getType()!=shape::matrix)
            break;
        rot=0.0; xs=1.0; ys=1.0; xo=0.0; yo=0.0;x=0.0;y=0.0;
        xpos = (formulaData *) mat->getPropertyByName("xpos");
        ypos = (formulaData *) mat->getPropertyByName("ypos");
        xorigin = (formulaData *)mat->getPropertyByName("xorigin");
        yorigin = (formulaData *)mat->getPropertyByName("yorigin");
        rotation = (formulaData *)mat->getPropertyByName("rotation");
        xscale = (formulaData *)mat->getPropertyByName("xscale");
        yscale = (formulaData *)mat->getPropertyByName("yscale");
        if(xpos!=nullptr&&ypos!=nullptr)
            {
            x=f->parse(xpos);
            y=f->parse(ypos);
            }
        if(xorigin!=nullptr&&yorigin!=nullptr)
            {
            xo=f->parse(xorigin);
            yo=f->parse(yorigin);
            }
        if(rotation!=nullptr)
            rot=f->parse(rotation);
        if(xscale!=nullptr&&yscale!=nullptr)
            {
            xs=f->parse(xscale);
            ys=f->parse(yscale);
            }
        double tfm[16];
        double ac=3.1415926535/180;
        tfm[0] = 1 ;	tfm[1] = 0 ;	tfm[2] = 0 ;	tfm[3] = x ;
        tfm[4] = 0 ;	tfm[5] = 1 ;	tfm[6] = 0 ;	tfm[7] = y ;
        tfm[8] = 0 ;	tfm[9] = 0 ;	tfm[10] = 1 ;	tfm[11] = 0 ;
        tfm[12] = 0 ;	tfm[13] = 0 ;	tfm[14] = 0 ;	tfm[15] = 1 ;
        glMultTransposeMatrixd(tfm);
        tfm[0] = 1 ;	tfm[1] = 0 ;	tfm[2] = 0 ;	tfm[3] = xo ;
        tfm[4] = 0 ;	tfm[5] = 1 ;	tfm[6] = 0 ;	tfm[7] = yo ;
        tfm[8] = 0 ;	tfm[9] = 0 ;	tfm[10] = 1 ;	tfm[11] = 0 ;
        tfm[12] = 0 ;	tfm[13] = 0 ;	tfm[14] = 0 ;	tfm[15] = 1 ;
        glMultTransposeMatrixd(tfm);
        tfm[0] = cos(rot*ac) ;	tfm[1] = -sin(rot*ac) ;	tfm[2] = 0 ;	tfm[3] = 0 ;
        tfm[4] = sin(rot*ac) ;	tfm[5] = cos(rot*ac) ;	tfm[6] = 0 ;	tfm[7] = 0 ;
        tfm[8] = 0 ;	tfm[9] = 0 ;	tfm[10] = 1 ;	tfm[11] = 0 ;
        tfm[12] = 0 ;	tfm[13] = 0 ;	tfm[14] = 0 ;	tfm[15] = 1 ;
        glMultTransposeMatrixd(tfm);
        tfm[0] = xs ;	tfm[1] = 0 ;	tfm[2] = 0 ;	tfm[3] = 0 ;
        tfm[4] = 0 ;	tfm[5] = ys ;	tfm[6] = 0 ;	tfm[7] = 0 ;
        tfm[8] = 0 ;	tfm[9] = 0 ;	tfm[10] = 1 ;	tfm[11] = 0 ;
        tfm[12] = 0 ;	tfm[13] = 0 ;	tfm[14] = 0 ;	tfm[15] = 1 ;
        glMultTransposeMatrixd(tfm);
        tfm[0] = 1 ;	tfm[1] = 0 ;	tfm[2] = 0 ;	tfm[3] = -xo ;
        tfm[4] = 0 ;	tfm[5] = 1 ;	tfm[6] = 0 ;	tfm[7] = -yo ;
        tfm[8] = 0 ;	tfm[9] = 0 ;	tfm[10] = 1 ;	tfm[11] = 0 ;
        tfm[12] = 0 ;	tfm[13] = 0 ;	tfm[14] = 0 ;	tfm[15] = 1 ;
        glMultTransposeMatrixd(tfm);
        md = (matrixData *)mat->getPropertyByName("tfmatrix");
    }
}
void OGLplotter::ogltransform(double x, double y, double r, double xs, double ys, double ox, double oy)
{
    double tfm[16];
    double ac=3.1415926535/180;
    tfm[0] = 1 ;	tfm[1] = 0 ;	tfm[2] = 0 ;	tfm[3] = x ;
    tfm[4] = 0 ;	tfm[5] = 1 ;	tfm[6] = 0 ;	tfm[7] = y ;
    tfm[8] = 0 ;	tfm[9] = 0 ;	tfm[10] = 1 ;	tfm[11] = 0 ;
    tfm[12] = 0 ;	tfm[13] = 0 ;	tfm[14] = 0 ;	tfm[15] = 1 ;
    glMultTransposeMatrixd(tfm);
    tfm[0] = 1 ;	tfm[1] = 0 ;	tfm[2] = 0 ;	tfm[3] = ox ;
    tfm[4] = 0 ;	tfm[5] = 1 ;	tfm[6] = 0 ;	tfm[7] = oy ;
    tfm[8] = 0 ;	tfm[9] = 0 ;	tfm[10] = 1 ;	tfm[11] = 0 ;
    tfm[12] = 0 ;	tfm[13] = 0 ;	tfm[14] = 0 ;	tfm[15] = 1 ;
    glMultTransposeMatrixd(tfm);
    tfm[0] = cos(r*ac) ;	tfm[1] = -sin(r*ac) ;	tfm[2] = 0 ;	tfm[3] = 0 ;
    tfm[4] = sin(r*ac) ;	tfm[5] = cos(r*ac) ;	tfm[6] = 0 ;	tfm[7] = 0 ;
    tfm[8] = 0 ;	tfm[9] = 0 ;	tfm[10] = 1 ;	tfm[11] = 0 ;
    tfm[12] = 0 ;	tfm[13] = 0 ;	tfm[14] = 0 ;	tfm[15] = 1 ;
    glMultTransposeMatrixd(tfm);
    tfm[0] = xs ;	tfm[1] = 0 ;	tfm[2] = 0 ;	tfm[3] = 0 ;
    tfm[4] = 0 ;	tfm[5] = ys ;	tfm[6] = 0 ;	tfm[7] = 0 ;
    tfm[8] = 0 ;	tfm[9] = 0 ;	tfm[10] = 1 ;	tfm[11] = 0 ;
    tfm[12] = 0 ;	tfm[13] = 0 ;	tfm[14] = 0 ;	tfm[15] = 1 ;
    glMultTransposeMatrixd(tfm);
    tfm[0] = 1 ;	tfm[1] = 0 ;	tfm[2] = 0 ;	tfm[3] = -ox ;
    tfm[4] = 0 ;	tfm[5] = 1 ;	tfm[6] = 0 ;	tfm[7] = -oy ;
    tfm[8] = 0 ;	tfm[9] = 0 ;	tfm[10] = 1 ;	tfm[11] = 0 ;
    tfm[12] = 0 ;	tfm[13] = 0 ;	tfm[14] = 0 ;	tfm[15] = 1 ;
    glMultTransposeMatrixd(tfm);
}
Matrix OGLplotter::mtransform(Matrix m, double x, double y, double r, double xs, double ys, double ox, double oy)
{
    Matrix tfm;
    double ac=3.1415926535/180;
    tfm.m[0] = 1 ;	tfm.m[1] = 0 ;	tfm.m[2] = 0 ;	tfm.m[3] = x ;
    tfm.m[4] = 0 ;	tfm.m[5] = 1 ;	tfm.m[6] = 0 ;	tfm.m[7] = y ;
    tfm.m[8] = 0 ;	tfm.m[9] = 0 ;	tfm.m[10] = 1 ;	tfm.m[11] = 0 ;
    tfm.m[12] = 0 ;	tfm.m[13] = 0 ;	tfm.m[14] = 0 ;	tfm.m[15] = 1 ;
    m = multiplyMatrices(tfm, m);
    tfm.m[0] = 1 ;	tfm.m[1] = 0 ;	tfm.m[2] = 0 ;	tfm.m[3] = ox ;
    tfm.m[4] = 0 ;	tfm.m[5] = 1 ;	tfm.m[6] = 0 ;	tfm.m[7] = oy ;
    tfm.m[8] = 0 ;	tfm.m[9] = 0 ;	tfm.m[10] = 1 ;	tfm.m[11] = 0 ;
    tfm.m[12] = 0 ;	tfm.m[13] = 0 ;	tfm.m[14] = 0 ;	tfm.m[15] = 1 ;
    m = multiplyMatrices(tfm, m);
    tfm.m[0] = cos(r*ac) ;	tfm.m[1] = -sin(r*ac) ;	tfm.m[2] = 0 ;	tfm.m[3] = 0 ;
    tfm.m[4] = sin(r*ac) ;	tfm.m[5] = cos(r*ac) ;	tfm.m[6] = 0 ;	tfm.m[7] = 0 ;
    tfm.m[8] = 0 ;	tfm.m[9] = 0 ;	tfm.m[10] = 1 ;	tfm.m[11] = 0 ;
    tfm.m[12] = 0 ;	tfm.m[13] = 0 ;	tfm.m[14] = 0 ;	tfm.m[15] = 1 ;
    m = multiplyMatrices(tfm, m);
    tfm.m[0] = xs ;	tfm.m[1] = 0 ;	tfm.m[2] = 0 ;	tfm.m[3] = 0 ;
    tfm.m[4] = 0 ;	tfm.m[5] = ys ;	tfm.m[6] = 0 ;	tfm.m[7] = 0 ;
    tfm.m[8] = 0 ;	tfm.m[9] = 0 ;	tfm.m[10] = 1 ;	tfm.m[11] = 0 ;
    tfm.m[12] = 0 ;	tfm.m[13] = 0 ;	tfm.m[14] = 0 ;	tfm.m[15] = 1 ;
    m = multiplyMatrices(tfm, m);
    tfm.m[0] = 1 ;	tfm.m[1] = 0 ;	tfm.m[2] = 0 ;	tfm.m[3] = -ox ;
    tfm.m[4] = 0 ;	tfm.m[5] = 1 ;	tfm.m[6] = 0 ;	tfm.m[7] = -oy ;
    tfm.m[8] = 0 ;	tfm.m[9] = 0 ;	tfm.m[10] = 1 ;	tfm.m[11] = 0 ;
    tfm.m[12] = 0 ;	tfm.m[13] = 0 ;	tfm.m[14] = 0 ;	tfm.m[15] = 1 ;
    return multiplyMatrices(tfm, m);
}
Matrix OGLplotter::applyTransformationHierarchyToMatrix(Matrix m, matrixData * md)
{
    double rot=0.0, xs=1.0, ys=1.0, xo=0.0, yo=0.0,x=0.0,y=0.0;
    shape * mat;
    formulaData * xpos;
    formulaData * ypos;
    formulaData * xorigin;
    formulaData * yorigin;
    formulaData * rotation;
    formulaData * xscale;
    formulaData * yscale;
    while (md!=nullptr)
    {
        mat = md->getData();
        if (mat==nullptr)
            break;
        if (mat->getType()!=shape::matrix)
            break;
        rot=0.0; xs=1.0; ys=1.0; xo=0.0; yo=0.0;x=0.0;y=0.0;
        xpos = (formulaData *) mat->getPropertyByName("xpos");
        ypos = (formulaData *) mat->getPropertyByName("ypos");
        xorigin = (formulaData *)mat->getPropertyByName("xorigin");
        yorigin = (formulaData *)mat->getPropertyByName("yorigin");
        rotation = (formulaData *)mat->getPropertyByName("rotation");
        xscale = (formulaData *)mat->getPropertyByName("xscale");
        yscale = (formulaData *)mat->getPropertyByName("yscale");
        if(xpos!=nullptr&&ypos!=nullptr)
            {
            x=f->parse(xpos);
            y=f->parse(ypos);
            }
        if(xorigin!=nullptr&&yorigin!=nullptr)
            {
            xo=f->parse(xorigin);
            yo=f->parse(yorigin);
            }
        if(rotation!=nullptr)
            rot=f->parse(rotation);
        if(xscale!=nullptr&&yscale!=nullptr)
            {
            xs=f->parse(xscale);
            ys=f->parse(yscale);
            }
        Matrix tfm;
        double ac=3.1415926535/180;
        tfm.m[0] = 1 ;	tfm.m[1] = 0 ;	tfm.m[2] = 0 ;	tfm.m[3] = x ;
        tfm.m[4] = 0 ;	tfm.m[5] = 1 ;	tfm.m[6] = 0 ;	tfm.m[7] = y ;
        tfm.m[8] = 0 ;	tfm.m[9] = 0 ;	tfm.m[10] = 1 ;	tfm.m[11] = 0 ;
        tfm.m[12] = 0 ;	tfm.m[13] = 0 ;	tfm.m[14] = 0 ;	tfm.m[15] = 1 ;
        m = multiplyMatrices(tfm, m);
        tfm.m[0] = 1 ;	tfm.m[1] = 0 ;	tfm.m[2] = 0 ;	tfm.m[3] = xo ;
        tfm.m[4] = 0 ;	tfm.m[5] = 1 ;	tfm.m[6] = 0 ;	tfm.m[7] = yo ;
        tfm.m[8] = 0 ;	tfm.m[9] = 0 ;	tfm.m[10] = 1 ;	tfm.m[11] = 0 ;
        tfm.m[12] = 0 ;	tfm.m[13] = 0 ;	tfm.m[14] = 0 ;	tfm.m[15] = 1 ;
        m = multiplyMatrices(tfm, m);
        tfm.m[0] = cos(rot*ac) ;	tfm.m[1] = -sin(rot*ac) ;	tfm.m[2] = 0 ;	tfm.m[3] = 0 ;
        tfm.m[4] = sin(rot*ac) ;	tfm.m[5] = cos(rot*ac) ;	tfm.m[6] = 0 ;	tfm.m[7] = 0 ;
        tfm.m[8] = 0 ;	tfm.m[9] = 0 ;	tfm.m[10] = 1 ;	tfm.m[11] = 0 ;
        tfm.m[12] = 0 ;	tfm.m[13] = 0 ;	tfm.m[14] = 0 ;	tfm.m[15] = 1 ;
        m = multiplyMatrices(tfm, m);
        tfm.m[0] = xs ;	tfm.m[1] = 0 ;	tfm.m[2] = 0 ;	tfm.m[3] = 0 ;
        tfm.m[4] = 0 ;	tfm.m[5] = ys ;	tfm.m[6] = 0 ;	tfm.m[7] = 0 ;
        tfm.m[8] = 0 ;	tfm.m[9] = 0 ;	tfm.m[10] = 1 ;	tfm.m[11] = 0 ;
        tfm.m[12] = 0 ;	tfm.m[13] = 0 ;	tfm.m[14] = 0 ;	tfm.m[15] = 1 ;
        m = multiplyMatrices(tfm, m);
        tfm.m[0] = 1 ;	tfm.m[1] = 0 ;	tfm.m[2] = 0 ;	tfm.m[3] = -xo ;
        tfm.m[4] = 0 ;	tfm.m[5] = 1 ;	tfm.m[6] = 0 ;	tfm.m[7] = -yo ;
        tfm.m[8] = 0 ;	tfm.m[9] = 0 ;	tfm.m[10] = 1 ;	tfm.m[11] = 0 ;
        tfm.m[12] = 0 ;	tfm.m[13] = 0 ;	tfm.m[14] = 0 ;	tfm.m[15] = 1 ;
        m = multiplyMatrices(tfm, m);
        md = (matrixData *)mat->getPropertyByName("tfmatrix");
    }
    return m;
}
void OGLplotter::oglMtransform(Matrix m)
{
    glMultTransposeMatrixd(m.m);
}
vertex OGLplotter::vtxMtransform(Matrix m, vertex v)
{
    Matrix tfm;
    tfm.m[0] = 1 ;	tfm.m[1] = 0 ;	tfm.m[2] = 0 ;	tfm.m[3] = v.x ;
    tfm.m[4] = 0 ;	tfm.m[5] = 1 ;	tfm.m[6] = 0 ;	tfm.m[7] = v.y ;
    tfm.m[8] = 0 ;	tfm.m[9] = 0 ;	tfm.m[10] = 1 ;	tfm.m[11] = 0 ;
    tfm.m[12] = 0 ;	tfm.m[13] = 0 ;	tfm.m[14] = 0 ;	tfm.m[15] = 1 ;
    m = multiplyMatrices(m, tfm); // m, tfm
    return vertex(m.m[3], m.m[7], v.r, v.g, v.b, v.a);
}
Matrix OGLplotter::transformationToMatrix(double x, double y, double r, double xs, double ys, double ox, double oy)
{
    Matrix newm;
    for (int i=0;i<16;i++)
        newm.m[i]=0.0;
    for (int i=0;i<16;i+=5)
        newm.m[i]=1.0;
    return mtransform(newm, x, y, r, xs, ys, ox, oy);
}
Matrix OGLplotter::multiplyMatrices(Matrix m1, Matrix m2)
{
    Matrix res;
    for (int i=0;i<16;i++)
        res.m[i]=0;
    for(int i = 0; i < 4; ++i)
        for(int j = 0; j < 4; ++j)
            for(int k = 0; k < 4; ++k)
            {
                res.m[i*4+j] += m1.m[i*4+k] * m2.m[k*4+j];
            }
    return res;
}
vertex OGLplotter::vtxtransform(vertex v, double x, double y, double r, double xs, double ys, double ox, double oy)
{
    double tfm[9];
    double ac=3.1415926535/180, nx, ny;
    v.x+=x+ox;
    v.y+=y+oy;

    v.x*=xs;
    v.y*=ys;

    tfm[0] = cos(r*ac) ;	tfm[1] = -sin(r*ac) ;
    tfm[3] = sin(r*ac) ;	tfm[4] = cos(r*ac) ;
    nx = v.x, ny = v.y;
    v.x = tfm[0]*nx + tfm[1]*ny;
    v.y = tfm[3]*nx + tfm[4]*ny;

    v.x-=ox;
    v.y-=oy;
    return v;
}

vertex OGLplotter::applyTransformationHierarchyToVertex(vertex v, matrixData * md)
{
    double rot=0.0, xs=1.0, ys=1.0, xo=0.0, yo=0.0,x=0.0,y=0.0;
    shape * mat;
    formulaData * xpos;
    formulaData * ypos;
    formulaData * xorigin;
    formulaData * yorigin;
    formulaData * rotation;
    formulaData * xscale;
    formulaData * yscale;
    while (md!=nullptr)
    {
        mat = md->getData();
        if (mat==nullptr)
            break;
        if (mat->getType()!=shape::matrix)
            break;
        rot=0.0; xs=1.0; ys=1.0; xo=0.0; yo=0.0;x=0.0;y=0.0;
        xpos = (formulaData *) mat->getPropertyByName("xpos");
        ypos = (formulaData *) mat->getPropertyByName("ypos");
        xorigin = (formulaData *)mat->getPropertyByName("xorigin");
        yorigin = (formulaData *)mat->getPropertyByName("yorigin");
        rotation = (formulaData *)mat->getPropertyByName("rotation");
        xscale = (formulaData *)mat->getPropertyByName("xscale");
        yscale = (formulaData *)mat->getPropertyByName("yscale");
        if(xpos!=nullptr&&ypos!=nullptr)
            {
            x=f->parse(xpos);
            y=f->parse(ypos);
            }
        if(xorigin!=nullptr&&yorigin!=nullptr)
            {
            xo=f->parse(xorigin);
            yo=f->parse(yorigin);
            }
        if(rotation!=nullptr)
            rot=f->parse(rotation);
        if(xscale!=nullptr&&yscale!=nullptr)
            {
            xs=f->parse(xscale);
            ys=f->parse(yscale);
            }
        double tfm[6];
        double ac=3.1415926535/180, nx, ny;
        v.x+=x+xo;
        v.y+=y+yo;
        tfm[0] = cos(rot*ac) ;	tfm[1] = -sin(rot*ac) ;
        tfm[3] = sin(rot*ac) ;	tfm[4] = cos(rot*ac) ;
        nx = v.x, ny = v.y;
        v.x = tfm[0]*nx + tfm[1]*ny;
        v.y = tfm[3]*nx + tfm[4]*ny;
        v.x*=xs;
        v.y*=ys;
        v.x-=xo;
        v.y-=yo;
        md = (matrixData *)mat->getPropertyByName("tfmatrix");
    }
    return v;
}


vertex OGLplotter::applyTransformationHierarchyToVertexNoFirst(vertex v, matrixData * md)
{
    double rot=0.0, xs=1.0, ys=1.0, xo=0.0, yo=0.0,x=0.0,y=0.0;
    shape * mat;
    if (md==nullptr) return v;
    mat = md->getData();
    if (mat==nullptr) return v;
    if (mat->getType()!=shape::matrix) return v;
    md = (matrixData *)mat->getPropertyByName("tfmatrix");
    formulaData * xpos;
    formulaData * ypos;
    formulaData * xorigin;
    formulaData * yorigin;
    formulaData * rotation;
    formulaData * xscale;
    formulaData * yscale;
    while (md!=nullptr)
    {
        mat = md->getData();
        if (mat==nullptr)
            break;
        if (mat->getType()!=shape::matrix)
            break;
        rot=0.0; xs=1.0; ys=1.0; xo=0.0; yo=0.0;x=0.0;y=0.0;
        xpos = (formulaData *) mat->getPropertyByName("xpos");
        ypos = (formulaData *) mat->getPropertyByName("ypos");
        xorigin = (formulaData *)mat->getPropertyByName("xorigin");
        yorigin = (formulaData *)mat->getPropertyByName("yorigin");
        rotation = (formulaData *)mat->getPropertyByName("rotation");
        xscale = (formulaData *)mat->getPropertyByName("xscale");
        yscale = (formulaData *)mat->getPropertyByName("yscale");
        if(xpos!=nullptr&&ypos!=nullptr)
            {
            x=f->parse(xpos);
            y=f->parse(ypos);
            }
        if(xorigin!=nullptr&&yorigin!=nullptr)
            {
            xo=f->parse(xorigin);
            yo=f->parse(yorigin);
            }
        if(rotation!=nullptr)
            rot=f->parse(rotation);
        if(xscale!=nullptr&&yscale!=nullptr)
            {
            xs=f->parse(xscale);
            ys=f->parse(yscale);
            }
        double tfm[6];
        double ac=3.1415926535/180, nx, ny;
        v.x+=x+xo;
        v.y+=y+yo;
        tfm[0] = cos(rot*ac) ;	tfm[1] = -sin(rot*ac) ;
        tfm[3] = sin(rot*ac) ;	tfm[4] = cos(rot*ac) ;
        nx = v.x, ny = v.y;
        v.x = tfm[0]*nx + tfm[1]*ny;
        v.y = tfm[3]*nx + tfm[4]*ny;
        v.x*=xs;
        v.y*=ys;
        v.x-=xo;
        v.y-=yo;
        md = (matrixData *)mat->getPropertyByName("tfmatrix");
    }
    return v;
}


void OGLplotter::oglLine(std::vector<vertex> points, double width)
{
    oglLine(points, width, false);
}

void OGLplotter::oglLine(std::vector<vertex> points, double width, bool loop)
{
    double r=points[0].r,b=points[0].b,g=points[0].g,a=points[0].a;
    int s = points.size();
    if (s<2) return;
    std::vector<vertex> todraw, temp;
    if (s==2)
    {
        todraw = linecon(0, 0, points[0].x, points[0].y, points[1].x, points[1].y,width,r,g,b,a,true, false, 0,0);
        temp = linecon(points[0].x, points[0].y, points[1].x, points[1].y, 0, 0,width,r,g,b,a,false, true,
                       todraw[0].x,todraw[0].y);
        todraw.insert(todraw.end(), temp.begin(), temp.end());
    }
    else if (!loop)
    {
        todraw = linecon(0, 0, points[0].x, points[0].y, points[1].x, points[1].y,width,r,g,b,a,true, false, 0,0);
        for(int i=1;i<s-1;i++)
        {
            temp = linecon(points[i-1].x, points[i-1].y,
                    points[i].x, points[i].y,
                    points[i+1].x, points[i+1].y,
                    width,r,g,b,a,false,false,
                    todraw[2*i-1].x,todraw[2*i-1].y);
            todraw.insert(todraw.end(), temp.begin(), temp.end());
        }
        temp = linecon(points[s-2].x, points[s-2].y,points[s-1].x, points[s-1].y,0, 0,
                    width,r,g,b,a,false,true, todraw[s*2-3].x,todraw[s*2-3].y);
        todraw.insert(todraw.end(), temp.begin(), temp.end());
    }
    else
    {
        temp = linecon(points[s-1].x, points[s-1].y,
                points[0].x, points[0].y,
                points[1].x, points[1].y,
                width,r,g,b,a,false,false, 0, 0);
        todraw.insert(todraw.end(), temp.begin(), temp.end());
        for(int i=1;i<s-1;i++)
        {
            temp = linecon(points[i-1].x, points[i-1].y,
                    points[i].x, points[i].y,
                    points[i+1].x, points[i+1].y,
                    width,r,g,b,a,false,false,
                    todraw[2*i-1].x,todraw[2*i-1].y);
            todraw.insert(todraw.end(), temp.begin(), temp.end());
        }
        temp = linecon(points[s-2].x, points[s-2].y,
                points[s-1].x, points[s-1].y,
                points[0].x, points[0].y,
                width,r,g,b,a,false,false,
                todraw[s*2-3].x,todraw[s*2-3].y);
        todraw.insert(todraw.end(), temp.begin(), temp.end());
        temp = linecon(points[s-1].x, points[s-1].y,
                points[0].x, points[0].y,
                points[1].x, points[1].y,
                width,r,g,b,a,false,false,
                todraw[s*2-1].x,todraw[s*2-1].y);
        todraw.insert(todraw.end(), temp.begin(), temp.end());
    }
    ogldraw(GL_TRIANGLE_STRIP,todraw);
}

void OGLplotter::resetView()
{
    resetView(true);
}
void OGLplotter::resetView(bool refreshq)
{
    scale=1.0;
    centx=centy=oldcentx=oldcenty=xorig=yorig=0;
    if (owner!=nullptr)
    {
        owner->getUIAPI()->setStatusBarString("Zoom: "+roundto2(100/scale)+"%", 0);
        owner->getUIAPI()->setStatusBarString("View x: "+roundto2(centx)+"", 1);
        owner->getUIAPI()->setStatusBarString("View y: "+roundto2(centy)+"", 2);
    }
    //statusBar->SetStatusText(sk,3);
    //owner->getUIAPI()->setStatusBarString("Mouse x: "+roundto2(xx)+"", 4);
    //owner->getUIAPI()->setStatusBarString("Mouse y: "+roundto2(yy)+"", 5);
    itIsDragged=false;
    if (refreshq)
        refresh();
}

void OGLplotter::mouseWheelMoved(wxMouseEvent& event)
{
    if (event.GetWheelRotation()>0&&scale>0.01)
        scale*=0.9;
    if (scale<100&&event.GetWheelRotation()<0)
        scale*=1.1;
    owner->getUIAPI()->setStatusBarString("Zoom: "+roundto2(100/scale)+"%", 0);
    refresh();
}
void OGLplotter::motion(long x, long y)
{
    SetFocus();
    {
        int ww, hh;
        this->GetClientSize(&ww, &hh);
        long xx=0, yy=0;
        xx = centx + (- ww/2 + x)*scale;
        yy = centy + (- hh/2 + y)*scale;
        owner->getUIAPI()->setStatusBarString("Zoom: "+roundto2(100/scale)+"%", 0);
        owner->getUIAPI()->setStatusBarString("View x: "+roundto2(centx)+"", 1);
        owner->getUIAPI()->setStatusBarString("View y: "+roundto2(centy)+"", 2);
        //statusBar->SetStatusText(sk,3);
        owner->getUIAPI()->setStatusBarString("Mouse x: "+roundto2(xx)+"", 4);
        owner->getUIAPI()->setStatusBarString("Mouse y: "+roundto2(yy)+"", 5);
    }
    if (itIsDragged)
    {
        centx=oldcentx+(xorig-x)*scale;
        centy=oldcenty+(yorig-y)*scale;
        refresh();
    }
}
std::string OGLplotter::roundto2(double n)
{
    std::stringstream stream;
    stream << std::fixed << std::setprecision(2) << n;
    return stream.str();
}
void OGLplotter::mouseMoved(wxMouseEvent& event) /// not actually used iirc
{
    long px=0,py=0;
    event.GetPosition(&px,&py);

    int ww, hh;
    this->GetClientSize(&ww, &hh);

    long xx=0, yy=0;
    xx = centx + (- ww/2 + px)*scale;
    yy = centy + (- hh/2 + py)*scale;
    if (itIsDragged)
    {
        centx=oldcentx+(xorig-px)*scale;
        centy=oldcenty+(yorig-py)*scale;
        refresh();
    }

}
bool OGLplotter::isItOnScreen(double x, double y, double w, double h)
{
    int ww, hh;
    this->GetClientSize(&ww, &hh);
    double sw, sh;
    sw = (double)ww;
    sh = (double)hh;

    if (x> centx + (sw/2.0)*scale) return false;
    if (y> centy + (sh/2.0)*scale) return false;
    if (x+w< centx - (sw/2.0)*scale) return false;
    if (y+h< centy - (sh/2.0)*scale) return false;
    if (w < 4 && h < 4) return false;
    return true;
}
//if (!isItOnScreenMinMax(testXmin, testYmin, testXmax, testYmax)
bool OGLplotter::isItOnScreenMinMax(double x1, double y1, double x2, double y2)
{
    int ww, hh;
    this->GetClientSize(&ww, &hh);
    double sw, sh;
    sw = (double)ww;
    sh = (double)hh;

    if (x1> centx + (sw/2.0)*scale) return false;
    if (y1> centy + (sh/2.0)*scale) return false;
    if (x2< centx - (sw/2.0)*scale) return false;
    if (y2< centy - (sh/2.0)*scale) return false;
    if (abs(x1-x2)<4 && abs(y1-y2)<4) return false;
    return true;
}
void OGLplotter::mouseDown(wxMouseEvent& event)
{
    SetFocus();
    if (event.LeftDown())
        {
        long px=0,py=0;
        event.GetPosition(&px,&py);
        int ww, hh;
        this->GetClientSize(&ww, &hh);

        long xx=0, yy=0;
        xx = centx + (- ww/2 + px)*scale;
        yy = centy + (- hh/2 + py)*scale;

        if (nodes!=nullptr)
        {
            branch * br = nodes->getRootBranch();
            if (br!=nullptr)
            {
                br = br->getClickedBranch(xx,yy);
                owner->getUIAPI()->buildRHSUIForBranch(br);
            }
        }
        itIsDragged=true;
        oldcentx=centx;
        oldcenty=centy;
        xorig=px;
        yorig=py;
        }
    else if (event.RightDown())
    if (nodes!=nullptr)
        {
        long px=0,py=0;
        event.GetPosition(&px,&py);
        int ww, hh;
        this->GetClientSize(&ww, &hh);

        long xx=0, yy=0;
        xx = centx + (- ww/2 + px)*scale;
        yy = centy + (- hh/2 + py)*scale;

        branch * br = nodes->getRootBranch();
        if (br!=nullptr)
            {
            br = br->getClickedBranch(xx,yy);
            owner->getUIAPI()->setSecondaryBranch(br);
            }
        }
}
void OGLplotter::mouseReleased(wxMouseEvent& event)
{
    if (event.LeftUp())
    {
    itIsDragged=false;
    }
}
double OGLplotter::GET_ABS(double x) {return x>0?x:-x;}

std::vector<vertex> OGLplotter::linecon(double x0, double y0, double x1, double y1, double x2, double y2,
            double w,double Cr, double Cg, double Cb, double a, bool begining, bool ending, double px, double py)
{
    std::vector<vertex> p;
    if (begining && ending)
        return p;
    if (begining)
    {
        x0 = 2*x1 - x2;
        y0 = 2*y1 - y2;
    }
    if (ending)
    {
        x2 = 2*x1 - x0;
        y2 = 2*y1 - y0;
    }

    double r01, r12, r20; //sides of the triangle

    r01 = sqrt( (x0-x1)*(x0-x1) + (y0-y1)*(y0-y1) );
    r12 = sqrt( (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) );
    r20 = sqrt( (x0-x2)*(x0-x2) + (y0-y2)*(y0-y2) );

    double rr01, rr12, rr20; //squares of the sides

    rr01 = r01*r01;
    rr12 = r12*r12;
    rr20 = r20*r20;

    double s = (r01+r12+r20)/2; // the semiperimeter;

    //now calculate the distance from point 0 to where the bisector of angle 1 crosses the line 0 to 2
    double z = (rr01+rr20-rr12)/(2*r01*r20); // a temporary value
    double rx = 2*sqrt(
                       ( r01*r12*s*(s-r20) )*( 0.5 + (rr20-rr01-rr12)/(4*r01*r12) )
                       /(1 - z*z)
                       )/(r01+r12);
    double o = rx/r20; // the ratio in which 1's bisector cuts the line from point 0 to point 2
    double xs = (1-o)*x0+o*x2;//coordinates where the cut happens
    double ys = (1-o)*y0+o*y2;
    double ds1 = sqrt( (xs-x1)*(xs-x1) + (ys-y1)*(ys-y1) );//distance between that point and point 1
    double dxs1 = (xs - x1)/ds1;//the unit vector of the bisector
    double dys1 = (ys - y1)/ds1;
    double tx = w*dys1/2;//the unit vector times the width of the line we want
    double ty = w*dxs1/2;
    double testx = x1-ty,testy = y1-tx;//need this to test if the points zigzag along the polygon

    //if the new line crosses the old one, flip the sides
    if ((((x0-px)*(testy-py) - (y0-py)*(testx-px))*((x1   -px)*(testy-py) - (y1   -py)*(testx-px)) < 0)&&
        (((px-x0)*(y1   -y0) - (py-y0)*(x1   -x0))*((testx-x0)*(y1   -y0) - (testy-y0)*(x1   -x0)) < 0))
    {
        tx*=-1;
        ty*=-1;
    }
    p.push_back(vertex(x1+ty, y1+tx, Cr,Cg,Cb,a));
    p.push_back(vertex(x1-ty, y1-tx, Cr,Cg,Cb,a));
    return p;
}

void OGLplotter::line_raw( double x1, double y1, double x2, double y2,
	double w,
	double Cr, double Cg, double Cb)
{
    std::vector<vertex> p;
	p.push_back(vertex(x1,y1,Cr,Cg,Cb));
	p.push_back(vertex(x2,y2,Cr,Cg,Cb));
	oglLine(p,w);
}
void OGLplotter::line_raw( double x1, double y1, double x2, double y2,
            double Cr, double Cg, double Cb)
{
    std::vector<vertex> p;
	p.push_back(vertex(x1,y1,Cr,Cg,Cb));
	p.push_back(vertex(x2,y2,Cr,Cg,Cb));
	oglLine(p,1);
}
void OGLplotter::line_raw( double x1, double y1, double x2, double y2)
{
    std::vector<vertex> p;
	p.push_back(vertex(x1,y1,0,0,0));
	p.push_back(vertex(x2,y2,0,0,0));
	oglLine(p,1);
}






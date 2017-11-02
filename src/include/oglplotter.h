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



#ifndef OGLPLOTTER_H
#define OGLPLOTTER_H

#include <wx/wx.h>
#include <wx/sizer.h>
#include <time.h>
#include <drawingapi.h>
#include <composite.h>
#include <shape.h>
#include <dataholder.h>
#include "alldatatypes.h"
#include <colorconv.h>
#include <formulaparser.h>
#include <vector>
#include <map>
#include <wx/settings.h>
#include <wx/font.h>
#include <wx/glcanvas.h>
#include <string>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <SOIL.h>
#include <exception>
#include <iomanip> // setprecision
#include <sstream> // stringstream
#include <iostream> // for diagnostic purposes
//#include <../ftgl/FTGL/ftgl.h>

/*Creating code
<resource>
    <object class="Custom" name="ID_OGLPLOTTER1" subclass="OGLplotter" variable="oglplotter" member="yes">
        <creating_code>$(THIS) = new $(CLASS)($(PARENT),$(ID),$(POS),$(SIZE),0);</creating_code>
        <style></style>
        <enabled>0</enabled>
        <hidden>1</hidden>
        <handler function="Onplotter1MouseMove" entry="EVT_MOTION" />
        <handler function="OnResize" entry="EVT_SIZE" />
    </object>
</resource>
*/
// default: $(THIS) = new $(CLASS)($(PARENT),$(ID),$(POS),$(SIZE),$(STYLE),wxDefaultValidator,$(NAME));



struct Vertex
{
    float x, y, r, g, b, a;
    Vertex (double xx, double yy, double rr, double gg, double bb, double aa):
        x(xx), y(yy), r(rr), g(gg), b(bb), a(aa){}
    Vertex (double xx, double yy, double rr, double gg, double bb):
        x(xx), y(yy), r(rr), g(gg), b(bb), a(1.0){}
    Vertex (double xx, double yy):
        x(xx), y(yy), r(0.0), g(0.0), b(0.0), a(1.0){}
    Vertex (double xx, double yy, colorconv col):
        x(xx), y(yy)
    {
        r = col.oglr();
        g = col.oglg();
        b = col.oglb();
        a = col.ogla();
    }
};


struct transformation
{
    float x, y, r, sx, sy, ox, oy;
    transformation()
    {
        x=y=r=ox=oy=0.0;
        sx=sy=1.0;
    }
};

struct texture
{
    GLuint t;
    int w, h;
    unsigned char * i;
    texture (GLuint t_, int w_, int h_):t(t_), w(w_), h(h_), i(nullptr){}
    texture (unsigned char * i_, int w_, int h_):t(0), w(w_), h(h_), i(i_){}
    texture ():t(0), w(1), h(1), i(nullptr){}
};

typedef struct Vertex vertex;

class styleframe;
class wxGLAttributes;

class OGLplotter : public wxGLCanvas, public drawingAPI
{
    public:
        OGLplotter(wxWindow* parent);
        ~OGLplotter();
        void redraw();
        void refresh();
        void updatesize();
        void resetView(bool refreshq);
        void resetView();
        void checksize();
        void mouseMoved(wxMouseEvent& event);
        void mouseDown(wxMouseEvent& event);
        void mouseWheelMoved(wxMouseEvent& event);
        void mouseReleased(wxMouseEvent& event);
        void motion(long x, long y);
        void drawForBranch(branch * br);

        void createFormulaParser(bool actualGlobalVariables);
        void updateFormulaParserDataHolder();

        void setDrawableCoordinates(drawable * drawn,Matrix Transform,bool globvar=true);

        void Paintit(wxPaintEvent& event);
        std::string roundto2(double n);
        bool isItOnScreen(double x, double y, double w, double h);
        bool isItOnScreenMinMax(double x1, double y1, double x2, double y2);
    private:

        float ttx, tty;
        double textw, texth;
        wxGLContext * wglc;
        composite * drawnComposite;
        dataPoint * getSurfaceDataPoint(dataPoint * ofThisOne);

        void drawDrawable(drawable * drawn,Matrix Transform,bool globvar=true);
        void Render();
        void OnPaint(wxPaintEvent& event);
        void OnKeyDown(wxKeyEvent& event);
        void applyTransformationHierarchy(matrixData * md);
        vertex applyTransformationHierarchyToVertex(vertex v, matrixData * md);
        vertex applyTransformationHierarchyToVertexNoFirst(vertex v, matrixData * md);
        void ogltransform(double x, double y, double r, double xs, double ys, double ox, double oy);
        vertex vtxtransform(vertex v, double x, double y, double r, double xs, double ys, double ox, double oy);
        Matrix mtransform(Matrix m, double x, double y, double r, double xs, double ys, double ox, double oy);
        Matrix applyTransformationHierarchyToMatrix(Matrix m, matrixData * md);
        void oglMtransform(Matrix m);
        vertex vtxMtransform(Matrix m, vertex v);
        Matrix multiplyMatrices(Matrix m1, Matrix m2);
        Matrix transformationToMatrix(double x, double y, double r, double xs, double ys, double ox, double oy);
        void oglLine(std::vector<vertex> points, double width, bool loop);
        void oglLine(std::vector<vertex> points, double width);

        void ogldraw(GLenum mode, std::vector<vertex> points);
        void oglImage(texture gluimage, colorconv blend);
        void oglText(std::string text, colorconv blend);

        void oglTextSetWH(std::string text, colorconv blend);
        void calcCoordsForBranch(branch * br);

        bool FTInit, FTFaceInit, ILInit;
        FT_Library FTLib;
        //FTGLPixmapFont FTFace{"sa"}, currentFace{"sh"}, initFace{"e"};
        FT_Face FTFace, currentFace, initFace;
        branch * brdata;

        GLuint vbo1;
        GLuint plainvbo, imvbo, textvbo;
        //GLuint tex;
        bool glgenflag, glgentexflag;
        bool pvflag, ivflag, tvflag;

        std::map<std::string, FT_Face> ftfaces;
        //std::map<std::string, FTGLPixmapFont> ftfaces;
        //std::map<std::string, FTGLPixmapFont*> ftfaces;
        std::map<std::string, bool> ftfacesinit;

        void initARBs();
        void identity();
        void setView();

        double scale, centx, centy, oldcentx, oldcenty, xorig, yorig;
        bool itIsDragged, clientStatesAreSet;
        bool drawsPrimaryBranch, drawsSecondaryBranch;
        void enableClientStates();

        std::map<std::string, texture> textures;
        std::map<std::string, int> ws, hs;

        PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
        PFNGLBINDVERTEXARRAYPROC glBindVertexArray;

        PFNGLBINDVERTEXARRAYAPPLEPROC glBindVertexArrayAPPLE;
        PFNGLGENVERTEXARRAYSAPPLEPROC glGenVertexArraysAPPLE;
        PFNGLGENBUFFERSARBPROC glGenBuffersARB;
        PFNGLBINDBUFFERARBPROC glBindBufferARB;
        PFNGLBUFFERDATAARBPROC glBufferDataARB;
        PFNGLENABLEVERTEXATTRIBARRAYARBPROC glEnableVertexAttribArrayARB;
        PFNGLVERTEXATTRIBPOINTERARBPROC glVertexAttribPointerARB;
        PFNGLDRAWARRAYSEXTPROC glDrawArraysEXT;
        PFNGLVERTEXPOINTEREXTPROC glVertexPointerEXT;
        PFNGLENABLECLIENTSTATEIEXTPROC glEnableClientStateiEXT;

        bool run;

        double GET_ABS(double x);

        std::vector<vertex> linecon(double x0, double y0, double x1, double y1, double x2, double y2,
            double w,double Cr, double Cg, double Cb, double a, bool begining, bool ending, double px, double py);

        void line_raw( double x1, double y1, double x2, double y2,
            double w,
            double Cr, double Cg, double Cb);
        void line_raw( double x1, double y1, double x2, double y2,
            double Cr, double Cg, double Cb);
        void line_raw( double x1, double y1, double x2, double y2);
        DECLARE_EVENT_TABLE()

};

#endif // OGLPLOTTER_H

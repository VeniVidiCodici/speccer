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



#include "datamodel.h"
#include "dataholder.h"
#include "uiapi.h"
#include "drawingapi.h"

dataModel::dataModel(UIAPI * u, drawingAPI * j)
{
    uapi = u;
    japi = j;
    styleName="";
    drawables["circle"] = new shape("circle",shape::circle);
    drawables["arc"] = new shape("arc",shape::arc);
    drawables["rectangle"] = new shape("rectangle",shape::rectangle);
    drawables["roundrect"] = new shape("roundrect",shape::roundrect);
    drawables["convex"] = new shape("convex",shape::convex);
    drawables["points"] = new shape("points",shape::points);
    drawables["image"] = new shape("image",shape::image);
    drawables["text"] = new shape("text",shape::text);
    drawables["matrix"] = new shape("matrix",shape::matrix);
    drawables["origin"] = new shape("origin", shape::root);
    drawables["branching point"] = new shape("branching point", shape::brpoint);
    drawables["bounding box"] = new shape("bounding box", shape::bbox);
    drawables["anchor"] = new shape("anchor", shape::anchor);
    loadCompositeShapes();
    firstTimeShowing = true;
    currentNode = nullptr;
}

dataModel::~dataModel()
{
    for (int i=0;i<nodes.size();i++)
        delete nodes[i];
    for (int i=0;i<composites.size();i++)
        delete composites[i];
    for (std::map<std::string, drawable *>::iterator it=drawables.begin(); it!=drawables.end(); ++it)
        delete it->second;
}
void dataModel::loadCompositeShapes()
{
    std::string filename,dir;
    std::vector<std::string> files;
    dir = "composites/";
    files = uapi->getAllFilesInFolder(dir,"comp");
    for(int i=0;i<files.size();i++)
        {
        importComposite(files[i]);
        }
}
void dataModel::importComposite(std::string path)
{
    composite * newC = new composite(path);
    std::ifstream ifs(path, std::ios::binary);
    std::string version="none", isize;
    version = newC->rs(ifs);
    isize = newC->rs(ifs);
    int isz = (int)strtod(isize.c_str(), NULL);
    newC->set_intsize(isz);

    std::string serror = newC->load(ifs, version);
    ifs.close();
    if (serror!="")
    {
        uapi->prompt("Error loading "+path+": "+serror);
        delete newC;
    }
    else
    {
        newC->loadSurfaceDataPoints();
        std::string fname=path;
        std::size_t found = fname.find_last_of("/\\");
        if (found!=std::string::npos)
        {
            fname = fname.substr(found+1);
        }
        found = fname.find_last_of(".");
        if (found!=std::string::npos)
        {
            fname = fname.substr(0,found);
        }
        newC->setName(fname);
        drawables[newC->getName()] = newC;
    }
}

dataHolder * dataModel::getNodeByName(std::string dname)
{
    for(int i=0;i<nodes.size();i++)
    {
        if (nodes[i]->getName()==dname)
            return nodes[i];
    }
    return nullptr;
}

drawable * dataModel::getDrawableByName(std::string name)
{
    if(drawables.find(name) != drawables.end())
        return drawables[name];
    else
        return nullptr;
}
bool dataModel::drawableExists(std::string name)
{
    if(drawables.find(name) != drawables.end())
        return true;
    else
        return false;
}
std::vector<drawable *> dataModel::getAllDrawables()
{
    std::vector<drawable *> temp;
    for (std::map<std::string, drawable *>::iterator it=drawables.begin(); it!=drawables.end(); ++it)
        temp.push_back(it->second);
    return temp;
}

bool dataModel::saveAsStyle(std::string filename)
{
    return true;
}
bool dataModel::openStyle(std::string filename)
{
    styleName = filename; /// temporary solution
    std::ifstream ifs(filename, std::ios::binary);
    std::string version="none", isize;
    version = rs(ifs);
    isize =rs(ifs);
    int isz = (int)strtod(isize.c_str(), NULL);
    set_intsize(isz);

    std::string serror = load(ifs, version);
    ifs.close();
    if (serror!="")
        uapi->prompt(serror);
    return true;
}
std::string dataModel::getName()
{
    return styleName;
}
bool dataModel::saveStyle(std::string filename) /// obsolete iirc
{
    return true;
}
dataHolder * dataModel::getCurrentNode()
{
    return currentNode;
}
void dataModel::removeAllNodes()
{
    for(int i=0;i<nodes.size();i++)
        if (nodes[i]!=currentNode) /// find a solution to delete it too
            delete nodes[i];
    nodes.clear();
}
bool dataModel::nodeExists(std::string name)
{
    for(int i=0;i<nodes.size();i++)
        if(nodes[i]->getName()==name)
            return true;
    return false;
}

bool dataModel::compositeExists(std::string name)
{
    for(int i=0;i<composites.size();i++)
        if(composites[i]->getName()==name)
            return true;
    return false;
}

dataHolder * dataModel::addANewNode(std::string name, bool switchToIt)
{
    if (nodeExists(name))
        return nullptr;
    int m = uapi->getMode(); //cheap quick fix
    uapi->setMode(0);
    dataHolder * dh = new dataHolder(uapi, japi, this);
    dh->setName(name);
    dh->addNewDrawableFromModel("origin");
    dh->addNewDrawableFromModel("anchor");
    dh->addNewDrawableFromModel("branching point");
    dh->addNewDrawableFromModel("bounding box");
    nodes.push_back(dh);
    uapi->addDataHolderToStylesMenu(dh);
    uapi->setMode(m);
    if (switchToIt)
        {
            uapi->setMode(0);
            switchToDataHolder(dh);
        }
    return dh;
}
dataHolder * dataModel::addANewComposite(std::string name, bool switchToIt)
{
    if (compositeExists(name))
        return nullptr;
    int m = uapi->getMode(); //cheap quick fix
    uapi->setMode(1);
    dataHolder * dh = new dataHolder(uapi, japi, this);
    dh->setName(name);
    composites.push_back(dh);
    uapi->addDataHolderToCompositesMenu(dh);
    uapi->setMode(m);
    if (switchToIt)
        {
            uapi->setMode(1);
            switchToDataHolder(dh);
        }
    return dh;
}
std::vector<dataHolder *> dataModel::getNodes()
{
    return nodes;
}
bool dataModel::switchToDataHolder(dataHolder * toThisOne)
{
    if (toThisOne==nullptr)
        return false;
    currentNode = toThisOne;
    uapi->setTitle(toThisOne->getName());
    uapi->setDataHolder(toThisOne);
    japi->setDataHolder(toThisOne);
    japi->updateFormulaParserDataHolder();
    if (!firstTimeShowing)
        japi->refresh();
    uapi->updateLHSUIPlacedShapes();
    uapi->clearRHSUI();
    uapi->clearLHSUIV();
    toThisOne->redraw();
    toThisOne->refreshFieldsFGS();
    firstTimeShowing = false;
    return true;
}
void dataModel::setAPIs(UIAPI * u, drawingAPI * j)
{
    uapi = u;
    japi = j;
}


std::string dataModel::save(std::ofstream& _out)
{
    /*
    things to save:
    1. std::vector<dataHolder *> nodes;
    2. Note: the name is not saved. The name is assumed to be the name of the file sans the extension.
        2.1. THe name variable n functionality to be removed later.
    */
    wi(_out,nodes.size());
    std::string error;
    for (dataHolder * obj : nodes)
        {
        error = obj->save(_out);
        if (error!="")
            return error;
        }
    return "";
}
std::string dataModel::load(std::ifstream& _in, std::string version)
{
    if (version == "1.0.0")
    {
        int s;
        s = ri(_in);
        for(int i=0;i<nodes.size();i++)
            if (nodes[i]!=nullptr)
                {
                delete nodes[i];
                }
        nodes.clear();
        uapi->resetStylesMenu();
        for (int i=0;i<s;i++)
        {
            dataHolder * dh = new dataHolder(uapi, japi, this);
            dh->set_intsize(_intsize);
            std::string error = dh->load(_in, version);
            if (error!="")
                return error;
            nodes.push_back(dh);
            uapi->addDataHolderToStylesMenu(dh);
        }
        if (nodes.size()>0)
            switchToDataHolder(nodes[0]);
    }
    else
        return "Unsupported program version: "+version;
    return "";
}

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



#include "tree.h"
#include "uiapi.h"
#include "drawingapi.h"
#include "branch.h"

tree::tree(UIAPI * u, drawingAPI * j)
{
    uapi = u;
    japi = j;

    rootBranch = new branch(1, this);

    uapi->setWMode(UIAPI::browser);
    japi->setNodes(this);
    japi->setWMode(drawingAPI::browser);

    std::string dir;
    std::vector<std::string> files;
    dir = "styles/";
    files = uapi->getAllFilesInFolder(dir,"styl");
    for(int i=0;i<files.size();i++)
        {
        dataModel * dm = new dataModel(u, j);
        styles.push_back(dm);
        styles[i]->openStyle(files[i]);
        }
    if (styles.size()==0)
    {
        styles.push_back(new dataModel(u, j));
        uapi->prompt("Error: no styles found");
    }
    currentStyle = styles[0];
}

tree::~tree()
{
    //dtor
}
dataModel * tree::getCurrentStyle()
{
    return currentStyle;
}
void tree::setCurrentStyleByName(std::string sname)
{
    for(int i=0;i<styles.size();i++)
        if (styles[i]->getName()==sname)
            currentStyle = styles[i];
}

std::vector<std::string> tree::getModelNames()
{
    std::vector<std::string> str;
    for(int i=0;i<styles.size();i++)
        str.push_back(styles[i]->getName());
    return str;
}
branch * tree::getRootBranch()
{
    return rootBranch;
}
bool tree::setRootBranch(branch * rb)
{
    if (rb==nullptr)
        return false;
    rootBranch = rb;
    return true;
}

void tree::setFilepath(std::string newfilepath)
{
    filepath = newfilepath;
}
std::string tree::getFilepath()
{
    return filepath;
}
void tree::setSaved(bool s)
{
    saved = s;
}
bool tree::getSaved()
{
    return saved;
}
void tree::setChanged(bool c)
{
    changed = c;
}
bool tree::getChanged()
{
    return changed;
}

std::string tree::save(std::ofstream& _out)
{
    rootBranch->massSetSaved(false);
    std::string sc = rootBranch->save(_out);
    return sc;
}
std::string tree::load(std::ifstream& _in, std::string version)
{
    if (version == "1.0.0")
    {
        rootBranch->uproot();
        rootBranch = new branch(1, this);
        rootBranch->set_intsize(_intsize);
        std::string sc = rootBranch->load(_in, version);
        return sc;
    }
    else
        return "Unsupported program version: "+version;
    return "";
}

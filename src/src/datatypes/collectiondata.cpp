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



#include "collectiondata.h"

collectionData::collectionData(std::string _name,std::vector<vertInfo> value, std::string _prompt)
{
    prompt = _prompt;
    kind=collection;
    data=value;
    name=_name;
    constant=false;
    property=false;
}

collectionData::~collectionData()
{
    //dtor
}

void collectionData::setData(std::vector<vertInfo> value)
{
    data = value;
}

std::vector<vertInfo> collectionData::getData()
{
    return data;
}
void collectionData::updateDependency()
{
    data = ((collectionData*)dependedOn)->data;
}
void collectionData::duplicate()
{
    duplicated = new collectionData(name, data, prompt);
    duplicated -> setConstant(constant);
    duplicated -> setProperty(property);
    duplicated -> setSurfaceName(surfaceName);
    if (dependedOn==nullptr)
        duplicated->setDependantOn(nullptr);
    else
        duplicated -> setDependantOn(dependedOn->getDuplicated());
}
void collectionData::addToData(vertInfo vi)
{
    data.push_back(vi);
}
void collectionData::removeFromData(int index)
{
    if(index<data.size()&&index>=0)
        data.erase(data.begin()+index);
}

void collectionData::setDataAt(vertInfo value, int index)
{
    if(index<data.size()&&index>=0)
        data[index]=value;
}
vertInfo collectionData::getDataAt(int index)
{
    if(index<data.size()&&index>=0)
        return data[index];
    else
        return vertInfo{"Error: Index out of bounds.","Error: Index out of bounds.",colorconv(255,0,0,255)};
}

std::string collectionData::save(std::ofstream& _out)
{
    ws(_out,typeToString(kind));
    ws(_out, name);
    ws(_out, prompt);
    ws(_out, surfaceName);
    wi(_out, data.size());
    for (int i=0; i <data.size();i++)
    {
        ws(_out, data[i].x);
        ws(_out, data[i].y);
        wi(_out, data[i].col.getr());
        wi(_out, data[i].col.getg());
        wi(_out, data[i].col.getb());
        wi(_out, data[i].col.geta());
    }
    return "";
}
std::string collectionData::load(std::ifstream& _in, std::string version)
{
    if (version == "1.0.0")
    {
        int s, r, g, b, a;
        std::string x, y;
        name = rs(_in);
        prompt = rs(_in);
        surfaceName = rs(_in);
        s = ri(_in);
        for (int i=0; i<s; i++)
        {
            x = rs(_in);
            y = rs(_in);
            r = ri(_in);
            g = ri(_in);
            b = ri(_in);
            a = ri(_in);
            data.push_back(vertInfo{x,y,colorconv(r,g,b,a)});
        }
    }
    else
        return "Unsupported program version.";
    return "";
}




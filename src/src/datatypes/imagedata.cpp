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



#include "imagedata.h"

imageData::imageData(std::string _name,std::string value, std::string _prompt)
{
    prompt = _prompt;
    kind=image;
    data=value;
    name=_name;
    constant=false;
    property=false;
}

imageData::~imageData()
{
    //dtor
}

void imageData::setData(std::string value)
{
    data = value;
}

std::string imageData::getData()
{
    return data;
}
void imageData::updateDependency()
{
    data = ((imageData*)dependedOn)->data;
}
void imageData::duplicate()
{
    duplicated = new imageData(name, data, prompt);
    duplicated -> setConstant(constant);
    duplicated -> setProperty(property);
    duplicated -> setSurfaceName(surfaceName);
    if (dependedOn==nullptr)
        duplicated->setDependantOn(nullptr);
    else
        duplicated -> setDependantOn(dependedOn->getDuplicated());
}
std::string imageData::save(std::ofstream& _out)
{
    ws(_out,typeToString(kind));
    ws(_out, name);
    ws(_out, prompt);
    ws(_out, surfaceName);
    ws(_out, data);
    return "";
}
std::string imageData::load(std::ifstream& _in, std::string version)
{
    if (version == "1.0.0")
    {
        name = rs(_in);
        prompt = rs(_in);
        surfaceName = rs(_in);
        data = rs(_in);
    }
    else
        return "Unsupported program version.";
    return "";
}

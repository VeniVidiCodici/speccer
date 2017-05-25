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



#include "fontdata.h"

fontData::fontData(std::string _name,std::string value, std::string _prompt)
{
    prompt = _prompt;
    kind=font;
    data=value;
    name=_name;
    constant=false;
    property=false;
}

fontData::~fontData()
{
    //dtor
}

void fontData::setData(std::string value)
{
    data = value;
}

std::string fontData::getData()
{
    return data;
}
void fontData::updateDependency()
{
    data = ((fontData*)dependedOn)->data;
}
void fontData::duplicate()
{
    duplicated = new fontData(name, data, prompt);
    duplicated -> setConstant(constant);
    duplicated -> setProperty(property);
    duplicated -> setSurfaceName(surfaceName);
    if (dependedOn==nullptr)
        duplicated->setDependantOn(nullptr);
    else
        duplicated -> setDependantOn(dependedOn->getDuplicated());
}
std::string fontData::save(std::ofstream& _out)
{
    ws(_out,typeToString(kind));
    ws(_out, name);
    ws(_out, prompt);
    ws(_out, surfaceName);
    ws(_out, data);
    return "";
}
std::string fontData::load(std::ifstream& _in, std::string version)
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

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



#include "colordata.h"

colorData::colorData(std::string _name, colorconv value, std::string _prompt) : data(value)
{
    prompt = _prompt;
    kind=color;
    name=_name;
    constant=false;
    property=false;
}

colorData::~colorData()
{
}

void colorData::setData(colorconv value)
{
    data = value;
}

colorconv colorData::getData()
{
    return data;
}
void colorData::updateDependency()
{
    data = ((colorData*)dependedOn)->data;
}
void colorData::duplicate()
{
    duplicated = new colorData(name, data, prompt);
    duplicated -> setConstant(constant);
    duplicated -> setProperty(property);
    duplicated -> setSurfaceName(surfaceName);
    if (dependedOn==nullptr)
        duplicated->setDependantOn(nullptr);
    else
        duplicated -> setDependantOn(dependedOn->getDuplicated());
}
std::string colorData::save(std::ofstream& _out)
{
    ws(_out,typeToString(kind));
    ws(_out, name);
    ws(_out, prompt);
    ws(_out, surfaceName);
    wi(_out, data.getr());
    wi(_out, data.getg());
    wi(_out, data.getb());
    wi(_out, data.geta());
    return "";
}
std::string colorData::load(std::ifstream& _in, std::string version)
{
    if (version == "1.0.0")
    {
        int r, g, b, a;
        name = rs(_in);
        prompt = rs(_in);
        surfaceName = rs(_in);
        r = ri(_in);
        g = ri(_in);
        b = ri(_in);
        a = ri(_in);
        data.setColor(r,g,b,a);
    }
    else
        return "Unsupported program version.";
    return "";
}

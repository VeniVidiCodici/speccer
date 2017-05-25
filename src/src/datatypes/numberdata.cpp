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



#include "numberdata.h"

numberData::numberData(std::string _name,double value, std::string _prompt)
{
    prompt = _prompt;
    kind=number;
    data=value;
    name=_name;
    constant=false;
    property=false;
}
numberData::numberData(std::string _name,int value, std::string _prompt)
{
    prompt = _prompt;
    kind=number;
    data=value;
    name=_name;
    constant=false;
    property=false;
}

numberData::~numberData()
{
    //dtor
}

void numberData::setData(double value)
{
    data = value;
}

void numberData::setData(int value)
{
    data = value;
}

int numberData::getData()
{
    return data;
}
void numberData::updateDependency()
{
    data = ((numberData*)dependedOn)->data;
}
void numberData::duplicate()
{
    duplicated = new numberData(name, data, prompt);
    duplicated -> setConstant(constant);
    duplicated -> setProperty(property);
    duplicated -> setSurfaceName(surfaceName);
    if (dependedOn==nullptr)
        duplicated->setDependantOn(nullptr);
    else
        duplicated -> setDependantOn(dependedOn->getDuplicated());
}
std::string numberData::save(std::ofstream& _out)
{
    ws(_out,typeToString(kind));
    ws(_out, name);
    ws(_out, prompt);
    ws(_out, surfaceName);
    wi(_out, data);
    return "";
}
std::string numberData::load(std::ifstream& _in, std::string version)
{
    if (version == "1.0.0")
    {
        name = rs(_in);
        prompt = rs(_in);
        surfaceName = rs(_in);
        data = ri(_in);
    }
    else
        return "Unsupported program version.";
    return "";
}

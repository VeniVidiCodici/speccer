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



#include "matrixdata.h"

#include "formuladata.h"
#include <shape.h>

matrixData::matrixData(std::string _name,shape* value, std::string _prompt)
{
    prompt = _prompt;
    dataName = "none";
    kind=matrix;
    if (value!=nullptr)
    {
        if (value->getType()==shape::matrix)
            data=value;
    }
    else data=nullptr;
    name=_name;
    constant=false;
    property=false;
}

matrixData::~matrixData()
{
    //dtor
}

void matrixData::setData(shape* value)
{
    if (value!=nullptr)
    {
        if (value->getType()==shape::matrix)
            data=value;
    }
    else data=nullptr;
}

shape* matrixData::getData()
{
    return data;
}
void matrixData::updateDependency()
{
    data = ((matrixData*)dependedOn)->data;
}
void matrixData::duplicate()
{
    duplicated = new matrixData(name, data, prompt);
    duplicated -> setConstant(constant);
    duplicated -> setProperty(property);
    duplicated -> setSurfaceName(surfaceName);
    if (dependedOn==nullptr)
        duplicated->setDependantOn(nullptr);
    else
        duplicated -> setDependantOn(dependedOn->getDuplicated());
}
std::string matrixData::save(std::ofstream& _out)
{
    ws(_out,typeToString(kind));
    ws(_out, name);
    ws(_out, prompt);
    ws(_out, surfaceName);
    if (data==nullptr)
        ws(_out, "none");
    else
        ws(_out, data->getName());
    return "";
}
std::string matrixData::load(std::ifstream& _in, std::string version)
{
    if (version == "1.0.0")
    {
        name = rs(_in);
        prompt = rs(_in);
        surfaceName = rs(_in);
        dataName = rs(_in);
        data = nullptr;
    }
    else
        return "Unsupported program version.";
    return "";
}
std::string matrixData::getDataName()
{
    return dataName;
}














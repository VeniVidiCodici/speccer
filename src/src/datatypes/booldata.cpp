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



#include "booldata.h"

boolData::boolData(std::string _name,bool value, std::string _prompt)
{
    prompt = _prompt;
    kind=boolean;
    data=value;
    name=_name;
    constant=false;
    property=false;
}

boolData::~boolData()
{
    //dtor
}

void boolData::setData(bool value)
{
    data = value;
}
bool boolData::getData()
{
    return data;
}
void boolData::updateDependency()
{
    if(dependedOn!=nullptr)
        data = ((boolData*)dependedOn)->data;
}

void boolData::duplicate()
{
    duplicated = new boolData(name, data, prompt);
    duplicated -> setConstant(constant);
    duplicated -> setProperty(property);
    duplicated -> setSurfaceName(surfaceName);
    if (dependedOn==nullptr)
        duplicated->setDependantOn(nullptr);
    else
        duplicated -> setDependantOn(dependedOn->getDuplicated());
}
std::string boolData::save(std::ofstream& _out)
{
    ws(_out,typeToString(kind));
    ws(_out, name);
    ws(_out, prompt);
    ws(_out, surfaceName);
    if (data)
        wi(_out, 1);
    else
        wi(_out, 0);
    return "";
}
std::string boolData::load(std::ifstream& _in, std::string version)
{
    if (version == "1.0.0")
        {
            name = rs(_in);
            prompt = rs(_in);
            surfaceName = rs(_in);
            int t = ri(_in);
            if (t==0) data = false;
            else if (t==1) data = true;
            else return "File is corrupted. Unable to open the file.";
        }
    else
        return "Unsupported program version.";
    return "";
}

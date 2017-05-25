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



#include "choicedata.h"

choiceData::choiceData(std::string _name,int value, std::string _prompt)
{
    prompt = _prompt;
    kind=choice;
    data=value;
    name=_name;
    constant=false;
    property=false;
}

choiceData::~choiceData()
{
    //dtor
}

void choiceData::setData(int value)
{
    data = value;
}
int choiceData::getData()
{
    return data;
}
void choiceData::updateDependency()
{
    if(dependedOn!=nullptr)
        data = ((choiceData*)dependedOn)->data;
}

void choiceData::duplicate()
{
    duplicated = new choiceData(name, data, prompt);
    duplicated -> setConstant(constant);
    duplicated -> setProperty(property);
    duplicated -> setSurfaceName(surfaceName);
    for (std::map<int, std::string>::iterator it=options.begin(); it!=options.end(); ++it)
        ((choiceData*)duplicated)->addAnOption(it->first, it->second);
    if (dependedOn==nullptr)
        duplicated->setDependantOn(nullptr);
    else
        duplicated -> setDependantOn(dependedOn->getDuplicated());
}

std::map<int, std::string>* choiceData::getOptions()
{
    return &options;
}

bool choiceData::addAnOption(int value,std::string name)
{
    if (options.find(value)==options.end())
    {
        options[value]=name;
        return true;
    }
    return false;
}
bool choiceData::removeAnOption(int value)
{
    if (options.find(value)!=options.end())
    {
        options.erase(value);
        return true;
    }
    return false;
}
bool choiceData::removeAnOption(std::string name)
{
    for (std::map<int, std::string>::iterator it=options.begin(); it!=options.end(); ++it)
    {
        if (it->second==name)
        {
            options.erase(it);
            return true;
        }
    }
    return false;
}
std::string choiceData::save(std::ofstream& _out)
{
    ws(_out,typeToString(kind));
    ws(_out, name);
    ws(_out, prompt);
    ws(_out, surfaceName);
    wi(_out, data);
    wi(_out, options.size());
    for (std::map<int, std::string>::iterator it=options.begin(); it!=options.end(); ++it)
    {
        wi(_out, it->first);
        ws(_out, it->second);
    }
    return "";
}
std::string choiceData::load(std::ifstream& _in, std::string version)
{
    if (version == "1.0.0")
    {
        int s, fr;
        std::string sec;
        name = rs(_in);
        prompt = rs(_in);
        surfaceName = rs(_in);
        data = ri(_in);
        s = ri(_in);
        for (int i=0; i<s; i++)
        {
            fr = ri(_in);
            sec = rs(_in);
            options[fr] = sec;
        }
    }
    else
        return "Unsupported program version.";
    return "";
}

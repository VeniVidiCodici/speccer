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



#include "composite.h"

composite::composite(std::string whatName)
{
    critical = false;
    duplicated=nullptr;
    loadedSurfaceDataPoints = false;
    name=whatName;
    _x = _y = _w = _h = _tx = _ty = _tw = _th = 0.0;
    compositeProperties.push_back(new matrixData("tfmatrix", nullptr,"Tfm.\r\nmatrix"));
}

composite::~composite()
{
    for (drawable* obj : elements)
        delete obj;
    elements.clear();
    for (dataPoint* obj : compositeProperties)
        delete obj;
    compositeProperties.clear();
}
void composite::loadSurfaceDataPoints()
{
    if (loadedSurfaceDataPoints)
        return;
    loadedSurfaceDataPoints=true;
    for(int i=0;i<elements.size();i++)
    {
        std::vector<dataPointUnit> sdpu=elements[i]->getSurfaceDataPoints();
        for(int k=0;k<sdpu.size();k++)
        {
            bool flag=false;
            for(int j=0;j<compositeProperties.size();j++)
            {
                if (sdpu[k].name==compositeProperties[j]->getName()
                    &&sdpu[k].kind==compositeProperties[j]->getType())
                {
                    flag = true;
                    break;
                }
                if (flag)
                    continue;
            }
            if(!flag)
            {
                switch (sdpu[k].kind)
                {
                case dataPoint::boolean:
                    {boolData * dp = new boolData(sdpu[k].name,false,sdpu[k].name);
                    compositeProperties.push_back(dp);}
                break;
                case dataPoint::formula:
                    {formulaData * dp = new formulaData(sdpu[k].name,"0",sdpu[k].name);
                    compositeProperties.push_back(dp);}
                break;
                case dataPoint::number:
                    {numberData * dp = new numberData(sdpu[k].name,0,sdpu[k].name);
                    compositeProperties.push_back(dp);}
                break;
                case dataPoint::color:
                    {colorData * dp = new colorData(sdpu[k].name,colorconv(255,255,255,255),sdpu[k].name);
                    compositeProperties.push_back(dp);}
                break;
                case dataPoint::image:
                    {imageData * dp = new imageData(sdpu[k].name,"",sdpu[k].name);
                    compositeProperties.push_back(dp);}
                break;
                case dataPoint::collection:
                    {std::vector<vertInfo> vi;
                    collectionData * dp = new collectionData(sdpu[k].name,vi,sdpu[k].name);
                    compositeProperties.push_back(dp);}
                break;
                case dataPoint::font:
                    {fontData * dp = new fontData(sdpu[k].name,"",sdpu[k].name);
                    compositeProperties.push_back(dp);}
                break;
                case dataPoint::text:
                    {textData * dp = new textData(sdpu[k].name,"",sdpu[k].name);
                    compositeProperties.push_back(dp);}
                break;
                case dataPoint::matrix:
                    {matrixData * dp = new matrixData(sdpu[k].name,nullptr,sdpu[k].name);
                    compositeProperties.push_back(dp);}
                break;
                case dataPoint::choice:
                    {choiceData * dp = new choiceData(sdpu[k].name,0,sdpu[k].name);
                    compositeProperties.push_back(dp);}
                break;
                }
            }
        }
    }
}

std::vector<drawable *> composite::getDrawables()
{
    return elements;
}
bool composite::itIsCritical()
{
    return false;
}

std::vector<shape *> composite::getShapes()
{
    std::vector<shape *> toReturn;
    for (drawable* obj : elements)
        {
            std::vector<shape *> temp=obj->getShapes();
            for (shape* sh : temp)
                toReturn.push_back(sh);
        }
    return toReturn;
}
std::vector<dataPoint *> composite::getProperties()
{
    return compositeProperties;
}

bool composite::addCompositeProperty(dataPoint * property)
{
    if (property==nullptr)
        return false;
    compositeProperties.push_back(property);
    return true;
}
bool composite::addElement(drawable * element)
{
    if (element==nullptr)
        return false;
    bool flag=true;
    if (element->isSimplex())
        if(((shape *)element)->getType()==shape::anchor||
           ((shape *)element)->getType()==shape::bbox||
           ((shape *)element)->getType()==shape::brpoint||
           ((shape *)element)->getType()==shape::root)
                flag = false;
    if (flag)
        elements.push_back(element);
    return true;
}
bool composite::addDependency(dataPoint * dependent, dataPoint * dependedOn)
{
    if (dependedOn==nullptr||dependent==nullptr)
        return false;
    else return dependent->setDependantOn(dependedOn);
}

bool composite::propertyOrShapeExists(std::string propName)
{
    for (dataPoint * obj : compositeProperties)
        {
            if(obj->getName()==propName)
                return true;
        }
    for (drawable * obj : elements)
        {
            if(obj->getName()==propName)
                return true;
        }
    return false;
}

bool composite::propertyExists(std::string propName)
{
    for (dataPoint * obj : compositeProperties)
        {
            if(obj->getName()==propName)
                return true;
        }
    return false;
}

bool composite::elementExists(std::string elementName)
{
    for (drawable * obj : elements)
        {
            if(obj->getName()==elementName)
                return true;
        }
    return false;
}
drawable * composite::getElementByName(std::string elementName)
{
    for (drawable * obj : elements)
        {
            if(obj->getName()==elementName)
                return obj;
        }
    return nullptr;
}
dataPoint * composite::getPropertyByName(std::string propName)
{
    for (dataPoint * obj : compositeProperties)
        {
            if(obj->getName()==propName)
                return obj;
        }
    return nullptr;
}
dataPoint * composite::getPropertyByNameAndType(std::string propName, dataPoint::type propType)
{
    for (dataPoint * obj : compositeProperties)
        {
            if(obj->getName()==propName&&obj->getType()==propType)
                return obj;
        }
    return nullptr;
}
void composite::duplicate()
{
    duplicated = new composite(name);
    for (dataPoint* obj : compositeProperties)
        {obj->duplicate();}
    for (dataPoint* obj : compositeProperties)
        {((composite *)duplicated)->addCompositeProperty(obj->getDuplicated());}
    for (drawable * obj : elements)
        {obj->duplicate();}
    for (drawable* obj : elements)
        {((composite *)duplicated)->addElement(obj->getDuplicated());}
}
void composite::endDuplication()
{
    for (dataPoint* obj : compositeProperties)
        {obj->endDuplication();}
    for (drawable* obj : elements)
        {obj->endDuplication();}
    duplicated = nullptr;
}
std::vector<dataPointUnit> composite::getSurfaceDataPoints()
{
    std::vector<dataPointUnit> dpu;
    for(int i=0;i<compositeProperties.size();i++)
    {
        if (compositeProperties[i]->getSurfaceName()=="")
            continue;
        bool flag=false;
        for(int j=0;j<dpu.size();j++)
        {
            if (compositeProperties[i]->getSurfaceName()==dpu[j].name&&compositeProperties[i]->getType()==dpu[j].kind)
            {
                flag = true;
                break;
            }
            if (flag)
                continue;
        }
        if (!flag)
            dpu.push_back(dataPointUnit{compositeProperties[i]->getSurfaceName(), compositeProperties[i]->getType()});
    }
    for(int i=0;i<elements.size();i++)
    {
        std::vector<dataPointUnit> sdpu=elements[i]->getSurfaceDataPoints();
        for(int k=0;k<sdpu.size();k++)
        {
            bool flag=false;
            for(int j=0;j<dpu.size();j++)
            {
                if (sdpu[k].name==dpu[j].name&&sdpu[k].kind==dpu[j].kind)
                {
                    flag = true;
                    break;
                }
                if (flag)
                    continue;
            }
            if (!flag)
                dpu.push_back(sdpu[k]);
        }
    }
    return dpu;
}
std::string composite::save(std::ofstream& _out)
{
    ws(_out,"composite");
    ws(_out,name);
    wi(_out,compositeProperties.size());
    std::string error;
    for (dataPoint * obj : compositeProperties)
        {
        error = obj->save(_out);
        if (error!="")
            return error;
        }
    wi(_out,elements.size());
    for (drawable* obj : elements)
        {
        error = obj->save(_out);
        if (error!="")
            return error;
        }
    return "";
}
std::string composite::load(std::ifstream& _in, std::string version)
{
    if (version == "1.0.0")
    {
        int s;
        name = rs(_in);
        name = rs(_in);
        s = ri(_in);
        for (dataPoint* obj : compositeProperties)
            delete obj;
        compositeProperties.clear();
        boolData aux("",true,"");
        for (int i=0;i<s;i++)
        {
            std::string n, a;
            n = rs(_in);
            if (aux.stringToType(n) == dataPoint::invalid)
                return "Invalid data type: "+n+"; in "+name+"::load, of number "+std::to_string(i);
            switch(aux.stringToType(n))
            {
                case dataPoint::formula:{
                    formulaData * nd = new formulaData("","","");
                    nd->set_intsize(_intsize);
                    a = nd->load(_in, version);
                    if (a!="") return a;
                    compositeProperties.push_back(nd);}
                break;
                case dataPoint::number:{
                    numberData * nd = new numberData("",0,"");
                    nd->set_intsize(_intsize);
                    a = nd->load(_in, version);
                    if (a!="") return a;
                    compositeProperties.push_back(nd);}
                break;
                case dataPoint::color:{
                    colorData * nd = new colorData("",colorconv(0,0,0,0),"");
                    nd->set_intsize(_intsize);
                    a = nd->load(_in, version);
                    if (a!="") return a;
                    compositeProperties.push_back(nd);}
                break;
                case dataPoint::image:{
                    imageData * nd = new imageData("","","");
                    nd->set_intsize(_intsize);
                    a = nd->load(_in, version);
                    if (a!="") return a;
                    compositeProperties.push_back(nd);}
                break;
                case dataPoint::boolean:{
                    boolData * nd = new boolData("",true,"");
                    nd->set_intsize(_intsize);
                    a = nd->load(_in, version);
                    if (a!="") return a;
                    compositeProperties.push_back(nd);}
                break;
                case dataPoint::collection:{
                    std::vector<vertInfo> v;
                    collectionData * nd = new collectionData("",v,"");
                    nd->set_intsize(_intsize);
                    a = nd->load(_in, version);
                    if (a!="") return a;
                    compositeProperties.push_back(nd);}
                break;
                case dataPoint::font:{
                    fontData * nd = new fontData("","","");
                    nd->set_intsize(_intsize);
                    a = nd->load(_in, version);
                    if (a!="") return a;
                    compositeProperties.push_back(nd);}
                break;
                case dataPoint::text:{
                    textData * nd = new textData("","","");
                    nd->set_intsize(_intsize);
                    a = nd->load(_in, version);
                    if (a!="") return a;
                    compositeProperties.push_back(nd);}
                break;
                case dataPoint::matrix:{
                    matrixData * nd = new matrixData("",nullptr,"");
                    nd->set_intsize(_intsize);
                    a = nd->load(_in, version);
                    if (a!="") return a;
                    compositeProperties.push_back(nd);}
                break;
                case dataPoint::choice:{
                    choiceData * nd = new choiceData("",0,"");
                    nd->set_intsize(_intsize);
                    a = nd->load(_in, version);
                    if (a!="") return a;
                    compositeProperties.push_back(nd);}
                break;
            }
        }
        s = ri(_in);
        for (drawable * obj : elements)
            delete obj;
        elements.clear();
        for (int i=0;i< s; i++)
        {
            std::string n, a;
            shape ax("", shape::invalid);
            n = rs(_in);
            if (n=="shape")
            {
                n = rs(_in);
                if (ax.stringToType(n) == shape::invalid)
                    return "Invalid shape type: "+n+" at shape number "+std::to_string(s);
                shape * nd = new shape("",ax.stringToType(n));
                nd->set_intsize(_intsize);
                a = nd->load(_in, version);
                if (a!="") return a;
                elements.push_back(nd);
            }
            else if (n=="composite")
            {
                composite * nd = new composite("");
                nd->set_intsize(_intsize);
                a = nd->load(_in, version);
                if (a!="") return a;
                elements.push_back(nd);
            }
            else return "Invalid drawable type: "+n;
        }

        //start reassigning matrix pointers
        std::map<std::string, shape *>namesToPointers;
        for(int i=0;i<elements.size();i++)
        {
            if (elements[i]->isSimplex())
            if (((shape *) elements[i])->getType()==shape::matrix)
            {
                namesToPointers[elements[i]->getName()] = (shape *) elements[i];
            }
        }
        for(int i=0;i<elements.size();i++)
        {
            if (elements[i]->getPropertyByName("tfmatrix")!=nullptr)
            if (elements[i]->getPropertyByName("tfmatrix")->getType()==dataPoint::matrix)
            {
                matrixData * md = (matrixData *) elements[i]->getPropertyByName("tfmatrix");
                if (namesToPointers.find(md->getDataName())!=namesToPointers.end())
                    md->setData(namesToPointers.find(md->getDataName())->second);
                else
                    md->setData(nullptr);
            }
        }
        namesToPointers.clear();
        //end reassigning matrix pointers
    }
    else
        return "Unsupported program version: "+ version;
    return "";
}

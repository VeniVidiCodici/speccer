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



#include "shape.h"
shape::shape(std::string whatName, shapeType kind, bool crt)
{
    duplicated=nullptr;
    name=whatName;
    type = kind;
    critical = crt;
    _x = _y = _w = _h = _tx = _ty = _tw = _th = 0.0;
}
bool shape::itIsCritical() {return critical;}
shape::shape(std::string whatName, shapeType kind)
{
    duplicated=nullptr;
    name=whatName;
    type = kind;
    critical = false;
    if (type!=root)
        addPositionProperties();
    switch(type)
    {
    case root:
        critical=true;
    break;
    case anchor:
        {
        critical=true;
        formulaData * fd = (formulaData *)getPropertyByName("xpos");
        if (fd!=nullptr)
            fd->setData("110");
        }
    break;
    case brpoint:
        {
        critical=true;
        formulaData * fd = (formulaData *)getPropertyByName("xpos");
        if (fd!=nullptr)
            fd->setData("300");
        fd = (formulaData *)getPropertyByName("ypos");
        if (fd!=nullptr)
            fd->setData("-20");
        }
    break;
    case bbox:
        {
        critical=true;
        shapeProperties.push_back(new formulaData("width","220","Width"));
        shapeProperties.push_back(new formulaData("height","120","Height"));
        formulaData * fd = (formulaData *)getPropertyByName("xpos");
        if (fd!=nullptr)
            fd->setData("100");
        fd = (formulaData *)getPropertyByName("ypos");
        if (fd!=nullptr)
            fd->setData("-60");
        }
    break;
    case circle:
        shapeProperties.push_back(new formulaData("radius","100","Radius"));
        shapeProperties.push_back(new numberData("sides",24,"Sides"));
        addShapeProperties();
        addTransformationProperties();
    break;
    case arc:
        shapeProperties.push_back(new formulaData("radius","100","Radius"));
        shapeProperties.push_back(new formulaData("angle","90","Angle"));
        shapeProperties.push_back(new numberData("sides",24,"Sides"));
        shapeProperties.push_back(new boolData("iol",true,"Inner\noutline"));
        shapeProperties.push_back(new boolData("ool",true,"Outer\noutline"));
        addShapeProperties();
        addTransformationProperties();
    break;
    case rectangle:
        shapeProperties.push_back(new formulaData("width","162","Width"));
        shapeProperties.push_back(new formulaData("height","100","Height"));
        addShapeProperties();
        addTransformationProperties();
    break;
    case roundrect:
        shapeProperties.push_back(new formulaData("width","162","Width"));
        shapeProperties.push_back(new formulaData("height","100","Height"));
        shapeProperties.push_back(new formulaData("cradius","10","Corner\nradius"));
        shapeProperties.push_back(new numberData("sides",8,"Corner\nsides"));
        addShapeProperties();
        addTransformationProperties();
    break;
    case convex:
        {std::vector<vertInfo> nopoints;
        shapeProperties.push_back(new collectionData("points",nopoints,""));
        addShapeProperties();
        addTransformationProperties();}
    break;
    case text:
        shapeProperties.push_back(new textData("text","","Text"));
        shapeProperties.push_back(new fontData("font","","Font"));
        shapeProperties.push_back(new numberData("size",12,"Size"));
        //shapeProperties.push_back(new boolData("italic",false,"Italic"));
        //shapeProperties.push_back(new boolData("bold",false,"Bold"));
        //shapeProperties.push_back(new boolData("underlined",false,"Underlined"));
        //shapeProperties.push_back(new boolData("strikethrough",false,"Strikethrough"));
        //addShapeProperties();
        shapeProperties.push_back(new colorData("fillcolor",colorconv(0,0,0,255),"Fill color"));
        addTransformationProperties();
    break;
    case image:
        shapeProperties.push_back(new imageData("image","\\","Image"));
        shapeProperties.push_back(new colorData("blend",colorconv(255,255,255,255),"Blend color"));
        addTransformationProperties();
    break;
    case points:
        {
        std::vector<vertInfo> nopoints;
        shapeProperties.push_back(new collectionData("points",nopoints,""));
        shapeProperties.push_back(new choiceData("type",1,"Primitive type"));
        choiceData * cd = (choiceData *)getPropertyByName("type");
        cd->addAnOption(1,"Lines");
        cd->addAnOption(2,"Line strip");
        cd->addAnOption(3,"Triangles");
        cd->addAnOption(4,"Triangle strip");
        cd->addAnOption(5,"Triangle fan");
        cd->addAnOption(6,"Quads");
        addTransformationProperties();
        }
    break;
    case matrix:
        addTransformationProperties();
        shapeProperties.push_back(new boolData("grid",true,"Display grid"));
        shapeProperties.push_back(new colorData("gridcolor",colorconv(255,0,0,255),"Grid color"));
        shapeProperties.push_back(new boolData("tfm",true,"Display\ntransformation"));
        shapeProperties.push_back(new colorData("tfmcolor",colorconv(128,0,0,255),"Transformation\ncolor"));
    break;
    }
    if (type!=root)
        shapeProperties.push_back(new matrixData("tfmatrix", nullptr,"Tfm.\r\nmatrix"));
    //shapeProperties.push_back(new boolData("constant",false));
    //shapeProperties.push_back(new boolData("property",false));
}

shape::~shape()
{
    for (dataPoint* obj : shapeProperties)
        delete obj;
    shapeProperties.clear();
}
void shape::addPositionProperties()
{
    shapeProperties.push_back(new formulaData("xpos","0","X"));
    shapeProperties.push_back(new formulaData("ypos","0","Y"));
}

void shape::addShapeProperties()
{
    shapeProperties.push_back(new colorData("fillcolor",colorconv(255,255,255,255),"Fill color"));
    shapeProperties.push_back(new numberData("outlinethickness",3,"Outline\r\nthickness"));
    shapeProperties.push_back(new colorData("outlinecolor",colorconv(0,0,0,255),"Outline\r\ncolor"));
}

void shape::addTransformationProperties()
{
    shapeProperties.push_back(new formulaData("xorigin","0","X origin"));
    shapeProperties.push_back(new formulaData("yorigin","0","Y origin"));
    shapeProperties.push_back(new formulaData("rotation","0","Rotation"));
    shapeProperties.push_back(new formulaData("xscale","1","X scale"));
    shapeProperties.push_back(new formulaData("yscale","1","Y scale"));
}
std::vector<dataPointUnit> shape::getSurfaceDataPoints()
{
    std::vector<dataPointUnit> dpu;
    for(int i=0;i<shapeProperties.size();i++)
    {
        if (shapeProperties[i]->getSurfaceName()=="")
            continue;
        bool flag=false;
        for(int j=0;j<dpu.size();j++)
        {
            if (shapeProperties[i]->getSurfaceName()==dpu[j].name&&shapeProperties[i]->getType()==dpu[j].kind)
            {
                flag = true;
                break;
            }
            if (flag)
                continue;
        }
        if (!flag)
            dpu.push_back(dataPointUnit{shapeProperties[i]->getSurfaceName(), shapeProperties[i]->getType()});
    }
    return dpu;
}
std::vector<shape *> shape::getShapes()
{
    std::vector<shape *>temp;
    temp.push_back(this);
    return temp;
}
std::vector<dataPoint *> shape::getProperties()
{
    return shapeProperties;
}
bool shape::propertyExists(std::string propName)
{
    for (dataPoint * obj : shapeProperties)
        {
            if(obj->getName()==propName)
                return true;
        }
    return false;
}

dataPoint * shape::getPropertyByName(std::string propName)
{
    for (dataPoint * obj : shapeProperties)
        {
            if(obj->getName()==propName)
                return obj;
        }
    return nullptr;
}
void shape::addPropertyDuringDuplication(dataPoint * newProperty)
{
    if (newProperty!=nullptr)
    shapeProperties.push_back(newProperty);
}
void shape::duplicate()
{
    duplicated = new shape(name, type, critical);
    for (dataPoint* obj : shapeProperties)
        {obj->duplicate();}
    for (dataPoint* obj : shapeProperties)
        {((shape *)duplicated)->addPropertyDuringDuplication(obj->getDuplicated());}
}
void shape::endDuplication()
{
    for (dataPoint* obj : shapeProperties)
        {obj->endDuplication();}
    duplicated = nullptr;
}
std::string shape::save(std::ofstream& _out)
{
    ws(_out,"shape");
    ws(_out,typeToString(type));
    ws(_out,name);
    wi(_out,shapeProperties.size());
    std::string error;
    for (dataPoint * obj : shapeProperties)
        {
        error = obj->save(_out);
        if (error!="")
            return error;
        }
    return "";
}
std::string shape::load(std::ifstream& _in, std::string version)
{
    if (version == "1.0.0")
    {
        int s;
        name = rs(_in);
        s = ri(_in);
        for (dataPoint* obj : shapeProperties)
            delete obj;
        shapeProperties.clear();
        boolData aux("",true,"");
        for (int i=0;i<s;i++)
        {
            std::string n, a;
            n = rs(_in);
            if (aux.stringToType(n) == dataPoint::invalid)
                return "Invalid data type: "+n+";"+std::to_string(i);
            switch(aux.stringToType(n))
            {
                case dataPoint::formula:{
                    formulaData * nd = new formulaData("","","");
                    nd->set_intsize(_intsize);
                    a = nd->load(_in, version);
                    if (a!="") return a;
                    shapeProperties.push_back(nd);}
                break;
                case dataPoint::number:{
                    numberData * nd = new numberData("",0,"");
                    nd->set_intsize(_intsize);
                    a = nd->load(_in, version);
                    if (a!="") return a;
                    shapeProperties.push_back(nd);}
                break;
                case dataPoint::color:{
                    colorData * nd = new colorData("",colorconv(0,0,0,0),"");
                    nd->set_intsize(_intsize);
                    a = nd->load(_in, version);
                    if (a!="") return a;
                    shapeProperties.push_back(nd);}
                break;
                case dataPoint::image:{
                    imageData * nd = new imageData("","","");
                    nd->set_intsize(_intsize);
                    a = nd->load(_in, version);
                    if (a!="") return a;
                    shapeProperties.push_back(nd);}
                break;
                case dataPoint::boolean:{
                    boolData * nd = new boolData("",true,"");
                    nd->set_intsize(_intsize);
                    a = nd->load(_in, version);
                    if (a!="") return a;
                    shapeProperties.push_back(nd);}
                break;
                case dataPoint::collection:{
                    std::vector<vertInfo> v;
                    collectionData * nd = new collectionData("",v,"");
                    nd->set_intsize(_intsize);
                    a = nd->load(_in, version);
                    if (a!="") return a;
                    shapeProperties.push_back(nd);}
                break;
                case dataPoint::font:{
                    fontData * nd = new fontData("","","");
                    nd->set_intsize(_intsize);
                    a = nd->load(_in, version);
                    if (a!="") return a;
                    shapeProperties.push_back(nd);}
                break;
                case dataPoint::text:{
                    textData * nd = new textData("","","");
                    nd->set_intsize(_intsize);
                    a = nd->load(_in, version);
                    if (a!="") return a;
                    shapeProperties.push_back(nd);}
                break;
                case dataPoint::matrix:{
                    matrixData * nd = new matrixData("",nullptr,"");
                    nd->set_intsize(_intsize);
                    a = nd->load(_in, version);
                    if (a!="") return a;
                    shapeProperties.push_back(nd);}
                break;
                case dataPoint::choice:{
                    choiceData * nd = new choiceData("",0,"");
                    nd->set_intsize(_intsize);
                    a = nd->load(_in, version);
                    if (a!="") return a;
                    shapeProperties.push_back(nd);}
                break;
            }
        }
    }
    else
        return "Unsupported program version: "+version;
    return "";
}












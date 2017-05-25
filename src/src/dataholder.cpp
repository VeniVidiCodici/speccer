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



#include "dataholder.h"
#include "../styleframe.h"
#include <fstream>
#include <iomanip>
#include <iostream>



dataHolder::dataHolder(UIAPI * userInterface, drawingAPI * drawer, dataModel * modelArg)
{
    filepath = name = "";
    changed = saved = false;
    model = modelArg;
    uapi=userInterface;
    japi=drawer;
    removeAllCustomFields();
}

dataHolder::~dataHolder()
{
    for(int i=0;i<drawablesv.size();i++)
        delete drawablesv[i];
    for (std::map<std::string, formulaData *>::iterator it=customFields.begin(); it!=customFields.end(); ++it)
        delete it->second;
}

void dataHolder::setName(std::string newName)
{
    name = newName;
}
std::string dataHolder::getName()
{
    return name;
}

void dataHolder::setFilepath(std::string newfilepath)
{
    filepath = newfilepath;
}
std::string dataHolder::getFilepath()
{
    return filepath;
}
void dataHolder::setSaved(bool s)
{
    saved = s;
}
bool dataHolder::getSaved()
{
    return saved;
}
void dataHolder::setChanged(bool c)
{
    changed = c;
}
bool dataHolder::getChanged()
{
    return changed;
}

bool dataHolder::openStyle(std::string filename)
{
    return model->openStyle(filename);
}
bool dataHolder::saveStyle(std::string filename)
{
    return model->saveStyle(filename);
}

std::string dataHolder::save(std::ofstream& _out)
{
    std::vector<formulaData *> dps = getAllCustomFields();
    ws(_out,"node");
    ws(_out,name);
    wi(_out,dps.size());
    std::string error;
    for (formulaData * obj : dps)
        {
        error = obj->save(_out);
        if (error!="")
            return error;
        }
    wi(_out,drawablesv.size());
    for (drawable* obj : drawablesv)
        {
        error = obj->save(_out);
        if (error!="")
            return error;
        }
    return "";
}
std::string dataHolder::load(std::ifstream& _in, std::string version)
{
    if (version == "1.0.0")
    {
        int s;
        std::string wt = rs(_in);
        if (wt!="node")
            return "Wrong file format: "+wt;
        name = rs(_in);
        s = ri(_in);

        for (std::map<std::string, formulaData *>::iterator it=customFields.begin(); it!=customFields.end(); ++it)
            delete it->second;
        customFields.clear();
        boolData aux("",true,"");
        for (int i=0;i<s;i++)
        {
            std::string n, a;
            n = rs(_in);
            if (aux.stringToType(n) == dataPoint::invalid)
                return "Invalid data type: "+n;
            switch(aux.stringToType(n))
            {
                case dataPoint::formula:{
                    formulaData * nd = new formulaData("","","");
                    nd->set_intsize(_intsize);
                    a = nd->load(_in, version);
                    if (a!="") return a;
                    customFields[nd->getName()] = nd;
                    }
                break;
                default: return "Unsupported custom data: "+n;break;
            }
        }
        s = ri(_in);
        for (drawable * obj : drawablesv)
            delete obj;
        drawablesv.clear();
        for (int i=0;i< s; i++)
        {
            std::string n, a;
            shape ax("", shape::invalid);
            n = rs(_in);
            if (n=="shape")
            {
                n = rs(_in);
                if (ax.stringToType(n) == shape::invalid)
                    return "Invalid shape type: "+n;
                shape * nd = new shape("",ax.stringToType(n));
                nd->set_intsize(_intsize);
                a = nd->load(_in, version);
                if (a!="") return a;
                drawablesv.push_back(nd);
            }
            else if (n=="composite")
            {
                composite * nd = new composite("");
                a = nd->load(_in, version);
                nd->set_intsize(_intsize);
                if (a!="") return a;
                drawablesv.push_back(nd);
            }
            else return "Invalid drawable type: "+n;
        }

        //start reassigning matrix pointers
        std::map<std::string, shape *>namesToPointers;
        for(int i=0;i<drawablesv.size();i++)
        {
            if (drawablesv[i]->isSimplex())
            if (((shape *) drawablesv[i])->getType()==shape::matrix)
            {
                namesToPointers[drawablesv[i]->getName()] = (shape *) drawablesv[i];
            }
        }
        for(int i=0;i<drawablesv.size();i++)
        {
            if (drawablesv[i]->getPropertyByName("tfmatrix")!=nullptr)
            if (drawablesv[i]->getPropertyByName("tfmatrix")->getType()==dataPoint::matrix)
            {
                matrixData * md = (matrixData *) drawablesv[i]->getPropertyByName("tfmatrix");
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
        return "Unsupported program version: "+version;
    return "";
}

bool dataHolder::saveComposite(std::string filename)
{
    std::fstream ffs;
	ffs.open (filename, std::fstream::trunc | std::fstream::out);
	ffs.write((char *)this, sizeof(this));
	ffs.close();

    return true;
}
bool dataHolder::openComposite(std::string filename)
{
    return true;
}



std::vector<dataPointUnit> dataHolder::getSurfaceDataPoints()
{
    std::vector<dataPointUnit> dpu;
    for(int i=0;i<drawablesv.size();i++)
    {
        std::vector<dataPointUnit> sdpu=drawablesv[i]->getSurfaceDataPoints();
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
            dpu.push_back(sdpu[k]);
        }
    }
    return dpu;
}




drawable * dataHolder::getDrawableByName(std::string drName)
{
    int id = getDrawableIdByName(drName);
    if (id==-1)
        return nullptr;
    return drawablesv[id];
}
shape * dataHolder::getShapeByType(shape::shapeType kind)
{
    for(int i=0;i<drawablesv.size();i++)
        if (drawablesv[i]->isSimplex())
            {
            shape * sh = (shape *)drawablesv[i];
            if (sh->getType()==kind)
                return sh;
            }
    return nullptr;
}
bool dataHolder::drawableExists(std::string drName)
{
    if (getDrawableIdByName(drName)>=0)
        return true;
    return false;
}
void dataHolder::removeAllDrawables()
{
    for(int i=0;i<drawablesv.size();i++)
        delete drawablesv[i];
    drawablesv.clear();
}
std::vector<drawable *> dataHolder::getAllDrawables()
{
    return drawablesv; // other classes shouldn't be able to access the list directly, so don't pass a pointer
}
bool dataHolder::addNewDrawableFromModel(std::string thisOne)
{
    std::string newDrawableName=thisOne;
    if (!model->drawableExists(thisOne))
        return false;
    int counter=2;
    while (drawableExists(newDrawableName))
    {
        newDrawableName = std::to_string(counter)+ ". "+thisOne;
        if (counter++>100)
            return false;
    }
    if (model->getDrawableByName(thisOne)==nullptr) // is this check really necessary?
        return false;
    model->getDrawableByName(thisOne)->duplicate();
    drawable * newDrawable = model->getDrawableByName(thisOne)->getDuplicated();
    model->getDrawableByName(thisOne)->endDuplication();
    drawablesv.push_back(newDrawable);
    newDrawable->setName(newDrawableName);
    return true;
}
bool dataHolder::removeDrawable(std::string thisOne)
{
    int id = getDrawableIdByName(thisOne);
    if (id<0)
        return false;
    delete drawablesv[id];
    drawablesv.erase(drawablesv.begin()+id);
    return true;
}
bool dataHolder::renameDrawable(std::string currentName, std::string newName)
{
	if (!drawableExists(currentName))
        return false;
	if (drawableExists(newName))
        return false;
	int id = getDrawableIdByName(currentName);
	if (id<0)
        return false;
    drawablesv[id]->setName(newName);
	return true;
}
bool dataHolder::swapDrawables(int id1, int id2)
{
    if (id1<0||id2<0||id1>drawablesv.size()-1||id2>drawablesv.size()-1)
        return false; //one of the indices is out of bounds, abort the operation
    if (id1==id2)
        return true; //no need to swap something with itself
    drawable * t = drawablesv[id1];
    drawablesv[id1] = drawablesv[id2];
    drawablesv[id2] = t;
    return true;
}
int dataHolder::getDrawableIdByName(std::string drName)
{
    for(int i=0;i<drawablesv.size();i++)
        if (drawablesv[i]->getName()==drName)
            return i;
    return -1;
}
drawable * dataHolder::getDrawableById(int id)
{
    if (id<0||id>drawablesv.size()-1)
        return nullptr;
    return drawablesv[id];
}
int dataHolder::getNumberOfDrawables()
{
    return drawablesv.size();
}
void dataHolder::setdbgstr(std::string arg)
{
    uapi->debugstr=arg;
    uapi->unbug(arg);
}

bool dataHolder::drawableExistsInModel(std::string drName)
{
    return model->drawableExists(drName);
}
drawable * dataHolder::getDrawableFromModelByName(std::string drName)
{
    return model->getDrawableByName(drName);
}
std::vector<drawable *> dataHolder::getAllDrawablesInModel()
{
    return model->getAllDrawables();
}
void dataHolder::redraw()
{
    //japi->redraw();
}
std::vector<formulaData *> dataHolder::getAllCustomFields()
{
    std::vector<formulaData *> temp;
    for (std::map<std::string, formulaData *>::iterator it=customFields.begin();it!=customFields.end(); ++it)
        temp.push_back(it->second);
    return temp;
}
std::vector<dataPoint *> dataHolder::getAllCustomFieldsDP()
{
    std::vector<dataPoint *> temp;
    for (std::map<std::string, formulaData *>::iterator it=customFields.begin();it!=customFields.end(); ++it)
        temp.push_back((dataPoint*) it->second);
    return temp;
}
void dataHolder::removeAllCustomFields()
{
    customFieldValues.clear();
    for (std::map<std::string, formulaData *>::iterator it=customFields.begin();
        it!=customFields.end(); ++it)
            customFieldValues[it->first] = it->second->getData();
    customFields.clear();
    if (uapi->getMode()!=1)
        {
        getField("n","1");
        getField("sc","2");
        getField("sw","100");
        getField("sh","300");
        getField("pc","0");
        getField("pw","0");
        getField("ph","0");
        getField("dc","0");
        getField("dw","0");
        getField("dh","0");
        getField("udc","0");
        getField("udw","0");
        getField("udh","0");
        }
}
void dataHolder::addANewDrawable(drawable * d)
{
    if (d!=nullptr)
        drawablesv.push_back(d);
}

std::string dataHolder::getField(std::string sk, std::string defaultValue)
{
    std::string tempstr="0";
    if (defaultValue=='\0') /// this silly mistake caused DAYS of delay
        defaultValue = "0";
    if (customFieldValues.find(sk)!=customFieldValues.end())
        {
        tempstr = customFieldValues[sk];
        }
    else
        {
        tempstr = defaultValue;
        customFieldValues[sk] = tempstr;
        }
    if (customFields.find(sk)!=customFields.end())
        {
        }
    else
        {
        customFields[sk] = new formulaData(sk,tempstr,sk);
        }
    return tempstr;
}
std::string dataHolder::setField(std::string sk, std::string value)
{
    customFieldValues[sk] = value;
    if (customFields.find(sk)!=customFields.end())
        {
        customFields[sk]->setData(value);
        }
    else
        {
        customFields[sk] = new formulaData(sk,value,sk);
        }
    return value;
}
bool dataHolder::refreshFieldsFGS()
{
    return uapi->refreshFieldsFGS();
}










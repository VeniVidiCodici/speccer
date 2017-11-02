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



#include "branch.h"
#include "formulaparser.h"
#include "tree.h"
#include "drawingapi.h"

branch::branch(int i, tree * arg)
{
    id = i;
    ptree = arg;
    parentid = -1;
    node = "Leaf";
    x = y = n= w = h = sc= sw= sh= dc= dw= dh= udc= udw= udh =rootx=rooty=0;
    parent = nullptr;
    saved = false;

//    cacheHash = "";
}

branch::~branch()
{
    if (parent!=nullptr)
        parent->removeChild(this);
    for(int i=0;i<properties.size();i++)
        delete properties[i];
    properties.clear();
}
void branch::uproot()
{
    for(int i=0;i<children.size();i++)
    {
        children[i]->uproot();
    }
    children.clear();
    delete this;
}
void branch::setSaved(bool isItSaved)
{
    saved = isItSaved;
}
void branch::massSetSaved(bool isItSaved)
{
    saved = isItSaved;
    for(int i=0;i<children.size();i++)
    {
        children[i]->massSetSaved(isItSaved);
    }
}
bool branch::propertyExists(std::string sk, dataPoint::type kind)
{
    for(int i=0;i<properties.size();i++)
        if (properties[i]->getName()==sk)
            if (properties[i]->getType()==kind)
                return true;
    return false;
}
dataPoint * branch::getProperty(std::string sk, dataPoint::type kind)
{
    for(int i=0;i<properties.size();i++)
        if (properties[i]->getName()==sk)
            if (properties[i]->getType()==kind)
                return properties[i];
    dataPoint * dp;
    switch (kind)
    {
    case dataPoint::boolean:
        dp = new boolData(sk,false, sk);
        break;
        case dataPoint::formula:
        dp = new formulaData(sk, "", sk);
        break;
        case dataPoint::number:
        dp = new numberData(sk, 0, sk);
        break;
        case dataPoint::color:
        dp = new colorData(sk, colorconv(0, 0, 0, 255), sk);
        break;
        case dataPoint::image:
        dp = new imageData(sk, "", sk);
        break;
        case dataPoint::collection:{
        std::vector<vertInfo> v;
        dp = new collectionData(sk, v, sk);}
        break;
        case dataPoint::font:
        dp = new fontData(sk, "", sk);
        break;
        case dataPoint::text:
        dp = new textData(sk, "", sk);
        break;
        case dataPoint::matrix:
        dp = new matrixData(sk, nullptr, sk);
        break;
        case dataPoint::choice:
        dp = new choiceData(sk, 0, sk);
        break;
    }
    properties.push_back(dp);
    return dp;
}
dataPoint * branch::getPropertyDontAdd(std::string sk, dataPoint::type kind)
{
    for(int i=0;i<properties.size();i++)
        if (properties[i]->getName()==sk)
            if (properties[i]->getType()==kind)
                return properties[i];
    return nullptr;
}

int branch::getId()
{
    return id;
}
void branch::setId(int i)
{
    id = i;
}
int branch::getParentid()
{
    return parentid;
}
void branch::setParentid(int i)
{
    parentid = i;
}
std::string branch::getNode()
{
    return node;
}
void branch::setNode(std::string n)
{
    node = n;
}
std::vector<dataPoint *> branch::getProperties()
{
    return properties;
}
dataPoint * branch::getPropertyByName(std::string n)
{
    for(int i=0;i<properties.size();i++)
        if (properties[i]->getName()==n)
            return properties[i];
    return nullptr;
}
dataPoint * branch::getPropertyById(int i)
{
    if (i>=0&&i<properties.size())
        return properties[i];
    return nullptr;
}
bool branch::addAProperty(dataPoint * prop)
{
    if (prop!=nullptr)
    {
        properties.push_back(prop);
        return true;
    }
    return false;
}
void branch::deleteAllProperties()
{
    for(int i=0;i<properties.size();i++)
        delete properties[i];
    properties.clear();
}
void branch::addChild()
{
    branch * nbr = new branch(id+1, ptree);
    nbr->setParent(this);
    children.push_back(nbr);
}
void branch::addChild(branch * nbr)
{
    if (nbr==nullptr)
        return;
    nbr->setParent(this);
    children.push_back(nbr);
}
std::string branch::deleteThis()
{
    if (parent==nullptr)
        return "Can't delete the root branch.";
    for(int i=0;i<children.size();i++)
        parent->addChild(children[i]);
    parent->removeChild(this);
    delete this;
    return "";
}
void branch::removeChild(branch * nbr)
{
    for(int i=0;i<children.size();i++)
        if (nbr==children[i])
            children.erase(children.begin()+i);
}
void branch::removeChildren()
{
    children.erase(children.begin(), children.end());
}
std::string branch::swapWith(branch * target)
{
    if (target==nullptr)
        return "No target selected.";
    branch * cycleCheck = target;
    while (cycleCheck->getParent()!=nullptr)
    {
        if (cycleCheck->getParent()==this)
            return "The two branches are in ancestor-descendant relationship.";
        cycleCheck = cycleCheck->getParent();
    }
    cycleCheck = this;
    while (cycleCheck->getParent()!=nullptr)
    {
        if (cycleCheck->getParent()==target)
            return "The two branches are in ancestor-descendant relationship.";
        cycleCheck = cycleCheck->getParent();
    }
    int thisId, targetId;
    if (parent==nullptr)
    {
        return "The root branch can't be swapped.";
        if (!ptree->setRootBranch(target))
            return "Couldn't swap root branch.";
        targetId = target->getParent()->getIdOf(target);
        if (targetId<0)
            ptree->setRootBranch(this);
        if (!target->getParent()->setIdTo(targetId, this))
            ptree->setRootBranch(this);
        target->setParent(nullptr);
        return "";
    }
    if (target->getParent()==nullptr)
    {
        return "The target root branch can't be swapped.";
        if (!ptree->setRootBranch(this))
            return "Couldn't swap target root branch.";
        thisId = parent->getIdOf(this);
        if (thisId<0)
            ptree->setRootBranch(target);
        if (!parent->setIdTo(thisId, target))
            ptree->setRootBranch(target);
        setParent(nullptr);
        return "";
    }
    targetId = target->getParent()->getIdOf(target);
    thisId = parent->getIdOf(this);
    if (thisId<0 || targetId<0)
    {
        return "Either branch precedes the root.";
    }
    if (!parent->setIdTo(thisId, target))
    {
        return "Couldn't swap parent with target.";
    }
    if (!target->getParent()->setIdTo(targetId, this))
    {
        parent->setIdTo(thisId, this);
    }
    branch * tp = parent;
    setParent(target->getParent());
    target->setParent(tp);
    return "";
}
std::string branch::makeChildOf(branch * target)
{
    if (target==nullptr)
        return "No target selected.";
    branch * cycleCheck = target;
    while (cycleCheck->getParent()!=nullptr)
    {
        if (cycleCheck->getParent()==this)
            return "The target is a descendant of the branch to be set as a child. A cyclic dependancy would occur.";
        cycleCheck = cycleCheck->getParent();
    }
    parent->removeChild(this);
    parent = target;
    target->addChild(this);
    return "";
}
std::string branch::swapWithOnce(branch * target)
{
    if (target==nullptr)
        return "No target selected.";
    int thisId, targetId;
    if (parent==nullptr)
    {
        if (!ptree->setRootBranch(target))
            return "Couldn't swap root branch.";
        targetId = target->getParent()->getIdOf(target);
        if (targetId<0)
            ptree->setRootBranch(this);
        if (!target->getParent()->setIdTo(targetId, this))
            ptree->setRootBranch(this);
        setParent(target->getParent());
        target->forceSetParent(nullptr);
    }
    else if (target->getParent()==nullptr)
    {
        if (!ptree->setRootBranch(this))
            return "Couldn't swap target root branch.";
        thisId = parent->getIdOf(this);
        if (thisId<0)
            ptree->setRootBranch(target);
        if (!parent->setIdTo(thisId, target))
            ptree->setRootBranch(target);
        target->setParent(parent);
        forceSetParent(nullptr);
    }
    else
    {
        targetId = target->getParent()->getIdOf(target);
        thisId = parent->getIdOf(this);
        if (thisId<0 || targetId<0)
        {
            return "Either one of the parent is not set correctly or one of the branches precedes the root.";
        }
        if (!parent->setIdTo(thisId, target))
        {
            return "Couldn't swap parent with target.";
        }
        if (!target->getParent()->setIdTo(targetId, this))
        {
            parent->setIdTo(thisId, this);
        }
        branch * tp = parent;
        setParent(target->getParent());
        target->setParent(tp);
    }
    branch * dummyBranch = new branch(20,ptree);
    for(int i=0;i<children.size();i++)
    {
        dummyBranch->addChild(children[i]);
    }
    removeChildren();
    std::vector<branch *> otherChildren = target->getChildren();
    for (int i=0;i<otherChildren.size();i++)
    {
        addChild(otherChildren[i]);
    }
    target->removeChildren();
    std::vector<branch *> dummyChildren = dummyBranch->getChildren();
    for (int i=0;i<dummyChildren.size();i++)
        target->addChild(dummyChildren[i]);
    delete dummyBranch;
    return "";
}
std::string branch::makeChildOfOnce(branch * target)
{
    if (target==nullptr)
        return "No target selected.";
    branch * cycleCheck = target;
    while (cycleCheck->getParent()!=nullptr)
    {
        if (cycleCheck->getParent()==this)
            return "The target is a descendant of the branch to be set as a child. A cyclic dependancy would occur.";
        cycleCheck = cycleCheck->getParent();
    }
    for(int i=0;i<children.size();i++)
    {
        parent->addChild(children[i]);
        removeChild(children[i]);
    }
    parent->removeChild(this);
    parent = target;
    target->addChild(this);
    return "";
}
std::string branch::uprootThis()
{
    if (parent==nullptr)
        return "Can't delete the root branch.";
    for(int i=0;i<children.size();i++)
        children[i]->uprootThis();
    delete this;
    return "";
}

std::string branch::moveUp()
{
    if (parent==nullptr)
        return "This operation cannot be applied to the root";
    return parent->moveChildUp(this);
}
std::string branch::moveDown()
{
    if (parent==nullptr)
        return "This operation cannot be applied to the root";
    return parent->moveChildDown(this);
}

std::string branch::addSibling(branch * other)
{
    if (other==nullptr)
        return "No other branch selected";
    if (parent==nullptr)
        return "This operation cannot be applied to the root";
    return parent->insertChildBetween(this, other);
}
std::string branch::moveChildUp(branch * target)
{
    if (target==nullptr)
        return "No target selected";
    for(int i=0;i<children.size();i++)
    {
        if (children[i]==target)
        {
            if (i<children.size()-1)
            {
                branch * temp = children[i+1];
                children[i+1] = children[i];
                children[i] = temp;
                return "";
            }
            else
                return "";//"The selected branch is already at outermost position.";
        }
    }
    return "The selected branch couldn't be found in its parent children. Integrity is violated.";
}
std::string branch::moveChildDown(branch * target)
{
    if (target==nullptr)
        return "No target selected";
    for(int i=0;i<children.size();i++)
    {
        if (children[i]==target)
        {
            if (i>0)
            {
                branch * temp = children[i-1];
                children[i-1] = children[i];
                children[i] = temp;
                return "";
            }
            else
                return ""; // "The selected branch is already at outermost position.";
        }
    }
    return "The selected branch couldn't be found in its parent children. Integrity is violated.";
}
std::string branch::insertChildBetween(branch * target1, branch * target2)
{
    if (target1==nullptr || target2==nullptr)
        return "No targets selected";
    for(int i=0;i<children.size();i++)
    {
        if (children[i]==target1)
        {
            if (i>0)
            {
                if (children[i-1]==target2)
                {
                    branch * neu = new branch(id+1, ptree);
                    neu->setParent(this);
                    children.insert(children.begin()+i,neu);
                    return "";
                }
            }
        }
        if (children[i]==target2)
        {
            if (i>0)
            {
                if (children[i-1]==target1)
                {
                    branch * neu = new branch(id+1, ptree);
                    neu->setParent(this);
                    children.insert(children.begin()+i,neu);
                    return "";
                }
            }
        }
    }
    return "The two selected branches aren't next to each other.";
}
std::string branch::insertChild(branch * other)
{
    if (other==nullptr)
        return "No second branch selected";
    if (other->getParent()!=this)
        return "The second branch isn't a direct child of the first one";
    branch * nbr = new branch(id+1, ptree);
    nbr->setParent(this);
    for(int i=0;i<children.size();i++)
    {
        if (children[i]==other)
        {
            children[i]=nbr;
            break;
        }
    }
    nbr->addChild(other);
    return "";
}

int branch::getIdOf(branch * br)
{
    for(int i=0;i<children.size();i++)
        if (br==children[i])
            return i;
    return -1;
}
bool branch::setIdTo(int id, branch *br)
{
    if (id<0||id>=children.size())
        return false;
    if (br==nullptr)
        return false;
    children[id] = br;
    return true;
}
void branch::calculateMetrics(int _n, shape * bboxShape, formulaParser * f, drawingAPI * dapi)
{
    if (f==nullptr||dapi==nullptr)
        return ;
    pw = ph = pc = 0;
    for(int i=0;i<children.size();i++)
        children[i]->calculateMetrics(i+1, bboxShape, f, dapi);
    sc = 0;
    n = _n;
    dapi->calcCoordsForBranch(this);
    if (bboxShape!=nullptr)
    {
        f->setBranch(this);
        dataPoint * dp = bboxShape->getPropertyByName("width");
        if (dp!=nullptr)
        if (dp->getType()==dataPoint::formula)
        {
            w = f->parse(((formulaData *)dp));
        }

        dp = bboxShape->getPropertyByName("height");
        if (dp!=nullptr)
        if (dp->getType()==dataPoint::formula)
        {
            h = f->parse(((formulaData *)dp));
        }
        // the formulas should be constant
    }
    dw = udw = 0;
    dh = udh = 0;
    dc = udc = 0;
    if (children.size()==0)
    {
        udw = w;
        udh = h;
        udc = 1;
    }

    setdc(children.size());

    for(int i=0;i<children.size();i++)
    {
        children[i]->setpc(udc);
        children[i]->setpw(udw);
        children[i]->setph(udh);
        dw += children[i]->getw();
        dh += children[i]->geth();
        if (children[i]->getudw()>0)
            udw += children[i]->getudw();
        else
            udw += children[i]->getw();
        if (children[i]->getudh()>0)
            udh += children[i]->getudh();
        else
            udh += children[i]->geth();
        if (children[i]->getudc()>0)
            udc += children[i]->getudc();
        else
            udc ++;
    }

    //some bug fixes in the default styles
    if (udw < w) udw = w;
    if (udh < h) udh = h;

    if (parent ==nullptr)
    {
        sw = udw;
        sh = udh;
        sc = udc;
    }

    for(int i=0;i<children.size();i++)
        {
        children[i]->setsc(udc); // dc
        children[i]->setsw(udw); // dw
        children[i]->setsh(udh); // dh
        }
}

void branch::callDraw(drawingAPI * dapi)
{
    dapi->drawForBranch(this);
    for(int i=0;i<children.size();i++)
        children[i]->callDraw(dapi);
}
void branch::setRoots(drawingAPI * dapi)
{

}
void branch::setBrPoints(drawingAPI * dapi)
{

}
branch * branch::getClickedBranch(int cx, int cy)
{
    branch * br;
    for(int i=0;i<children.size();i++)
        {
            br = children[i]->getClickedBranch(cx, cy);
            if (br!=nullptr)
                return br;
        }
    if (cx>x+rootx&&cy>y+rooty&&cx<x+rootx+w&&cy<y+rooty+h)
        return this;
    return nullptr;
}

void branch::setChildrenRootXY(int rx, int ry)
{
    for(int i=0;i<children.size();i++)
    {
        children[i]->setrootx(rx);
        children[i]->setrooty(ry);
    }
}
int branch::getn()
{
    return n;
}
int branch::getw()
{
    return w;
}
int branch::geth()
{
    return h;
}
int branch::getx()
{
    return x;
}
int branch::gety()
{
    return y;
}
int branch::getrootx()
{
    return rootx;
}
int branch::getrooty()
{
    return rooty;
}
int branch::getsc()
{
    return sc;
}
int branch::getsw()
{
    return sw;
}
int branch::getsh()
{
    return sh;
}
int branch::getpc()
{
    return pc;
}
int branch::getpw()
{
    return pw;
}
int branch::getph()
{
    return ph;
}
int branch::getdc()
{
    return dc;
}
int branch::getdw()
{
    return dw;
}
int branch::getdh()
{
    return dh;
}
int branch::getudc()
{
    return udc;
}
int branch::getudw()
{
    return udw;
}
int branch::getudh()
{
    return udh;
}
void branch::setn(int value)
{
    n = value;
}
void branch::setW(int value)
{
    w = value;
}
void branch::seth(int value)
{
    h = value;
}
void branch::setx(int value)
{
    x = value;
}
void branch::sety(int value)
{
    y = value;
}
void branch::setrootx(int value)
{
    rootx = value;
}
void branch::setrooty(int value)
{
    rooty = value;
}
void branch::setsc(int value)
{
    sc = value;
}
void branch::setsw(int value)
{
    sw = value;
}
void branch::setsh(int value)
{
    sh = value;
}
void branch::setpc(int value)
{
    pc = value;
}
void branch::setpw(int value)
{
    pw = value;
}
void branch::setph(int value)
{
    ph = value;
}
void branch::setdc(int value)
{
    dc = value;
}
void branch::setdw(int value)
{
    dw = value;
}
void branch::setdh(int value)
{
    dh = value;
}
void branch::setudc(int value)
{
    udc = value;
}
void branch::setudw(int value)
{
    udw = value;
}
void branch::setudh(int value)
{
    udh = value;
}

branch * branch::getParent()
{
    return parent;
}
bool branch::setParent(branch * p)
{
    if (p!=nullptr)
    {
        parent = p;
        return true;
    }
    return false;
}
void branch::forceSetParent(branch * p)
{
    parent = p;
}

std::vector<branch *> branch::getChildren()
{
    return children;
}
bool branch::addAChild(branch * ch)
{
    addChild(ch);
    return true;
    if (ch==nullptr)
        return false;
    children.push_back(ch);
    return true;
}


std::string branch::save(std::ofstream& _out)
{
    if (!saved)
    {
        saved = true;
        std::string errr="";
        wi(_out,properties.size());
        for (dataPoint * obj : properties)
        {
            errr=obj->save(_out);
            if (errr!="")
                return errr;
        }
        wi(_out,children.size());
        for (branch * obj : children)
        {
            errr=obj->save(_out);
            if (errr!="")
                return errr;
        }
        return "";
    }
    else
        {
        return "Cyclic dependency in nodes - will abort saving and cycle begining.";
        }
}
std::string branch::load(std::ifstream& _in, std::string version)
{
    if (version == "1.0.0")
    {
        int s;
        s = ri(_in);
        for (dataPoint* obj : properties)
            delete obj;
        properties.clear();
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
                    properties.push_back(nd);}
                break;
                case dataPoint::number:{
                    numberData * nd = new numberData("",0,"");
                    nd->set_intsize(_intsize);
                    a = nd->load(_in, version);
                    if (a!="") return a;
                    properties.push_back(nd);}
                break;
                case dataPoint::color:{
                    colorData * nd = new colorData("",colorconv(0,0,0,0),"");
                    nd->set_intsize(_intsize);
                    a = nd->load(_in, version);
                    if (a!="") return a;
                    properties.push_back(nd);}
                break;
                case dataPoint::image:{
                    imageData * nd = new imageData("","","");
                    nd->set_intsize(_intsize);
                    a = nd->load(_in, version);
                    if (a!="") return a;
                    properties.push_back(nd);}
                break;
                case dataPoint::boolean:{
                    boolData * nd = new boolData("",true,"");
                    nd->set_intsize(_intsize);
                    a = nd->load(_in, version);
                    if (a!="") return a;
                    properties.push_back(nd);}
                break;
                case dataPoint::collection:{
                    std::vector<vertInfo> v;
                    collectionData * nd = new collectionData("",v,"");
                    nd->set_intsize(_intsize);
                    a = nd->load(_in, version);
                    if (a!="") return a;
                    properties.push_back(nd);}
                break;
                case dataPoint::font:{
                    fontData * nd = new fontData("","","");
                    nd->set_intsize(_intsize);
                    a = nd->load(_in, version);
                    if (a!="") return a;
                    properties.push_back(nd);}
                break;
                case dataPoint::text:{
                    textData * nd = new textData("","","");
                    nd->set_intsize(_intsize);
                    a = nd->load(_in, version);
                    if (a!="") return a;
                    properties.push_back(nd);}
                break;
                case dataPoint::matrix:{
                    matrixData * nd = new matrixData("",nullptr,"");
                    nd->set_intsize(_intsize);
                    a = nd->load(_in, version);
                    if (a!="") return a;
                    properties.push_back(nd);}
                break;
                case dataPoint::choice:{
                    choiceData * nd = new choiceData("",0,"");
                    nd->set_intsize(_intsize);
                    a = nd->load(_in, version);
                    if (a!="") return a;
                    properties.push_back(nd);}
                break;
            }

        }

        s = ri(_in);
        for (int i=0;i<s;i++)
        {
            branch * loadedBranch;
            loadedBranch = new branch(id+1, ptree);
            loadedBranch->set_intsize(_intsize);
            loadedBranch->setParent(this);
            loadedBranch->setParentid(id);
            std::string sc = loadedBranch->load(_in, version);
            if (sc!="")
                return sc;
            children.push_back(loadedBranch);
        }
    }
    else
        return "Unsupported program version: "+version;
    return "";
}






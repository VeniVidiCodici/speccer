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



#include "nodedata.h"

/*
Is this class actually used somewhere?
Not sure, somebody check it out.
*/

nodeData::nodeData(nodeData * higherNode)
{
    parent=higherNode;
}

nodeData::~nodeData()
{
    for (nodeData* obj : children)
        delete obj;
    children.clear();
    for (dataPoint* obj : data)
        delete obj;
    data.clear();
}
bool nodeData::dataExists(std::string dataName)
{
    for (dataPoint * obj : data)
        {
            if(obj->getName()==dataName)
                return true;
        }
    return false;
}
dataPoint * nodeData::getData(std::string dataName)
{
    for (dataPoint * obj : data)
        {
            if(obj->getName()==dataName)
                return obj;
        }
    return nullptr;
}
int nodeData::getNumberOfDataPoints()
{
    return data.size();
}
dataPoint * nodeData::getNthDataPoint(int n)
{
    if (n>static_cast<int>(data.size())||n<1)
        return nullptr;
    return data[n-1];
}
bool nodeData::itIsRoot()
{
    return (parent==nullptr);
}
int nodeData::getNumberOfChildren()
{
    return children.size();
}
nodeData * nodeData::getNthChild(int n)
{
    if (n>static_cast<int>(children.size())||n<1)
        return nullptr;
    return children[n-1];
}
bool nodeData::moveNthChildUpInSiblingsHierarchy(int n)
{
    n--;
    if (n>static_cast<int>(children.size())-2||n<0)
        return false;
    nodeData * temp = children[n];
    children[n]=children[n+1];
    children[n+1]=temp;
    return true;
}
bool nodeData::moveNthChildDownInSiblingsHierarchy(int n)
{
    n--;
    if (n>static_cast<int>(children.size())-1||n<1)
        return false;
    nodeData * temp = children[n];
    children[n]=children[n-1];
    children[n-1]=temp;
    return true;
}
bool nodeData::removeChild(nodeData * child)
{
    for(int i=0;i<static_cast<int>(children.size());i++)
        if (children[i]==child)
            {
            children.erase(children.begin()+i);
            return true;
            }
    return false;
}
bool nodeData::removeChild(int n)
{
    n--;
    if (n>static_cast<int>(children.size())-1||n<0)
        return false;
    children.erase(children.begin()+n);
    return true;
}
bool nodeData::addChild(nodeData * child)
{
    if (child==nullptr)
        return false;
    children.push_back(child);
    child->parent = this;
    return true;
}
bool nodeData::changeParent(nodeData * newParent)
{
    if (newParent==nullptr)
        return false;
    parent->removeChild(this);
    parent=newParent;
    parent->addChild(this);
    return true;
}
bool nodeData::itIsAncestorOf(nodeData * thisOne)
{
    if (thisOne==nullptr)
        return false;
    return thisOne->itIsDescendantOf(this);
}
bool nodeData::itIsDescendantOf(nodeData * thisOne)
{
    if (thisOne==nullptr)
        return false;
    if (parent==thisOne)
        return true;
    return false;
}
bool nodeData::removeThisNodeAsLink()
{
    if (parent==nullptr)
        return false;
    for (nodeData* obj : children)
    {
        parent->addChild(obj);
    }
    while(children.size()>0)
    {
        nodeData * temp = children[0];
        children.erase(children.begin());
        delete temp;
    }
    delete this;
    return true;
}
bool nodeData::addDataPoint(dataPoint * newDataPoint)
{
    if (newDataPoint==nullptr)
        return false;
    data.push_back(newDataPoint);
    return true;
}

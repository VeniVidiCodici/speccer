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



#ifndef NODEDATA_H
#define NODEDATA_H
#include "booldata.h"
#include "colordata.h"
#include "formuladata.h"
#include "numberdata.h"
#include "drawable.h"
#include <vector>

class nodeData
{
    public:
        nodeData(nodeData * higherNode);
        ~nodeData();
        bool dataExists(std::string dataName);
        dataPoint * getData(std::string dataName);
        int getNumberOfDataPoints();
        dataPoint * getNthDataPoint(int n);
        bool itIsRoot();
        int getNumberOfChildren();
        nodeData * getNthChild(int n);
        bool moveNthChildUpInSiblingsHierarchy(int n);
        bool moveNthChildDownInSiblingsHierarchy(int n);
        bool removeChild(nodeData * child);
        bool removeChild(int n);
        bool addChild(nodeData * child);
        bool changeParent(nodeData * newParent);
        bool itIsAncestorOf(nodeData * thisOne);
        bool itIsDescendantOf(nodeData * thisOne);
        bool removeThisNodeAsLink();
        bool addDataPoint(dataPoint * newDataPoint);
    protected:
    private:
        nodeData * parent;
        std::vector<nodeData *> children;
        std::vector<dataPoint *> data;
};

#endif // NODEDATA_H

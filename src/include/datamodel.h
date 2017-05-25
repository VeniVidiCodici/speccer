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



#ifndef DATAMODEL_H
#define DATAMODEL_H
#include "shape.h"
#include "composite.h"
#include <map>
#include <vector>
#include <istream>
#include <fstream>
#include <string>
#include "alldatatypes.h"


class dataHolder;
class UIAPI;
class drawingAPI;

class dataModel : public serialisable
{
    public:
        dataModel(UIAPI * u, drawingAPI * j);
        ~dataModel();
        drawable * getDrawableByName(std::string name);
        bool drawableExists(std::string name);
        std::vector<drawable *> getAllDrawables();

        bool saveAsStyle(std::string filename);
        bool saveStyle(std::string filename);
        bool openStyle(std::string filename);

        dataHolder * addANewNode(std::string name, bool switchToIt);
        dataHolder * addANewComposite(std::string name, bool switchToIt);
        bool switchToDataHolder(dataHolder * toThisOne);
        void setAPIs(UIAPI * u, drawingAPI * j);
        bool nodeExists(std::string name);
        bool compositeExists(std::string name);

        std::vector<dataHolder *> getNodes();
        dataHolder * getCurrentNode();
        void removeAllNodes();

        dataHolder * getNodeByName(std::string dname);

        std::string getName();
        std::string save(std::ofstream& _out);
        std::string load(std::ifstream& _in, std::string version);
    protected:
    private:
        std::string styleName;
        void loadCompositeShapes();
        void importComposite(std::string path);
    	std::map<std::string, drawable *> drawables;
    	std::vector<dataHolder *> nodes;
    	std::vector<dataHolder *> composites;
    	dataHolder * currentNode;
    	UIAPI * uapi;
    	drawingAPI * japi;
    	bool firstTimeShowing; //cheap quick fix for an issue
};
#endif // DATAMODEL_H

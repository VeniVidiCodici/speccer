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



#ifndef DATAHOLDER_H
#define DATAHOLDER_H
#include "datamodel.h"
#include "uiapi.h"
#include "drawingapi.h"
#include <string>
#include "nodedata.h"

#include "alldatatypes.h"
#include <map>

#include "serialisable.h"
#include "shape.h"

class styleframe;

class dataHolder : public serialisable
{
    public:
        dataHolder(UIAPI * userInterface, drawingAPI * drawer, dataModel * modelArg);
        ~dataHolder();

        drawable * getDrawableByName(std::string drName);
        shape * getShapeByType(shape::shapeType kind);
        bool drawableExists(std::string drName);
        std::vector<drawable *> getAllDrawables();
        bool addNewDrawableFromModel(std::string thisOne);
        bool removeDrawable(std::string thisOne);
        bool renameDrawable(std::string currentName, std::string newName);
        void removeAllDrawables();
        void addANewDrawable(drawable * d);
        bool swapDrawables(int id1, int id2);
        int getDrawableIdByName(std::string drName);
        drawable * getDrawableById(int id);
        int getNumberOfDrawables();



        bool drawableExistsInModel(std::string drName);
        drawable * getDrawableFromModelByName(std::string drName);
        std::vector<drawable *> getAllDrawablesInModel();

        void redraw();
        bool saveComposite(std::string filename);
        bool openComposite(std::string filename);
        bool openStyle(std::string filename);
        bool saveStyle(std::string filename);

        std::vector<formulaData *> getAllCustomFields();
        std::vector<dataPoint *> getAllCustomFieldsDP();
        void removeAllCustomFields();
        std::string getField(std::string sk, std::string defaultValue);
        std::string setField(std::string sk, std::string value);
        bool refreshFieldsFGS();

        dataModel * getDataModel(){return model;}
        UIAPI * getUIAPI(){return uapi;}
        drawingAPI * getDAPI(){return japi;}
        nodeData * getRoot(){return root;}

        void setdbgstr(std::string arg);
        void setName(std::string newName);
        std::string getName();
        void setFilepath(std::string newfilepath);
        std::string getFilepath();
        void setSaved(bool s);
        bool getSaved();
        void setChanged(bool c);
        bool getChanged();
        std::string save(std::ofstream& _out);
        std::string load(std::ifstream& _in, std::string version);

        std::vector<dataPointUnit> getSurfaceDataPoints();

    protected:
    private:
        std::string name, filepath;
        bool saved, changed;
        std::vector<drawable *> drawablesv;
        std::map<shape *, int> tfmids;
        std::map<std::string, formulaData *> customFields;
        std::map<std::string, std::string> customFieldValues;
        std::map<std::string, dataPoint *> customControls;
        dataModel * model;
        UIAPI * uapi;
        drawingAPI * japi;
        nodeData * root;

        bool errorOpenFile(std::string why);
};

#endif // DATAHOLDER_H

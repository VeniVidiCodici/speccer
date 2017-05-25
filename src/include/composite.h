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



#ifndef COMPOSITE_H
#define COMPOSITE_H

#include <drawable.h>
#include <shape.h>
#include "alldatatypes.h"

class composite : public drawable
{
    public:
        composite(std::string whatName);
        ~composite();
        std::vector<shape *> getShapes();
        std::vector<dataPoint *> getProperties();
        std::vector<drawable *> getDrawables();
        void updateProperties();
        bool isSimplex(){return false;};
        bool addCompositeProperty(dataPoint * property);
        bool addElement(drawable * element);
        bool addDependency(dataPoint * dependent, dataPoint * dependedOn);
        bool propertyOrShapeExists(std::string propName);
        bool propertyExists(std::string propName);
        bool elementExists(std::string elementName);
        drawable * getElementByName(std::string elementName);
        dataPoint * getPropertyByName(std::string propName);
        dataPoint * getPropertyByNameAndType(std::string propName, dataPoint::type propType);
        bool itIsCritical();
        void duplicate();
        void endDuplication();
        std::string save(std::ofstream& _out);
        std::string load(std::ifstream& _in, std::string version);

        std::vector<dataPointUnit> getSurfaceDataPoints();
        void loadSurfaceDataPoints();
    protected:
    private:
        std::vector<dataPoint *> compositeProperties;
        std::vector<drawable *> elements;
        bool loadedSurfaceDataPoints;
};

#endif // COMPOSITE_H

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



#ifndef SHAPE_H
#define SHAPE_H
#include "alldatatypes.h"
#include <vector>
#include "drawable.h"

class shape : public drawable
{
    public:
        enum shapeType {circle, rectangle, convex, points, image, text, matrix, root, anchor, brpoint, bbox,
        roundrect, arc, invalid};
        shape(std::string whatName, shapeType kind);
        shape(std::string whatName, shapeType kind, bool irrelevant);
        ~shape();
        std::vector<shape *> getShapes();
        std::vector<dataPoint *> getProperties();
        bool isSimplex(){return true;};
        bool propertyExists(std::string propName);
        dataPoint * getPropertyByName(std::string propName);
        void duplicate();
        void endDuplication();
        void addPropertyDuringDuplication(dataPoint * newProperty);
        shapeType getType(){return type;};
        bool itIsCritical();
        std::string dg;
        std::string typeToString(shapeType t)
        {
            switch(t)
            {
                case circle: return "circle";
                case rectangle: return "rectangle";
                case convex: return "convex";
                case points: return "points";
                case image: return "image";
                case matrix: return "matrix";
                case root: return "root";
                case text: return "text";
                case anchor: return "anchor";
                case brpoint: return "brpoint";
                case bbox: return "bbox";
                case roundrect: return "roundrect";
                case arc: return "arc";
                case invalid:
                default: return "invalid";
            }
        };
        shapeType stringToType(std::string t)
        {
            if (t== "circle") return circle;
            if (t== "rectangle") return rectangle;
            if (t== "convex") return convex;
            if (t== "image") return image;
            if (t== "points") return points;
            if (t== "root") return root;
            if (t== "anchor") return anchor;
            if (t== "text") return text;
            if (t== "matrix") return matrix;
            if (t== "brpoint") return brpoint;
            if (t== "bbox") return bbox;
            if (t== "roundrect") return roundrect;
            if (t== "arc") return arc;
            return invalid;
        };
        std::string save(std::ofstream& _out);
        std::string load(std::ifstream& _in, std::string version);

        std::vector<dataPointUnit> getSurfaceDataPoints();
    protected:
    private:
        std::vector<dataPoint *> shapeProperties;
        shapeType type;
        void addPositionProperties();
        void addShapeProperties();
        void addTransformationProperties();
};

#endif // SHAPE_H

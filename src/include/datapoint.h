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



#ifndef DATAPOINT_H
#define DATAPOINT_H
#include "colorconv.h"
#include "serialisable.h"

class dataPoint : public serialisable
{
    public:
        dataPoint(){dependedOn=nullptr;duplicated=nullptr;surfaceName="";}
        virtual ~dataPoint(){};
        enum type {formula, number, color, image, boolean, collection, font, text, matrix, choice, invalid};
        type getType(){return kind;};
        std::string getName(){return name;};
        std::string getPrompt(){return prompt;};
        bool itIsConstant(){return constant;};
        bool itIsProperty(){return property;};
        void setConstant(bool value){constant=value;};
        void setProperty(bool value){property=value;};
        bool itIsThisOne(std::string withThisname,type withThisType)
            {return (withThisType==kind&&withThisname==name);};
        bool setDependantOn(dataPoint * thisOne)
        {
            if (thisOne==nullptr)
                {
                    dependedOn = nullptr;
                    return true;
                }
            if (thisOne->getType()!=kind)
                return false;
            dependedOn = thisOne;
            return true;
        };
        void clearDependency(){dependedOn=nullptr;};
        virtual void updateDependency()=0;
        virtual void duplicate()=0;
        dataPoint * getDuplicated(){return duplicated;};
        void endDuplication(){duplicated=nullptr;};
        std::string typeToString(type t)
        {
            switch(t)
            {
                case formula: return "formula";
                case number: return "number";
                case color: return "color";
                case image: return "image";
                case boolean: return "boolean";
                case collection: return "collection";
                case font: return "font";
                case text: return "text";
                case matrix: return "matrix";
                case choice: return "choice";
                case invalid:
                default: return "invalid";
            }
        };
        type stringToType(std::string t)
        {
            if (t== "formula") return formula;
            if (t== "number") return number;
            if (t== "color") return color;
            if (t== "image") return image;
            if (t== "boolean") return boolean;
            if (t== "collection") return collection;
            if (t== "font") return font;
            if (t== "text") return text;
            if (t== "matrix") return matrix;
            if (t== "choice") return choice;
            return invalid;
        };
        virtual std::string save(std::ofstream& _out) = 0;
        virtual std::string load(std::ifstream& _in, std::string version) = 0;
        std::string getSurfaceName(){return surfaceName;};
        void setSurfaceName(std::string sn){surfaceName = sn;};
        void clearSurfaceName(){surfaceName = "";};
    protected:
        type kind;
        std::string name, prompt, surfaceName;
        // the two variables below are actually not used
        bool constant;//!< Whether or not the datapoint is editable in the *style* editor.
        bool property;//!< Whether or not the datapoint is editable in the *tree* editor.
        dataPoint * dependedOn;
        dataPoint * duplicated;
    private:
};

struct dataPointUnit
{
    std::string name;
    dataPoint::type kind;
};

#endif // DATAPOINT_H

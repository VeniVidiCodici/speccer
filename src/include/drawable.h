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



#ifndef DRAWABLE_H
#define DRAWABLE_H
#include <vector>
#include <map>
#include <string>
#include "serialisable.h"
#include "alldatatypes.h"

class shape;
class dataPoint;

class drawable : public serialisable
{
    public:
        drawable();
        virtual ~drawable();
        virtual std::vector<shape *> getShapes()=0;
        virtual std::vector<dataPoint *> getProperties()=0;
        virtual bool isSimplex()=0;
        virtual bool propertyExists(std::string propName)=0;
        virtual dataPoint * getPropertyByName(std::string propName)=0;
        virtual void duplicate()=0;
        drawable * getDuplicated(){return duplicated;}
        virtual void endDuplication()=0;
        std::string getName(){return name;};
        void setName(std::string newName){name=newName;}
        virtual bool itIsCritical()=0;

        virtual std::vector<dataPointUnit> getSurfaceDataPoints()=0;

        double getx(){return _x;};
        double gety(){return _y;};
        double getw(){return _w;};
        double geth(){return _h;};
        double gettx(){return _tx;};
        double getty(){return _ty;};
        double gettw(){return _tw;};
        double getth(){return _th;};
        void setx(double v){_x=v;};
        void sety(double v){_y=v;};
        void setW(double v){_w=v;};
        void seth(double v){_h=v;};
        void settx(double v){_tx=v;};
        void setty(double v){_ty=v;};
        void settw(double v){_tw=v;};
        void setth(double v){_th=v;};

        virtual std::string save(std::ofstream& _out) = 0;
        virtual std::string load(std::ifstream& _in, std::string version) = 0;
    protected:
        drawable * duplicated;
        std::string name;
        bool critical;
        double _x, _y, _w, _h, _tx, _ty, _tw, _th;
    private:
};

#endif // DRAWABLE_H

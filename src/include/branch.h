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



#ifndef BRANCH_H
#define BRANCH_H
#include "alldatatypes.h"
#include <string>
#include <vector>
#include <map>
#include "drawingapi.h"
#include "shape.h"

class formulaParser;
class tree;
class drawingAPI;

class branch : public serialisable
{
    public:
        branch(int i, tree * arg);
        ~branch();

        std::string save(std::ofstream& _out);
        std::string load(std::ifstream& _in, std::string version);
        void uproot();
        void setSaved(bool isItSaved);
        void massSetSaved(bool isItSaved);

        int getId();
        void setId(int i);
        int getParentid();
        void setParentid(int i);
        std::string getNode();
        void setNode(std::string n);
        std::vector<dataPoint *> getProperties();
        dataPoint * getPropertyByName(std::string n);
        dataPoint * getPropertyById(int i);
        bool addAProperty(dataPoint * prop);
        void deleteAllProperties();

        void calculateMetrics(int _n, shape * bboxShape, formulaParser * f, drawingAPI * dapi);
        void callDraw(drawingAPI * dapi);

        void setRoots(drawingAPI * dapi);
        void setBrPoints(drawingAPI * dapi);

        void setChildrenRootXY(int rx, int ry);

        int getn();
        int getw();
        int geth();
        int getx();
        int gety();
        int getrootx();
        int getrooty();
        int getsc();
        int getsw();
        int getsh();
        int getpc();
        int getpw();
        int getph();
        int getdc();
        int getdw();
        int getdh();
        int getudc();
        int getudw();
        int getudh();
        void setn(int value);
        void setW(int value);
        void seth(int value);
        void setx(int value);
        void sety(int value);
        void setrootx(int value);
        void setrooty(int value);
        void setsc(int value);
        void setsw(int value);
        void setsh(int value);
        void setpc(int value);
        void setpw(int value);
        void setph(int value);
        void setdc(int value);
        void setdw(int value);
        void setdh(int value);
        void setudc(int value);
        void setudw(int value);
        void setudh(int value);

        branch * getParent();
        bool setParent(branch * p);
        void forceSetParent(branch * p);

        std::vector<branch *> getChildren();
        bool addAChild(branch * ch);

        branch * getClickedBranch(int cx, int cy);

        void addChild();
        void addChild(branch * nbr);
        void removeChild(branch * nbr);
        void removeChildren();
        std::string deleteThis();
        std::string swapWith(branch * target);
        std::string makeChildOf(branch * target);

        std::string swapWithOnce(branch * target);
        std::string makeChildOfOnce(branch * target);
        std::string uprootThis();

        std::string moveUp();
        std::string moveDown();

        std::string addSibling(branch * other);
        std::string insertChild(branch * other);

        std::string moveChildUp(branch * target);
        std::string moveChildDown(branch * target);
        std::string insertChildBetween(branch * target1, branch * target2);

        int getIdOf(branch * br);
        bool setIdTo(int id, branch *br);

        dataPoint * getProperty(std::string sk, dataPoint::type kind);
        dataPoint * getPropertyDontAdd(std::string sk, dataPoint::type kind);
        bool propertyExists(std::string sk, dataPoint::type kind);

        std::map<formulaData *, double> cache;
        //std::string cacheHash;
        //std::string getCurrentHash();
    protected:
    private:
        int id, parentid;
        bool saved;
        std::string node;
        std::vector<dataPoint *> properties;
        int n, sc, sw, sh, dc, dw, dh, udc, udw, udh, w, h, x, y, rootx, rooty, pc, pw, ph;
        branch * parent;
        tree * ptree;
        std::vector<branch *> children;
};

#endif // BRANCH_H

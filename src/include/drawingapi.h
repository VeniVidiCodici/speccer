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



#ifndef DRAWINGAPI_H
#define DRAWINGAPI_H

#include <string>
#include "tree.h"
#include "branch.h"

class dataHolder;
class drawable;
class formulaParser;
class UIAPI;

struct Matrix
{
    double m[16];
    Matrix()
    {
        m[0]  = 1.0; m[1]  = 0.0; m[2]  = 0.0; m[3]  = 0.0;
        m[4]  = 0.0; m[5]  = 1.0; m[6]  = 0.0; m[7]  = 0.0;
        m[8]  = 0.0; m[9]  = 0.0; m[10] = 1.0; m[11] = 0.0;
        m[12] = 0.0; m[13] = 0.0; m[14] = 0.0; m[15] = 1.0;
    }
};

class drawingAPI
{
    public:
        int x, y;
        formulaParser *f;
        enum window {editor, browser};
        double dragx, dragy;
        double scale,centx,centy, oldcentx, oldcenty, xorig, yorig;
        std::string debug;
        drawingAPI();
        virtual ~drawingAPI();
        virtual void redraw()=0;
        virtual void refresh()=0;
        void setDataHolder(dataHolder * toThisOne){owner=toThisOne;};
        virtual double getULCornerX(drawable * ofThisOne)=0;
        virtual double getULCornerY(drawable * ofThisOne)=0;
        virtual double getWidth(drawable * ofThisOne)=0;
        virtual double getHeight(drawable * ofThisOne)=0;
        virtual void updatesize()=0;
        virtual void resetView()=0;
        virtual void checksize()=0;
        virtual void motion(long x, long y)=0;
        virtual void drawForBranch(branch * br)=0;
        virtual void calcCoordsForBranch(branch * br)=0;
        bool refreshFields;

        virtual void createFormulaParser(bool actualGlobalVariables)=0;
        virtual void updateFormulaParserDataHolder()=0;

        void setWMode(window wm){wMode = wm;};
        window getWMode(){return wMode;};
        void setNodes(tree * tnodes){nodes = tnodes;};
        void setUIAPI(UIAPI * u){uapi = u;};
        virtual void setDrawableCoordinates(drawable * drawn,Matrix Transform, bool globvar)=0;
    protected:
        dataHolder * owner;
        UIAPI * uapi;
        unsigned int w, h;
        virtual void drawDrawable(drawable * drawn,Matrix Transform, bool globvar)=0;
        bool itIsDragged;
        window wMode;
        tree * nodes;
    private:
};

#endif // DRAWINGAPI_H

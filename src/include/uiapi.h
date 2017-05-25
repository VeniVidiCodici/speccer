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



#ifndef UIAPI_H
#define UIAPI_H
#include "drawable.h"
#include "collectiondata.h"
#include <vector>

class dataHolder;
class branch;

class UIAPI
{
    public:
        UIAPI();
        virtual ~UIAPI(); // All UIAPIs should clear the RHS UI when destroyed <!>
        enum window {editor, browser};
        virtual bool buildRHSUIForDrawable(drawable * thisOne)=0;
        virtual bool clearRHSUI()=0;
        virtual bool updateLHSUIFromDataModel()=0;
        virtual bool updateLHSUIPlacedShapes()=0;
        virtual bool buildRHSUIForVertex(collectionData * thisOne, int index)=0;
        virtual bool refreshFieldsFGS()=0;
        virtual bool refreshControlsFGS()=0;
        virtual void prompt(std::string prompt)=0;
        virtual bool clearLHSUIV()=0;
        virtual bool updateLHSUIV(std::vector<vertInfo> * vertices)=0;
        virtual void setDataHolder(dataHolder * toThisOne)=0;
        virtual void addDataHolderToStylesMenu(dataHolder * thisOne)=0;
        virtual void addDataHolderToCompositesMenu(dataHolder * thisOne)=0;
        std::string debugstr;
        virtual void unbug(std::string arg)=0;
        virtual void setTitle(std::string title)=0;
        virtual std::vector<std::string> getAllFilesInFolder(std::string folder)=0;
        virtual std::vector<std::string> getAllFilesInFolder(std::string folder, std::string extension)=0;
        virtual bool buildRHSUIForBranch(branch * thisOne)=0;
        virtual void setSecondaryBranch(branch * toThisOne)=0;
        virtual branch * getPrimaryBranch()=0;
        virtual branch * getSecondaryBranch()=0;
        virtual void resetStylesMenu()=0;
        virtual void setStatusBarString(std::string str, int n)=0;
        void setMode(int newMode){mode=newMode;};
        int getMode(){return mode;};

        void setWMode(window wm){wMode = wm;};
        window getWMode(){return wMode;};
    protected:
        drawable * selectedDrawable;
        dataHolder * owner;
        int mode;// 0 - style editor; 1 - composite editor
        window wMode;
        std::string currentWorkingDirectory;
    private:
};

#endif // UIAPI_H

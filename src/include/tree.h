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



#ifndef TREE_H
#define TREE_H
#include <vector>
#include <string>
#include "datamodel.h"
#include "serialisable.h"

class UIAPI;
class drawingAPI;
class branch;

class tree : public serialisable
{
    public:
        tree(UIAPI * u, drawingAPI * j);
        ~tree();
        dataModel * getCurrentStyle();
        void setCurrentStyleByName(std::string sname);
        std::vector<std::string> getModelNames();
        branch * getRootBranch();
        bool setRootBranch(branch * rb);
        std::string save(std::ofstream& _out);
        std::string load(std::ifstream& _in, std::string version);

        void setFilepath(std::string newfilepath);
        std::string getFilepath();
        void setSaved(bool s);
        bool getSaved();
        void setChanged(bool c);
        bool getChanged();
    protected:
    private:
        std::string filepath;
        bool saved, changed;
        branch * selectedBranch;
        branch * secondarySelectedBranch;
        UIAPI * uapi;
        drawingAPI * japi;
        branch * rootBranch;
        std::vector<dataModel *> styles;
        dataModel * currentStyle;
};

#endif // TREE_H

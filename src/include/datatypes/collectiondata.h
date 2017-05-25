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



#ifndef COLLECTIONDATA_H
#define COLLECTIONDATA_H

#include <datapoint.h>
#include <colorconv.h>
#include <vector>

struct vertInfo
{
    std::string x, y;
    colorconv col;
};

class collectionData : public dataPoint
{
    public:
        collectionData(std::string _name, std::vector<vertInfo> value, std::string _prompt);
        ~collectionData();
        void setData(std::vector<vertInfo> value);
        std::vector<vertInfo> getData();
        void updateDependency();
        void duplicate();
        void addToData(vertInfo vi);
        void removeFromData(int index);
        void setDataAt(vertInfo value, int index);
        vertInfo getDataAt(int index);

        std::string save(std::ofstream& _out);
        std::string load(std::ifstream& _in, std::string version);
    protected:
    private:
        std::vector<vertInfo> data;
};

#endif // COLLECTIONDATA_H

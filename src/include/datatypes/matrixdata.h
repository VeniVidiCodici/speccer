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



#ifndef MATRIXDATA_H
#define MATRIXDATA_H

#include <datapoint.h>

class shape;

class matrixData : public dataPoint
{
    public:
        matrixData(std::string _name, shape* value, std::string _prompt);
        ~matrixData();
        void setData(shape* value);
        shape* getData();
        void updateDependency();
        void duplicate();

        std::string save(std::ofstream& _out);
        std::string load(std::ifstream& _in, std::string version);
        std::string getDataName();
    protected:
    private:
        shape* data;
        std::string dataName;
};

#endif // MATRIXDATA_H

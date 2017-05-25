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



#ifndef NUMBERDATA_H
#define NUMBERDATA_H

#include <datapoint.h>


class numberData : public dataPoint
{
    public:
        numberData(std::string _name,double value, std::string _prompt);
        numberData(std::string _name,int value, std::string _prompt);
        ~numberData();
        void setData(double value);
        void setData(int value);
        int getData();
        void updateDependency();
        void duplicate();

        std::string save(std::ofstream& _out);
        std::string load(std::ifstream& _in, std::string version);
    protected:
    private:
        int data;
};

#endif // NUMBERDATA_H

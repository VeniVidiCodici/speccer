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



#ifndef FORMULADATA_H
#define FORMULADATA_H

#include <datapoint.h>


class formulaData : public dataPoint
{
    public:
        formulaData(std::string _name, std::string value, std::string _prompt);
        ~formulaData();
        void setData(std::string value);
        std::string getData();
        void updateDependency();
        void duplicate();

        std::string save(std::ofstream& _out);
        std::string load(std::ifstream& _in, std::string version);
    protected:
    private:
        std::string data;
};

#endif // FORMULADATA_H

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



#ifndef SERIALISABLE_H
#define SERIALISABLE_H
#include <fstream>
#include <iostream>
#include <string>

class serialisable
{
    public:
        serialisable();
        virtual ~serialisable();
        virtual std::string save(std::ofstream& _out) = 0;
        virtual std::string load(std::ifstream& _in, std::string version) = 0;

        void wi(std::ofstream& _out, int _i){_out.write(reinterpret_cast<char*>(&_i),sizeof(int));};
        void wf(std::ofstream& _out, float _i){_out.write(reinterpret_cast<char*>(&_i),sizeof(float));};
        void wd(std::ofstream& _out, double _i){_out.write(reinterpret_cast<char*>(&_i),sizeof(double));};
        void ws(std::ofstream& _out, std::string _i){_i+='\0';_out.write(_i.c_str(),_i.length());};

        void set_intsize(int _is){_intsize = _is;};

        std::string rs(std::ifstream& _in)
        {
            std::string _i="niks";
            if (_in.good())
            std::getline ( _in, _i, '\0' );
            return _i;
        };
        int ri(std::ifstream& _in)
            {
                int _i=27;
                if (_in.good())
                    _in.read(reinterpret_cast<char*>(&_i),_intsize);
                return _i;
            };
        float rf(std::ifstream& _in)
            {float _i=0.037;if (_in.good())_in.read(reinterpret_cast<char*>(&_i),sizeof(float));return _i;};
        double rd(std::ifstream& _in)
            {double _i=0.037;if (_in.good())_in.read(reinterpret_cast<char*>(&_i),sizeof(double));return _i;};

    protected:
        int _intsize;
    private:
};

#endif // SERIALISABLE_H

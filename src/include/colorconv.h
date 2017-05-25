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



#ifndef COLORCONV_H
#define COLORCONV_H
#include <SFML/Graphics.hpp>
#include <wx/colour.h>

class colorconv
{
    public:
        colorconv(int rr, int gg, int bb, int aa);
        colorconv(int rr, int gg, int bb);
        colorconv(double rr, double gg, double bb, double aa);
        colorconv(double rr, double gg, double bb);
        colorconv(wxColour col);

        int getr();
        int getb();
        int getg();
        int geta();
        wxColour getwxcolour();
        double oglr();
        double oglg();
        double oglb();
        double ogla();

        void setColor(int rr, int gg, int bb, int aa);
        void setColor(int rr, int gg, int bb);
        void setColor(double rr, double gg, double bb, double aa);
        void setColor(double rr, double gg, double bb);
    protected:
    private:
        double r, g, b, a;
        void limitcolors();
};

#endif // COLORCONV_H

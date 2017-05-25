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



#include "colorconv.h"

colorconv::colorconv(int rr, int gg, int bb, int aa)
{
    r=rr;
    g=gg;
    b=bb;
    a=aa;
    limitcolors();
}
colorconv::colorconv(int rr, int gg, int bb)
{
    r=rr;
    g=gg;
    b=bb;
    a=255;
    limitcolors();
}
colorconv::colorconv(double rr, double gg, double bb, double aa)
{
    r=rr;
    g=gg;
    b=bb;
    a=aa;
    limitcolors();
}
colorconv::colorconv(double rr, double gg, double bb)
{
    r=rr;
    g=gg;
    b=bb;
    a=255.0;
    limitcolors();
}

colorconv::colorconv(wxColour col)
{
    r=col.Red();
    g=col.Green();
    b=col.Blue();
    if(col.Alpha()==wxALPHA_OPAQUE)
        a=255;
    else
       a=col.Alpha();
    limitcolors();
}

void colorconv::setColor(int rr, int gg, int bb, int aa)
{
    r=rr;
    g=gg;
    b=bb;
    a=aa;
    limitcolors();
}
void colorconv::setColor(int rr, int gg, int bb)
{
    r=rr;
    g=gg;
    b=bb;
    limitcolors();
}
void colorconv::setColor(double rr, double gg, double bb, double aa)
{
    r=rr;
    g=gg;
    b=bb;
    a=aa;
    limitcolors();
}
void colorconv::setColor(double rr, double gg, double bb)
{
    r=rr;
    g=gg;
    b=bb;
    limitcolors();
}

int colorconv::getr()
{
    return (int)r;
}
int colorconv::getb()
{
    return (int)b;
}
int colorconv::getg()
{
    return (int)g;
}
int colorconv::geta()
{
    return (int)a;
}

wxColour colorconv::getwxcolour()
{
    return wxColour((int)r,(int)g,(int)b,(int)a);
}

void colorconv::limitcolors()
{
    a=std::min(255.0,std::max(a,0.0));
    r=std::min(255.0,std::max(r,0.0));
    g=std::min(255.0,std::max(g,0.0));
    b=std::min(255.0,std::max(b,0.0));
}
double colorconv::oglr()
{
    return r/255.0;
}
double colorconv::oglg()
{
    return g/255.0;
}
double colorconv::oglb()
{
    return b/255.0;
}
double colorconv::ogla()
{
    return a/255.0;
}

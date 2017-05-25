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



#ifndef ALLDATATYPES_H
#define ALLDATATYPES_H

#include <datapoint.h>
#include <booldata.h>
#include <collectiondata.h>
#include <colordata.h>
#include <fontdata.h>
#include <formuladata.h>
#include <imagedata.h>
#include <matrixdata.h>
#include <numberdata.h>
#include <textdata.h>
#include <choicedata.h>

#endif // ALLDATATYPES_H

#ifndef sysslash
    #if defined(_WIN32) //windows
        #define  sysslash "\\"
    #endif // defined
    #if defined(_APPLE_) && defined(_MACH_)    //mac
        #define sysslash "/"
    #endif // defined
    #if defined(linux) || defined(__linux)  //linux
        #define sysslash "/"
    #endif // defined
#endif


#ifndef sysslash
    #define sysslash "/"
#endif // sysslash

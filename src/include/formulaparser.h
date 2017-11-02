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



#ifndef FORMULAPARSER_H
#define FORMULAPARSER_H
#include <string>
#include <dataholder.h>
#include <map>
#include <iostream>
#include <exception>
#include <formuladata.h>

class branch;

class formulaParser
{
    public:
        formulaParser(bool actualGlobalVariables, dataHolder * _dataRepo);
        virtual ~formulaParser();
        double parsestring(std::string expression, bool useGlobalVariables=true);
        double parse(formulaData * formula, bool useGlobalVariables=true);
        bool isItCorrect(std::string expression, bool useGlobalVariables=true);
        void setDataHolder(dataHolder * _dataRepo);
        void setDrawnComposite(composite * c);
        void setBranch(branch * br);
        branch * getBranch();
        std::string getAPropertyFromText(std::string arg);
        std::string applyPropertyToText(std::string arg, std::string prop, std::string data);

        std::vector<std::string> getTagsFromText(std::string input);
        std::string applyTagsToText(std::string input,
                                    std::vector<std::string> data);
    protected:
    private:
        bool useGlobVar;
        void calcGlobVar();
        double n, sc, sw, sh, dc, dw, dh, udc, udw, udh, pc, pw, ph;
        std::string input;
        int inputIndex, inputLength;
        dataHolder * dataRepo;
        composite * drawn;
        branch * currentNode;
        double E();
        double T();
        double F();
        double B();
        bool checkE();
        bool checkT();
        bool checkF();
        bool checkB();
        bool iterateInput();
        bool clearSpaces();
};

#endif // FORMULAPARSER_H

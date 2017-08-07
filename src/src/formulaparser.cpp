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



#include "formulaparser.h"
#include <stdlib.h>
#include <ctype.h>
#include <string>
#include <math.h>
#include "branch.h"

formulaParser::formulaParser(bool actualGlobalVariables, dataHolder * _dataRepo)
{
    useGlobVar=actualGlobalVariables;
    dataRepo = _dataRepo;
    n=1.0;
    sc=1.0;
    sw=0.0;
    sh=0.0;
    pc=0.0;
    pw=0.0;
    ph=0.0;
    dc=0.0;
    dw=0.0;
    dh=0.0;
    udc=0.0;
    udw=0.0;
    udh=0.0;
}

formulaParser::~formulaParser()
{
    //dtor
}
void formulaParser::setDataHolder(dataHolder * _dataRepo)
{
    dataRepo = _dataRepo;
}
void formulaParser::calcGlobVar()
{

}

std::string formulaParser::getAPropertyFromText(std::string arg) /// for now only from text data
{
    std::string buffer ="";
    inputIndex = 0;
    inputLength = arg.length();
    if (inputLength<3) return "";
    while (arg[inputIndex] != '[')
    {
        if (inputIndex>=inputLength-1)
                return "";
        inputIndex++;
    }
    do {
        if (inputIndex>=inputLength-1)
            return "";
        inputIndex++;
        if (arg[inputIndex]!=']')
            buffer+=arg[inputIndex];
    } while(arg[inputIndex]!=']');
    return buffer;
}
std::vector<std::string> formulaParser::getTagsFromText(std::string input)
{
    std::vector<std::string> res;
    std::string buffer ="";
    int inputIndex = 0;
    int inputLength = input.length();
    if (inputLength<3) return res;
    while(inputIndex<inputLength)
        {
        buffer ="";
        while (input[inputIndex] != '[')
        {
            if (inputIndex>=inputLength-1)
                    return res;
            inputIndex++;
        }
        do {
            if (inputIndex>=inputLength-1)
                return res;
            inputIndex++;
            if (input[inputIndex]!=']')
                buffer+=input[inputIndex];
        } while(input[inputIndex]!=']');
        res.push_back(buffer);
        }
    return res;
}
std::string formulaParser::applyPropertyToText(std::string arg, std::string prop, std::string data)
{
    std::string buffer ="[";
    inputIndex = 0;
    inputLength = arg.length();
    if (inputLength<3) return arg;
    while (arg[inputIndex] != '[')
    {
        if (inputIndex>=inputLength-1)
                return arg;
        inputIndex++;
    }
    do {
        if (inputIndex>=inputLength-1)
            return arg;
        inputIndex++;
        buffer+=arg[inputIndex];
    } while(arg[inputIndex]!=']');

    size_t start_pos = arg.find(buffer);
    if(start_pos == std::string::npos)
        return arg;
    arg.replace(start_pos, buffer.length(), data);

    return arg;
}
std::string formulaParser::applyTagsToText(std::string input,
                                           std::vector<std::string> data)
{
    std::vector<std::string> res;
    std::string buffer ="";
    int inputIndex = 0;
    int inputLength = input.length();
    if (inputLength<3) return "";
    bool flag;
    while(inputIndex<inputLength)
        {
        buffer ="[";
        flag  = true;
        while (input[inputIndex] != '[')
        {
            if (inputIndex>=inputLength-1)
                flag = false;
            inputIndex++;
        }
        if (flag)
            do {
                if (inputIndex>=inputLength-1)
                    return input+"     tdv!!";
                inputIndex++;
                buffer+=input[inputIndex];
            } while(input[inputIndex]!=']');
        if (flag)
            res.push_back(buffer);
        }

    for (int i = 0; i < res.size() && i < data.size(); i++)
    {
        size_t start_pos = input.find(res[i]);
        if(start_pos == std::string::npos)
            return "NF: "+res[i]+" " +input;
        input.replace(start_pos, res[i].length(), data[i]);
    }
    return input;//+"\n"+std::to_string(res.size())+", "+std::to_string(data.size());
}
/**
    important:
        widths and heights of branches are really only the widths and heights of the respective bounding boxes
        and bounding boxes in turn cannot use any global variables
        global variables:
            n - the position of the leaf in its parent hierarchy, i.e. whichth the leaf is
            SC, SH, SW - siblings count, siblings total height, siblings total width
            DC, DH, DW - descendants count, descendants total height, descendants total width
            UDC, UDH, UDW - ultimate descendants count, total height and width
            ultimate as in not having any descendants themselves
        if there are any transformation matrixes applied on the bounding box, those don't change its dimensions
        for the purpose of calculating global variables
        local variables:
            <element>x, <element>y, <element>w, <element>h - position and dimensions of an element (untransformed)
            <element>tx, <element>ty, <element>tw, <element>th - position and dimensions of an element (transformed, not yet implemented)
            [variable] - adds another formula to be defined in the composite shape or the branch,
                if it doesn't exist already
        available functions:
            sin()
            cos()
            tan()
            cotan()
            +arctrigs - all trigs in radians
            round() - rounds & converts to int
            sqrt()
            abs()
        available constants:
            pi
            tau
            phi
        available operators:
            + - * / ^ _
            A ^ B - returns the max of A, B
            A _ B - returns the min of A, B
    */

double formulaParser::parse(std::string expression, bool useGlobalVariables)
{
    //return 1.0;
    useGlobVar = useGlobalVariables;
    input = expression;
    inputLength = input.length();
    inputIndex=0;
    return E();
}
double formulaParser::E()
    {
    double res = T();
    clearSpaces();
    while(input[inputIndex] == '+' || (input[inputIndex] == '-'))
        {
        if (!iterateInput())
            return 0.0;
        if (input[inputIndex-1]=='+')
            res+=T();
        else
            res-=T();
        clearSpaces();
        }
    return res;
    }

double formulaParser::T()
   {
   double res = F();
   clearSpaces();
   while((input[inputIndex] == '*') || (input[inputIndex] == '/')//)
         || (input[inputIndex] == '^') || (input[inputIndex] == '_'))
        {
            if(!iterateInput())
                return 0.0;
            if (input[inputIndex-1]=='*')
                res*=F();
            else if (input[inputIndex-1]=='/')
            {
                double temp = F();
                if (temp == 0)
                    res=0;
                else
                    res/=temp;
            }
            else if (input[inputIndex-1]=='^')
            {
                double temp = F();
                if (temp > res)
                    return temp;
                else
                    return res;
            }
            else
            {
                double temp = F();
                if (temp < res)
                    return temp;
                else
                    return res;
            }
            clearSpaces();
        }
   return res;
   }

double formulaParser::F()
   {
   double res=0.0;
   try
   {
   std::string buffer="";
   if (!clearSpaces())
        return 0.0;
   if(isalpha(input[inputIndex]))
      do {
            buffer+=input[inputIndex];
            {
            if (buffer=="pi") res = 3.1415926535;
            if (buffer== "tau") res = 2*3.1415926535;
            if (buffer== "phi") res = 1.6180339887;
            if (currentNode==nullptr)
            {
                std::string sres="no";
                if (buffer== "n") sres = "n";
                if (buffer== "sc"||buffer== "SC") sres = "sc";
                if (buffer== "sh"||buffer== "SH") sres = "sh";
                if (buffer== "sw"||buffer== "SW") sres = "sw";
                if (buffer== "pc"||buffer== "PC") sres = "pc";
                if (buffer== "ph"||buffer== "PH") sres = "ph";
                if (buffer== "pw"||buffer== "PW") sres = "pw";
                if (buffer== "dc"||buffer== "DC") sres = "dc";
                if (buffer== "dh"||buffer== "DH") sres = "dh";
                if (buffer== "dw"||buffer== "DW") sres = "dw";
                if (buffer== "udc"||buffer== "UDC") sres = "udc";
                if (buffer== "udw"||buffer== "UDW") sres = "udw";
                if (buffer== "udh"||buffer== "UDH") sres = "udh";

                if (sres!="no")
                    res = strtod(dataRepo->getField(sres,"0").c_str(), NULL);
            }
            else
            {
                if (buffer== "n") res = currentNode->getn();
                if (buffer== "sc"||buffer== "SC") res = currentNode->getsc();
                if (buffer== "sh"||buffer== "SH") res = currentNode->getsh();
                if (buffer== "sw"||buffer== "SW") res = currentNode->getsw();
                if (buffer== "pc"||buffer== "PC") res = currentNode->getpc();
                if (buffer== "ph"||buffer== "PH") res = currentNode->getph();
                if (buffer== "pw"||buffer== "PW") res = currentNode->getpw();
                if (buffer== "dc"||buffer== "DC") res = currentNode->getdc();
                if (buffer== "dh"||buffer== "DH") res = currentNode->getdh();
                if (buffer== "dw"||buffer== "DW") res = currentNode->getdw();
                if (buffer== "udc"||buffer== "UDC") res = currentNode->getudc();
                if (buffer== "udw"||buffer== "UDW") res = currentNode->getudw();
                if (buffer== "udh"||buffer== "UDH") res = currentNode->getudh();
            }
            if (buffer== "sin")
                {
                if(!iterateInput()) return 0.0;
                if (!clearSpaces()) return 0.0;
                res = sin(B());
                }
            if (buffer== "cos")
                {
                if(!iterateInput()) return 0.0;
                if (!clearSpaces()) return 0.0;
                res = cos(B());
                }
            if (buffer== "tan")
                {
                if(!iterateInput()) return 0.0;
                if (!clearSpaces()) return 0.0;
                res = tan(B());
                }
            if (buffer== "cotan")
                {
                if(!iterateInput()) return 0.0;
                if (!clearSpaces()) return 0.0;
                res = 1/tan(B());
                }
            if (buffer== "asin")
                {
                if(!iterateInput()) return 0.0;
                if (!clearSpaces()) return 0.0;
                res = asin(B());
                }
            if (buffer== "acos")
                {
                if(!iterateInput()) return 0.0;
                if (!clearSpaces()) return 0.0;
                res = acos(B());
                }
            if (buffer== "atan")
                {
                if(!iterateInput()) return 0.0;
                if (!clearSpaces()) return 0.0;
                res = atan(B());
                }
            if (buffer== "acotan")
                {
                if(!iterateInput()) return 0.0;
                if (!clearSpaces()) return 0.0;
                res = 0.5*3.1415926535-atan(B());
                }
            if (buffer== "round")
                {
                if(!iterateInput()) return 0.0;
                if (!clearSpaces()) return 0.0;
                res = round(B());
                }
            if (buffer== "sqrt")
                {
                if(!iterateInput()) return 0.0;
                if (!clearSpaces()) return 0.0;
                res = sqrt(B());
                }
            if (buffer== "abs")
                {
                if(!iterateInput()) return 0.0;
                if (!clearSpaces()) return 0.0;
                res = abs(B());
                }
            }
            if(!iterateInput())
                break;
         }
      while(isalnum(input[inputIndex]));
   else if(isdigit(input[inputIndex]))
      {
                do
                {
                buffer+=input[inputIndex];
                res=strtod(buffer.c_str(), NULL);
                if(!iterateInput())
                    break;
                }
                while(isdigit(input[inputIndex])||input[inputIndex]=='.'||input[inputIndex]==',');

      }
   else if(input[inputIndex] == '(')
        {
        return B();
        }
   else if(input[inputIndex] == '<')
        {
        if(!iterateInput())
                return 0.0;
        res=0.0;
        do {
                buffer+=input[inputIndex];
                iterateInput();
                if (inputIndex>=inputLength-1)
                    return 0.0;
        } while(input[inputIndex]!='>');
        if(!iterateInput())
            return 0.0;
        if (!dataRepo->drawableExists(buffer))
            return 0.0;
        drawable * tempdr;
        if (useGlobVar)
            tempdr = dataRepo->getDrawableByName(buffer);
        else
            tempdr = drawn->getElementByName(buffer); /// only finds elements one layer (of composites) deep
        /// todo: find elements at any depth
        if (tempdr==nullptr)
            return 0.0;
        if (input[inputIndex]=='t')
        {
            if(!iterateInput())
                return 0.0;
            switch(input[inputIndex])
                {
                case 'x':res = tempdr->gettx();break;
                case 'y':res = tempdr->getty();break;
                case 'w':res = tempdr->gettw();break;
                case 'h':res = tempdr->getth();break;
                default: res=0.0;break;//actual error;
                }

        }
        else
        switch(input[inputIndex])
        {
            case 'x':res = tempdr->getx();break;
            case 'y':res = tempdr->gety();break;
            case 'w':res = tempdr->getw();break;
            case 'h':res = tempdr->geth();break;
            default: res=0.0;break;//actual error;
        }
        iterateInput();
        return res;
        }
   else if (input[inputIndex] == '[')
        {
        do {
            if (!iterateInput())
                return 0.0;
            if (input[inputIndex]!=']')
                buffer+=input[inputIndex];
        } while(input[inputIndex]!=']');
        if (currentNode!=nullptr)
        {
            dataPoint* tempd = currentNode->getPropertyByName(buffer);
            if (tempd==nullptr)
                return 0.0;
            if (tempd->getType()!=dataPoint::formula)
                return 0.0;
            res = parse(((formulaData *)tempd)->getData(), true);
        }
        else if (useGlobVar)
            res = strtod( dataRepo->getField(buffer,"0").c_str(), NULL);
        else
            {
            dataPoint* tempd = drawn->getPropertyByName(buffer);
            if (tempd==nullptr)
                return 0.0;
            if (tempd->getType()!=dataPoint::formula)
                return 0.0;
            res = parse(((formulaData *)tempd)->getData(), true);
            }
            /// the second argument should tell if the upper level is composite or not
            /// to do: add this functionality
        iterateInput();
        return res;
        }
    else
      return 0.0;
    }
    catch(std::exception& e)
    {
        std::cout << "exception: " << input << "< w/ >" << inputIndex << "\n";
        std::cout << "   of type: " << e.what() << '\n';
    }
    catch(int e)
    {
        std::cout << "exception: " << input << "< w/ >" << inputIndex << "; " << e << "\n";
    }
    catch(...)
    {
        std::cout << "exception neither int nor standard: " << input << "< w/ >" << inputIndex << "\n";
    }
    return res;
   }

void formulaParser::setDrawnComposite(composite * c)
{
    drawn = c;
}
void formulaParser::setBranch(branch * br)
{
    currentNode = br;
}
branch * formulaParser::getBranch()
{
    return currentNode;
}
double formulaParser::B()
{
    double res = 0.0;
    if(input[inputIndex] == '(')
      {
      if (!clearSpaces())
          return 0.0;
      if(!iterateInput())
          return 0.0;
      res = E();
      if (!clearSpaces()) return 0.0;
      if(input[inputIndex] != ')') return 0.0;
      iterateInput();
      }
    return res;
}

bool formulaParser::clearSpaces()
{
    if (isspace(input[inputIndex]))
    do {
            if(!iterateInput())
                return false;
      }while(isspace(input[inputIndex]));
    return true;
}

bool formulaParser::iterateInput()
{
    if (inputIndex<inputLength-1)
        {
            inputIndex++;
            return true;
        }
    return false;
}
bool formulaParser::checkE()
{
    return true;
}
bool formulaParser::checkT()
{
    return true;
}
bool formulaParser::checkF()
{
    return true;
}
bool formulaParser::checkB()
{
    return true;
}
bool formulaParser::isItCorrect(std::string expression, bool useGlobalVariables)
{
    input = expression;
    inputLength = input.length();
    inputIndex=0;
    return checkE();
}

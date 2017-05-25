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


#ifndef SPECCERMAIN_H
#define SPECCERMAIN_H

//(*Headers(speccerFrame)
#include <wx/listctrl.h>
#include <wx/sizer.h>
#include <wx/menu.h>
#include <wx/button.h>
#include <wx/frame.h>
#include <wx/statusbr.h>
//*)


class speccerFrame: public wxFrame
{
    public:

        speccerFrame(wxWindow* parent,wxWindowID id = -1);
        virtual ~speccerFrame();

    private:

        //(*Handlers(speccerFrame)
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        void OnaddstyleClick(wxCommandEvent& event);
        void OnaddtreeClick(wxCommandEvent& event);
        //*)

        //(*Identifiers(speccerFrame)
        static const long ID_BUTTON1;
        static const long ID_BUTTON2;
        static const long ID_LISTCTRL1;
        static const long ID_LISTCTRL2;
        static const long ID_BUTTON3;
        static const long ID_BUTTON10;
        static const long ID_BUTTON4;
        static const long ID_BUTTON9;
        static const long ID_BUTTON5;
        static const long ID_BUTTON8;
        static const long idMenuQuit;
        static const long idMenuAbout;
        static const long ID_STATUSBAR1;
        //*)

        //(*Declarations(speccerFrame)
        wxButton* deletestyle;
        wxButton* addstyle;
        wxButton* edittree;
        wxButton* addtree;
        wxButton* deletetree;
        wxButton* editstyle;
        wxButton* clonetree;
        wxListCtrl* stylelist;
        wxStatusBar* StatusBar1;
        wxButton* clonestyle;
        wxListCtrl* treelist;
        //*)

        DECLARE_EVENT_TABLE()
};

#endif // SPECCERMAIN_H

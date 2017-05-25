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



#include "speccerMain.h"
#include <wx/msgdlg.h>
#include "styleframe.h"
#include "treeFrame.h"


//(*InternalHeaders(speccerFrame)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//helper functions
enum wxbuildinfoformat {
    short_f, long_f };

wxString wxbuildinfo(wxbuildinfoformat format)
{
    wxString wxbuild(wxVERSION_STRING);

    if (format == long_f )
    {
#if defined(__WXMSW__)
        wxbuild << _T("-Windows");
#elif defined(__UNIX__)
        wxbuild << _T("-Linux");
#endif

#if wxUSE_UNICODE
        wxbuild << _T("-Unicode build");
#else
        wxbuild << _T("-ANSI build");
#endif // wxUSE_UNICODE
    }

    return wxbuild;
}

//(*IdInit(speccerFrame)
const long speccerFrame::ID_BUTTON1 = wxNewId();
const long speccerFrame::ID_BUTTON2 = wxNewId();
const long speccerFrame::ID_LISTCTRL1 = wxNewId();
const long speccerFrame::ID_LISTCTRL2 = wxNewId();
const long speccerFrame::ID_BUTTON3 = wxNewId();
const long speccerFrame::ID_BUTTON10 = wxNewId();
const long speccerFrame::ID_BUTTON4 = wxNewId();
const long speccerFrame::ID_BUTTON9 = wxNewId();
const long speccerFrame::ID_BUTTON5 = wxNewId();
const long speccerFrame::ID_BUTTON8 = wxNewId();
const long speccerFrame::idMenuQuit = wxNewId();
const long speccerFrame::idMenuAbout = wxNewId();
const long speccerFrame::ID_STATUSBAR1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(speccerFrame,wxFrame)
    //(*EventTable(speccerFrame)
    //*)
END_EVENT_TABLE()

speccerFrame::speccerFrame(wxWindow* parent,wxWindowID id)
{
    //(*Initialize(speccerFrame)
    wxMenuItem* MenuItem2;
    wxFlexGridSizer* fgs;
    wxMenuItem* MenuItem1;
    wxMenu* Menu1;
    wxMenuBar* MenuBar1;
    wxMenu* Menu2;

    Create(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE, _T("wxID_ANY"));
    fgs = new wxFlexGridSizer(5, 2, 0, 0);
    fgs->AddGrowableCol(0);
    fgs->AddGrowableCol(1);
    fgs->AddGrowableRow(1);
    addtree = new wxButton(this, ID_BUTTON1, _("Add tree"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON1"));
    fgs->Add(addtree, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    addstyle = new wxButton(this, ID_BUTTON2, _("Add style"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON2"));
    fgs->Add(addstyle, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    treelist = new wxListCtrl(this, ID_LISTCTRL1, wxDefaultPosition, wxDefaultSize, wxLC_LIST|wxSIMPLE_BORDER, wxDefaultValidator, _T("ID_LISTCTRL1"));
    treelist->SetMinSize(wxDLG_UNIT(this,wxSize(100,100)));
    treelist->Hide();
    fgs->Add(treelist, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    stylelist = new wxListCtrl(this, ID_LISTCTRL2, wxDefaultPosition, wxDefaultSize, wxLC_LIST|wxSIMPLE_BORDER, wxDefaultValidator, _T("ID_LISTCTRL2"));
    stylelist->SetMinSize(wxDLG_UNIT(this,wxSize(100,100)));
    stylelist->Hide();
    fgs->Add(stylelist, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    edittree = new wxButton(this, ID_BUTTON3, _("Edit tree"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON3"));
    edittree->Hide();
    fgs->Add(edittree, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    editstyle = new wxButton(this, ID_BUTTON10, _("Edit style"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON10"));
    editstyle->Hide();
    fgs->Add(editstyle, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    clonetree = new wxButton(this, ID_BUTTON4, _("Clone tree"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON4"));
    clonetree->Hide();
    fgs->Add(clonetree, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    clonestyle = new wxButton(this, ID_BUTTON9, _("Clone style"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON9"));
    clonestyle->Hide();
    fgs->Add(clonestyle, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    deletetree = new wxButton(this, ID_BUTTON5, _("Delete tree"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON5"));
    deletetree->Hide();
    fgs->Add(deletetree, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    deletestyle = new wxButton(this, ID_BUTTON8, _("Delete style"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON8"));
    deletestyle->Hide();
    fgs->Add(deletestyle, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    SetSizer(fgs);
    MenuBar1 = new wxMenuBar();
    Menu1 = new wxMenu();
    MenuItem1 = new wxMenuItem(Menu1, idMenuQuit, _("Quit\tAlt-F4"), _("Quit the application"), wxITEM_NORMAL);
    Menu1->Append(MenuItem1);
    MenuBar1->Append(Menu1, _("&File"));
    Menu2 = new wxMenu();
    MenuItem2 = new wxMenuItem(Menu2, idMenuAbout, _("About\tF1"), _("Show info about this application"), wxITEM_NORMAL);
    Menu2->Append(MenuItem2);
    MenuBar1->Append(Menu2, _("Help"));
    SetMenuBar(MenuBar1);
    StatusBar1 = new wxStatusBar(this, ID_STATUSBAR1, 0, _T("ID_STATUSBAR1"));
    int __wxStatusBarWidths_1[1] = { -1 };
    int __wxStatusBarStyles_1[1] = { wxSB_NORMAL };
    StatusBar1->SetFieldsCount(1,__wxStatusBarWidths_1);
    StatusBar1->SetStatusStyles(1,__wxStatusBarStyles_1);
    SetStatusBar(StatusBar1);
    fgs->Fit(this);
    fgs->SetSizeHints(this);
    Center();

    Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&speccerFrame::OnaddtreeClick);
    Connect(ID_BUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&speccerFrame::OnaddstyleClick);
    Connect(idMenuQuit,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&speccerFrame::OnQuit);
    Connect(idMenuAbout,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&speccerFrame::OnAbout);
    //*)
}

speccerFrame::~speccerFrame()
{
    //(*Destroy(speccerFrame)
    //*)
}

void speccerFrame::OnQuit(wxCommandEvent& event)
{
    Close();
}

void speccerFrame::OnAbout(wxCommandEvent& event)
{
    wxString msg = wxbuildinfo(long_f);
    wxMessageBox(msg, _("Welcome to..."));
}

void speccerFrame::OnaddstyleClick(wxCommandEvent& event)
{
    styleframe* Frame = new styleframe(this);
    Frame->Show();
}

void speccerFrame::OnaddtreeClick(wxCommandEvent& event)
{
    treeFrame* Frame = new treeFrame(this);
    Frame->Show();
}

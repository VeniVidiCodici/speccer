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



#include "styleframe.h"
#include <fstream>
#include <iostream>
//(*InternalHeaders(styleframe)
#include <wx/settings.h>
#include <wx/font.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(styleframe)
const long styleframe::ID_STATICTEXT1 = wxNewId();
const long styleframe::ID_STATICTEXT2 = wxNewId();
const long styleframe::ID_STATICTEXT4 = wxNewId();
const long styleframe::ID_LISTCTRL1 = wxNewId();
const long styleframe::ID_LISTCTRL2 = wxNewId();
const long styleframe::ID_LISTCTRL3 = wxNewId();
const long styleframe::ID_BUTTON5 = wxNewId();
const long styleframe::ID_BUTTON6 = wxNewId();
const long styleframe::ID_BUTTON3 = wxNewId();
const long styleframe::ID_BUTTON4 = wxNewId();
const long styleframe::ID_BUTTON2 = wxNewId();
const long styleframe::ID_BUTTON1 = wxNewId();
const long styleframe::ID_BUTTON7 = wxNewId();
const long styleframe::ID_BUTTON8 = wxNewId();
const long styleframe::ID_OGLPLOTTER1 = wxNewId();
const long styleframe::ID_STATICTEXT3 = wxNewId();
const long styleframe::ID_TEXTENTRYDIALOG1 = wxNewId();
const long styleframe::ID_MESSAGEDIALOG1 = wxNewId();
const long styleframe::ID_TOOLBAR1 = wxNewId();
//*)

const long styleframe::ID_GLCANVASMCTT = wxNewId();

BEGIN_EVENT_TABLE(styleframe,wxFrame)
    //(*EventTable(styleframe)
    //*)
END_EVENT_TABLE()

styleframe::styleframe(wxWindow* parent,wxWindowID id)
{
    //(*Initialize(styleframe)
    wxFlexGridSizer* FlexGridSizer4;
    wxGridBagSizer* GridBagSizer1;
    wxFlexGridSizer* FlexGridSizer2;
    wxFlexGridSizer* fieldsfgs;
    wxFlexGridSizer* FlexGridSizer1;

    Create(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE, _T("wxID_ANY"));
    FlexGridSizer1 = new wxFlexGridSizer(0, 4, 0, 0);
    FlexGridSizer1->AddGrowableCol(1);
    FlexGridSizer1->AddGrowableRow(0);
    GridBagSizer1 = new wxGridBagSizer(0, 0);
    GridBagSizer1->AddGrowableRow(1);
    aslabel = new wxStaticText(this, ID_STATICTEXT1, _("Available shapes"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT1"));
    wxFont aslabelFont = wxSystemSettings::GetFont(wxSYS_OEM_FIXED_FONT);
    if ( !aslabelFont.Ok() ) aslabelFont = wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT);
    aslabelFont.SetWeight(wxBOLD);
    aslabel->SetFont(aslabelFont);
    GridBagSizer1->Add(aslabel, wxGBPosition(0, 0), wxDefaultSpan, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    pslabel = new wxStaticText(this, ID_STATICTEXT2, _("Placed shapes"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT2"));
    wxFont pslabelFont = wxSystemSettings::GetFont(wxSYS_OEM_FIXED_FONT);
    if ( !pslabelFont.Ok() ) pslabelFont = wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT);
    pslabelFont.SetWeight(wxBOLD);
    pslabel->SetFont(pslabelFont);
    GridBagSizer1->Add(pslabel, wxGBPosition(0, 1), wxGBSpan(1, 2), wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    vlabel = new wxStaticText(this, ID_STATICTEXT4, _("Vertices"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT4"));
    wxFont vlabelFont(10,wxSWISS,wxFONTSTYLE_NORMAL,wxBOLD,false,_T("Sans"),wxFONTENCODING_DEFAULT);
    vlabel->SetFont(vlabelFont);
    GridBagSizer1->Add(vlabel, wxGBPosition(0, 3), wxGBSpan(1, 2), wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    DrawablesList = new wxListCtrl(this, ID_LISTCTRL1, wxDefaultPosition, wxDefaultSize, wxLC_LIST|wxSIMPLE_BORDER, wxDefaultValidator, _T("ID_LISTCTRL1"));
    DrawablesList->SetMinSize(wxSize(100,300));
    GridBagSizer1->Add(DrawablesList, wxGBPosition(1, 0), wxGBSpan(2, 1), wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    PlacedShapesList = new wxListCtrl(this, ID_LISTCTRL2, wxDefaultPosition, wxDefaultSize, wxLC_LIST|wxSIMPLE_BORDER, wxDefaultValidator, _T("ID_LISTCTRL2"));
    PlacedShapesList->SetMinSize(wxSize(130,300));
    GridBagSizer1->Add(PlacedShapesList, wxGBPosition(1, 1), wxGBSpan(1, 2), wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    VerticesList = new wxListCtrl(this, ID_LISTCTRL3, wxDefaultPosition, wxDefaultSize, wxLC_LIST|wxSIMPLE_BORDER, wxDefaultValidator, _T("ID_LISTCTRL3"));
    VerticesList->SetMinSize(wxSize(80,300));
    VerticesList->Disable();
    GridBagSizer1->Add(VerticesList, wxGBPosition(1, 3), wxGBSpan(1, 2), wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    vuButton = new wxButton(this, ID_BUTTON5, _("/\\"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON5"));
    vuButton->SetMinSize(wxSize(40,30));
    vuButton->Disable();
    GridBagSizer1->Add(vuButton, wxGBPosition(2, 3), wxDefaultSpan, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    vdButton = new wxButton(this, ID_BUTTON6, _("\\/"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON6"));
    vdButton->SetMinSize(wxSize(40,30));
    vdButton->Disable();
    GridBagSizer1->Add(vdButton, wxGBPosition(2, 4), wxDefaultSpan, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    avButton = new wxButton(this, ID_BUTTON3, _("+"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON3"));
    avButton->SetMinSize(wxSize(40,30));
    avButton->Disable();
    GridBagSizer1->Add(avButton, wxGBPosition(3, 3), wxDefaultSpan, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    rvButton = new wxButton(this, ID_BUTTON4, _("-"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON4"));
    rvButton->SetMinSize(wxSize(40,30));
    rvButton->Disable();
    GridBagSizer1->Add(rvButton, wxGBPosition(3, 4), wxDefaultSpan, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    removeButton = new wxButton(this, ID_BUTTON2, _("Remove"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON2"));
    removeButton->Disable();
    GridBagSizer1->Add(removeButton, wxGBPosition(3, 1), wxGBSpan(1, 2), wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    addButton = new wxButton(this, ID_BUTTON1, _("Add"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON1"));
    GridBagSizer1->Add(addButton, wxGBPosition(3, 0), wxDefaultSpan, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    subutton = new wxButton(this, ID_BUTTON7, _("/\\"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON7"));
    subutton->SetMinSize(wxSize(40,30));
    subutton->Disable();
    GridBagSizer1->Add(subutton, wxGBPosition(2, 1), wxDefaultSpan, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    GridBagSizer1->Add(65,3,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, wxDLG_UNIT(this,wxSize(1,0)).GetWidth());
    svbutton = new wxButton(this, ID_BUTTON8, _("\\/"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON8"));
    svbutton->SetMinSize(wxSize(40,30));
    svbutton->Disable();
    GridBagSizer1->Add(svbutton, wxGBPosition(2, 2), wxDefaultSpan, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer1->Add(GridBagSizer1, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    oglplotter = new OGLplotter(this);
    FlexGridSizer1->Add(oglplotter, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer4 = new wxFlexGridSizer(0, 1, 0, 0);
    selectedShapeLabel = new wxStaticText(this, ID_STATICTEXT3, _("No shape selected"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT3"));
    wxFont selectedShapeLabelFont = wxSystemSettings::GetFont(wxSYS_OEM_FIXED_FONT);
    if ( !selectedShapeLabelFont.Ok() ) selectedShapeLabelFont = wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT);
    selectedShapeLabelFont.SetWeight(wxBOLD);
    selectedShapeLabel->SetFont(selectedShapeLabelFont);
    FlexGridSizer4->Add(selectedShapeLabel, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer2 = new wxFlexGridSizer(0, 2, 0, 0);
    FlexGridSizer2->Add(100,1,1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
    FlexGridSizer2->Add(100,1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
    FlexGridSizer4->Add(FlexGridSizer2, 1, wxALL|wxEXPAND|wxFIXED_MINSIZE|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer1->Add(FlexGridSizer4, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    fieldsfgs = new wxFlexGridSizer(0, 2, 0, 0);
    fieldsfgs->Add(100,4,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
    fieldsfgs->Add(100,4,1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer1->Add(fieldsfgs, 1, wxALL|wxEXPAND|wxFIXED_MINSIZE|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer1->Add(3,3,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer1->Add(300,3,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    SetSizer(FlexGridSizer1);
    renameDialog = new wxTextEntryDialog(this, wxEmptyString, wxEmptyString, wxEmptyString, wxOK|wxCANCEL|wxCENTRE|wxWS_EX_VALIDATE_RECURSIVELY, wxDefaultPosition);
    infoBox = new wxMessageDialog(this, wxEmptyString, _("Message"), wxOK|wxCANCEL, wxDefaultPosition);
    FlexGridSizer1->Fit(this);
    FlexGridSizer1->SetSizeHints(this);
    Center();

    Connect(ID_LISTCTRL1,wxEVT_COMMAND_LIST_ITEM_ACTIVATED,(wxObjectEventFunction)&styleframe::OnDrawablesListItemActivated);
    Connect(ID_LISTCTRL2,wxEVT_COMMAND_LIST_ITEM_SELECTED,(wxObjectEventFunction)&styleframe::OnPlacedShapesListItemSelect);
    Connect(ID_LISTCTRL2,wxEVT_COMMAND_LIST_ITEM_ACTIVATED,(wxObjectEventFunction)&styleframe::OnPlacedShapesListItemActivated);
    Connect(ID_LISTCTRL3,wxEVT_COMMAND_LIST_ITEM_SELECTED,(wxObjectEventFunction)&styleframe::OnVerticesListItemSelect);
    Connect(ID_BUTTON5,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&styleframe::OnvuButtonClick);
    Connect(ID_BUTTON6,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&styleframe::OnvdButtonClick);
    Connect(ID_BUTTON3,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&styleframe::OnavButtonClick);
    Connect(ID_BUTTON4,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&styleframe::OnrvButtonClick);
    Connect(ID_BUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&styleframe::OnremoveButtonClick);
    Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&styleframe::OnaddButtonClick);
    Connect(ID_BUTTON7,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&styleframe::OnsubuttonClick);
    Connect(ID_BUTTON8,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&styleframe::OnsvbuttonClick);
    oglplotter->Connect(wxEVT_MOTION,(wxObjectEventFunction)&styleframe::OnoglplotterMouseMove,0,this);
    oglplotter->Connect(wxEVT_SIZE,(wxObjectEventFunction)&styleframe::OnoglplotterResize,0,this);
    //*)

    fontname="";
    fontdir="";

    #if defined(_WIN32) //windows
        fontdir = "%WINDIR%\\fonts\\";
    #endif // defined
    #if defined(_APPLE_) && defined(_MACH_)    //mac
        fontdir = "/Library/Fonts/";
    #endif // defined
    #if defined(linux) || defined(__linux)  //linux
        fontdir = "/usr/share/fonts/truetype/ubuntu-font-family";
    #endif // defined

    mode = 0;//style editor
    rhs = FlexGridSizer2;
    fieldFGS = fieldsfgs;
    statusBar = new wxStatusBar(this, wxID_ANY, wxST_SIZEGRIP);

    statusBar->SetFieldsCount(6);
    int widths[] = { 130, 130, 130, -1, 130, 130 };
    statusBar->SetStatusWidths(WXSIZEOF(widths), widths);

    SetStatusBar(statusBar);

    wMode = editor;

    f4=FlexGridSizer4;
    f2=FlexGridSizer2;
    mainfgs = mainfgs;
    lhs = GridBagSizer1;
    ffgs = fieldsfgs;

    int t;
    nodes = new wxMenu;
    ind = wxNewId();int * idAddANewNode = &ind;nodes->Append(*idAddANewNode, wxT("&New node"));
    nodes->AppendSeparator();
    composites = new wxMenu;
    icm = wxNewId();int * idAddANewComposite = &icm;composites->Append(*idAddANewComposite, wxT("&New composite"));
    composites->AppendSeparator();

    canvas = oglplotter;
    canvas->setUIAPI(this);
    fp = new formulaParser(false,nullptr);
    canvas->f = fp;
    model = new dataModel(this, canvas);
    updateLHSUIFromDataModel();
    model->setAPIs(this, canvas);
    DH = model->addANewNode("Leaf", true);
    fp->setDataHolder(DH);

    debugstr="";
    asl = wxNewId();
    psl = wxNewId();
    vl = wxNewId();
    pl = wxNewId();
    gvl = wxNewId();
    rv = wxNewId();

    int * skasl = &asl;
    int * skpsl = &psl;
    int * skvl = &vl;
    int * skpl = &pl;
    int * skgvl = &gvl;
    int * skrv = &rv;

    currentWorkingDirectory="";

    menubar = new wxMenuBar;
    canvasm = new wxMenu;
    canvasm->Append(*skrv, wxT("&Reset view"));

    wxMenu * file = new wxMenu;
    idsave = wxNewId();int * idSave = &idsave;file->Append(*idSave, wxT("&Save"));
    idsaveas = wxNewId();int * idSaveAs = &idsaveas;file->Append(*idSaveAs, wxT("Save &as"));
    idopen = wxNewId();int * idOpen = &idopen;file->Append(*idOpen, wxT("&Open"));

    menubar->Append(file, wxT("&File"));
    menubar->Append(canvasm, wxT("&View"));
    menubar->Append(nodes, wxT("&Nodes"));
    menubar->Append(composites, wxT("&Composite shapes"));
    SetMenuBar(menubar);

    Connect(*idSave, wxEVT_COMMAND_MENU_SELECTED,wxCommandEventHandler(styleframe::save));
    Connect(*idSaveAs, wxEVT_COMMAND_MENU_SELECTED,wxCommandEventHandler(styleframe::saveAs));
    Connect(*idOpen, wxEVT_COMMAND_MENU_SELECTED,wxCommandEventHandler(styleframe::open));
    Connect(*idAddANewNode, wxEVT_COMMAND_MENU_SELECTED,wxCommandEventHandler(styleframe::addANewNode));
    Connect(*idAddANewComposite, wxEVT_COMMAND_MENU_SELECTED,wxCommandEventHandler(styleframe::addANewComposite));
    Connect(*skrv, wxEVT_COMMAND_MENU_SELECTED,wxCommandEventHandler(styleframe::OnViewReset));
    Centre();
}

styleframe::~styleframe()
{
    canvas->f = nullptr;
    if (model!=nullptr)
        delete model;
    if (fp!=nullptr)
        delete fp;
}
void styleframe::setTitle(std::string title)
{
    if(mode==0)
        SetLabel(wxString(title+" - style editor"));
    else
        SetLabel(wxString(title+" - composite editor"));
}
void styleframe::addDataHolderToStylesMenu(dataHolder * thisOne)
{
    int t = wxNewId();
    int * id = &t;
    int * pmode = &mode;
    nodes->Append(*id, wxString(thisOne->getName()));
    dataModel *m = model;
    nodes->Bind(wxEVT_COMMAND_MENU_SELECTED,
         [thisOne, m, pmode](wxCommandEvent& event)
                    {
                        m->switchToDataHolder(thisOne);
                        *pmode = 0;
                    },t);
}
void styleframe::addDataHolderToCompositesMenu(dataHolder * thisOne)
{
    int t = wxNewId();
    int * id = &t;
    int * pmode = &mode;
    composites->Append(*id, wxString(thisOne->getName()));
    dataModel *m = model;
    composites->Bind(wxEVT_COMMAND_MENU_SELECTED,
         [thisOne, m, pmode](wxCommandEvent& event)
                    {
                        m->switchToDataHolder(thisOne);
                        *pmode = 1;
                    },t);
}


void styleframe::OnResize(wxSizeEvent& event)
{
    canvas->updatesize();
    canvas->redraw();
}

std::string styleframe::saveDialog(std::string dir, std::string extension)
{
    wxFileDialog saveFileDialog(this, _("Save"), "", wxString(dir),
                       extension
                       , wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
    if (saveFileDialog.ShowModal() == wxID_CANCEL)
        return "-1";
    currentWorkingDirectory = saveFileDialog.GetDirectory().ToStdString();
    return saveFileDialog.GetPath().ToStdString();
}

std::string styleframe::openDialog(std::string dir, std::string extension)
{
    wxFileDialog openFileDialog(this, _("Open"), "", wxString(dir),
                       extension,
                       wxFD_OPEN|wxFD_FILE_MUST_EXIST);
    if (openFileDialog.ShowModal() == wxID_CANCEL)
        return "-1";
    currentWorkingDirectory = openFileDialog.GetDirectory().ToStdString();
    return openFileDialog.GetPath().ToStdString();
}

void styleframe::save(wxCommandEvent& event)
{
    std::string filename,dir;
    if (!DH->getSaved())
    {
        if (mode==0)
        {
            filename = saveDialog("styles"+std::string(sysslash), "Style files (*.styl)|*.styl");
        }
        else
        {
            filename = saveDialog("composites"+std::string(sysslash), "Composite files (*.comp)|*.comp");
        }

        if (filename!="-1")
        {
            DH->setSaved(true);
            DH->setFilepath(filename);
        }
    }
    else filename = DH->getFilepath();

    if (filename!="-1")
        {
        if (mode==0)
        {
            std::ofstream ofs(filename, std::ofstream::out|std::ofstream::binary|std::ofstream::trunc);
            DH->ws(ofs,"1.0.0");
            DH->ws(ofs,std::to_string(sizeof(int)));
            std::string serror = model->save(ofs);
            ofs.close();
            if (serror!="")
                prompt(serror);
        }
        else
            {
            std::ofstream ofs(filename, std::ios::binary);
            DH->ws(ofs,"1.0.0");
            DH->ws(ofs,std::to_string(sizeof(int)));
            std::string serror = DH->save(ofs);
            ofs.close();
            if (serror!="")
                prompt(serror);
            }
        }
}
void styleframe::saveAs(wxCommandEvent& event)
{
    std::string filename,dir;
    if (mode==0)
        {
            filename = saveDialog("styles"+std::string(sysslash), "Style files (*.styl)|*.styl");
        }
    else
        {
            filename = saveDialog("composites"+std::string(sysslash), "Composite files (*.comp)|*.comp");
        }
    if (filename!="-1")
    {
        DH->setSaved(true);
        DH->setFilepath(filename);
        if (mode==0)
        {
            std::ofstream ofs(filename, std::ofstream::out|std::ofstream::binary|std::ofstream::trunc);
            DH->ws(ofs,"1.0.0");
            DH->ws(ofs,std::to_string(sizeof(int)));
            std::string serror = model->save(ofs);
            ofs.close();
            if (serror!="")
                prompt(serror);
        }
        else
        {
            std::ofstream ofs(filename, std::ofstream::out|std::ofstream::binary|std::ofstream::trunc);
            DH->ws(ofs,"1.0.0");
            DH->ws(ofs,std::to_string(sizeof(int)));
            std::string serror = DH->save(ofs);
            ofs.close();
            if (serror!="")
                prompt(serror);
        }
    }
}

std::vector<std::string> styleframe::getAllFilesInFolder(std::string folder)
{
    return getAllFilesInFolder("folder","styl");
}
std::vector<std::string> styleframe::getAllFilesInFolder(std::string folder, std::string extension)
{
    std::vector<std::string> files;

    wxString wxfolder = wxString(folder);
    wxArrayString *wxfiles = new wxArrayString;
    wxDir dir(wxfolder);
    wxString wtr;


    /// might be the cause of some instances of segfault at window closure!
    dir.GetAllFiles(wxfolder, wxfiles, wxString("*."+extension), wxDIR_FILES|wxDIR_NO_FOLLOW );
    /// might be the cause of some instances of segfault at window closure!

    for(int i=0;i<wxfiles->GetCount();i++)
        {
        wtr = wxfiles->Item(i);
        files.push_back(wtr.ToStdString());
        }

    return files;
}
void styleframe::open(wxCommandEvent& event)
{
    if (!DH->getSaved()||DH->getChanged())
    {
        if (wxMessageBox(_("There have been modifications. Do you wish to save first?"), _("Save?"),
                         wxICON_QUESTION | wxYES_NO, this) == wxYES )
            save(event);
    }
    std::string filename,dir;
    if (mode==0)
        {
            filename = openDialog("styles"+std::string(sysslash), "Style files (*.styl)|*.styl");
        }
        else
        {
            filename = openDialog("composites"+std::string(sysslash), "Composite files (*.comp)|*.comp");
        }
    if (filename!="-1")
    {
        DH->setSaved(true);
        DH->setFilepath(filename);
        if (mode==0)
        {
            std::ifstream ifs(filename, std::ios::binary);
            std::string version="none", isize;
            version = model->rs(ifs);
            isize =model->rs(ifs);
            int isz = (int)strtod(isize.c_str(), NULL);
            model->set_intsize(isz);

            std::string serror = model->load(ifs, version);
            ifs.close();
            if (serror!="")
                prompt(serror);
            updateLHSUIPlacedShapes();
        }
        else
        {
            std::ifstream ifs(filename, std::ios::binary);
            std::string version="none", isize;
            version = DH->rs(ifs);
            isize = DH->rs(ifs);
            int isz = (int)strtod(isize.c_str(), NULL);
            DH->set_intsize(isz);

            std::string serror = DH->load(ifs, version);
            ifs.close();
            if (serror!="")
                prompt(serror);
            updateLHSUIPlacedShapes();
        }
    }
}
void styleframe::saveAsComposite(wxCommandEvent& event)
{
    //no longer used
}
void styleframe::saveAsANewComposite(wxCommandEvent& event)
{
    //no longer used
}
void styleframe::openComposite(wxCommandEvent& event)
{
    //no longer used
}
void styleframe::switchToNode(wxCommandEvent& event)
{

}
void styleframe::resetStylesMenu()
{
    /// currently done in the lazy way
    delete nodes;
    nodes = new wxMenu;
    int * idAddANewNode = &ind;
    nodes->Append(*idAddANewNode, wxT("&New node"));
    nodes->AppendSeparator();
    menubar->Append(nodes, wxT("&Nodes"));
    Connect(*idAddANewNode, wxEVT_COMMAND_MENU_SELECTED,wxCommandEventHandler(styleframe::addANewNode));
}
void styleframe::setDataHolder(dataHolder * toThisOne)
{
    DH = owner = toThisOne;
}
void styleframe::addANewNode(wxCommandEvent& event)
{
    wxTextEntryDialog nameGetter(this,wxString("Name for the new node:"),wxString("Add a new node"));
    nameGetter.ShowModal();
    std::string name = nameGetter.GetValue().ToStdString();
    dataHolder * result = model->addANewNode(name, true);
    if (result==nullptr)
        prompt("Adding unsuccessful - there's already a node with such name.");
}
void styleframe::addANewComposite(wxCommandEvent& event)
{
    wxTextEntryDialog nameGetter(this,wxString("Name for the new composite shape:"),wxString("Add a new composite shape"));
    nameGetter.ShowModal();
    std::string name = nameGetter.GetValue().ToStdString();
    dataHolder * result = model->addANewComposite(name, true);
    if (result==nullptr)
        prompt("Adding unsuccessful - there's already a composite shape with such name.");
}

void styleframe::OnViewReset(wxCommandEvent& WXUNUSED(event))
{
  canvas->resetView();
}

void styleframe::Onplotter1MouseMove(wxMouseEvent& event)
{
    canvas->checksize();
    canvas->x=event.GetPosition().x;
    canvas->y=event.GetPosition().y;
    canvas->redraw();
    double x, y;
    x=canvas->x;
    y=canvas->y;
    std::string sk = debugstr;
}
void styleframe::setStatusBarString(std::string str, int n)
{
    if (n>=0&&n<=5)
    {
        statusBar->SetStatusText(wxString(str),n);
    }
}
double styleframe::roundto2(double n)
{
    return round(n*100.0)/100.0;
}

void styleframe::toggleDrawables(wxCommandEvent& event)
{
    if (lhs->IsShown(aslabel))
    {
        lhs->Hide(aslabel);
        lhs->Hide(addButton);
        lhs->Hide(DrawablesList);
        lhs->RecalcSizes();
    }
    else
    {
        lhs->Show(aslabel);
        lhs->Show(addButton);
        lhs->Show(DrawablesList);
        lhs->RecalcSizes();
    }
}
void styleframe::toggleShapes(wxCommandEvent& event)
{

}
void styleframe::toggleVertices(wxCommandEvent& event)
{

}
void styleframe::toggleProperties(wxCommandEvent& event)
{
    if (mainfgs->IsShown(f4))
        mainfgs->Hide(f4);
    else
        mainfgs->Show(f4);
}
void styleframe::toggleVariables(wxCommandEvent& event)
{
    if (mainfgs->IsShown(ffgs))
        mainfgs->Hide(ffgs);
    else
        mainfgs->Show(ffgs);
}

bool styleframe::refreshFieldsFGS()
{
    clearFieldsFGS();
    for(formulaData * obj : DH->getAllCustomFields())
    {
        std::string name=obj->getName();
        if(fieldLabels.find(name) == fieldLabels.end()&&
           fieldControls.find(name) == fieldControls.end())
            {
            fieldLabels[name] = new wxStaticText(this,wxID_ANY,_(obj->getPrompt()+":"));
            fieldFGS->Add(fieldLabels[name], 1,
                       wxALL|
                       wxEXPAND|
                       wxALIGN_CENTER_HORIZONTAL|
                       wxALIGN_CENTER_VERTICAL|
                       wxALIGN_RIGHT,
                       5);
            fieldControls[name] = new wxTextCtrl(this,wxID_ANY);
            *((wxTextCtrl *)fieldControls[name])<<obj->getData();
            formulaData * fordata = obj;
            wxTextCtrl * tex = (wxTextCtrl *)fieldControls[name];
            drawingAPI * rdraw = owner->getDAPI();
            dataHolder * dh = owner;
            tex->Bind(wxEVT_KEY_DOWN,
            [fordata, tex, rdraw, this](wxKeyEvent& event)
                    {
                        if (event.GetKeyCode()==WXK_UP)
                        {
                            long i = tex->GetInsertionPoint();
                            scrollNumberUp(tex,i);
                        }
                        else if (event.GetKeyCode()==WXK_DOWN)
                        {
                            long i = tex->GetInsertionPoint();
                            scrollNumberDown(tex,i);
                        }
                        else
                            event.Skip();
                        fordata->setData(tex->GetLineText(0).ToStdString());
                        rdraw->refresh();
                    });
            tex->Bind(wxEVT_TEXT,
                 [fordata, tex, rdraw, this, dh](wxCommandEvent& event)
                            {
                                fordata->setData(tex->GetLineText(0).ToStdString());
                                dh->setField(fordata->getName(), fordata->getData());
                                rdraw->refresh();
                            });
            fieldFGS->Add(fieldControls[name], 1,
                           wxALL|
                           wxEXPAND|
                           wxALIGN_CENTER_HORIZONTAL|
                           wxALIGN_CENTER_VERTICAL,
                           5);
            }
    }
    fieldFGS->Layout();
    return true;
}
void styleframe::clearFieldsFGS()
{
    for (std::map<std::string, wxControl *>::iterator it=fieldControls.begin(); it!=fieldControls.end(); ++it)
        {
        	if (it->second!=nullptr) //<!> why is it sometimes a null pointer <?>
			{
				fieldFGS->Detach(it->second);
				delete it->second;
			}
        }
    for (std::map<std::string, wxControl *>::iterator it=fieldLabels.begin(); it!=fieldLabels.end(); ++it)
        {
            if (it->second!=nullptr) //<!> why is it sometimes a null pointer <?>
			{
				fieldFGS->Detach(it->second);
				delete it->second;
			}
        }
	fieldControls.clear();
	fieldLabels.clear();
    fieldFGS->Layout();
}
void styleframe::OnDrawablesListItemSelect(wxListEvent& event)
{
    subutton->Enable(false);
    svbutton->Enable(false);
    vuButton->Enable(false);
    vdButton->Enable(false);
    removeButton->Enable(false);
    return;
	clearRHSUI();
	long itemIndex = -1;
	while ((itemIndex = DrawablesList->GetNextItem(
			itemIndex,wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED)) != wxNOT_FOUND)
	{
		std::string str=DrawablesList->GetItemText(itemIndex).ToStdString();
		if (DH->drawableExistsInModel(str))
			buildRHSUIForDrawable(DH->getDrawableFromModelByName(str));
		break;
	}
}

void styleframe::OnaddButtonClick(wxCommandEvent& event)
{
    subutton->Enable(false);
    svbutton->Enable(false);
    vuButton->Enable(false);
    vdButton->Enable(false);
	long itemIndex = -1;
	while ((itemIndex = DrawablesList->GetNextItem(
			itemIndex,wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED)) != wxNOT_FOUND)
	{
		std::string str=DrawablesList->GetItemText(itemIndex).ToStdString();
		if (DH->drawableExistsInModel(str))
			{
			    removeButton->Enable(false);
				DH->addNewDrawableFromModel(str);
				updateLHSUIPlacedShapes();
				DH->redraw();
				Refresh();
				f2->Layout();
				f2->RecalcSizes();
				f4->Layout();
				f4->RecalcSizes();
			}
		break;
	}
}

void styleframe::OnremoveButtonClick(wxCommandEvent& event)
{
    subutton->Enable(false);
    svbutton->Enable(false);
    vuButton->Enable(false);
    vdButton->Enable(false);
	clearRHSUI();
	long itemIndex = -1;
	while ((itemIndex = PlacedShapesList->GetNextItem(
			itemIndex,wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED)) != wxNOT_FOUND)
	{
		std::string str=PlacedShapesList->GetItemText(itemIndex).ToStdString();
		if (DH->drawableExists(str))
			{
			    bool flag=false;
			    if (!DH->getDrawableByName(str)->isSimplex())
                    flag=true;
                else
                {
                    if (!((shape *)DH->getDrawableByName(str))->itIsCritical())
                        flag=true;
                }
			    if (flag)
                    {
                    removeButton->Enable(false);
                    DH->removeDrawable(str);
                    selectedShapeLabel->SetLabel("No shape selected");
                    updateLHSUIPlacedShapes();
                    DH->redraw();
                    Refresh();
                    f2->Layout();
                    f2->RecalcSizes();
                    f4->Layout();
                    f4->RecalcSizes();
                    }
			}
		break;
	}
}

void styleframe::OnPlacedShapesListItemSelect(wxListEvent& event)
{
    removeButton->Enable(false);
	avButton->Enable(false);
	VerticesList->Enable(false);
	rvButton->Enable(false);
	vuButton->Enable(false);
	vdButton->Enable(false);
	subutton->Enable(false);
	svbutton->Enable(false);
	clearRHSUI();
	clearLHSUIV();
	long itemIndex = -1;
	while ((itemIndex = PlacedShapesList->GetNextItem(
			itemIndex,wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED)) != wxNOT_FOUND)
	{
		std::string str=PlacedShapesList->GetItemText(itemIndex).ToStdString();
		if (DH->drawableExists(str))
			{
				buildRHSUIForDrawable(DH->getDrawableByName(str));
				selectedShapeLabel->SetLabel(str);
				removeButton->Enable(true);
				if (itemIndex>0)
                    subutton->Enable(true);
				if (itemIndex<DH->getNumberOfDrawables()-1)
                    svbutton->Enable(true);
                if (DH->getDrawableByName(str)->itIsCritical())
                {
                    subutton->Enable(false);
                    svbutton->Enable(false);
                    removeButton->Enable(false);
                }
                if (DH->getDrawableIdByName(str)>0)
                    if (DH->getDrawableById(DH->getDrawableIdByName(str)-1)->itIsCritical())
                        subutton->Enable(false);
				if (DH->getDrawableByName(str)->isSimplex())
                {
                    shape *simplex = (shape*)DH->getDrawableByName(str);
                    if (simplex->getType()==shape::convex||simplex->getType()==shape::points)
                    {
						collectionData * cd = (collectionData *)simplex->getPropertyByName("points");
						std::vector<vertInfo> vi=cd->getData();
						updateLHSUIV(&vi);
                        avButton->Enable(true);
                        VerticesList->Enable(true);
                    }
                }
				Refresh();
				f2->Layout();
				f2->RecalcSizes();
				f4->Layout();
				f4->RecalcSizes();
			}
		break;
	}
}

void styleframe::OnPlacedShapesListItemActivated(wxListEvent& event)
{
	long itemIndex = -1;
	subutton->Enable(false);
	svbutton->Enable(false);
	while ((itemIndex = PlacedShapesList->GetNextItem(
			itemIndex,wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED)) != wxNOT_FOUND)
	{
		std::string str=PlacedShapesList->GetItemText(itemIndex).ToStdString();
		if (DH->drawableExists(str))
			{
			    if (DH->getDrawableByName(str)->itIsCritical())
                {
                    subutton->Enable(false);
                    subutton->Enable(false);
                    return;
                }
			    if (itemIndex>0)
                    subutton->Enable(true);
				if (itemIndex<DH->getNumberOfDrawables()-1)
                    svbutton->Enable(true);
				wxTextEntryDialog dlg(this,"","Enter new name:",PlacedShapesList->GetItemText(itemIndex));
				if ( dlg.ShowModal() == wxID_OK )
					{
					wxString wxstr = dlg.GetValue();
					std::string nstr=wxstr.ToStdString();
					if (!DH->drawableExists(nstr))
					{
					    if (nstr=="global")
                        {
                            prompt("The name \"global\" is reserved for future use.");
                        }
						else if (DH->renameDrawable(str, nstr))
						{
							selectedShapeLabel->SetLabel(nstr);
							PlacedShapesList->SetItemText(itemIndex,wxstr);
						}
						else
						{
							wxMessageDialog info(this,
								"Shape named this way already exists or the selected one doesn't.",
								"Rename failed");
							info.ShowModal();
						}
					}
					else if (nstr!=str)
					{
						wxMessageDialog info(this,"Shape named this way already exists.","Rename failed");
							info.ShowModal();
					}
					}
			}
		break;
	}
}

void styleframe::OnDrawablesListItemActivated(wxListEvent& event)
{
    vuButton->Enable(false);
    vdButton->Enable(false);
    subutton->Enable(false);
    svbutton->Enable(false);
	long itemIndex = -1;
	while ((itemIndex = DrawablesList->GetNextItem(
			itemIndex,wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED)) != wxNOT_FOUND)
	{
		std::string str=DrawablesList->GetItemText(itemIndex).ToStdString();
		if (DH->drawableExistsInModel(str))
			{
				DH->addNewDrawableFromModel(str);
				updateLHSUIPlacedShapes();
				DH->redraw();
				Refresh();
				f2->Layout();
				f2->RecalcSizes();
				f4->Layout();
				f4->RecalcSizes();
			}
		break;
	}
}

void styleframe::OnVerticesListItemSelect(wxListEvent& event)
{
    removeButton->Enable(false);
    subutton->Enable(false);
    svbutton->Enable(false);
    long itemIndex2 = -1;
    bool flag=false;
    shape *simplex=nullptr;
	while ((itemIndex2 = PlacedShapesList->GetNextItem(
			itemIndex2,wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED)) != wxNOT_FOUND)
        {
		std::string str=PlacedShapesList->GetItemText(itemIndex2).ToStdString();
		if (DH->drawableExists(str))
			{
				buildRHSUIForDrawable(DH->getDrawableByName(str));
				selectedShapeLabel->SetLabel(str);
				if (DH->getDrawableByName(str)->isSimplex())
                {
                    simplex = (shape*)DH->getDrawableByName(str);
                    if (simplex->getType()==shape::convex||simplex->getType()==shape::points)
                    {
                        flag=true;
                        rvButton->Enable(true);
                        vuButton->Enable(true);
                        vdButton->Enable(true);
                    }
                }
            }
        }
	if (!flag)
		return;
    clearRHSUI();
	long itemIndex = -1;
	while ((itemIndex = VerticesList->GetNextItem(
			itemIndex,wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED)) != wxNOT_FOUND)
	{
	    collectionData * cd = (collectionData*)simplex->getPropertyByName("points");
		buildRHSUIForVertex(cd,itemIndex);
		Refresh();
		f2->Layout();
		f2->RecalcSizes();
		f4->Layout();
		f4->RecalcSizes();
		break;
	}
}

void styleframe::OnavButtonClick(wxCommandEvent& event)
{
	long itemIndex2 = -1;
    bool flag=false;
    shape *simplex=nullptr;
	while ((itemIndex2 = PlacedShapesList->GetNextItem(
			itemIndex2,wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED)) != wxNOT_FOUND)
        {
		std::string str=PlacedShapesList->GetItemText(itemIndex2).ToStdString();
		if (DH->drawableExists(str))
			{
				buildRHSUIForDrawable(DH->getDrawableByName(str));
				selectedShapeLabel->SetLabel(str);
				if (DH->getDrawableByName(str)->isSimplex())
                {
                    simplex = (shape*)DH->getDrawableByName(str);
                    if (simplex->getType()==shape::convex||simplex->getType()==shape::points)
                    {
                        flag=true;
                    }
                }
            }
        }
	if (!flag)
		return;
    vuButton->Enable(true);
    vdButton->Enable(true);
	vertInfo vi{"0","0",colorconv(0,0,0,255)};
	((collectionData*)simplex->getPropertyByName("points"))->addToData(vi);
	collectionData * cd = (collectionData *)simplex->getPropertyByName("points");
	std::vector<vertInfo> vi2=cd->getData();
	updateLHSUIV(&vi2);
}

void styleframe::OnrvButtonClick(wxCommandEvent& event)
{
	long itemIndex2 = -1;
    bool flag=false;
    shape *simplex=nullptr;
	while ((itemIndex2 = PlacedShapesList->GetNextItem(
			itemIndex2,wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED)) != wxNOT_FOUND)
        {
		std::string str=PlacedShapesList->GetItemText(itemIndex2).ToStdString();
		if (DH->drawableExists(str))
			{
				buildRHSUIForDrawable(DH->getDrawableByName(str));
				selectedShapeLabel->SetLabel(str);
				if (DH->getDrawableByName(str)->isSimplex())
                {
                    simplex = (shape*)DH->getDrawableByName(str);
                    if (simplex->getType()==shape::convex||simplex->getType()==shape::points)
                    {
                        flag=true;
                    }
                }
            }
        }
	if (!flag)
		return;
    clearRHSUI();
    vuButton->Enable(false);
    vdButton->Enable(false);
	long itemIndex = -1;
	while ((itemIndex = VerticesList->GetNextItem(
			itemIndex,wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED)) != wxNOT_FOUND)
	{
		((collectionData*)simplex->getPropertyByName("points"))->removeFromData(itemIndex);
		collectionData * cd = (collectionData *)simplex->getPropertyByName("points");
		std::vector<vertInfo> vi=cd->getData();
		updateLHSUIV(&vi);
		clearRHSUI();
		break;
	}
}

void styleframe::OnvuButtonClick(wxCommandEvent& event)
{
    removeButton->Enable(false);
    long itemIndex2 = -1;
    bool flag=false;
    shape *simplex=nullptr;
	while ((itemIndex2 = PlacedShapesList->GetNextItem(
			itemIndex2,wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED)) != wxNOT_FOUND)
        {
		std::string str=PlacedShapesList->GetItemText(itemIndex2).ToStdString();
		if (DH->drawableExists(str))
			{
				buildRHSUIForDrawable(DH->getDrawableByName(str));
				selectedShapeLabel->SetLabel(str);
				if (DH->getDrawableByName(str)->isSimplex())
                {
                    simplex = (shape*)DH->getDrawableByName(str);
                    if (simplex->getType()==shape::convex||simplex->getType()==shape::points)
                    {
                        flag=true;
                        rvButton->Enable(true);
                        vuButton->Enable(true);
                        vdButton->Enable(true);
                        break;
                    }
                }
            }
        }
	if (!flag)
		return;
    clearRHSUI();
	long itemIndex = -1;
	while ((itemIndex = VerticesList->GetNextItem(
			itemIndex,wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED)) != wxNOT_FOUND)
	{
	    collectionData * cd = (collectionData*)simplex->getPropertyByName("points");
	    if (itemIndex<=1)
            vuButton->Enable(false);
        if (itemIndex==0)
            break;
        VerticesList->SetItemState(itemIndex,0, wxLIST_STATE_SELECTED);
        VerticesList->SetItemState(itemIndex-1,wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
        std::string s1, s2;
        s1 = VerticesList->GetItemText(itemIndex);
        s2 = VerticesList->GetItemText(itemIndex-1);
        VerticesList->SetItemText(itemIndex,s2);
        VerticesList->SetItemText(itemIndex-1,s1);
        vertInfo v = cd->getDataAt(itemIndex), v1 = cd->getDataAt(itemIndex-1);
		cd->setDataAt(v, itemIndex-1);
		cd->setDataAt(v1, itemIndex);
		buildRHSUIForVertex(cd,itemIndex-1);
		vdButton->Enable(true);
		Refresh();
		f2->Layout();
		f2->RecalcSizes();
		f4->Layout();
		f4->RecalcSizes();
		break;
	}
}

void styleframe::OnvdButtonClick(wxCommandEvent& event)
{
        removeButton->Enable(false);
    long itemIndex2 = -1;
    bool flag=false;
    shape *simplex=nullptr;
	while ((itemIndex2 = PlacedShapesList->GetNextItem(
			itemIndex2,wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED)) != wxNOT_FOUND)
        {
		std::string str=PlacedShapesList->GetItemText(itemIndex2).ToStdString();
		if (DH->drawableExists(str))
			{
				buildRHSUIForDrawable(DH->getDrawableByName(str));
				selectedShapeLabel->SetLabel(str);
				if (DH->getDrawableByName(str)->isSimplex())
                {
                    simplex = (shape*)DH->getDrawableByName(str);
                    if (simplex->getType()==shape::convex||simplex->getType()==shape::points)
                    {
                        flag=true;
                        rvButton->Enable(true);
                        vuButton->Enable(true);
                        vdButton->Enable(true);
                        break;
                    }
                }
            }
        }
	if (!flag)
		return;
    clearRHSUI();
	long itemIndex = -1;
	while ((itemIndex = VerticesList->GetNextItem(
			itemIndex,wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED)) != wxNOT_FOUND)
	{
	    collectionData * cd = (collectionData*)simplex->getPropertyByName("points");
	    if (itemIndex>=cd->getData().size()-3)
            vdButton->Enable(false);
	    if (itemIndex>cd->getData().size()-2)
            break;
        VerticesList->SetItemState(itemIndex,0, wxLIST_STATE_SELECTED);
        VerticesList->SetItemState(itemIndex+1,wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
        std::string s1, s2;
        s1 = VerticesList->GetItemText(itemIndex);
        s2 = VerticesList->GetItemText(itemIndex+1);
        VerticesList->SetItemText(itemIndex,s2);
        VerticesList->SetItemText(itemIndex+1,s1);
        vertInfo v = cd->getDataAt(itemIndex), v1 = cd->getDataAt(itemIndex+1);
		cd->setDataAt(v, itemIndex+1);
		cd->setDataAt(v1, itemIndex);
		buildRHSUIForVertex(cd,itemIndex+1);
		vuButton->Enable(true);
		Refresh();
		f2->Layout();
		f2->RecalcSizes();
		f4->Layout();
		f4->RecalcSizes();
		break;
	}
}

void styleframe::OnsubuttonClick(wxCommandEvent& event)
{
    removeButton->Enable(false);
	long itemIndex = -1;
	while ((itemIndex = PlacedShapesList->GetNextItem(
			itemIndex,wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED)) != wxNOT_FOUND)
	{
		std::string str=PlacedShapesList->GetItemText(itemIndex).ToStdString();
		if (DH->drawableExists(str))
			{
				int id = DH->getDrawableIdByName(str);
				if(DH->getDrawableByName(str)->itIsCritical())
                {
                    subutton->Enable(false);
                    svbutton->Enable(false);
                    return;
                }
				if (id<=2)
                    subutton->Enable(false);
                if (id<=1)
                    return;
                if (DH->getDrawableById(id-1)->itIsCritical())
                    return;
                DH->swapDrawables(id,id-1);
				removeButton->Enable(true);
				svbutton->Enable(true);
				PlacedShapesList->SetItemState(itemIndex,0, wxLIST_STATE_SELECTED);
                PlacedShapesList->SetItemState(itemIndex-1,wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
                std::string s1, s2;
                s1 = PlacedShapesList->GetItemText(itemIndex);
                s2 = PlacedShapesList->GetItemText(itemIndex-1);
                PlacedShapesList->SetItemText(itemIndex,s2);
                PlacedShapesList->SetItemText(itemIndex-1,s1);

				clearRHSUI();
				buildRHSUIForDrawable(DH->getDrawableById(id-1));
			}
		break;
	}
}

void styleframe::OnsvbuttonClick(wxCommandEvent& event)
{
    removeButton->Enable(false);
	long itemIndex = -1;
	while ((itemIndex = PlacedShapesList->GetNextItem(
			itemIndex,wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED)) != wxNOT_FOUND)
	{
		std::string str=PlacedShapesList->GetItemText(itemIndex).ToStdString();
		if (DH->drawableExists(str))
			{
			    if(DH->getDrawableByName(str)->itIsCritical())
                {
                    svbutton->Enable(false);
                    return;
                }
				int id = DH->getDrawableIdByName(str);
				int syze = DH->getNumberOfDrawables();
				if (id>=syze-3)
                    svbutton->Enable(false);
                if (id>=syze-1)
                    return;
                DH->swapDrawables(id,id+1);
				removeButton->Enable(true);
				subutton->Enable(true);
				PlacedShapesList->SetItemState(itemIndex,0, wxLIST_STATE_SELECTED);
                PlacedShapesList->SetItemState(itemIndex+1,wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
                std::string s1, s2;
                s1 = PlacedShapesList->GetItemText(itemIndex);
                s2 = PlacedShapesList->GetItemText(itemIndex+1);
                PlacedShapesList->SetItemText(itemIndex,s2);
                PlacedShapesList->SetItemText(itemIndex+1,s1);

                clearRHSUI();
				buildRHSUIForDrawable(DH->getDrawableById(id+1));
			}
		break;
	}
}

void styleframe::OnClose(wxCloseEvent& event)
{

}

void styleframe::OnoglplotterMouseMove(wxMouseEvent& event)
{
    long px=0,py=0;
    event.GetPosition(&px,&py);
    canvas->motion(px,py);
    std::string sk = debugstr;
}
void styleframe::unbug(std::string k)
{
    statusBar->SetStatusText(k,0);
    statusBar->SetStatusText(k,1);
    statusBar->SetStatusText(k,2);
    statusBar->SetStatusText(k,3);
    statusBar->SetStatusText(k,4);
    statusBar->SetStatusText(k,5);
}

void styleframe::OnoglplotterResize(wxSizeEvent& event)
{
}
void styleframe::setSecondaryBranch(branch * toThisOne)// not used in here
{

}
bool styleframe::buildRHSUIForBranch(branch * thisOne)// not used in here
{
    return true;
}
branch * styleframe::getPrimaryBranch()// not used in here
{
    return nullptr;
}
branch * styleframe::getSecondaryBranch()// not used in here
{
    return nullptr;
}

		//		<?> Do the events need to unbind <?>

bool styleframe::buildRHSUIForDrawable(drawable * thisOne)
{
    if (thisOne==nullptr)
        return false;
    selectedShapeLabel->SetLabel(thisOne->getName());
    for(dataPoint * obj : thisOne->getProperties())
    {
        std::string name=obj->getName();
        if(labels.find(name) == labels.end()&&
           controls.find(name) == controls.end())
            {
            labels[name] = new wxStaticText(this,wxID_ANY,_(obj->getPrompt()+":"));
            dataPoint::type kind = obj->getType();
            if (kind==dataPoint::boolean||kind==dataPoint::text||kind==dataPoint::formula
                ||kind==dataPoint::image||kind==dataPoint::color||kind==dataPoint::number)
            {
                if (obj->getSurfaceName()!="")
                    labels[name]->SetLabelText(wxString(obj->getPrompt()+":\n(as "+obj->getSurfaceName()+")"));
                dataPoint * tdp = obj;
                wxWindow * tw = this;
                wxStaticText * wst = (wxStaticText *)(labels[name]);
                labels[name]->Bind(wxEVT_RIGHT_DOWN,
                                   [tdp,tw, wst](wxMouseEvent& event)
                                   {
                                        wxTextEntryDialog ted(tw,
                                                              wxString("Enter a new value (empty string to remove):"),
                                                              wxString("Enter surface name"),
                                                              wxString(tdp->getSurfaceName())
                                                              );
                                        if (ted.ShowModal()==wxID_OK)
                                            tdp->setSurfaceName(ted.GetValue().ToStdString());
                                        if (tdp->getSurfaceName()!="")
                                            wst->SetLabelText(wxString(tdp->getPrompt()+
                                                            ":\n(as "+tdp->getSurfaceName()+")"));
                                        else
                                            wst->SetLabelText(wxString(tdp->getPrompt()+":"));
                                        /// this will likely be used in the future:
                                        /*wxMenu popup;
                                        dataPoint * ttdp = tdp;
                                        if (tdp->getSurfaceName()!="")
                                            popup.Append(wxID_ANY, wxString("Set/Delete surface name (none currently)"));
                                        else
                                            popup.Append(wxID_ANY, wxString("Set/Delete surface name (currently "+tdp->getSurfaceName()+")"));
                                        popup.Bind(wxEVT_COMMAND_MENU_SELECTED,
                                                   [ttdp](wxCommandEvent& event)
                                                   {

                                                   }
                                                   );
                                        PopupMenu(&popup);*/
                                   });
            }
            rhs->Add(labels[name], 1,
                       wxALL|
                       wxEXPAND|
                       wxALIGN_CENTER_HORIZONTAL|
                       wxALIGN_CENTER_VERTICAL|
                       wxALIGN_RIGHT,
                       5);
            switch(obj->getType())
                {
                case dataPoint::boolean:
                	{
                	controls[name] = new wxCheckBox(this,wxID_ANY,"");
                    ((wxCheckBox *)controls[name])->SetValue(((boolData *)obj)->getData());
                    boolData * ifdata = (boolData *)obj;
                    wxCheckBox * check = (wxCheckBox *)controls[name];
                    drawingAPI * rdraw = owner->getDAPI();
					check->Bind(wxEVT_CHECKBOX,
						 [ifdata, check, rdraw](wxCommandEvent& event)
									{
										ifdata->setData(check->GetValue());
										rdraw->refresh();
									});
                	}
                    break;
                case dataPoint::number:
                	{
                    controls[name] = new wxSpinCtrl(this,wxID_ANY);
                    ((wxSpinCtrl *)controls[name])->SetValue(((numberData *)obj)->getData());
                    numberData * numdata = (numberData *)obj;
                    wxSpinCtrl * spin = (wxSpinCtrl *)controls[name];
                    drawingAPI * rdraw = owner->getDAPI();
					spin->Bind(wxEVT_SPINCTRL,
						 [numdata, spin, rdraw](wxCommandEvent& event)
									{
										numdata->setData(spin->GetValue());
										rdraw->refresh();
									});
					}
                    break;
                case dataPoint::formula:
                	{
                	controls[name] = new wxTextCtrl(this,wxID_ANY);
                    *((wxTextCtrl *)controls[name])<<((formulaData *)obj)->getData();
                    formulaData * fordata = (formulaData *)obj;
                    wxTextCtrl * tex = (wxTextCtrl *)controls[name];
                    drawingAPI * rdraw = owner->getDAPI();
                    dataHolder * dh = owner;
					tex->Bind(wxEVT_KEY_DOWN,
						 [fordata, tex, rdraw,dh,this](wxKeyEvent& event)
									{
									    if (event.GetKeyCode()==WXK_UP)
                                        {
                                            long i = tex->GetInsertionPoint();
                                            scrollNumberUp(tex,i);
                                        }
                                        else if (event.GetKeyCode()==WXK_DOWN)
                                        {
                                            long i = tex->GetInsertionPoint();
                                            scrollNumberDown(tex,i);
                                        }
                                        else
                                            event.Skip();
										fordata->setData(tex->GetLineText(0).ToStdString());
										rdraw->refreshFields=true;
										rdraw->refresh();
										dh->refreshFieldsFGS();
									});
					tex->Bind(wxEVT_TEXT,
						 [fordata, tex, rdraw,dh](wxCommandEvent& event)
									{
										fordata->setData(tex->GetLineText(0).ToStdString());
										rdraw->refreshFields=true;
										rdraw->refresh();
										dh->refreshFieldsFGS();
									});

                	}
                    break;
                case dataPoint::color:
                	{
                	colorconv col(((colorData*)obj)->getData());
                    controls[name] = new wxColourPickerCtrl(
                                                            this,
                                                            wxID_ANY,
                                                            *wxBLACK,
                                                            wxDefaultPosition,
                                                            wxDefaultSize,
                                                            wxCLRP_USE_TEXTCTRL//|
                                                            //wxCLRP_SHOW_LABEL
                                                            );
                    ((wxColourPickerCtrl *)controls[name])->SetColour(wxColour(
																	col.getr(),
																	col.getg(),
																	col.getb(),
																	col.geta()
																	)
															);
					colorData * coldata = (colorData *)obj;
                    wxColourPickerCtrl * colr = (wxColourPickerCtrl *)controls[name];
                    drawingAPI * rdraw = owner->getDAPI();
					colr->Bind(wxEVT_COLOURPICKER_CHANGED,
						 [coldata, colr, rdraw](wxCommandEvent& event)
									{
										coldata->setData(colorconv(colr->GetColour()));
										rdraw->refresh();
									});
					}
                    break;
                case dataPoint::collection:
                    {
                    controls[name] = new wxStaticText(this,wxID_ANY,
                                    _("Edit individual\nvertices from\nthe menu\nto the left."),
                                                 wxDefaultPosition,
                                                 wxSize(100,150));}
                    break;
                case dataPoint::font:
                    {
                        controls[name] = new wxFilePickerCtrl 	(this,wxID_ANY,"","Choose a font",
                            "True type (*.ttf)|*.ttf|Open type (*.otf)|*.otf",
                            wxDefaultPosition,wxDefaultSize,wxFD_OPEN|wxFD_FILE_MUST_EXIST);
                        fontData * fndata = (fontData *)obj;
                        wxFilePickerCtrl * fontpc = (wxFilePickerCtrl *)controls[name];
                        drawingAPI * rdraw = owner->getDAPI();
                        wxFileName temp;
                        if (fndata->getData()=="")
                        {
                            temp.SetFullName(fontname);
                            temp.SetPath(fontdir);
                            fontpc->SetFileName(temp);
                        }
                        else
                        {
                            temp.Assign(_(fndata->getData()));
                            fontpc->SetPath(fndata->getData());
                            fontpc->SetFileName(temp);
                        }


                        fontpc->Bind(wxEVT_FILEPICKER_CHANGED,
                             [fndata, fontpc, rdraw](wxCommandEvent& event)
                                        {
                                            fndata->setData(fontpc->GetPath().ToStdString());
                                            rdraw->refresh();
                                        });
                    }
                    break;
                case dataPoint::image:
                    {
                    controls[name] = new wxFilePickerCtrl 	(this,wxID_ANY,"","Choose an image",
"All files|*.bmp;*.png;*.gif;*.jpg;*.jpeg|PNG files (*.png)|*.png|Bitmap files (*.bmp)|*.bmp|GIF files (*.gif)|*.gif|JPG files (*.jpg)|*.jpg;*.jpeg",
                            wxDefaultPosition,wxDefaultSize,wxFD_OPEN|wxFD_FILE_MUST_EXIST);
                    imageData * imdata = (imageData *)obj;
                    wxFilePickerCtrl * filepc = (wxFilePickerCtrl *)controls[name];
                    drawingAPI * rdraw = owner->getDAPI();
                    filepc->Bind(wxEVT_FILEPICKER_CHANGED,
						 [imdata, filepc, rdraw, this](wxCommandEvent& event)
									{
									    wxFileName wfn = filepc->GetPath();
									    if (currentWorkingDirectory=="")
                                            wfn.MakeRelativeTo();
                                        else
                                            wfn.MakeRelativeTo(wxString(currentWorkingDirectory));
                                        imdata->setData(wfn.GetFullPath().ToStdString());
                                        rdraw->refresh();
									});
                    }
                    break;
                case dataPoint::text:
                    {
                	controls[name] = new wxTextCtrl(this,wxID_ANY,
                                                 wxEmptyString,
                                                 wxDefaultPosition,
                                                 wxSize(100,150),
                                                 wxTE_MULTILINE);
                    *((wxTextCtrl *)controls[name])<<((textData *)obj)->getData();
                    textData * fordata = (textData *)obj;
                    wxTextCtrl * tex = (wxTextCtrl *)controls[name];
                    drawingAPI * rdraw = owner->getDAPI();
					tex->Bind(wxEVT_TEXT,
						 [fordata, tex, rdraw](wxCommandEvent& event)
									{
										fordata->setData(tex->GetValue().ToStdString());
										rdraw->refresh();
									});
                	}
                    break;
                case dataPoint::matrix:
                    {
                    int n=0;
                    matrixData * mat = (matrixData*)obj;
                	controls[name] = new wxChoice(this,wxID_ANY);
                    wxChoice * choice = (wxChoice*)controls[name];
                    if (mat->getData()==nullptr)
                        choice->Append("No matrix was set or saved", (void*)nullptr);
                    else
                        choice->Append("None", (void*)nullptr);
                    choice->SetSelection(wxNOT_FOUND);
                    if (mat->getData()==nullptr)
                        choice->SetSelection(0);
                    n++;
                    for (drawable * drawabl: owner->getAllDrawables())
                    {
                        bool flag = true;
                        drawable * unloop = drawabl;
                        dataPoint * dp;
                        while(unloop!=nullptr)
                        {
                            if (unloop==thisOne)
                            {
                                flag=false;
                                break;
                            }
                            dp = unloop->getPropertyByName("tfmatrix");
                            if (dp==nullptr)
                                unloop=nullptr;
                            else if (dp->getType()!=dataPoint::matrix)
                                unloop=nullptr;
                            else
                                unloop = (drawable *) ((matrixData *)dp)->getData();
                        }
                        if (flag)
                        if (drawabl->isSimplex())
                        {
                            if (((shape *)drawabl)->getType()==shape::matrix)
                                choice->Append(drawabl->getName(), (void*)drawabl);
                            if (mat->getData()==(shape*) drawabl)
                                choice->SetSelection(n);
                            n++;
                        }
                    }
                    if (choice->GetSelection()==wxNOT_FOUND&&false)
                        {
                            choice->SetSelection(0);
                            mat->setData(nullptr);
                        }
                    drawingAPI * rdraw = owner->getDAPI();
                    wxFrame * boss = this;
					choice->Bind(wxEVT_CHOICE,
						 [mat, choice, rdraw, boss](wxCommandEvent& event)
									{
                                        if (choice->GetSelection()==0||choice->GetSelection()==wxNOT_FOUND)
                                            {
                                                mat->setData(nullptr);
                                            }
                                        else
                                            {
                                                mat->setData((shape *)choice->GetClientData(
                                                                            choice->GetSelection()));
                                            }
										rdraw->refresh();
									});
                	}
                    break;
                case dataPoint::choice:
                    {
                        bool whether=false;
                        int data;
                        choiceData * ch = (choiceData*)obj;
                        controls[name] = new wxChoice(this,wxID_ANY);
                        wxChoice * choice = (wxChoice*)controls[name];

                        data = ch->getData();
                        std::map<int, std::string> * options = ch->getOptions();

                        for (std::map<int, std::string>::iterator it=options->begin(); it!=options->end(); ++it)
                        {
                            choice->Append(it->second,((void *)(&(it->first))));
                            if (data==it->first)
                                whether=true;
                        }
                        if (whether)
                            choice->SetSelection(data-1);
                        else
                            choice->SetSelection(wxNOT_FOUND);

                        drawingAPI * rdraw = owner->getDAPI();
                        wxFrame * boss = this;
                        choice->Bind(wxEVT_CHOICE,
                             [ch, choice, rdraw, boss](wxCommandEvent& event)
                                        {
                                            if (choice->GetSelection()==0||choice->GetSelection()==wxNOT_FOUND)
                                                {
                                                }
                                            else
                                                {
                                                    ch->setData(
                                                    *((int *)choice->GetClientData(choice->GetSelection())));
                                                }
                                            rdraw->refresh();
                                        });
                	}
                    break;
                }
            if (controls[name]!=nullptr)
            rhs->Add(controls[name], 1,
                           wxALL|
                           wxEXPAND|
                           wxALIGN_CENTER_HORIZONTAL|
                           wxALIGN_CENTER_VERTICAL,
                           5);
            }
    }

    rhs->Layout();
    return true;
}
void styleframe::scrollNumberDown(wxTextCtrl * tex, long pos)
{
    if (isItNeg(tex,pos))
        scroll5to6(tex,pos);
    else
    {
        if (isItAll0(tex, pos))
            switchPosNeg(tex,pos);
        else
            scroll6to5(tex,pos);
    }
    tex->SetInsertionPoint(pos);
}
void styleframe::scrollNumberUp(wxTextCtrl * tex, long pos)
{
    if (isItNeg(tex,pos))
    {
        if (isItAll0(tex, pos))
            switchPosNeg(tex,pos);
        else
            scroll6to5(tex,pos);
    }
    else
        scroll5to6(tex,pos);
    tex->SetInsertionPoint(pos);
}
bool styleframe::isItNeg(wxTextCtrl * tex, long pos)
{
    if (pos<1)
        return false;
    wxString c = tex->GetRange(pos-1,pos);
    while(isItDigit(c)&&pos>1)
    {
        pos--;
        c = tex->GetRange(pos-1,pos);
    }
    if (c==_T("-")) return true;
    return false;
}
void styleframe::switchPosNeg(wxTextCtrl * tex, long pos)
{
    if (pos<1)
    {
        if (pos==0)
        {
            wxString c2 = tex->GetRange(0,1);
            if (c2==_T("-")) tex->Replace(0,1,_T("+"));
            else if (c2==_T("+")) tex->Replace(0,1,_T("-"));
            else tex->Replace(0,1,_T("-")+c2);
        }
        return;
    }
    wxString c = tex->GetRange(pos-1,pos);
    while(isItDigit(c)&&pos>1)
    {
        pos--;
        c = tex->GetRange(pos-1,pos);
    }
    if (c==_T("-")) tex->Replace(pos-1,pos,_T("+"));
    else if (c==_T("+")) tex->Replace(pos-1,pos,_T("-"));
    else tex->Replace(pos-1,pos,_T("-")+c);
}
bool styleframe::isItAll0(wxTextCtrl * tex, long pos)
{
    if (pos==0)
        return (tex->GetRange(0,1)==_T("0"));
    if (pos<0)
        return false;
    wxString c = tex->GetRange(pos-1,pos);
    while((c==_T("0")||c==_T("."))&&pos>1)
    {
        pos--;
        c = tex->GetRange(pos-1,pos);
    }
    if ((c==_T("0")||c==_T("."))&&pos<=1)
        return true;
    if (!isItDigit(c))
        return true;
    return false;
}
bool styleframe::isItDigit(wxString c)
{
    if (c==_T("0")) return true;
    if (c==_T("1")) return true;
    if (c==_T("2")) return true;
    if (c==_T("3")) return true;
    if (c==_T("4")) return true;
    if (c==_T("5")) return true;
    if (c==_T("6")) return true;
    if (c==_T("7")) return true;
    if (c==_T("8")) return true;
    if (c==_T("9")) return true;
    if (c==_T(".")) return true;
    return false;
}
void styleframe::scroll5to6(wxTextCtrl * tex, long pos)
{
    if (pos>0)
    {
        wxString c = tex->GetRange(pos-1,pos);
        if (c==_T("."))
        {
            scroll5to6(tex,pos-1);
            tex->SetInsertionPoint(pos);
            return;
        }
        wxString d = c;
        if (c==_T("0")) c=_T("1");else
        if (c==_T("1")) c=_T("2");else
        if (c==_T("2")) c=_T("3");else
        if (c==_T("3")) c=_T("4");else
        if (c==_T("4")) c=_T("5");else
        if (c==_T("5")) c=_T("6");else
        if (c==_T("6")) c=_T("7");else
        if (c==_T("7")) c=_T("8");else
        if (c==_T("8")) c=_T("9");else
        if (c==_T("9")) c=_T("0");
        if (d==c)
            tex->Replace(pos-1,pos,c+_T("1"));
        else
            tex->Replace(pos-1,pos,c);
        if (c==_T("0"))
        {
            scroll5to6(tex,pos-1);
            tex->SetInsertionPoint(pos);
        }
    }
    else
    {
        wxString c = tex->GetRange(0,1);
        tex->Replace(0,1,_T("1")+c);
    }
}
void styleframe::scroll6to5(wxTextCtrl * tex, long pos)
{
    if (pos>0)
    {
        if (isItAll0(tex,pos))
        {
            switchPosNeg(tex,pos);
            tex->SetInsertionPoint(pos);
            return;
        }
        wxString c = tex->GetRange(pos-1,pos);
        if (c==_T("."))
        {
            scroll6to5(tex,pos-1);
            tex->SetInsertionPoint(pos);
            return;
        }
        wxString d = c;
        if (c==_T("0")) c=_T("9");else
        if (c==_T("1")) c=_T("0");else
        if (c==_T("2")) c=_T("1");else
        if (c==_T("3")) c=_T("2");else
        if (c==_T("4")) c=_T("3");else
        if (c==_T("5")) c=_T("4");else
        if (c==_T("6")) c=_T("5");else
        if (c==_T("7")) c=_T("6");else
        if (c==_T("8")) c=_T("7");else
        if (c==_T("9")) c=_T("8");
        if (d==c)
            switchPosNeg(tex,pos);
        else
            tex->Replace(pos-1,pos,c);
        if (c==_T("9"))
            {
            scroll6to5(tex,pos-1);
            tex->SetInsertionPoint(pos);
            }
    }
}

bool styleframe::clearRHSUI()
{
    for (std::map<std::string, wxControl *>::iterator it=controls.begin(); it!=controls.end(); ++it)
        {
        	if (it->second!=nullptr) //<!> why is it sometimes a null pointer <?>
			{
				rhs->Detach(it->second);
				delete it->second;
			}
        }
    for (std::map<std::string, wxControl *>::iterator it=labels.begin(); it!=labels.end(); ++it)
        {
            if (it->second!=nullptr) //<!> why is it sometimes a null pointer <?>
			{
				rhs->Detach(it->second);
				delete it->second;
			}
        }
	controls.clear();
	labels.clear();
    rhs->Layout();
    return true;
}

bool styleframe::refreshControlsFGS()
{

}
void styleframe::prompt(std::string prompt)
{
    wxMessageDialog info2(this,prompt,"Info:");
    info2.ShowModal();
}

bool styleframe::updateLHSUIFromDataModel()
{
    DrawablesList->DeleteAllItems();
    long i=0;
    for (drawable * obj: model->getAllDrawables())
    if (!obj->itIsCritical())
    {
        /*wxListItem * wli = new wxListItem();
        wxListItem& ipak = *wli;
        ipak.SetText(obj->getName());
        ipak.SetData((void *)obj);*/
        //LHSUI->InsertItem(ipak);
        // I never got that to work, would be a better way to implement things

        DrawablesList->InsertItem(i,obj->getName());
        i++;
    }
    return true;
}
bool styleframe::updateLHSUIPlacedShapes()
{
    PlacedShapesList->DeleteAllItems();
    long i=0;
    for (drawable * obj: owner->getAllDrawables())
    {
        PlacedShapesList->InsertItem(i,obj->getName());
        i++;
    }
    return true;
}
bool styleframe::buildRHSUIForVertex(collectionData * thisOne, int index)
{
    clearRHSUI();
    if (thisOne==nullptr)
        return false;
    labels["x"] = new wxStaticText(this,wxID_ANY,_("X:"));
    rhs->Add(labels["x"], 1,wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT,5);
    controls["x"] = new wxTextCtrl(this,wxID_ANY);
    wxTextCtrl * dbg=(wxTextCtrl *)(controls["x"]);

    *dbg<<_(thisOne->getDataAt(index).x);
    rhs->Add(controls["x"], 1,wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL,5);
    wxTextCtrl * tex = (wxTextCtrl *)controls["x"];
    drawingAPI * rdraw = owner->getDAPI();
    wxListCtrl * quickfix=VerticesList;
    dataHolder * dh = owner;
    tex->Bind(wxEVT_KEY_DOWN,
    [tex, rdraw, quickfix, thisOne, index, this](wxKeyEvent& event)
            {
                if (event.GetKeyCode()==WXK_UP)
                {
                    long i = tex->GetInsertionPoint();
                    scrollNumberUp(tex,i);
                }
                else if (event.GetKeyCode()==WXK_DOWN)
                {
                    long i = tex->GetInsertionPoint();
                    scrollNumberDown(tex,i);
                }
                else
                    event.Skip();
                vertInfo v = thisOne->getDataAt(index);
                v.x=tex->GetLineText(0).ToStdString();
                thisOne->setDataAt(v,index);
                quickfix->SetItemText(index,"("+v.x+"; "+v.y+")");
                rdraw->refresh();
            });
    tex->Bind(wxEVT_TEXT,
    [thisOne,index,tex, rdraw, quickfix, dh](wxCommandEvent& event)
            {
                vertInfo v = thisOne->getDataAt(index);
                v.x=tex->GetLineText(0).ToStdString();
                thisOne->setDataAt(v,index);
                quickfix->SetItemText(index,"("+v.x+"; "+v.y+")");
                rdraw->refreshFields=true;
                rdraw->refresh();
                dh->refreshFieldsFGS();
            });

    labels["y"] = new wxStaticText(this,wxID_ANY,_("Y:"));
    rhs->Add(labels["y"], 1,
                       wxALL|
                       wxEXPAND|
                       wxALIGN_CENTER_HORIZONTAL|
                       wxALIGN_CENTER_VERTICAL|
                       wxALIGN_RIGHT,
                       5);

    controls["y"] = new wxTextCtrl(this,wxID_ANY);
    *((wxTextCtrl *)controls["y"])<<_(thisOne->getDataAt(index).y);
    wxTextCtrl * tex2 = (wxTextCtrl *)controls["y"];
    tex2->Bind(wxEVT_KEY_DOWN,
    [tex2, rdraw, quickfix, thisOne, index, this](wxKeyEvent& event)
            {
                if (event.GetKeyCode()==WXK_UP)
                {
                    long i = tex2->GetInsertionPoint();
                    scrollNumberUp(tex2,i);
                }
                else if (event.GetKeyCode()==WXK_DOWN)
                {
                    long i = tex2->GetInsertionPoint();
                    scrollNumberDown(tex2,i);
                }
                else
                    event.Skip();
                vertInfo v = thisOne->getDataAt(index);
                v.y=tex2->GetLineText(0).ToStdString();
                thisOne->setDataAt(v,index);
                quickfix->SetItemText(index,"("+v.x+"; "+v.y+")");
                rdraw->refresh();
            });
    tex2->Bind(wxEVT_TEXT,
    [thisOne,index,tex2, rdraw,quickfix, dh](wxCommandEvent& event)
        {
            vertInfo v = thisOne->getDataAt(index);
            v.y=tex2->GetLineText(0).ToStdString();
            thisOne->setDataAt(v,index);
            quickfix->SetItemText(index,"("+v.x+"; "+v.y+")");
            rdraw->refreshFields=true;
            rdraw->refresh();
            dh->refreshFieldsFGS();
        });

    rhs->Add(controls["y"], 1,
                           wxALL|
                           wxEXPAND|
                           wxALIGN_CENTER_HORIZONTAL|
                           wxALIGN_CENTER_VERTICAL,
                           5);
    labels["col"] = new wxStaticText(this,wxID_ANY,_("color:"));
    rhs->Add(labels["col"], 1,
                       wxALL|
                       wxEXPAND|
                       wxALIGN_CENTER_HORIZONTAL|
                       wxALIGN_CENTER_VERTICAL|
                       wxALIGN_RIGHT,
                       5);

    colorconv col(thisOne->getDataAt(index).col);
    controls["col"] = new wxColourPickerCtrl(this,wxID_ANY);
    ((wxColourPickerCtrl *)controls["col"])->SetColour(wxColour(
                                                    col.getr(),
                                                    col.getg(),
                                                    col.getb(),
                                                    col.geta()
                                                    )
                                            );
    wxColourPickerCtrl * colr = (wxColourPickerCtrl *)controls["col"];
    colr->Bind(wxEVT_COLOURPICKER_CHANGED,
               [thisOne,index,colr, rdraw](wxCommandEvent& event)
                    {
                        vertInfo v = thisOne->getDataAt(index);
                        v.col=colorconv(colr->GetColour());
                        thisOne->setDataAt(v,index);
                        rdraw->refresh();
                    });

    rhs->Add(controls["col"], 1,
                           wxALL|
                           wxEXPAND|
                           wxALIGN_CENTER_HORIZONTAL|
                           wxALIGN_CENTER_VERTICAL,
                           5);
    rhs->Layout();
    return true;
}
bool styleframe::clearLHSUIV()
{
    VerticesList->DeleteAllItems();
    return true;
}
bool styleframe::updateLHSUIV(std::vector<vertInfo> * vertices)
{
    clearLHSUIV();
    for (int i=0;i<vertices->size();++i)
    {
        vertInfo * vi = &(vertices->at(i));
        VerticesList->InsertItem(i,"("+vi->x+"; "+vi->y+")");
    }
    return true;
}



















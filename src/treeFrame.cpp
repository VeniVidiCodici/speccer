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



#include "treeFrame.h"
#include <iostream>

//(*InternalHeaders(treeFrame)
#include <wx/settings.h>
#include <wx/font.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(treeFrame)
const long treeFrame::ID_OGLPLOTTER = wxNewId();
const long treeFrame::ID_STATICTEXT1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(treeFrame,wxFrame)
	//(*EventTable(treeFrame)
	//*)
END_EVENT_TABLE()

treeFrame::treeFrame(wxWindow* parent,wxWindowID id)
{

	//(*Initialize(treeFrame)
	wxFlexGridSizer* propsfgs;
	wxFlexGridSizer* nodesfgs;
	wxFlexGridSizer* FlexGridSizer1;

	Create(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE, _T("wxID_ANY"));
	FlexGridSizer1 = new wxFlexGridSizer(0, 3, 0, 0);
	FlexGridSizer1->AddGrowableCol(0);
	FlexGridSizer1->AddGrowableRow(0);
	oglplotter = new OGLplotter(this);
	FlexGridSizer1->Add(oglplotter, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	nodesfgs = new wxFlexGridSizer(0, 1, 0, 0);
	nodeLabel = new wxStaticText(this, ID_STATICTEXT1, _("No node selected."), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT1"));
	wxFont nodeLabelFont = wxSystemSettings::GetFont(wxSYS_OEM_FIXED_FONT);
	if ( !nodeLabelFont.Ok() ) nodeLabelFont = wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT);
	nodeLabelFont.SetWeight(wxBOLD);
	nodeLabel->SetFont(nodeLabelFont);
	nodesfgs->Add(nodeLabel, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	propsfgs = new wxFlexGridSizer(0, 2, 0, 0);
	propsfgs->Add(150,5,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	propsfgs->Add(150,5,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	nodesfgs->Add(propsfgs, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer1->Add(nodesfgs, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer1->Add(5,500,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer1->Add(500,5,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer1->Add(300,5,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	SetSizer(FlexGridSizer1);
	FlexGridSizer1->Fit(this);
	FlexGridSizer1->SetSizeHints(this);

	oglplotter->Connect(wxEVT_LEFT_DOWN,(wxObjectEventFunction)&treeFrame::OnoglplotterLeftDown,0,this);
	oglplotter->Connect(wxEVT_MOTION,(wxObjectEventFunction)&treeFrame::OnoglplotterMouseMove,0,this);
	//*)

	propertiesFGS = propsfgs;
	owner = nullptr;
	wMode = browser;
	selectedBranch = nullptr;
    secondaryBranch = nullptr;

    statusBar = new wxStatusBar(this, wxID_ANY, wxST_SIZEGRIP);

    statusBar->SetFieldsCount(6);
    int widths[] = { 130, 130, 130, -1, 130, 130 };
    statusBar->SetStatusWidths(WXSIZEOF(widths), widths);

    SetStatusBar(statusBar);

    currentWorkingDirectory="";

    menubar = new wxMenuBar;
    wxMenu * file = new wxMenu;
    idsave = wxNewId();int * idSave = &idsave;file->Append(*idSave, wxT("&Save"));
    idsaveas = wxNewId();int * idSaveAs = &idsaveas;file->Append(*idSaveAs, wxT("Save &as"));
    idopen = wxNewId();int * idOpen = &idopen;file->Append(*idOpen, wxT("&Open"));

    rv = wxNewId();
    int * skrv = &rv;
    wxMenu * canvasm = new wxMenu;
    canvasm->Append(*skrv, wxT("&Reset view"));

    menubar->Append(file, wxT("&File"));
    menubar->Append(canvasm, wxT("&View"));
    SetMenuBar(menubar);

    Connect(*idSave, wxEVT_COMMAND_MENU_SELECTED,wxCommandEventHandler(treeFrame::save));
    Connect(*idSaveAs, wxEVT_COMMAND_MENU_SELECTED,wxCommandEventHandler(treeFrame::saveAs));
    Connect(*idOpen, wxEVT_COMMAND_MENU_SELECTED,wxCommandEventHandler(treeFrame::open));
    Connect(*skrv, wxEVT_COMMAND_MENU_SELECTED,wxCommandEventHandler(treeFrame::OnViewReset));



    wxBitmap bmAdd, bmInsert, bmAddSibling, bmRemove, bmRemoveall,
    bmSwitchall, bmParent, bmSwitch, bmParentall, bmMoveUp, bmMoveDown;

    if(!bmAdd.LoadFile("icons"+std::string(sysslash)+"add.png",wxBITMAP_TYPE_PNG))
    {
        prompt("Couldn't find icons"+std::string(sysslash)+"add.png - will use a substitute icon instead.");
        bmAdd = wxArtProvider::GetBitmap(wxART_REDO ,wxART_TOOLBAR);
    }
    if(!bmInsert.LoadFile("icons"+std::string(sysslash)+"insert.png",wxBITMAP_TYPE_PNG))
    {
        prompt("Couldn't find icons"+std::string(sysslash)+"insert.png - will use a substitute icon instead.");
        bmInsert = wxArtProvider::GetBitmap(wxART_REDO ,wxART_TOOLBAR);
    }
    if(!bmAddSibling.LoadFile("icons"+std::string(sysslash)+"addsibling.png",wxBITMAP_TYPE_PNG))
    {
        prompt("Couldn't find icons"+std::string(sysslash)+"addsibling.png - will use a substitute icon instead.");
        bmAddSibling = wxArtProvider::GetBitmap(wxART_REDO ,wxART_TOOLBAR);
    }
    if(!bmRemove.LoadFile("icons"+std::string(sysslash)+"delete.png",wxBITMAP_TYPE_PNG))
    {
        prompt("Couldn't find icons"+std::string(sysslash)+"delete.png - will use a substitute icon instead.");
        bmRemove = wxArtProvider::GetBitmap(wxART_REDO ,wxART_TOOLBAR);
    }
    if(!bmRemoveall.LoadFile("icons"+std::string(sysslash)+"uproot.png",wxBITMAP_TYPE_PNG))
    {
        prompt("Couldn't find icons"+std::string(sysslash)+"uproot.png - will use a substitute icon instead.");
        bmRemoveall = wxArtProvider::GetBitmap(wxART_REDO ,wxART_TOOLBAR);
    }
    if(!bmSwitchall.LoadFile("icons"+std::string(sysslash)+"swapall.png",wxBITMAP_TYPE_PNG))
    {
        prompt("Couldn't find icons"+std::string(sysslash)+"swapall.png - will use a substitute icon instead.");
        bmSwitchall = wxArtProvider::GetBitmap(wxART_REDO ,wxART_TOOLBAR);
    }
    if(!bmSwitch.LoadFile("icons"+std::string(sysslash)+"swap.png",wxBITMAP_TYPE_PNG))
    {
        prompt("Couldn't find icons"+std::string(sysslash)+"swap.png - will use a substitute icon instead.");
        bmSwitch = wxArtProvider::GetBitmap(wxART_REDO ,wxART_TOOLBAR);
    }
    if(!bmParent.LoadFile("icons"+std::string(sysslash)+"parent.png",wxBITMAP_TYPE_PNG))
    {
        prompt("Couldn't find icons"+std::string(sysslash)+"parent.png - will use a substitute icon instead.");
        bmParent = wxArtProvider::GetBitmap(wxART_REDO ,wxART_TOOLBAR);
    }
    if(!bmParentall.LoadFile("icons"+std::string(sysslash)+"parentall.png",wxBITMAP_TYPE_PNG))
    {
        prompt("Couldn't find icons"+std::string(sysslash)+"parentall.png - will use a substitute icon instead.");
        bmParentall = wxArtProvider::GetBitmap(wxART_REDO ,wxART_TOOLBAR);
    }
    if(!bmMoveUp.LoadFile("icons"+std::string(sysslash)+"moveup.png",wxBITMAP_TYPE_PNG))
    {
        prompt("Couldn't find icons"+std::string(sysslash)+"moveup.png - will use a substitute icon instead.");
        bmMoveUp = wxArtProvider::GetBitmap(wxART_REDO ,wxART_TOOLBAR);
    }
    if(!bmMoveDown.LoadFile("icons"+std::string(sysslash)+"movedown.png",wxBITMAP_TYPE_PNG))
    {
        prompt("Couldn't find icons"+std::string(sysslash)+"movedown.png - will use a substitute icon instead.");
        bmMoveDown = wxArtProvider::GetBitmap(wxART_REDO ,wxART_TOOLBAR);
    }

    wxToolBar *toolbar = CreateToolBar();
    toolbar->AddTool(wxID_ADD, bmAdd, wxT("Add a new child branch to currently selected branch"));
    toolbar->AddTool(wxID_APPLY, bmAddSibling, wxT("Add a new branch between currently selected branches"));
    toolbar->AddTool(wxID_INDENT, bmInsert, wxT("Insert a new child branch between currently selected branches"));
    toolbar->AddSeparator();
    toolbar->AddTool(wxID_REPLACE, bmSwitchall, wxT("Swap selected branches and all their descendants"));
    toolbar->AddTool(wxID_REFRESH, bmSwitch, wxT("Swap selected branches"));
    toolbar->AddTool(wxID_JUMP_TO, bmParentall, wxT("Parent selected branch and all its descendants to secondary branch"));
    toolbar->AddTool(wxID_ITALIC, bmParent, wxT("Parent selected branch to secondary branch"));
    toolbar->AddSeparator();
    toolbar->AddTool(wxID_HIGHEST, bmMoveDown, wxT("Move selected branch down the siblings hierarchy"));
    toolbar->AddTool(wxID_LOWEST, bmMoveUp, wxT("Move selected branch up the siblings hierarchy"));
    toolbar->AddSeparator();
    toolbar->AddTool(wxID_REMOVE, bmRemove, wxT("Remove selected branch"));
    toolbar->AddTool(wxID_REVERT, bmRemoveall, wxT("Remove selected branch and all its descendants"));
    toolbar->AddSeparator();

    canvas = oglplotter;
    canvas->setUIAPI(this);
    canvas->setWMode(drawingAPI::browser);
    fp = new formulaParser(false,nullptr);
    canvas->f = fp;
    data = new tree(this, canvas);
    canvas->setNodes(data);
    dataModel * dm = data->getCurrentStyle();
    dataHolder * dh = dm->getCurrentNode();
    canvas->setDataHolder(dh);
    fp->setDataHolder(dh);

    wxStaticText * stt = new wxStaticText(toolbar,wxID_ANY, wxT("Style:    "));
    toolbar->AddControl(stt);
    wxChoice * choice = new wxChoice(toolbar,wxID_ANY);
    choice->SetSelection(wxNOT_FOUND);
    std::vector<std::string> styles = data->getModelNames();
    for(int i=0;i<styles.size();i++)
        choice->Append(styles[i]);
    drawingAPI * rdraw = canvas;
    wxFrame * boss = this;
    tree * tdata = data;
    choice->Bind(wxEVT_CHOICE,[choice, rdraw, boss, styles, tdata](wxCommandEvent& event)
        {
            if (choice->GetSelection()==wxNOT_FOUND)
                {
                }
            else
                {
                for(int i=0;i<styles.size();i++)
                    if (i==choice->GetSelection())
                        tdata->setCurrentStyleByName(styles[i]);
                }
            rdraw->refresh();
        });
    toolbar->AddControl(choice,wxT("Choose a tree style"));

    toolbar->Realize();

    Connect(wxID_ADD, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(treeFrame::addBranch));
    Connect(wxID_APPLY, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(treeFrame::addSiblingBranch));
    Connect(wxID_INDENT, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(treeFrame::insertBranch));

    Connect(wxID_REPLACE, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(treeFrame::swapBranches));
    Connect(wxID_REFRESH, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(treeFrame::swapBranchesOnce));
    Connect(wxID_JUMP_TO, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(treeFrame::parentBranch));
    Connect(wxID_ITALIC, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(treeFrame::parentBranchOnce));

    Connect(wxID_HIGHEST, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(treeFrame::moveBranchUp));
    Connect(wxID_LOWEST, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(treeFrame::moveBranchDown));

    Connect(wxID_REMOVE, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(treeFrame::deleteBranch));
    Connect(wxID_REVERT, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(treeFrame::uprootBranch));
}

void treeFrame::addBranch(wxCommandEvent& event)
{
    if (selectedBranch!=nullptr)
        selectedBranch->addChild();
    canvas->refresh();
}
void treeFrame::OnViewReset(wxCommandEvent& WXUNUSED(event))
{
  canvas->resetView();
}
void treeFrame::deleteBranch(wxCommandEvent& event)
{
    if (!prompt("Are you sure you want to delete this branch? The action cannot be undone.","Warning",
                wxYES_NO|wxICON_WARNING|wxSTAY_ON_TOP|wxCENTRE|wxNO_DEFAULT))
        return ;
    std::string res = "No branch selected";
    if (selectedBranch!=nullptr)
        res = selectedBranch->deleteThis();
    if (res!="")
        prompt(res);
    canvas->refresh();
}
void treeFrame::swapBranches(wxCommandEvent& event)
{
    std::string res = "No branch selected";
    if (selectedBranch!=nullptr&&secondaryBranch!=nullptr)
        res = selectedBranch->swapWith(secondaryBranch);
    if (res!="")
        prompt(res);
    canvas->refresh();
}
void treeFrame::parentBranch(wxCommandEvent& event)
{
    std::string res = "No branch selected";
    if (selectedBranch!=nullptr&&secondaryBranch!=nullptr)
        res = selectedBranch->makeChildOf(secondaryBranch);
    if (res!="")
        prompt(res);
    canvas->refresh();
}
void treeFrame::addSiblingBranch(wxCommandEvent& event)
{
    std::string res = "No branch selected";
    if (selectedBranch!=nullptr&&secondaryBranch!=nullptr)
        res = selectedBranch->addSibling(secondaryBranch);
    if (res!="")
        prompt(res);
    canvas->refresh();
}
void treeFrame::insertBranch(wxCommandEvent& event)
{
    std::string res = "No branch selected";
    if (selectedBranch!=nullptr&&secondaryBranch!=nullptr)
        res = selectedBranch->insertChild(secondaryBranch);
    if (res!="")
        prompt(res);
    canvas->refresh();
}
void treeFrame::uprootBranch(wxCommandEvent& event)
{
    if (!prompt("Are you sure you want to delete this branch and all its descendants? The action cannot be undone.",
                "Warning",
                wxYES_NO|wxICON_WARNING|wxSTAY_ON_TOP|wxCENTRE|wxNO_DEFAULT))
        return ;
    std::string res = "No branch selected";
    if (selectedBranch!=nullptr)
        res = selectedBranch->uprootThis();
    if (res!="")
        prompt(res);
    canvas->refresh();
}
void treeFrame::swapBranchesOnce(wxCommandEvent& event)
{
    std::string res = "No branch selected";
    if (selectedBranch!=nullptr&&secondaryBranch!=nullptr)
        res = selectedBranch->swapWithOnce(secondaryBranch);
    if (res!="")
        prompt(res);
    canvas->refresh();
}
void treeFrame::parentBranchOnce(wxCommandEvent& event)
{
    std::string res = "No branch selected";
    if (selectedBranch!=nullptr&&secondaryBranch!=nullptr)
        res = selectedBranch->makeChildOfOnce(secondaryBranch);
    if (res!="")
        prompt(res);
    canvas->refresh();
}

void treeFrame::moveBranchUp(wxCommandEvent& event)
{
    std::string res = "No branch selected";
    if (selectedBranch!=nullptr)
        res = selectedBranch->moveUp();
    if (res!="")
        prompt(res);
    canvas->refresh();
}
void treeFrame::moveBranchDown(wxCommandEvent& event)
{
    std::string res = "No branch selected";
    if (selectedBranch!=nullptr)
        res = selectedBranch->moveDown();
    if (res!="")
        prompt(res);
    canvas->refresh();
}

void treeFrame::resetStylesMenu() //not used here
{

}
branch * treeFrame::getPrimaryBranch()
{
    return selectedBranch;
}
branch * treeFrame::getSecondaryBranch()
{
    return secondaryBranch;
}

treeFrame::~treeFrame()
{
	//(*Destroy(treeFrame)
	//*)
	canvas->f = nullptr;
	if (owner!=nullptr)
        delete owner;
    delete fp;
}
bool treeFrame::buildRHSUIForDrawable(drawable * thisOne) //not used here
{
    return false;
}
std::string treeFrame::getAPropertyFromText(std::string input) /// for now only from text data
{
    std::string buffer ="";
    int inputIndex = 0;
    int inputLength = input.length();
    if (inputLength<3) return "";
    while (input[inputIndex] != '[')
    {
        if (inputIndex>=inputLength-1)
                return "";
        inputIndex++;
    }
    do {
        if (inputIndex>=inputLength-1)
            return "";
        inputIndex++;
        if (input[inputIndex]!=']')
            buffer+=input[inputIndex];
    } while(input[inputIndex]!=']');
    return buffer;
}
std::vector<std::string> treeFrame::getTagsFromText(std::string input)
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
void treeFrame::setSecondaryBranch(branch * toThisOne)
{
    secondaryBranch = toThisOne;
    if (selectedBranch==secondaryBranch)
        selectedBranch=nullptr;
    canvas->refresh();
}
bool treeFrame::buildRHSUIForBranch(branch * thisOne)
{
    //return false;
    selectedBranch = thisOne;
    if (selectedBranch==secondaryBranch)
        secondaryBranch=nullptr;
    canvas->refresh();
    clearRHSUI();
    if (thisOne!=nullptr)
    {
        int x = thisOne->getx(), y = thisOne->gety();
        std::string str = "Node at ("+std::to_string(x)+", "+std::to_string(y)+") selected";

        /// some diagnostic info to use when needed:

        /*str+="\n With id of "+std::to_string(thisOne->getId())+"\n  and address of "+std::to_string((int)thisOne);

        if (thisOne->getParent()==nullptr)
            str+="\n No parent - should be root";
        else
            str+="\n With parent id of " +std::to_string(thisOne->getParent()->getId())+
            "\n  and parent address of "+std::to_string((int)(thisOne->getParent()));
        str+="\n Children count: "+std::to_string(thisOne->getChildren().size());*/

        nodeLabel->SetLabel(wxString(str));

        int unid=0;

        for (drawable * dr: data->getCurrentStyle()->getNodeByName(thisOne->getNode())->getAllDrawables())
        {
            for(dataPoint * obj : dr->getProperties())
            {
                unid++;
                if(obj->getSurfaceName()=="")
                    continue;
                std::string surfnam = obj->getSurfaceName();
                obj = selectedBranch->getProperty(obj->getSurfaceName(),obj->getType());
                std::string name=obj->getName() + std::to_string(unid);
                if (labels.find(name) == labels.end()&&
                   controls.find(name) == controls.end())
                    {
                    labels[name] = new wxStaticText(this,wxID_ANY,_(surfnam+":"));
                    propertiesFGS->Add(labels[name], 1,
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
                            /// not implemented here
                            /*tex->Bind(wxEVT_KEY_DOWN,
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
                                            });*/
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
                                                                    wxCLRP_USE_TEXTCTRL
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
                                controls[name] = new wxStaticText(this,wxID_ANY,
                                wxString("Picking fonts is still not supported in tree editor."));
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
                                controls[name] =
                                new wxStaticText(this,wxID_ANY,
                                wxString("Editing matrices is not allowed in tree editor."));
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
                    propertiesFGS->Add(controls[name], 1,
                                   wxALL|
                                   wxEXPAND|
                                   wxALIGN_CENTER_HORIZONTAL|
                                   wxALIGN_CENTER_VERTICAL,
                                   5);
                    }
            }

            if (dr->isSimplex())
            {
                shape * sh = (shape *)dr;
                if (sh->getType() == shape::text)// for now only this
                {
                    dataPoint * td = sh->getPropertyByName("text");
                    if (td==nullptr) continue;
                    if (td->getType()!=dataPoint::text) continue;
                    if(td->getSurfaceName()!="") continue;
                    std::string txt = ((textData *) td)->getData();
                    std::vector<std::string> tags = getTagsFromText(txt);
                    for(int tg=0;tg<tags.size();tg++)
                        {
                        unid++;
                        std::string toBeSet = tags[tg];
                        std::string idname = toBeSet + std::to_string(unid);
                        if (toBeSet=="") continue;
                        dataPoint * tds = thisOne->getProperty(toBeSet,dataPoint::text);
                        if (tds->getType()!=dataPoint::text) continue;
                        textData * tdss = (textData *) tds;
                        labels[idname] = new wxStaticText(this,wxID_ANY,wxString(toBeSet+":"));
                        propertiesFGS->Add(labels[idname], 1,
                                           wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|
                                           wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT,5);
                        controls[idname] = new wxTextCtrl(this,wxID_ANY,
                                                         wxEmptyString,
                                                         wxDefaultPosition,
                                                         wxSize(100,30),
                                                         wxTE_MULTILINE);

                        wxTextCtrl * dbg=(wxTextCtrl *)(controls[idname]);

                        *dbg<<wxString(tdss->getData());

                        propertiesFGS->Add(controls[idname], 1,
                                           wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL,5);
                        wxTextCtrl * tex = (wxTextCtrl *)controls[idname];
                        drawingAPI * rdraw = owner->getDAPI();
                        tex->Bind(wxEVT_TEXT,
                        [tex, rdraw, tdss, tds](wxCommandEvent& event)
                                {
                                    tdss->setData(tex->GetValue().ToStdString());
                                    rdraw->refresh();
                                });
                        }
                    propertiesFGS->Layout();
                }
            }

        }
        propertiesFGS->Layout();
    }
    else
    {
        nodeLabel->SetLabel(wxT("No node selected"));
    }
    return true;
}

bool treeFrame::clearRHSUI()
{
    for (std::map<std::string, wxControl *>::iterator it=controls.begin(); it!=controls.end(); ++it)
        {
        	if (it->second!=nullptr) /// <!> why is it sometimes a null pointer <?>
			{
				propertiesFGS->Detach(it->second);
				delete it->second;
			}
        }
    for (std::map<std::string, wxControl *>::iterator it=labels.begin(); it!=labels.end(); ++it)
        {
            if (it->second!=nullptr) /// <!> why is it sometimes a null pointer <?>
			{
				propertiesFGS->Detach(it->second);
				delete it->second;
			}
        }
	controls.clear();
	labels.clear();
    propertiesFGS->Layout();
    return true;

    return true;
}
bool treeFrame::updateLHSUIFromDataModel() //not used here
{
    return false;
}
bool treeFrame::updateLHSUIPlacedShapes() //not used here
{
    return false;
}
bool treeFrame::buildRHSUIForVertex(collectionData * thisOne, int index) //not used here
{
    return false;
}
bool treeFrame::refreshFieldsFGS() //not used here
{
    return false;
}
bool treeFrame::refreshControlsFGS() //not used here
{
    return false;
}
void treeFrame::prompt(std::string prompt)
{
    wxMessageDialog info(this,prompt,"Info:");
    info.ShowModal();
}
bool treeFrame::prompt(std::string prompt, std::string caption, long stl)
{
    int res;
    wxMessageDialog info(this,prompt,caption, stl);
    res = info.ShowModal();
    if (res==wxID_OK||res==wxID_YES)
        return true;
    return false;
}
bool treeFrame::clearLHSUIV() //not used here
{
    return false;
}
bool treeFrame::updateLHSUIV(std::vector<vertInfo> * vertices) //not used here
{
    return false;
}
void treeFrame::setDataHolder(dataHolder * toThisOne)
{
    owner = toThisOne;
}
void treeFrame::addDataHolderToStylesMenu(dataHolder * thisOne) //not used here
{
}
void treeFrame::addDataHolderToCompositesMenu(dataHolder * thisOne) //not used here
{
}
void treeFrame::unbug(std::string arg)
{
    //statusBar->SetStatusText(arg,3);
}
void treeFrame::setTitle(std::string title)
{
    SetLabel(wxString(title+" - tree editor"));
}
void treeFrame::setStatusBarString(std::string str, int n)
{
    if (n>=0&&n<=5)
    {
        statusBar->SetStatusText(wxString(str),n);
    }
}
std::vector<std::string> treeFrame::getAllFilesInFolder(std::string folder)
{
    return getAllFilesInFolder(folder,"styl");
}
std::vector<std::string> treeFrame::getAllFilesInFolder(std::string folder, std::string extension)
{
    std::vector<std::string> files;

    wxString wxfolder = wxString(folder);
    wxArrayString *wxfiles = new wxArrayString;
    wxDir dir(wxfolder);
    wxString wtr;


    /// might be the reason for some instances of segfault at window closure!
    dir.GetAllFiles(wxfolder, wxfiles, wxString("*."+extension), wxDIR_FILES|wxDIR_NO_FOLLOW );
    /// might be the reason for some instances of segfault at window closure!

    for(int i=0;i<wxfiles->GetCount();i++)
        {
        wtr = wxfiles->Item(i);
        files.push_back(wtr.ToStdString());
        }

    return files;
}

void treeFrame::OnoglplotterLeftDown(wxMouseEvent& event)
{
    event.Skip();
}

void treeFrame::OnoglplotterRightDown(wxMouseEvent& event)
{
    event.Skip();
}

std::string treeFrame::saveDialog(std::string dir, std::string extension)
{
    wxFileDialog saveFileDialog(this, _("Save"), "", wxString(dir),
                       extension,
                       wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
    if (saveFileDialog.ShowModal() == wxID_CANCEL)
        return "-1";
    currentWorkingDirectory = saveFileDialog.GetDirectory().ToStdString();
    return saveFileDialog.GetPath().ToStdString();
}

std::string treeFrame::openDialog(std::string dir, std::string extension)
{
    wxFileDialog openFileDialog(this, _("Open"), "", wxString(dir),
                       extension,
                       wxFD_OPEN|wxFD_FILE_MUST_EXIST);
    if (openFileDialog.ShowModal() == wxID_CANCEL)
        return "-1";
    currentWorkingDirectory = openFileDialog.GetDirectory().ToStdString();
    return openFileDialog.GetPath().ToStdString();
}

void treeFrame::save(wxCommandEvent& WXUNUSED(event))
{
    std::string filename,dir;
    if (!data->getSaved())
    {
        filename = saveDialog("trees"+std::string(sysslash), "Tree files (*.tree)|*.tree");

        if (filename!="-1")
        {
            data->setSaved(true);
            data->setFilepath(filename);
        }
    }
    else
        filename = data->getFilepath();

    if (filename!="-1")
        {
            std::ofstream ofs(filename, std::ofstream::out|std::ofstream::binary|std::ofstream::trunc);
            data->ws(ofs,"1.0.0");
            data->ws(ofs,std::to_string(sizeof(int)));
            std::string error = data->save(ofs);
            if (error!="")
                prompt(error);
            ofs.close();
        }
}
void treeFrame::saveAs(wxCommandEvent& WXUNUSED(event))
{
    std::string filename,dir;
    filename = saveDialog("trees"+std::string(sysslash), "Tree files (*.tree)|*.tree");
    if (filename!="-1")
    {
        data->setSaved(true);
        data->setFilepath(filename);
        std::ofstream ofs(filename, std::ofstream::out|std::ofstream::binary|std::ofstream::trunc);
        data->ws(ofs,"1.0.0");
        data->ws(ofs,std::to_string(sizeof(int)));
        std::string error = data->save(ofs);
        if (error!="")
            prompt(error);
        ofs.close();
    }
}

void treeFrame::open(wxCommandEvent& event)
{
    if (!data->getSaved()||data->getChanged())
    {
        if (wxMessageBox(_("There have been modifications. Do you wish to save first?"), _("Save?"),
                         wxICON_QUESTION | wxYES_NO, this) == wxYES )
            save(event);
    }
    std::string filename,dir;
    filename = openDialog("trees"+std::string(sysslash), "Tree files (*.tree)|*.tree");
    if (filename!="-1")
    {
        data->setSaved(true);
        data->setFilepath(filename);
        std::ifstream ifs(filename, std::ios::binary);
        std::string version="none", isize;
        version = data->rs(ifs);
        isize =data->rs(ifs);
        int isz = (int)strtod(isize.c_str(), NULL);
        data->set_intsize(isz);

        std::string serror = data->load(ifs, version);
        ifs.close();
        if (serror!="")
            prompt(serror);
        updateLHSUIPlacedShapes();
    }
}

void treeFrame::OnoglplotterMouseMove(wxMouseEvent& event)
{
    long px=0,py=0;
    event.GetPosition(&px,&py);
    canvas->motion(px,py);
}










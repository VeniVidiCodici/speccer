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



#ifndef STYLEFRAME_H
#define STYLEFRAME_H

//(*Headers(styleframe)
#include <wx/listctrl.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/msgdlg.h>
#include <wx/textdlg.h>
#include <wx/toolbar.h>
#include <wx/gbsizer.h>
#include <wx/button.h>
#include <wx/frame.h>
//*)

#include "oglplotter.h"
#include <dataholder.h>
#include <datamodel.h>
#include <vector>
#include <alldatatypes.h>
#include <wx/wx.h>
#include <wx/menu.h>
#include <wx/statusbr.h>
#include <string>
#include <wx/arrstr.h>
#include <wx/dir.h>
#include <formulaparser.h>
#include <wx/spinctrl.h>
#include <wx/pickerbase.h>
#include <wx/colordlg.h>
#include <wx/filectrl.h>
#include <wx/filename.h>
#include <wx/clrpicker.h>
#include <wx/filepicker.h>

#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#if !wxUSE_GLCANVAS
    #error "OpenGL required: set wxUSE_GLCANVAS to 1 and rebuild the library"
#endif
enum
{
    SpinTimer = wxID_HIGHEST + 1
};


class styleframe: public wxFrame, public UIAPI
{
	public:

		styleframe(wxWindow* parent,wxWindowID id=wxID_ANY);
		virtual ~styleframe();

        void OnViewReset(wxCommandEvent& event);
        void save(wxCommandEvent& event);
        void saveAs(wxCommandEvent& event);
        void open(wxCommandEvent& event);
        void saveAsComposite(wxCommandEvent& event);
        void saveAsANewComposite(wxCommandEvent& event);
        void openComposite(wxCommandEvent& event);
        void switchToNode(wxCommandEvent& event);
        void addANewNode(wxCommandEvent& event);
        void addANewComposite(wxCommandEvent& event);
        bool buildRHSUIForBranch(branch * thisOne);

        wxMenuBar *menubar;
        wxMenu *view;
        wxMenu *canvasm;


        wxStatusBar* statusBar;


		//(*Declarations(styleframe)
		OGLplotter* oglplotter;
		wxListCtrl* PlacedShapesList;
		wxStaticText* aslabel;
		wxButton* subutton;
		wxTextEntryDialog* renameDialog;
		wxToolBar* vartoolbar;
		wxButton* vuButton;
		wxMessageDialog* infoBox;
		wxButton* avButton;
		wxListCtrl* VerticesList;
		wxButton* rvButton;
		wxButton* svbutton;
		wxButton* removeButton;
		wxStaticText* selectedShapeLabel;
		wxButton* vdButton;
		wxStaticText* vlabel;
		wxListCtrl* DrawablesList;
		wxStaticText* pslabel;
		wxButton* addButton;
		//*)

		dataHolder * DH;
        wxFlexGridSizer * f4;
        wxFlexGridSizer * f2;
        wxFlexGridSizer * mainfgs;
        wxFlexGridSizer * rhs; // right hand side
        wxGridBagSizer * lhs;
        wxFlexGridSizer * ffgs;
        double roundto2(double n);
        bool refreshFieldsFGS();
        void clearFieldsFGS();
        void unbug(std::string k);
        void setDataHolder(dataHolder * toThisOne);

        std::map<std::string, wxControl *> fieldControls;
        std::map<std::string, wxControl *> fieldLabels;
        wxFlexGridSizer * fieldFGS;

        bool buildRHSUIForDrawable(drawable * thisOne);
        bool clearRHSUI();
        bool updateLHSUIFromDataModel();
        bool updateLHSUIPlacedShapes();
        bool clearLHSUIV();
        bool updateLHSUIV(std::vector<vertInfo> * vertices);
        bool buildRHSUIForVertex(collectionData * thisOne, int index);
        bool refreshControlsFGS();
        void prompt(std::string prompt);

        void toggleDrawables(wxCommandEvent& event);
        void toggleShapes(wxCommandEvent& event);
        void toggleVertices(wxCommandEvent& event);
        void toggleProperties(wxCommandEvent& event);
        void toggleVariables(wxCommandEvent& event);

        void addDataHolderToStylesMenu(dataHolder * thisOne);
        void addDataHolderToCompositesMenu(dataHolder * thisOne);
        void setTitle(std::string title);
        void setSecondaryBranch(branch * toThisOne);
        branch * getPrimaryBranch();
        branch * getSecondaryBranch();
        void resetStylesMenu();

        std::vector<std::string> getAllFilesInFolder(std::string folder);
        std::vector<std::string> getAllFilesInFolder(std::string folder, std::string extension);

        std::string debugstr;

	protected:

		//(*Identifiers(styleframe)
		static const long ID_STATICTEXT1;
		static const long ID_STATICTEXT2;
		static const long ID_STATICTEXT4;
		static const long ID_LISTCTRL1;
		static const long ID_LISTCTRL2;
		static const long ID_LISTCTRL3;
		static const long ID_BUTTON5;
		static const long ID_BUTTON6;
		static const long ID_BUTTON3;
		static const long ID_BUTTON4;
		static const long ID_BUTTON2;
		static const long ID_BUTTON1;
		static const long ID_BUTTON7;
		static const long ID_BUTTON8;
		static const long ID_OGLPLOTTER1;
		static const long ID_STATICTEXT3;
		static const long ID_TEXTENTRYDIALOG1;
		static const long ID_MESSAGEDIALOG1;
		static const long ID_TOOLBAR1;
		//*)

		static const long ID_GLCANVASMCTT;

	private:

	    drawingAPI * canvas;
        dataModel * model;
        wxMenu * nodes;
        wxMenu * composites;

        void OnNewWindow(wxCommandEvent& event);
        void OnNewStereoWindow(wxCommandEvent& event);
        std::map<std::string, wxControl *> controls;
        std::map<std::string, wxControl *> labels;

        std::string fontdir, fontname;
        int asl,psl,vl,pl,gvl,rv,ind,icm;
        int idopen, idsave, idsaveas;
        formulaParser * fp;

        void scrollNumberDown(wxTextCtrl * tex, long pos);
        void scrollNumberUp(wxTextCtrl * tex, long pos);

        void scroll5to6(wxTextCtrl * tex, long pos);
        void scroll6to5(wxTextCtrl * tex, long pos);

        bool isItNeg(wxTextCtrl * tex, long pos);
        bool isItDigit(wxString c);
        void switchPosNeg(wxTextCtrl * tex, long pos);
        bool isItAll0(wxTextCtrl * tex, long pos);

        void setStatusBarString(std::string str, int n);

        std::string saveDialog(std::string dir, std::string extension);
        std::string openDialog(std::string dir, std::string extension);


		//(*Handlers(styleframe)
		void OnResize(wxSizeEvent& event);
		void Onplotter1MouseMove(wxMouseEvent& event);
		void OnDrawablesListItemSelect(wxListEvent& event);
		void OnButton1Click(wxCommandEvent& event);
		void OnButton2Click(wxCommandEvent& event);
		void OnaddButtonClick(wxCommandEvent& event);
		void OnremoveButtonClick(wxCommandEvent& event);
		void OnPlacedShapesListItemSelect(wxListEvent& event);
		void OnPlacedShapesListItemActivated(wxListEvent& event);
		void OnDrawablesListItemActivated(wxListEvent& event);
		void OnVerticesListItemSelect(wxListEvent& event);
		void OnavButtonClick(wxCommandEvent& event);
		void OnrvButtonClick(wxCommandEvent& event);
		void OnvuButtonClick(wxCommandEvent& event);
		void OnvdButtonClick(wxCommandEvent& event);
		void OnClose(wxCloseEvent& event);
		void OnGLCanvas1Paint(wxPaintEvent& event);
		void OnoglplotterMouseMove(wxMouseEvent& event);
		void OnoglplotterResize(wxSizeEvent& event);
		void OnsubuttonClick(wxCommandEvent& event);
		void OnsvbuttonClick(wxCommandEvent& event);
		//*)

		DECLARE_EVENT_TABLE()
};

#endif

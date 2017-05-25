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



#ifndef TREEFRAME_H
#define TREEFRAME_H
#include <map>
#include <string>
#include <uiapi.h>
#include "drawable.h"
#include "oglplotter.h"
#include <wx/arrstr.h>
#include <wx/dir.h>
#include <wx/artprov.h>
#include "tree.h"
#include <wx/combobox.h>
#include <wx/spinctrl.h>
#include <wx/filepicker.h>
#include <wx/clrpicker.h>
#include "branch.h"

#if !wxUSE_GLCANVAS
    #error "OpenGL required: set wxUSE_GLCANVAS to 1 and rebuild the library"
#endif

//(*Headers(treeFrame)
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/frame.h>
//*)


class treeFrame: public wxFrame, public UIAPI
{
	public:

		treeFrame(wxWindow* parent,wxWindowID id=wxID_ANY);
		~treeFrame();

		bool buildRHSUIForDrawable(drawable * thisOne); //not used here
		bool buildRHSUIForBranch(branch * thisOne);
        bool clearRHSUI();
        bool updateLHSUIFromDataModel(); //not used here
        bool updateLHSUIPlacedShapes(); //not used here
        bool buildRHSUIForVertex(collectionData * thisOne, int index); //not used here
        bool refreshFieldsFGS(); //not used here
        bool refreshControlsFGS(); //not used here
        void prompt(std::string prompt);
        bool prompt(std::string prompt, std::string caption, long stl = wxCENTRE);
        bool clearLHSUIV(); //not used here
        bool updateLHSUIV(std::vector<vertInfo> * vertices); //not used here
        void setDataHolder(dataHolder * toThisOne);
        void addDataHolderToStylesMenu(dataHolder * thisOne); //not used here
        void addDataHolderToCompositesMenu(dataHolder * thisOne); //not used here
        void unbug(std::string arg);
        void setTitle(std::string title);
        std::vector<std::string> getAllFilesInFolder(std::string folder);
        std::vector<std::string> getAllFilesInFolder(std::string folder, std::string extension);
        void setSecondaryBranch(branch * toThisOne);
        branch * getPrimaryBranch();
        branch * getSecondaryBranch();

        void addBranch(wxCommandEvent& event);
        void deleteBranch(wxCommandEvent& event);
        void swapBranches(wxCommandEvent& event);
        void parentBranch(wxCommandEvent& event);

        void addSiblingBranch(wxCommandEvent& event);
        void insertBranch(wxCommandEvent& event);
        void uprootBranch(wxCommandEvent& event);
        void swapBranchesOnce(wxCommandEvent& event);
        void parentBranchOnce(wxCommandEvent& event);

        void moveBranchUp(wxCommandEvent& event);
        void moveBranchDown(wxCommandEvent& event);

        void setStatusBarString(std::string str, int n);


        void save(wxCommandEvent& event);
        void saveAs(wxCommandEvent& event);
        void open(wxCommandEvent& event);
        void resetStylesMenu();

		//(*Declarations(treeFrame)
		OGLplotter* oglplotter;
		wxStaticText* nodeLabel;
		//*)

	protected:

		//(*Identifiers(treeFrame)
		static const long ID_OGLPLOTTER;
		static const long ID_STATICTEXT1;
		//*)

	private:

		//(*Handlers(treeFrame)
		void OnoglplotterLeftDown(wxMouseEvent& event);
		void OnoglplotterRightDown(wxMouseEvent& event);
		void OnoglplotterMouseMove(wxMouseEvent& event);
		//*)

//		fileIO * fio;
        formulaParser * fp;
		wxFlexGridSizer * propertiesFGS;
		wxStatusBar* statusBar;
		drawingAPI * canvas;
		int idopen, idsave, idsaveas, rv;
		wxMenuBar *menubar;
		tree * data;
		branch * selectedBranch;
		branch * secondaryBranch;
		std::map<std::string, wxControl *> controls;
		std::map<std::string, wxControl *> labels;

		std::string getAPropertyFromText(std::string input);
		std::vector<std::string> getTagsFromText(std::string input);
		std::string saveDialog(std::string dir, std::string extension);
        std::string openDialog(std::string dir, std::string extension);

        void OnViewReset(wxCommandEvent& event);

		DECLARE_EVENT_TABLE()
};

#endif

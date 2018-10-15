#ifndef _BLAMFRAME_H_
#define _BLAMFRAME_H_

#include <wx/wx.h>
#include <wx/spinctrl.h>
#include <wx/treectrl.h>

#include "truthTable.h"
#include "KarnaughMap.h"
#include "kmap.h"

/**
 * @short Application Main Frame
 * @author Robert Kovacevic <robert.kovacevic@etfos.hr>
 * @version 0.1
 */

class blamFrame : public wxFrame
{
	public:
		blamFrame( const wxString& title, const wxPoint& pos, const wxSize& pos, wxLocale& locale );
		void OnQuit( wxCommandEvent& event );
		void OnAbout( wxCommandEvent& event );
		void OnCellAdresses( wxCommandEvent& event );
		void OnShowZeros( wxCommandEvent& event );
		void OnLangCroatian( wxCommandEvent& event );
		void OnLangDefault( wxCommandEvent& event );
		void OnVarsChange( wxSpinEvent& event );

	private:
		void OnTruthTChange( wxGridEvent& event );
		void OnKMapChange( wxGridEvent& event );
		void OnSolve( wxCommandEvent& event );
		void OnSolSelect( wxTreeEvent& event );
		void OnSolutionTypeChange( wxCommandEvent& event );
		
		wxSpinCtrl* numberOfVariables;
		KMap* kmap;
		TruthTable* truthTable;
		wxTreeCtrl* solution;
		wxChoice* solutionType;
		list <KarnaughNode> solutionsList;
		bool solveSOP;
		
		wxLocale& m_locale;
		
		DECLARE_EVENT_TABLE()
};

class SolveTreeItemData : public wxTreeItemData
{
	public:
		SolveTreeItemData();
		vector<unsigned short int> values;
};

enum
{
	Menu_File_Quit = 100,
	Menu_File_About,
	Menu_Language_Croatian,
	Menu_Language_Default,
	Menu_Cell_Adresses,
	Menu_Show_Zeros,
	Vars_Count,
	Truth_Table,
	Karnaugh_Map,
	Solve_Button,
	Sol_Tree,
	Solution_Type
};

#endif // _BLAMFRAME_H_

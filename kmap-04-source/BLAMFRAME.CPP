#include <wx/wx.h>
#include <wx/notebook.h>
#include <wx/grid.h>
#include <wx/treectrl.h>
#include <wx/spinctrl.h>
#include <wx/config.h>

#include <list>
#include <vector>

#include "truthTable.h"
#include "kmap.h"

#include "blamFrame.h"

BEGIN_EVENT_TABLE( blamFrame, wxFrame )
	EVT_MENU( Menu_File_Quit, blamFrame::OnQuit )
	EVT_MENU( Menu_File_About, blamFrame::OnAbout )
	EVT_MENU( Menu_Language_Croatian, blamFrame::OnLangCroatian )
	EVT_MENU( Menu_Language_Default, blamFrame::OnLangDefault )
	EVT_MENU( Menu_Cell_Adresses, blamFrame::OnCellAdresses )
	EVT_MENU( Menu_Show_Zeros, blamFrame::OnShowZeros )
	EVT_SPINCTRL ( Vars_Count, blamFrame::OnVarsChange )
	EVT_GRID_CMD_CELL_CHANGE(Truth_Table, blamFrame::OnTruthTChange)
	EVT_GRID_CMD_CELL_CHANGE(Karnaugh_Map, blamFrame::OnKMapChange)
	EVT_BUTTON(Solve_Button, blamFrame::OnSolve)
	EVT_TREE_SEL_CHANGED(Sol_Tree, blamFrame::OnSolSelect)
	EVT_CHOICE(Solution_Type, blamFrame::OnSolutionTypeChange)
END_EVENT_TABLE()


blamFrame::blamFrame( const wxString& title, const wxPoint& pos, const wxSize& size, wxLocale& locale )
	: wxFrame((wxFrame *)NULL, -1, title, pos, size), m_locale(locale)
{
	/**** Variable initialization ****/
	solveSOP=1;
	
	/**** Icon *****/

	wxIcon icon("wxwin.ico", wxBITMAP_TYPE_ICO);
	SetIcon(icon);

	/**** Menu *****/
	
	wxMenu *menuFile = new wxMenu;
	
	// wxBitmap aboutBitmap(wxT("about.xpm"), wxBITMAP_TYPE_XPM );
	wxMenuItem* about=new wxMenuItem(0, Menu_File_About, _("&About"), _("About the program"));
	// about->SetBitmap(aboutBitmap);
	
	// wxBitmap exitBitmap(wxT("exit.xpm"), wxBITMAP_TYPE_XPM );
	wxMenuItem* exit=new wxMenuItem(0, Menu_File_Quit, _("E&xit"), _("Exit the program"));
	// exit->SetBitmap(exitBitmap);
	
	menuFile->Append( about );
	menuFile->AppendSeparator();
	menuFile->Append( exit );
	
	// Settings menu
	
	wxMenu *menuSettings = new wxMenu;
	wxMenu *menuLanguage = new wxMenu;
	
	wxMenuItem* english=new wxMenuItem(menuLanguage, Menu_Language_Default, _("English (default)"), _("Set language to english"), wxITEM_RADIO);
	wxMenuItem* croatian=new wxMenuItem(menuLanguage, Menu_Language_Croatian, _("Croatian"), _("Set language to croatian"), wxITEM_RADIO);
	
	menuLanguage->Append( english );
	menuLanguage->Append( croatian );
	
	menuSettings->Append(-1, _("Language"), menuLanguage, _("Set language (requires restart)"));
	
	menuSettings->AppendSeparator();
	
	wxMenuItem* showZeros=new wxMenuItem(0, Menu_Show_Zeros, _("Show zeros"), _("Show / hide zero values"), wxITEM_CHECK);
	wxMenuItem* showAdress=new wxMenuItem(0, Menu_Cell_Adresses, _("Show cell adresses"), _("Show / hide cell adresses in the K-map"), wxITEM_CHECK);
	
	menuSettings->Append( showZeros );
	menuSettings->Append( showAdress );
	
	wxMenuBar *menuBar = new wxMenuBar;
	menuBar->Append( menuFile, _( "&Program" ) );
	menuBar->Append( menuSettings, _( "&Settings" ) );
	
	
	SetMenuBar( menuBar );
	
	/**** Status Bar *****/
	
	CreateStatusBar();
	SetStatusText( _( "Welcome to Karnaugh Map Minimizer!" ) );
	
	/**** Main panel initialization *****/
	
	wxPanel* mainPanel=new wxPanel(this, -1, wxDefaultPosition, wxDefaultSize);//wxSize(500,650));
	
	/**** Notebook control initialization *****/
	
	wxNotebook* methodBook = new wxNotebook(mainPanel, -1);
	
	/**** Sizers initialization *****/
	
	wxBoxSizer* panelSizer=new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* mainSizer=new wxBoxSizer(wxHORIZONTAL);
	wxStaticBoxSizer* leftSizer=new wxStaticBoxSizer(new wxStaticBox(mainPanel, -1, _("Truth table")), wxVERTICAL);
	wxBoxSizer* rightSizer=new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* rightSizerTop=new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* rightSizerBottom=new wxBoxSizer(wxHORIZONTAL);
	
	panelSizer->Add(mainPanel, 1, wxEXPAND);
	rightSizer->Add(rightSizerTop, 0, wxEXPAND | wxBOTTOM, 10);
	rightSizer->Add(rightSizerBottom, 2, wxEXPAND);
	mainSizer->Add(leftSizer, 1, wxEXPAND | wxALL, 10);
	mainSizer->Add(rightSizer, 2, wxEXPAND | wxTOP | wxRIGHT | wxBOTTOM, 10);
	
	/**** Controls *****/
	
	/**** Trouth table *****/
	
	truthTable = new TruthTable(mainPanel, Truth_Table, 4, m_locale, wxDefaultPosition, wxSize(170,450), wxSIMPLE_BORDER, wxPanelNameStr);
	leftSizer->Add(truthTable, 1, wxEXPAND | wxALL, 5);
	/*
	leftSizer->Add(new wxButton(mainPanel, -1, wxT("Fill from function")), 0, wxEXPAND | wxALL, 5);
	leftSizer->Add(new wxButton(mainPanel, -1, wxT("Fill from sets")), 0, wxEXPAND | wxLEFT | wxBOTTOM | wxRIGHT, 5);
	*/
	
	/**** Karnaugh Map Notebook page *****/
	
	wxPanel* kmapPanel = new wxPanel(methodBook, -1);
	
	wxBoxSizer* kmapSizer = new wxBoxSizer(wxVERTICAL);
	
	/**** Karnaugh map grid control *****/
	
	kmap=new KMap(kmapPanel, Karnaugh_Map, 4, m_locale, wxDefaultPosition, wxSize(100,150), wxSIMPLE_BORDER);
	kmapSizer->Add(kmap, 1, wxEXPAND | wxALL, 5);
	
	/**** Solve Button ****/
	
	rightSizer->Add(new wxButton(mainPanel, Solve_Button, _("Solve")), 0, wxALIGN_RIGHT | wxTOP, 10);
	
	/**** Solutions tree control *****/
	
	rightSizer->Add(new wxStaticText(mainPanel, -1, _("Solution:")), 0, wxEXPAND | wxTOP, 0);
	
	solution=new wxTreeCtrl(mainPanel, Sol_Tree, wxDefaultPosition, wxDefaultSize, wxSIMPLE_BORDER | wxTR_HAS_BUTTONS );
	
	rightSizer->Add(solution, 1, wxEXPAND | wxTOP, 5);
	
	kmapPanel->SetSizer(kmapSizer);
	
	methodBook->AddPage(kmapPanel, _("Karnaugh map"));
	
	rightSizerBottom->Add(methodBook, 1, wxEXPAND  | wxBOTTOM, 10);
	
	/**** Settings controls ****/
	
	rightSizerTop->Add(new wxStaticText(mainPanel, -1, _("Number of variables: ")), 0, wxCENTER | wxRIGHT, 10);
	numberOfVariables=new wxSpinCtrl(mainPanel, Vars_Count, wxT("4"), wxDefaultPosition, wxSize(50,-1));
	numberOfVariables->SetRange(1, 8);
	rightSizerTop->Add(numberOfVariables, 0, wxCENTER);
	rightSizerTop->Add(new wxStaticText(mainPanel, -1, _("Type of solution: ")), 0, wxCENTER | wxLEFT | wxRIGHT, 10);
	solutionType = new wxChoice(mainPanel, Solution_Type);
	solutionType->Append(_("Sum of products"));
	solutionType->Append(_("Product of sums"));
	solutionType->SetSelection(0);
	rightSizerTop->Add(solutionType, 1,  wxCENTER);
	
	/**** Set sizers *****/
	
	mainPanel->SetSizer(mainSizer);
	mainSizer->SetSizeHints(mainPanel);
	
	this->SetSizerAndFit(panelSizer);
	panelSizer->SetSizeHints(this);
	this->SetAutoLayout(1);
	
	/**** Load configuration ****/
	
	wxConfig *config = new wxConfig(wxT("Karnaugh Map Minimizer"));
	wxString lang;
	if ( config->Read(wxT("Language"), &lang) ) 
	{
		if(lang==wxT("")) menuLanguage->Check(Menu_Language_Default, 1);
		if(lang==wxT("hr")) menuLanguage->Check(Menu_Language_Croatian, 1);
	}
	
	wxString adresses;
	if ( config->Read(wxT("Cell_Adresses"), &adresses) ) 
	{
		if(adresses==wxT("no")) 
		{
			menuSettings->Check(Menu_Cell_Adresses, 0);
			kmap->SetCellAdresses(0);
		}
		else 
		{
			menuSettings->Check(Menu_Cell_Adresses, 1);
			kmap->SetCellAdresses(1);
		}
	}
	else 
	{
		menuSettings->Check(Menu_Cell_Adresses, 1);
		kmap->SetCellAdresses(1);
	}
	
	wxString zeros;
	if ( config->Read(wxT("Show_Zeros"), &zeros) ) 
	{
		if(zeros==wxT("no")) 
		{
			menuSettings->Check(Menu_Show_Zeros, 0);
			kmap->SetShowZeros(0);
			truthTable->SetShowZeros(0);
		}
		else 
		{
			menuSettings->Check(Menu_Show_Zeros, 1);
			kmap->SetShowZeros(1);
			truthTable->SetShowZeros(1);
		}
	}
	else 
	{
		menuSettings->Check(Menu_Show_Zeros, 0);
		kmap->SetShowZeros(0);
		truthTable->SetShowZeros(0);
	}
	
	delete config;
	
}

void blamFrame::OnVarsChange( wxSpinEvent& event )
{
	solution->DeleteAllItems();
	kmap->ClearSolutions();
	
	truthTable->SetVars(numberOfVariables->GetValue());
	kmap->SetVars(numberOfVariables->GetValue());
}

void blamFrame::OnTruthTChange( wxGridEvent& event )
{
	solution->DeleteAllItems();
	kmap->ClearSolutions();
	
	kmap->Set(event.GetRow(), truthTable->GetCellValue(event.GetRow(), truthTable->GetNumberCols()-1));
}

void blamFrame::OnKMapChange( wxGridEvent& event )
{
	solution->DeleteAllItems();
	kmap->ClearSolutions();
	
	truthTable->SetCellValue(kmap->GetMapValue(event.GetCol(), event.GetRow()), truthTable->GetNumberCols()-1, kmap->GetCellValue(event.GetRow(), event.GetCol()));
}

void blamFrame::OnSolve( wxCommandEvent& event )
{
	this->SetStatusText( _( "Solving, please wait..." ) );
	solution->DeleteAllItems();
	kmap->ClearSolutions();
	
	KarnaughMap K(numberOfVariables->GetValue());
	for(int i=0; i<kmap->GetNumberRows(); i++)
	{
		for(int j=0; j<kmap->GetNumberCols(); j++)
		{
			if(solveSOP)
			{
				if(kmap->GetCellValue(i, j)==wxT("1"))
					K.Set(j,i, 1);
				if(kmap->GetCellValue(i, j)==wxT("0"))
					K.Set(j,i, 0);
				if(kmap->GetCellValue(i, j)==wxT("?"))
					K.Set(j,i, 2);
			}
			else
			{
				if(kmap->GetCellValue(i, j)==wxT("1"))
					K.Set(j,i, 0);
				if( (kmap->GetCellValue(i, j)==wxT("0")) || (kmap->GetCellValue(i, j)==wxT("")))
					K.Set(j,i, 1);
				if(kmap->GetCellValue(i, j)==wxT("?"))
					K.Set(j,i, 2);
			}
		}
	}
	K.Solve();
	
	wxString s=wxT("X = ");
	solution->AddRoot(s);
	
	solutionsList=K.GetSolutions();
	
	list <KarnaughNode>::const_iterator iter;

	if(solveSOP)
	{
		for (iter=solutionsList.begin(); iter != solutionsList.end(); iter++)
		{
			kmap->AddSolution(iter->values);
			
			wxString b; // A, B, C, D
			wxString a; // All the rest
			for(int i=0; i<iter->values.size(); i++)
			{
				int apos=numberOfVariables->GetValue()-4;
				if(apos<0) apos=0;
				int bpos=numberOfVariables->GetValue()-3;
				if(bpos<1) bpos=1;
				int cpos=numberOfVariables->GetValue()-2;
				if(cpos<2) cpos=2;
				int dpos=numberOfVariables->GetValue()-1;
				if(dpos<3) dpos=3;
				
				if((i!=apos)&&(i!=bpos)&&(i!=cpos)&&(i!=dpos))
				{
					if(iter->values[i]==1)
					{
						a.Append(wxString::Format(wxT("%c"), 68+(numberOfVariables->GetValue()-4-i)));
					}
					if(iter->values[i]==0)
					{
						a.Append(wxT("|"));
						a.Append(wxString::Format(wxT("%c"), 68+(numberOfVariables->GetValue()-4-i)));
					}
				}
				
				if(i==apos)
				{
					if(iter->values[i]==1)
						b.Append(wxT("A"));
					if(iter->values[i]==0)
						b.Append(wxT("|A"));
				}
				if(i==bpos)
				{
					if(iter->values[i]==1)
						b.Append(wxT("B"));
					if(iter->values[i]==0)
						b.Append(wxT("|B"));
				}
				if(i==cpos)
				{
					if(iter->values[i]==1)
						b.Append(wxT("C"));
					if(iter->values[i]==0)
						b.Append(wxT("|C"));
				}
				if(i==dpos)
				{
					if(iter->values[i]==1)
						b.Append(wxT("D"));
					if(iter->values[i]==0)
						b.Append(wxT("|D"));
				}
			}
			b=b+a;
			
			if(b==wxT("")) b=wxT("1");
			s+=b;
			
			SolveTreeItemData* bData=new SolveTreeItemData();
			bData->values=iter->values;
			solution->AppendItem(solution->GetRootItem(), b, -1, -1, bData);
			
			if(iter!=(--solutionsList.end())) s.Append(wxT(" + "));
		}
		
		if(s==wxT("X = ")) s=wxT("X = 0");
	}
	else
	{
		for (iter=solutionsList.begin(); iter != solutionsList.end(); iter++)
		{
			kmap->AddSolution(iter->values);
			
			wxString b=wxT("(");
			wxString a;
			for(int i=0; i<iter->values.size(); i++)
			{
				int apos=numberOfVariables->GetValue()-4;
				if(apos<0) apos=0;
				int bpos=numberOfVariables->GetValue()-3;
				if(bpos<1) bpos=1;
				int cpos=numberOfVariables->GetValue()-2;
				if(cpos<2) cpos=2;
				int dpos=numberOfVariables->GetValue()-1;
				if(dpos<3) dpos=3;
				
				if((i!=apos)&&(i!=bpos)&&(i!=cpos)&&(i!=dpos))
				{
					if(iter->values[i]==0)
					{
						a.Append(wxString::Format(wxT("%c+"), 68+(numberOfVariables->GetValue()-4-i)));
					}
					if(iter->values[i]==1)
					{
						a.Append(wxT("|"));
						a.Append(wxString::Format(wxT("%c+"), 68+(numberOfVariables->GetValue()-4-i)));
					}
				}
				
				if(i==apos)
				{
					if(iter->values[i]==0)
						b.Append(wxT("A+"));
					if(iter->values[i]==1)
						b.Append(wxT("|A+"));
				}
				if(i==bpos)
				{
					if(iter->values[i]==0)
						b.Append(wxT("B+"));
					if(iter->values[i]==1)
						b.Append(wxT("|B+"));
				}
				if(i==cpos)
				{
					if(iter->values[i]==0)
						b.Append(wxT("C+"));
					if(iter->values[i]==1)
						b.Append(wxT("|C+"));
				}
				if(i==dpos)
				{
					if(iter->values[i]==0)
						b.Append(wxT("D+"));
					if(iter->values[i]==1)
						b.Append(wxT("|D+"));
				}
			}
			b=b+a;
			if(b==wxT("")) b=wxT("0");
				else b.SetChar(b.size()-1, wxT(')'));
			s+=b;
			
			SolveTreeItemData* bData=new SolveTreeItemData();
			bData->values=iter->values;
			solution->AppendItem(solution->GetRootItem(), b, -1, -1, bData);
		}	
		if(s==wxT("X = ")) s=wxT("X = 1");
	}
	
	solution->Expand(solution->GetRootItem());
	solution->SetItemText(solution->GetRootItem(), s);
	
	SetStatusText( _( "Karnaugh map solved!" ) );
}

void blamFrame::OnSolSelect( wxTreeEvent& event )
{
	if(solution->GetRootItem()!=event.GetItem())
	{
		vector<unsigned short int> sol=((SolveTreeItemData*)solution->GetItemData(event.GetItem()))->values;
		kmap->SelectSolution(sol);
	}
	else
		kmap->ClearSelection();
}

void blamFrame::OnSolutionTypeChange( wxCommandEvent& event )
{
	if(event.GetSelection()==0) solveSOP=1;
	if(event.GetSelection()==1) solveSOP=0;
}

void blamFrame::OnQuit( wxCommandEvent& WXUNUSED( event ) )
{
	Close(TRUE);
}

void blamFrame::OnLangCroatian( wxCommandEvent& WXUNUSED( event ) )
{
	wxConfig *config = new wxConfig(wxT("Karnaugh Map Minimizer"));
	config->Write(wxT("Language"), wxT("hr"));
	delete config;
	wxMessageBox( _( "You have to restart the program for this change to take effect." ),
			_( "Restart required" ), wxOK | wxICON_INFORMATION, this );
}

void blamFrame::OnLangDefault( wxCommandEvent& WXUNUSED( event ) )
{
	wxConfig *config = new wxConfig(wxT("Karnaugh Map Minimizer"));
	config->Write(wxT("Language"), wxT(""));
	delete config;
	wxMessageBox( _( "You have to restart the program for this change to take effect." ),
			_( "Restart required" ), wxOK | wxICON_INFORMATION, this );
}

void blamFrame::OnCellAdresses( wxCommandEvent& WXUNUSED( event ) )
{
	wxConfig *config = new wxConfig(wxT("Karnaugh Map Minimizer"));
	if(kmap->GetCellAdresses())
	{
		config->Write(wxT("Cell_Adresses"), wxT("no"));
		kmap->SetCellAdresses(0);
	}
	else
	{
		config->Write(wxT("Cell_Adresses"), wxT("yes"));
		kmap->SetCellAdresses(1);
	}
	delete config;
}

void blamFrame::OnShowZeros( wxCommandEvent& WXUNUSED( event ) )
{
	wxConfig *config = new wxConfig(wxT("Karnaugh Map Minimizer"));
	if(kmap->GetShowZeros())
	{
		config->Write(wxT("Show_Zeros"), wxT("no"));
		kmap->SetShowZeros(0);
		truthTable->SetShowZeros(0);
	}
	else
	{
		config->Write(wxT("Show_Zeros"), wxT("yes"));
		kmap->SetShowZeros(1);
		truthTable->SetShowZeros(1);
	}
	delete config;
}

void blamFrame::OnAbout( wxCommandEvent& WXUNUSED( event ) )
{
	wxMessageBox( _( "This is a program for minimizing boolean functions using Karnaugh maps method."
	"\n\nCopyright (C) 2005. Robert Kovacevic"),
			_( "About Karnaugh Map Minimizer" ), wxOK | wxICON_INFORMATION, this );
}

SolveTreeItemData::SolveTreeItemData()
	:wxTreeItemData()
{
}


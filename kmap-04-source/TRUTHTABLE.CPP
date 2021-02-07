#include <wx/wx.h>
#include <wx/grid.h>

#include <cstdlib>
#include <ctime>
#include <math.h>

#include "truthTable.h"

DEFINE_EVENT_TYPE(wxEVT_VALUE_CHANGE)

BEGIN_EVENT_TABLE( TruthTable, wxGrid )
	EVT_GRID_CELL_RIGHT_CLICK( TruthTable::DisplayPopup )
	EVT_GRID_CELL_CHANGE(TruthTable::OnCellChange)
	EVT_MENU(MENU_SET1, TruthTable::OnMenuSet1)
	EVT_MENU(MENU_SET0, TruthTable::OnMenuSet0)
	EVT_MENU(MENU_SETDC, TruthTable::OnMenuSetDC)
	EVT_MENU(MENU_SETRAND, TruthTable::OnMenuSetRand)
END_EVENT_TABLE()

TruthTable::TruthTable(wxWindow* parent, wxWindowID id, unsigned int vars, wxLocale& locale, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
	:wxGrid(parent, id, pos, size, style, name), m_locale(locale)
{
	showZeros=0;

	this->CreateGrid((int)pow(2, vars), vars+1);
	
	this->SetVars(vars);
	
	this->SetSelectionMode(wxGrid::wxGridSelectRows);
	
	this->EnableDragGridSize(0);
	
	stateMenu = new wxMenu;
	
	// wxBitmap oneBitmap(wxT("1.xpm"), wxBITMAP_TYPE_XPM );
	wxMenuItem* one=new wxMenuItem(0, MENU_SET1, _("Set to 1"));
	// one->SetBitmap(oneBitmap);
	
	// wxBitmap zeroBitmap(wxT("0.xpm"), wxBITMAP_TYPE_XPM );
	wxMenuItem* zero=new wxMenuItem(0, MENU_SET0, _("Set to 0"));
	// zero->SetBitmap(zeroBitmap);
	
	// wxBitmap dcBitmap(wxT("dc.xpm"), wxBITMAP_TYPE_XPM );
	wxMenuItem* dc=new wxMenuItem(0, MENU_SETDC, _("Set to \"don't care\""));
	// dc->SetBitmap(dcBitmap);
	
	// wxBitmap randBitmap(wxT("rand.xpm"), wxBITMAP_TYPE_XPM );
	wxMenuItem* rand=new wxMenuItem(0, MENU_SETRAND, _("Set randomly"));
	// rand->SetBitmap(randBitmap);
	
	stateMenu->Append(one);
	stateMenu->Append(zero);
	stateMenu->Append(dc);
	stateMenu->AppendSeparator();
	stateMenu->Append(rand);
	
	this->SetDefaultCellAlignment(wxALIGN_CENTRE, wxALIGN_CENTRE);
}

void TruthTable::DisplayPopup(wxGridEvent& event)
{
	if(this->IsSelection() ) // && this->GetSelectedRows().Count()>1)  <--- library bug
	{
		this->popup=event.GetPosition();
		PopupMenu(stateMenu, event.GetPosition());
	}
	else
	{
		menuRow=event.GetRow();
		menuCol=event.GetCol();
		this->SetGridCursor(menuRow, menuCol);
		this->ClearSelection();
		this->SelectRow(menuRow);
		
		this->popup=event.GetPosition();
		PopupMenu(stateMenu, event.GetPosition());
	}
}

void TruthTable::OnCellChange(wxGridEvent& event)
{
	if( (this->GetCellValue(event.GetRow(), event.GetCol())!=wxT("1")) &&
		(this->GetCellValue(event.GetRow(), event.GetCol())!=wxT("")) &&
		(this->GetCellValue(event.GetRow(), event.GetCol())!=wxT("0")) )
	this->SetCellValue(event.GetRow(), event.GetCol(), wxT("?"));
	
	if(this->GetCellValue(event.GetRow(), event.GetCol())==wxT(""))
		if(showZeros) this->SetCellValue(event.GetRow(), event.GetCol(), wxT("0"));
	
	if(this->GetCellValue(event.GetRow(), event.GetCol())==wxT("0"))
		if(!showZeros) this->SetCellValue(event.GetRow(), event.GetCol(), wxT(""));
	
	event.Skip(1);
}

void TruthTable::OnMenuSet1(wxCommandEvent& event)
{
	for(int i=0; i<((int)pow(2, numberOfVariables)); i++)
	{
		if(this->IsInSelection(i, 0)) 
		{
			this->SetCellValue(i, numberOfVariables, wxT("1"));
			SendEvent( wxEVT_GRID_CELL_CHANGE, i, numberOfVariables );
		}
	}
}

void TruthTable::OnMenuSet0(wxCommandEvent& event)
{
	for(int i=0; i<((int)pow(2, numberOfVariables)); i++)
	{
		if(this->IsInSelection(i, 0)) 
		{
			if(showZeros) this->SetCellValue(i, numberOfVariables, wxT("0"));
				else this->SetCellValue(i, numberOfVariables, wxT(""));
			SendEvent( wxEVT_GRID_CELL_CHANGE, i, numberOfVariables );
		}
	}
}

void TruthTable::OnMenuSetDC(wxCommandEvent& event)
{
	for(int i=0; i<((int)pow(2, numberOfVariables)); i++)
	{
		if(this->IsInSelection(i, 0)) 
		{
			this->SetCellValue(i, numberOfVariables, wxT("?"));
			SendEvent( wxEVT_GRID_CELL_CHANGE, i, numberOfVariables );
		}
	}
}

void TruthTable::OnMenuSetRand(wxCommandEvent& event)
{
	srand(static_cast<unsigned>(time(0)));
	for(int i=0; i<((int)pow(2, numberOfVariables)); i++)
	{
		if(this->IsInSelection(i, 0)) 
		{
			if(rand()%2)
			{
				this->SetCellValue(i, numberOfVariables, wxT("1"));
				SendEvent( wxEVT_GRID_CELL_CHANGE, i, numberOfVariables );
			}
			else
			{
				if(showZeros) this->SetCellValue(i, numberOfVariables, wxT("0"));
					else this->SetCellValue(i, numberOfVariables, wxT(""));
				SendEvent( wxEVT_GRID_CELL_CHANGE, i, numberOfVariables );
			}
		}
	}
}

void TruthTable::SetVars(unsigned int vars)
{
	this->ClearGrid();
	numberOfVariables=vars;

	if(this->GetNumberCols()<(int)(vars+1)) this->AppendCols(vars+1-this->GetNumberCols());
	if(this->GetNumberCols()>(int)(vars+1)) this->DeleteCols(0, this->GetNumberCols()-vars-1);
	
	if(this->GetNumberRows()<((int)pow(2, vars))) this->AppendRows(((int)pow(2, vars))-this->GetNumberRows());
	if(this->GetNumberRows()>((int)pow(2, vars))) this->DeleteRows(0, this->GetNumberRows()-((int)pow(2, vars)));
	
	for(unsigned int i=0; i<vars; i++)
	{
		this->SetColLabelValue(i, wxString::Format(wxT("%c"), 65+i));
	}
	
	this->SetColLabelValue(vars, wxT("f"));
	
	for(int i=0; i<((int)pow(2, vars)); i++)
	{
		this->SetRowLabelValue(i, wxString::Format(wxT("%d"), i));
		if(showZeros) this->SetCellValue(i, numberOfVariables, wxT("0"));
	}
	
	for(int i=0; i<((int)pow(2, vars)); i++)
	{
		for(unsigned int j=0; j<vars; j++)
		{
			this->SetReadOnly(i, j, 1);
			if(!(i%2))
				this->SetCellBackgroundColour(i, j, wxColour(245, 245, 245));
			else
				this->SetCellBackgroundColour(i, j, wxColour(235, 235, 235));
			if((i%((int)pow(2, vars-j)))<((int)pow(2, vars-j-1))) this->SetCellValue(i, j, wxT("0"));
				else this->SetCellValue(i, j, wxT("1"));
		}
		this->SetCellBackgroundColour(i, vars, wxColour(215, 225, 255));
	}
	
	int w, h;
	wxPaintDC dc (this);
	dc.SetFont(this->GetLabelFont());
	dc.GetTextExtent( wxString::Format(wxT("%d"), (int)pow(2, vars)), &w, &h );
	
	this->SetRowLabelSize(w+15);
	
	this->AutoSizeColumns(1);
	this->SetColSize(vars, (int)(1.5*GetColSize(0)));
	
	this->ForceRefresh();
	
	this->AdjustScrollbars();
}

void TruthTable::SetShowZeros(bool s)
{
	showZeros=s;
	for(int i=0; i<(this->GetNumberRows()); i++)
	{
		if(showZeros && (this->GetCellValue(i, numberOfVariables)==wxT(""))) 
			this->SetCellValue(i, numberOfVariables, wxT("0"));
		else if(!showZeros && (this->GetCellValue(i, numberOfVariables)==wxT("0"))) 
			this->SetCellValue(i, numberOfVariables, wxT(""));
	}
	this->ForceRefresh();
}

bool TruthTable::GetShowZeros()
{
	return showZeros;
}

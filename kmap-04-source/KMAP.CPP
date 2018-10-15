#include <wx/wx.h>
#include <wx/grid.h>
#include <wx/font.h>

#include <cstdlib>
#include <ctime>
#include <math.h>

#include "kmap.h"


BEGIN_EVENT_TABLE( KMap, wxGrid )
	EVT_GRID_CELL_RIGHT_CLICK( KMap::DisplayPopup )
	EVT_GRID_CELL_CHANGE(KMap::OnCellChange)
	EVT_MENU(KMAP_MENU_SET1, KMap::OnMenuSet1)
	EVT_MENU(KMAP_MENU_SET0, KMap::OnMenuSet0)
	EVT_MENU(KMAP_MENU_SETDC, KMap::OnMenuSetDC)
	EVT_MENU(KMAP_MENU_SETRAND, KMap::OnMenuSetRand)
END_EVENT_TABLE()

KMap::KMap(wxWindow* parent, wxWindowID id, unsigned int vars, wxLocale& locale, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
	:wxGrid(parent, id, pos, size, style, name), m_locale(locale)
{
	width=(unsigned int)pow(2, ceil((double)vars/2));
	height=(unsigned int)pow(2, floor((double)vars/2));
	
	showZeros=0;
	
	this->CreateGrid( height,width);
	
	this->SetVars(vars);
	
	this->EnableDragGridSize(0);
	
	stateMenu = new wxMenu;
	
	// wxBitmap oneBitmap(wxT("1.xpm"), wxBITMAP_TYPE_XPM );
	wxMenuItem* one=new wxMenuItem(0, KMAP_MENU_SET1, _("Set to 1"));
	// one->SetBitmap(oneBitmap);
	
	// wxBitmap zeroBitmap(wxT("0.xpm"), wxBITMAP_TYPE_XPM );
	wxMenuItem* zero=new wxMenuItem(0, KMAP_MENU_SET0, _("Set to 0"));
	// zero->SetBitmap(zeroBitmap);
	
	// wxBitmap dcBitmap(wxT("dc.xpm"), wxBITMAP_TYPE_XPM );
	wxMenuItem* dc=new wxMenuItem(0, KMAP_MENU_SETDC, _("Set to \"don't care\""));
	// dc->SetBitmap(dcBitmap);
	
	// wxBitmap randBitmap(wxT("rand.xpm"), wxBITMAP_TYPE_XPM );
	wxMenuItem* rand=new wxMenuItem(0, KMAP_MENU_SETRAND, _("Set randomly"));
	// rand->SetBitmap(randBitmap);
	
	stateMenu->Append(one);
	stateMenu->Append(zero);
	stateMenu->Append(dc);
	stateMenu->AppendSeparator();
	stateMenu->Append(rand);

	
	this->SetDefaultCellAlignment(wxALIGN_CENTRE, wxALIGN_CENTRE);
	
	this->useCellAdresses=1;
	this->SetDefaultRenderer(new KMapGridCellRenderer);
	
	this->ForceRefresh();
}

void KMap::AddSolution(vector<unsigned short int> sol)
{
	for(int i=0; i<height; i++)
	{
		for(int j=0; j<width; j++)
		{
			vector<unsigned short int> m=this->GetMapBoolValue(j, i);
			bool is=1;
			for(int k=0; k<sol.size(); k++)
			{
				if( (sol[k]!=m[k]) && (sol[k]!=2) ) is=0;
			}
			
			if(is==1)
			{
				wxColour c=this->GetCellBackgroundColour(i, j);
				this->SetCellBackgroundColour(i, j, wxColour(c.Red()-40, c.Green()-30, c.Blue()));
			}
		}
	}
	this->ForceRefresh();
}

void KMap::SelectSolution(vector<unsigned short int> sol)
{
	this->ClearSelection();
	for(int i=0; i<height; i++)
	{
		for(int j=0; j<width; j++)
		{
			vector<unsigned short int> m=this->GetMapBoolValue(j, i);
			bool is=1;
			for(int k=0; k<sol.size(); k++)
			{
				if( (sol[k]!=m[k]) && (sol[k]!=2) ) is=0;
			}
			
			if(is==1)
			{
				this->SelectBlock(i, j, i, j, 1);
			}
		}
	}
	this->ForceRefresh();
}

void KMap::ClearSolutions()
{
	for(int i=0; i<this->GetNumberRows(); i++)
	{
		for(int j=0; j<this->GetNumberCols(); j++)
		{
			if( ((j/4+i/4)%2)==1) this->SetCellBackgroundColour(i, j, wxColour(245,245,245));
				else this->SetCellBackgroundColour(i, j, this->GetDefaultCellBackgroundColour());
		}
	}
	this->ForceRefresh();
}

void KMap::DisplayPopup(wxGridEvent& event)
{
	menuRow=event.GetRow();
	menuCol=event.GetCol();
	this->SetGridCursor(menuRow, menuCol);
	this->popup=event.GetPosition();
	PopupMenu(stateMenu, event.GetPosition());
}

void KMap::OnCellChange(wxGridEvent& event)
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

void KMap::OnMenuSet1(wxCommandEvent& event)
{
	if(this->IsSelection())
	{
		for(int i=0; i<height; i++)
		{
			for(int j=0; j<width; j++)
			{
				if(this->IsInSelection(i, j)) 
				{
					this->SetCellValue(i, j, wxT("1"));
					SendEvent( wxEVT_GRID_CELL_CHANGE, i, j );
				}
			}
		}
	}
	else
	{
		this->SetCellValue(menuRow, menuCol, wxT("1"));
		SendEvent( wxEVT_GRID_CELL_CHANGE, menuRow, menuCol );
	}
}

void KMap::OnMenuSet0(wxCommandEvent& event)
{
	if(this->IsSelection())
	{
		for(int i=0; i<height; i++)
		{
			for(int j=0; j<width; j++)
			{
				if(this->IsInSelection(i, j)) 
				{
					if(showZeros)
						this->SetCellValue(i, j, wxT("0"));
					else
						this->SetCellValue(i, j, wxT(""));
					SendEvent( wxEVT_GRID_CELL_CHANGE, i, j );
				}
			}
		}
	}
	else
	{
		if(showZeros) this->SetCellValue(menuRow, menuCol, wxT("0"));
			else this->SetCellValue(menuRow, menuCol, wxT(""));
		SendEvent( wxEVT_GRID_CELL_CHANGE, menuRow, menuCol );
	}
}

void KMap::OnMenuSetDC(wxCommandEvent& event)
{
	if(this->IsSelection())
	{
		for(int i=0; i<height; i++)
		{
			for(int j=0; j<width; j++)
			{
				if(this->IsInSelection(i, j)) 
				{
					this->SetCellValue(i, j, wxT("?"));
					SendEvent( wxEVT_GRID_CELL_CHANGE, i, j );
				}
			}
		}
	}
	else
	{
		this->SetCellValue(menuRow, menuCol, wxT("?"));
		SendEvent( wxEVT_GRID_CELL_CHANGE, menuRow, menuCol );
	}
}

void KMap::OnMenuSetRand(wxCommandEvent& event)
{
	srand(static_cast<unsigned>(time(0)));
	if(this->IsSelection())
	{
		for(int i=0; i<height; i++)
		{
			for(int j=0; j<width; j++)
			{
				if(this->IsInSelection(i, j)) 
				{
					if(rand()%2)
						this->SetCellValue(i, j, wxT("1"));
					else
					{
						if(showZeros) this->SetCellValue(i, j, wxT("0"));
							else this->SetCellValue(i, j, wxT(""));
					}
					SendEvent( wxEVT_GRID_CELL_CHANGE, i, j );
				}
			}
		}
	}
	else
	{
		if(rand()%2)
			this->SetCellValue(menuRow, menuCol, wxT("1"));
		else
		{
			if(showZeros) this->SetCellValue(menuRow, menuCol, wxT("0"));
				else this->SetCellValue(menuRow, menuCol, wxT(""));
		}
		SendEvent( wxEVT_GRID_CELL_CHANGE, menuRow, menuCol );
	}
}

void KMap::SetShowZeros(bool s)
{
	showZeros=s;
	for(int i=0; i<this->GetNumberCols(); i++)
	{
		for(int j=0; j<this->GetNumberRows(); j++)
		{
			if(showZeros && (this->GetCellValue(j, i)==wxT(""))) 
				this->SetCellValue(j, i, wxT("0"));
			else if(!showZeros && (this->GetCellValue(j, i)==wxT("0"))) 
				this->SetCellValue(j, i, wxT(""));	
		}
	}
	this->ForceRefresh();
}

bool KMap::GetShowZeros()
{
	return showZeros;
}

void KMap::SetVars(unsigned int vars)
{
	this->ClearGrid();
	
	numberOfVariables=vars;
	
	width=(unsigned int)pow(2, ceil((double)vars/2));
	height=(unsigned int)pow(2, floor((double)vars/2));
	
	// Fill map kmapValues with values that each cell in the map
	// has. Look here for rules:
	// http://www.allaboutcircuits.com/vol_4/chpt_8/3.html
	kmapValues.clear();
	if(numberOfVariables>2)
	{
		for(int i=0; i<height; i++)
		{
			for(int j=0; j<width; j++)
			{
				// Set every 4x4 block's first 4 vars to gray code
				if(width>=4)
					kmapValues[make_pair(j, i)]=GrayEncode(j%4+((i%4)*4));
				else
					kmapValues[make_pair(j, i)]=GrayEncode(j%4+((i%4)*width));
				// Combine 4x4 blocks into map (5th and higher variables)
				kmapValues[make_pair(j, i)]+=16*(j/4+((i/4)*(width/4)));
			}
		}
	}
	else
	{
		if(numberOfVariables==2)
		{
			kmapValues[make_pair(0, 0)]=0;
			kmapValues[make_pair(1, 0)]=1;
			kmapValues[make_pair(0, 1)]=2;
			kmapValues[make_pair(1, 1)]=3;
		}
		if(numberOfVariables==1)
		{
			kmapValues[make_pair(0, 0)]=0;
			kmapValues[make_pair(1, 0)]=1;
		}
	}

	if(this->GetNumberCols()<((int)(width))) this->AppendCols(width-this->GetNumberCols());
	if(this->GetNumberCols()>((int)(width))) this->DeleteCols(0, this->GetNumberCols()-width);
	
	if(this->GetNumberRows()<(((int)(height)))) this->AppendRows(height-this->GetNumberRows());
	if(this->GetNumberRows()>((int)(height))) this->DeleteRows(0, this->GetNumberRows()-height);
	
	
	for(int i=0; i<this->GetNumberRows(); i++)
	{
		vector<unsigned short int> a=GetMapBoolValue(0,i);
		for(int j=0; j<this->GetNumberCols(); j++)
		{
			vector<unsigned short int> b=GetMapBoolValue(j,i);
			for(unsigned int k=0; k<a.size(); k++)
			{
				if( a[k]!=b[k] ) a[k]=2;
			}
		}
		wxString r;
		for(unsigned int k=0; k<a.size(); k++)
		{
			if( a[k]!=2 ) r.Append(wxString::Format(wxT("%d"), a[k]));
		}
		this->SetRowLabelValue(i, r);
	}
	
	for(int i=0; i<this->GetNumberCols(); i++)
	{
		vector<unsigned short int> a=GetMapBoolValue(i,0);
		for(int j=0; j<this->GetNumberRows(); j++)
		{
			vector<unsigned short int> b=GetMapBoolValue(i,j);
			for(unsigned int k=0; k<a.size(); k++)
			{
				if( a[k]!=b[k] ) a[k]=2;
			}
		}
		wxString r;
		for(unsigned int k=0; k<a.size(); k++)
		{
			if( a[k]!=2 ) r.Append(wxString::Format(wxT("%d"), a[k]));
		}
		this->SetColLabelValue(i, r);
	}
	
	for(int i=0; i<this->GetNumberRows(); i++)
	{
		for(int j=0; j<this->GetNumberCols(); j++)
		{
			if( ((j/4+i/4)%2)==1) this->SetCellBackgroundColour(i, j, wxColour(245,245,245));
				else this->SetCellBackgroundColour(i, j, this->GetDefaultCellBackgroundColour());
			if(showZeros) this->SetCellValue(i, j, wxT("0"));
		}
	}
	
	int w, h;
	wxPaintDC dc (this);
	dc.SetFont(this->GetLabelFont());
	dc.GetTextExtent( this->GetRowLabelValue(0), &w, &h );
	
	this->SetRowLabelSize(w+15);
	
	//this->AutoSizeColumns(1);
	
	dc.GetTextExtent( this->GetColLabelValue(0), &w, &h );
	
	for(int i=0; i<this->GetNumberCols(); i++)
	{
		if(w<20) w=20;
		this->SetColSize(i, w+15);
		
	}
	
	this->ForceRefresh();
	
	this->AdjustScrollbars();
}

unsigned int KMap::GrayEncode(unsigned int g)
{
	switch(g)
	{
		case 0: return 0;
		case 1: return 1;
		case 2: return 3;
		case 3: return 2;
		
		case 4: return 4;
		case 5: return 5;
		case 6: return 7;
		case 7: return 6;
		
		case 8: return 12;
		case 9: return 13;
		case 10: return 15;
		case 11: return 14;
		
		case 12: return 8;
		case 13: return 9;
		case 14: return 11;
		case 15: return 10;
	}
	return 0; // avoid warning
}

unsigned int KMap::GetMapValue(unsigned int x, unsigned int y)
{
	return kmapValues[make_pair(x,y)];
}

vector<unsigned short int> KMap::GetMapBoolValue(unsigned int x, unsigned int y)
{
	vector<unsigned short int> b;
	unsigned int i=GetMapValue(x, y);
	while(i>0)
	{
		b.insert(b.begin(), i%2);
		i=i/2;
	}
	for(unsigned int j=b.size(); j<numberOfVariables; j++)
		b.insert(b.begin(), 0);
	return b;
}

void KMap::Set(unsigned int adr, wxString value)
{
	for(unsigned int i=0; i<height; i++)
	{
		for(unsigned int j=0; j<width; j++)
		{
			if(kmapValues[make_pair(j, i)]==adr)
			{
				this->SetCellValue(i, j, value);
				return;
			}
		}
	}
}

void KMap::SetCellAdresses(bool on)
{
	if(on==0)
	{
		this->useCellAdresses=0;
		this->SetDefaultRenderer(new wxGridCellStringRenderer);
		this->ForceRefresh();
	}
	if(on==1)
	{
		this->useCellAdresses=1;
		this->SetDefaultRenderer(new KMapGridCellRenderer);
		this->ForceRefresh();
	}
}

bool KMap::GetCellAdresses()
{
	return useCellAdresses;
}

// Grid cell renderer
void KMapGridCellRenderer::Draw(wxGrid& grid,
                              wxGridCellAttr& attr,
                              wxDC& dc,
                              const wxRect& rect,
                              int row, int col,
                              bool isSelected)
{
	wxGridCellStringRenderer::Draw(grid, attr, dc, rect, row, col, isSelected);
	
	wxFont* font = new wxFont(7, wxDEFAULT, wxNORMAL, wxNORMAL);
	
	font->SetFaceName(wxT("sans"));
	     
	dc.SetFont(*font);
	dc.SetTextForeground(wxColour(150, 150, 150));
	
	dc.SetPen(*wxGREY_PEN);
	dc.SetBrush(*wxTRANSPARENT_BRUSH);
	
	int adr=GetMapValue(col, row, grid.GetNumberCols(), grid.GetNumberRows());
	
	int w, h;
	
	dc.GetTextExtent(wxString::Format(wxT("%d"), adr), &w, &h);
	
	dc.DrawText(wxString::Format(wxT("%d"), adr), rect.GetX()+rect.GetWidth()-w-2, rect.GetY()+rect.GetHeight()-h-1);
}

unsigned int KMapGridCellRenderer::GrayEncode(unsigned int g)
{
	switch(g)
	{
		case 0: return 0;
		case 1: return 1;
		case 2: return 3;
		case 3: return 2;
		
		case 4: return 4;
		case 5: return 5;
		case 6: return 7;
		case 7: return 6;
		
		case 8: return 12;
		case 9: return 13;
		case 10: return 15;
		case 11: return 14;
		
		case 12: return 8;
		case 13: return 9;
		case 14: return 11;
		case 15: return 10;
	}
	return 0; // avoid warning
}

unsigned int KMapGridCellRenderer::GetMapValue(unsigned int x, unsigned int y, unsigned int width, unsigned int height)
{
	unsigned int r;
	
	if(width>2)
	{
		if(width>=4)
			r=GrayEncode(x%4+((y%4)*4));
		else
			r=GrayEncode(x%4+((y%4)*width));
		r+=16*(x/4+((y/4)*(width/4)));
		
		return r;
	}
	else
	{
		if(height==2)
		{
			if( (x==0) && (y==0) ) return 0;
			if( (x==1) && (y==0) ) return 1;
			if( (x==0) && (y==1) ) return 2;
			if( (x==1) && (y==1) ) return 3;
		}
		if(height==1)
		{
			if( (x==0) && (y==0) ) return 0;
			if( (x==1) && (y==0) ) return 1;
		}
	}
}

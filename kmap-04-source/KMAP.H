#ifndef _KMAP_H_
#define _KMAP_H_

/**
 * @short Karnaugh Map grid control
 * @author Robert Kovacevic <robert.kovacevic@etfos.hr>
 * @version 0.1
 */

#include <wx/wx.h>
#include <wx/grid.h>
 
#include <vector>
#include <map>
#include <utility>

using namespace std;

class KMap : public wxGrid
{
	public:
		KMap(wxWindow* parent, wxWindowID id, unsigned int vars, wxLocale& locale, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxWANTS_CHARS, const wxString& name = wxPanelNameStr);

		void SetVars(unsigned int vars);
		void Set(unsigned int adr, wxString value);
		unsigned int GetMapValue(unsigned int x, unsigned int y);
		void AddSolution(vector<unsigned short int> sol);
		void SelectSolution(vector<unsigned short int> sol);
		void ClearSolutions();
		void SetCellAdresses(bool on);
		bool GetCellAdresses();
		void SetShowZeros(bool s);
		bool GetShowZeros();
	private:
		void OnCellChange(wxGridEvent& event);
		void OnMenuSet1( wxCommandEvent& event );
		void OnMenuSet0( wxCommandEvent& event );
		void OnMenuSetDC( wxCommandEvent& event );
		void OnMenuSetRand( wxCommandEvent& event );
	
		unsigned int GrayEncode(unsigned int g);
		vector<unsigned short int> GetMapBoolValue(unsigned int x, unsigned int y);
		void DisplayPopup(wxGridEvent& event);
	
		wxMenu* stateMenu;
		unsigned int numberOfVariables;
		unsigned int labelRows;
		unsigned int labelCols;
		unsigned int width, height;
		map <pair<unsigned int, unsigned int>, unsigned int> kmapValues;
		unsigned int menuRow, menuCol;
		bool useCellAdresses;
		bool showZeros;
		
		wxPoint popup;
		
		 wxLocale& m_locale;
		
		DECLARE_EVENT_TABLE()
};

class KMapGridCellRenderer : public wxGridCellStringRenderer
{
public:
	virtual void Draw(wxGrid& grid,
                      wxGridCellAttr& attr,
                      wxDC& dc,
                      const wxRect& rect,
                      int row, int col,
                      bool isSelected);
	unsigned int GetMapValue(unsigned int x, unsigned int y, unsigned int width, unsigned int height);
	unsigned int GrayEncode(unsigned int g);
};

enum
{
	KMAP_MENU_SET1=100,
	KMAP_MENU_SET0,
	KMAP_MENU_SETDC,
	KMAP_MENU_SETRAND
};

#endif // _KMAP_H_

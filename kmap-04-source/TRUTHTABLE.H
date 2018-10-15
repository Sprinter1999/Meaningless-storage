#ifndef _TRUTHTABLE_H_
#define _TRUTHTABLE_H_

#include <wx/wx.h>
#include <wx/grid.h>

/**
 * @short Truth table grid control
 * @author Robert Kovacevic <robert.kovacevic@etfos.hr>
 * @version 0.1
 */

BEGIN_DECLARE_EVENT_TYPES()
    DECLARE_EVENT_TYPE(wxEVT_VALUE_CHANGE, -1)
END_DECLARE_EVENT_TYPES()
 
class TruthTable : public wxGrid
{
	public:
		TruthTable(wxWindow* parent, wxWindowID id, unsigned int vars, wxLocale& locale, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxWANTS_CHARS, const wxString& name = wxPanelNameStr);

		void DisplayPopup(wxGridEvent& event);
		void OnCellChange(wxGridEvent& event);
		void OnMenuSet1( wxCommandEvent& event );
		void OnMenuSet0( wxCommandEvent& event );
		void OnMenuSetDC( wxCommandEvent& event );
		void OnMenuSetRand(wxCommandEvent& event);
		
		void SetVars(unsigned int vars);
		void SetShowZeros(bool s);
		bool GetShowZeros();
	private:
	
		wxMenu* stateMenu;
		unsigned int numberOfVariables;
		unsigned int menuCol, menuRow;
		bool showZeros;
		
		wxPoint popup;
		
		wxLocale& m_locale;
		
		DECLARE_EVENT_TABLE()
};

enum
{
	MENU_SET1=100,
	MENU_SET0,
	MENU_SETDC,
	MENU_SETRAND
};

#endif // _TRUTHTABLE_H_

#include <wx/wx.h>
#include <wx/config.h>

#include "blam.h"
#include "blamFrame.h"

IMPLEMENT_APP(blamapp)

bool blamapp::OnInit()
{
	wxConfig *config = new wxConfig(wxT("Karnaugh Map Minimizer"));
	
	wxString lang;
	if ( config->Read(wxT("Language"), &lang) ) 
	{
		if(lang==wxT("hr")) m_locale.Init(wxLANGUAGE_CROATIAN);
	}
	else 
	{
		if(wxLocale::GetSystemLanguage()==wxLANGUAGE_CROATIAN)
		{
			m_locale.Init(wxLANGUAGE_CROATIAN);
			config->Write(wxT("Language"), wxT("hr"));
		}
	}
	
	delete config;
	
	m_locale.AddCatalog(wxT("blam"));
	
	blamFrame *frame = new blamFrame( _( "Karnaugh Map Minimizer" ), wxDefaultPosition, wxSize(450,700), m_locale );

	frame->Show(TRUE);
	SetTopWindow(frame);
	return TRUE;
} 

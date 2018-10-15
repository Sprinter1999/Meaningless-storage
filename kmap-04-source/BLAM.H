#ifndef _BLAM_H_
#define _BLAM_H_

/**
 * @short BLAM boolean function minimizer app
 * @author Robert Kovacevic <robert.kovacevic@etfos.hr>
 * @version 0.1
 */

class blamapp : public wxApp
{
	public:
		virtual bool OnInit();
		wxLocale m_locale;
};

#endif // _BLAM_H_

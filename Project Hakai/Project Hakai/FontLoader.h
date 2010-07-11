// Writen  By
// Kalith
// http://relishgames.com/forum/viewtopic.php?t=3767&highlight=font

#ifndef _FONTLOADER_H
#define _FONTLOADER_H

#include <hgefont.h>

#include <map>
#include <string>
#include <vector>

class FontLoader
{
public :
	static FontLoader* Instance();

	~FontLoader();

	hgeFont* GetFont(bool file, std::string sFontName, int nSize, bool bBold = false, bool bItalic = false);
	hgeFont* GetFontFromFile(std::string sFileName, int nSize, bool bBold = false, bool bItalic = false);
	void     DeleteFonts();

protected :

	FontLoader();

private:
	static FontLoader* m_pInstance;
	std::map<std::string, hgeFont*> lFntList;
	std::map<std::string, std::string> lFntPathList;
	std::vector<std::string> lTTFList;
};

#endif
#include "FontLoader.h"
#include <sstream>
#include <fstream>
#include <ios> 
#include "GameManager.h"

FontLoader* FontLoader::m_pInstance;

FontLoader* FontLoader::Instance()
{
	if (m_pInstance == NULL)
		m_pInstance = new FontLoader();

	return m_pInstance;
}


#define MAX_TEXTURE_SIZE 2048

struct CSymbolRange
{
   unsigned short   First;
   unsigned short   Last;
};

CHAR_DESC vChars[256];

//This is TTF file header
typedef struct _tagTT_OFFSET_TABLE
{
    unsigned short uMajorVersion;
    unsigned short uMinorVersion;
    unsigned short uNumOfTables;
    unsigned short uSearchRange;
    unsigned short uEntrySelector;
    unsigned short uRangeShift;
}TT_OFFSET_TABLE;

//Tables in TTF file and there placement and name (tag)
typedef struct _tagTT_TABLE_DIRECTORY
{
    char* szTag; //table name
    unsigned long uCheckSum; //Check sum
    unsigned long uOffset; //Offset from beginning of file
    unsigned long uLength; //length of the table in bytes

}TT_TABLE_DIRECTORY;

//Header of names table
typedef struct _tagTT_NAME_TABLE_HEADER
{
    unsigned short uFSelector; //format selector. Always 0
    unsigned short uNRCount; //Name Records count
    unsigned short uStorageOffset; //Offset for strings storage, from start of the table

}TT_NAME_TABLE_HEADER;

//Record in names table
typedef struct _tagTT_NAME_RECORD
{
    unsigned short uPlatformID;
    unsigned short uEncodingID;
    unsigned short uLanguageID;
    unsigned short uNameID;
    unsigned short uStringLength;
    unsigned short uStringOffset; //from start of storage area

}TT_NAME_RECORD;
/** \endcond
*/

using namespace std;

string ToString( float f )
{
	ostringstream s;
	s << f;
	return s.str();
} 

bool FileExists(string fileName)
{
	fstream f;
	f.open(fileName.c_str(), ios::in);
	if (f.is_open())
	{
		f.close();
		return true;
	}
	f.close();
	return false;
} 

FontLoader::FontLoader()
{
}

FontLoader::~FontLoader()
{
    DeleteFonts();
}

void FontLoader::DeleteFonts()
{
   // Delete fonts
   map<string, hgeFont*>::iterator iter;
   while (!lFntList.empty())
   {
      iter = lFntList.begin();
      delete iter->second;
      lFntList.erase(iter);
   }

   // Unreg TTF files
   vector<string>::iterator iter2;
   while (!lTTFList.empty())
   {
      iter2 = lTTFList.begin();
      RemoveFontResource(iter2->c_str());
      lTTFList.erase(iter2);
   }
}

bool PlaceSymbols(int nWidth, int nHeight, CSymbolRange *pRanges, int nRangeCount)
{
   int i, j;
   int x=1, y=1;

   for(i=0; i<nRangeCount; i++)
   {
      for(j=pRanges[i].First; j<=pRanges[i].Last; j++ )
      {
         if(y+vChars[j].h+1 >= nHeight) return false;
         if(x+vChars[j].w+1 >= nWidth)
         {
            x=1;
            y+=vChars[j].h+1;
            if(y+vChars[j].h+1 >= nHeight) return false;
         }

         vChars[j].x = x;
         vChars[j].y = y;
         x+=vChars[j].w+1;
      }
   }

   return true;
}

HTEXTURE TextureGenerate(char *szFontName,
                 int nSize,
                 bool bBold,
                 bool bItalic,
                 CSymbolRange *pRanges,
                 int nRangeCount)
{
   int i,j;
   int nWidth, nHeight;

   HDC         hBMDC;
   HBITMAP      hBM;
   BITMAPINFO   bmi;
   HFONT      hFont;
   ABCFLOAT   abc;
   TEXTMETRIC   tTextMetrics;

   HTEXTURE   tex;
   DWORD      *pPixels, *pTexData, dwPixel;

   int CurSmoothingValue;

   // Set font antialiasing
   SystemParametersInfo(SPI_GETFONTSMOOTHING, 0, &CurSmoothingValue, 0);
   SystemParametersInfo(SPI_SETFONTSMOOTHING, 1, 0, 0);

   // create font
   hFont = CreateFont
   (
      -nSize, 0, 0, 0, (bBold) ? FW_BOLD : FW_NORMAL,
      bItalic, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
      ANTIALIASED_QUALITY,
      //PROOF_QUALITY,
      DEFAULT_PITCH | FF_DONTCARE, szFontName
   );

   if(!hFont) return 0;

   // create and setup compatible DC
   hBMDC = CreateCompatibleDC(0);
   SetTextColor(hBMDC, RGB(255,255,255));
   SetBkColor(hBMDC, RGB(0,0,0));
   SetBkMode(hBMDC, TRANSPARENT);
   SetMapMode(hBMDC, MM_TEXT);
    SetTextAlign(hBMDC, TA_TOP);
   SelectObject(hBMDC, hFont);

   // calculate symbols metrics
   GetTextMetrics(hBMDC, &tTextMetrics);

   for (i = 0; i < nRangeCount; i++ )
   {
      for (j = pRanges[i].First; j <= pRanges[i].Last; j++ )
      {
         GetCharABCWidthsFloat(hBMDC, j, j, &abc);

         // reserve pixels for antialiasing
         vChars[j].a = int(abc.abcfA)-1;
         vChars[j].c = int(abc.abcfC)-1;
         vChars[j].w = int(abc.abcfB)+2;
         vChars[j].h = tTextMetrics.tmHeight;
      }
   }

   // calculate symbols placement
   nWidth=32; nHeight=32;

   for(;;)
   {
      if(PlaceSymbols(nWidth, nHeight, pRanges, nRangeCount)) break;

      if(nWidth<=nHeight) nWidth<<=1;
      else nHeight<<=1;

      if(nWidth > MAX_TEXTURE_SIZE || nHeight > MAX_TEXTURE_SIZE)
      {
         DeleteObject(hFont);
         DeleteDC(hBMDC);
         return 0;
      }
   }

   // create DC bitmap
   memset(&bmi, 0, sizeof(BITMAPINFO));
   bmi.bmiHeader.biWidth = nWidth;
   bmi.bmiHeader.biHeight = -nHeight;
   bmi.bmiHeader.biBitCount = 32;
   bmi.bmiHeader.biCompression = BI_RGB;
   bmi.bmiHeader.biPlanes = 1;
   bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);

   hBM = CreateDIBSection(hBMDC, &bmi, DIB_RGB_COLORS, (void**)&pPixels, 0, 0);
   if(!hBM)
   {
      DeleteObject(hFont);
      DeleteDC(hBMDC);
      return 0;
   }

   SelectObject(hBMDC, hBM);

   // draw symbols onto DC bitmap
   for (i = 0; i < nRangeCount; i++ )
   {
      for (j = pRanges[i].First; j <= pRanges[i].Last; j++ )
      {
         char c = (char)j;
         TextOut(hBMDC, vChars[j].x-vChars[j].a, vChars[j].y, &c, 1);
      }
   }
   GdiFlush();

   // transfer DC bitmap onto HGE texture with alpha channel
   tex = GameManager::Instance()->GetHGE()->Texture_Create(nWidth, nHeight);
   pTexData = GameManager::Instance()->GetHGE()->Texture_Lock(tex, false);

   for (i = 0; i < nHeight; i++)
   {
      for (j = 0; j < nWidth; j++)
      {
         dwPixel = pPixels[i*nWidth+j];
         dwPixel = 0xFFFFFF | ((dwPixel & 0xFF) << 24);
         pTexData[i*nWidth+j] = dwPixel;
      }
   }

   GameManager::Instance()->GetHGE()->Texture_Unlock(tex);

   // clean up
   DeleteObject(hBM);
   DeleteObject(hFont);
   DeleteDC(hBMDC);

   // Set back the old antialiasing state
   SystemParametersInfo(SPI_SETFONTSMOOTHING, CurSmoothingValue, 0, 0);

   return tex;
}

string IntToHex( int i )
{
   // [#] This function converts an integer to an hexadecimal number (formated).
   if (i < 0) i = 256+i;
   char* c = new char[256];
   itoa(i, c, 16);
   string s = c;
   delete[] c;

   if (s.size() == 1)
      s = "0" + s;

   return s;
}

int HexToInt( const char* c )
{
   int i;
   stringstream iss;
   iss << c;
   iss >> hex >> i;

   return i;
}

unsigned long HexToULong( const char* c )
{
   unsigned long u;
   stringstream iss;
   iss << c;
   iss >> hex >> u;

   return u;
}

void GetErrors(fstream* f)
{
   if (f->eof())
      GameManager::Instance()->GetHGE()->System_Log("#Error parsing font file : end of file");
   if (f->bad())
      GameManager::Instance()->GetHGE()->System_Log("#Error parsing font file : bad");
   else if (f->fail())
      GameManager::Instance()->GetHGE()->System_Log("#Error parsing font file : fail");
}

unsigned short GetUSHORT(fstream* f, int* pos)
{
   if (f->good() && *pos != -1)
   {
      f->seekg(*pos);
      char* tmp = new char[256];
      f->read(tmp, sizeof(unsigned short));
      string s = "";
      int i1 = tmp[0]; int i2 = tmp[1];
      s += IntToHex(i1);
      s += IntToHex(i2);
      unsigned short u = HexToInt(s.c_str());

      *pos += sizeof(unsigned short);

      delete[] tmp;

      return u;
   }
   else
   {
      GetErrors(f);
      *pos = -1;
      return 0;
   }
}

unsigned long GetULONG(fstream* f, int* pos)
{
   if (f->good() && *pos != -1)
   {
      f->seekg(*pos);
      char* tmp = new char[256];
      f->read(tmp, sizeof(unsigned long));
      string s = "";
      int i1 = tmp[0]; int i2 = tmp[1];  int i3 = tmp[2];  int i4 = tmp[3];
      s += IntToHex(i1);
      s += IntToHex(i2);
      s += IntToHex(i3);
      s += IntToHex(i4);
      unsigned long u = HexToULong(s.c_str());

      *pos += sizeof(unsigned long);

      delete[] tmp;

      return u;
   }
   else
   {
      GetErrors(f);
      *pos = -1;
      return 0;
   }
}

string GetFontName(string lpszFilePath)
{
   fstream f(lpszFilePath.c_str(), ios::in | ios::binary);
   string csRetVal = "";

   //lpszFilePath is the path to our font file
   if (f.is_open())
   {
      //define and read file header
      TT_OFFSET_TABLE ttOffsetTable;
      int pos = ios::beg;

      ttOffsetTable.uMajorVersion = GetUSHORT(&f, &pos);
      ttOffsetTable.uMinorVersion = GetUSHORT(&f, &pos);
      ttOffsetTable.uNumOfTables = GetUSHORT(&f, &pos);
      ttOffsetTable.uSearchRange = GetUSHORT(&f, &pos);
      ttOffsetTable.uEntrySelector = GetUSHORT(&f, &pos);
      ttOffsetTable.uRangeShift = GetUSHORT(&f, &pos);

      if (pos == -1)
         return csRetVal;

      //check is this is a true type font and the version is 1.0
      if(ttOffsetTable.uMajorVersion != 1 || ttOffsetTable.uMinorVersion != 0)
         return csRetVal;

      TT_TABLE_DIRECTORY tblDir;
      bool bFound = false;
      string csTemp;

      for (int i=0; i< ttOffsetTable.uNumOfTables; i++)
      {
          csTemp = "";
            tblDir.szTag = new char[4];
            f.read(tblDir.szTag, sizeof(char[4]));
            pos += sizeof(char[4]);
            tblDir.uCheckSum = GetULONG(&f, &pos);
            tblDir.uOffset = GetULONG(&f, &pos);
            tblDir.uLength = GetULONG(&f, &pos);

            if (pos == -1)
               return csRetVal;

            int x = 0;
            while (x < 4)
            {
                char c = tblDir.szTag[x];
                csTemp += c;
                x++;
            }

         delete[] tblDir.szTag;

         if (csTemp == "name")
         {
            //we found our table
            bFound = true;
            break;
         }
      }

      if (bFound)
      {
         //move to offset we got from Offsets Table
         pos = tblDir.uOffset;
         TT_NAME_TABLE_HEADER ttNTHeader;
         ttNTHeader.uFSelector = GetUSHORT(&f, &pos);
         ttNTHeader.uNRCount = GetUSHORT(&f, &pos);
         ttNTHeader.uStorageOffset = GetUSHORT(&f, &pos);

         if (pos == -1)
               return csRetVal;

         TT_NAME_RECORD ttRecord;
         bFound = true;
         for (int i=0; i<ttNTHeader.uNRCount; i++)
         {
            ttRecord.uPlatformID = GetUSHORT(&f, &pos);
            ttRecord.uEncodingID = GetUSHORT(&f, &pos);
            ttRecord.uLanguageID = GetUSHORT(&f, &pos);
            ttRecord.uNameID = GetUSHORT(&f, &pos);
            ttRecord.uStringLength = GetUSHORT(&f, &pos);
            ttRecord.uStringOffset = GetUSHORT(&f, &pos);

            if (pos == -1)
                  return csRetVal;

            //1 says that this is font name. 0 for example determines copyright info
            if (ttRecord.uNameID == 1)
            {
                csTemp = "";
               //save file position, so we can return to continue with search
               int nPos = f.tellg();
               f.seekg(tblDir.uOffset + ttRecord.uStringOffset + ttNTHeader.uStorageOffset);

               char* lpszNameBuf = new char[ttRecord.uStringLength + 1];
               f.read(lpszNameBuf, ttRecord.uStringLength);

               int i = 0;
               while (i < ttRecord.uStringLength)
               {
                   char c = lpszNameBuf[i];
                   if (c != '\0')
                            csTemp += c;
                   i++;
               }

               //yes, still need to check if the font name is not empty
               //if it is, continue the search
               if (csTemp.length() > 0)
               {
                  csRetVal = csTemp;
                  break;
               }
            }
         }
      }
   }

    return csRetVal;
}

hgeFont* FontLoader::GetFont(bool file, string szFontName, int nSize, bool bBold, bool bItalic)
{
   string fntName = szFontName;

   string id = fntName + "|" + ToString(nSize) + "|" + ToString(bBold) + "|" + ToString(bItalic);
   if (lFntList.find(id) != lFntList.end())
   {
      return lFntList[id];
   }
   else
   {
      if ( (FileExists(szFontName) && file) || !file)
      {
         bool fFound = false;
         vector<string>::iterator iter = lTTFList.begin();
         while (iter != lTTFList.end())
         {
            if (*iter == szFontName)
            {
               fFound = true;
               break;
            }
            iter++;
         }

         if (!fFound)
         {
            AddFontResource(szFontName.c_str());
            lTTFList.push_back(szFontName);
         }

         CSymbolRange pRanges[2];
         pRanges[0].First = 32;
         pRanges[0].Last = 256;

         // Generate the glyphs
         HTEXTURE fntTex = TextureGenerate(
            (char*)fntName.c_str(),
            nSize, bBold, bItalic,
            pRanges, 1
         );

         // Create the hgeFont interface
         hgeFont* f = lFntList[id] = new hgeFont(vChars, fntTex);

         return f;
      }
      else
      {
         return NULL;
      }
   }
}

hgeFont* FontLoader::GetFontFromFile(string szFontName, int nSize, bool bBold, bool bItalic)
{
   string fntName;
    if (lFntPathList.find(szFontName) == lFntPathList.end())
    {
        fntName = GetFontName(szFontName);
        lFntPathList[szFontName] = fntName;
    }
    else
        fntName = lFntPathList[szFontName];

    return GetFont(true, fntName, nSize, bBold, bItalic);
}
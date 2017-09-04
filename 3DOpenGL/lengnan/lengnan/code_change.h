#ifndef _CODE_CHANGE_
#define _CODE_CHANGE_

#endif

//char *unicode_to_ansi(char* pstr,LPCTSTR lpstr)
//{
//	//UnicodeÂë×ª»¯ÎªAscIIÂë
//	int iSize;
//	iSize = WideCharToMultiByte(CP_ACP, 0, lpstr, -1, NULL, 0, NULL, NULL);
//	pstr = new char[iSize + 1];
//	memset( ( void* )pstr, 0, sizeof( char ) * ( iSize + 1 ) );
//	WideCharToMultiByte(CP_ACP, 0, lpstr, -1, pstr, iSize, NULL, NULL);
//	return pstr;
//}
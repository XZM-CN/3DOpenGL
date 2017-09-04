// GLFont.h: interface for the CGLFont class.

#if !defined(AFX_GLFONT_H__88F1F000_50F5_452A_B95E_60ED83712FA5__INCLUDED_)
#define AFX_GLFONT_H__88F1F000_50F5_452A_B95E_60ED83712FA5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CGLFont  
{
public:
	void entext(float x,float y, LPCTSTR str,HFONT hFont,float r,float g,float b);
	void c3dtext(LPCTSTR str,HFONT hFont,float z);
	void Printfc3d(LPCTSTR strText,HFONT hFont,float z);
	void Printftext(int x, int y, LPCTSTR lpszText,HFONT hFont);
	void settext(float x,float y,LPCTSTR str,HFONT Font,float r,float g,float b);
	void drawtext3d(float x,float y, float z,LPCTSTR str,HFONT hFont,float r,float g,float b,float thickness);
	void BuildGLFont(int fontHeight);
	void PrintGLText(GLint x, GLint y, const char *string);	
	GLvoid glBuild3DFont(GLvoid);
	GLvoid Kill3DFont(GLvoid);
	GLvoid gl3DPrint(const char *fmt, ...);
	CGLFont();
	virtual ~CGLFont();
protected:
	HFONT hFont;
	GLuint	base;

	GLYPHMETRICSFLOAT gmf[256];	// 记录256个字符的信息
};

#endif
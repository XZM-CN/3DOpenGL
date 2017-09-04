#include "stdafx.h"

#include "GLFont.h"

//#ifdef _DEBUG
//#undef THIS_FILE
//static char THIS_FILE[]=__FILE__;
//#define new DEBUG_NEW
//#endif

//////////////////////////////////////////////////////////////////////
//unsigned int Base;
//////////////////////////////////////////////////////////////////////

CGLFont::CGLFont()
{
}
CGLFont::~CGLFont()
{
}
void CGLFont::entext( float x,float y, LPCTSTR str, HFONT hFont, float r, float g, float b) 
{  
	HDC hdc = wglGetCurrentDC();  
	SelectObject(hdc, hFont); 
	unsigned int Base = glGenLists(96);
	wglUseFontBitmaps(hdc, 32, 96,Base);
	glDisable(GL_TEXTURE_2D); 
	glDisable(GL_LIGHTING); 
	glPushAttrib(GL_LIST_BIT);  
	glColor3f(r,g,b);
	glRasterPos2f(x/100, y/100); 
	glListBase(Base - 32); 
	glCallLists(_tcslen(str), GL_UNSIGNED_BYTE, str); 
	glPopAttrib(); 
	glEnable(GL_LIGHTING); 
	glEnable(GL_TEXTURE_2D); 
	glDeleteLists(Base, 96); 
}

//////////////////////////////////////////////////////////////////
void CGLFont::c3dtext(LPCTSTR str,HFONT hFont,float z)
{ 
	glDisable(GL_TEXTURE_2D); 
	glDisable(GL_LIGHTING);
	Printfc3d(TEXT("���庺��"),hFont,z);
	glEnable(GL_LIGHTING);   
	glEnable(GL_TEXTURE_2D);
}
void CGLFont::Printfc3d(LPCTSTR strText,HFONT hFont,float z)
{
	HDC hdc = wglGetCurrentDC();
	HFONT hOldFont=(HFONT)::SelectObject(hdc,hFont);
	UCHAR * pChar=(UCHAR*)strText/*.GetBuffer(strText.GetLength())*/;
	int   nListNum;  
	DWORD dwChar;  
	GLYPHMETRICSFLOAT pgmf[1]; 
	glPushMatrix();   
	for(int i = 0; i <(int) _tcslen(strText)/*strText.GetLength()*/; i++)
	{ 
		if(IsDBCSLeadByte((BYTE)pChar[i]))
		{
			dwChar=(DWORD)((pChar[i]<<8)|pChar[i+1]);
			i++;
		}
		else dwChar = pChar[i];
		nListNum = glGenLists(1);  
		wglUseFontOutlines(hdc,	dwChar,1,nListNum,0.0f,z,WGL_FONT_POLYGONS,pgmf );
		glCallList(nListNum);  
		glDeleteLists(nListNum, 1);   
	} 
	glPopMatrix();    
	//strText.ReleaseBuffer();
	::SelectObject(hdc,hOldFont); 
}
////////////////////////////////////////////////////////////////////////
void CGLFont:: settext (float x,float y,LPCTSTR str,HFONT Font,float r,float g,float b)
{
	glPushMatrix();
	glPushAttrib(GL_CURRENT_BIT);
	glDisable(GL_TEXTURE_2D); 
	//glDisable(GL_LIGHTING); 
	glColor3f(r,g,b);  

	//--����任--begin
	double X = x;
	double Y = y;
	double Z;

	GLdouble ModeMatrix[16];
	GLdouble ProjectMatrix[16];
	GLint    ViewPort[4];

	glGetDoublev(GL_PROJECTION_MATRIX, ProjectMatrix);
	glGetDoublev(GL_MODELVIEW_MATRIX, ModeMatrix);
	glGetIntegerv(GL_VIEWPORT, ViewPort);

	gluUnProject((GLdouble)x,(GLdouble)y,(GLdouble)Z,ModeMatrix,ProjectMatrix,ViewPort, &X, &Y, &Z);
	//--����任--end

	glTranslatef(X,Y,-0.4f);

	Printftext (0,0, str,Font);    
	//glEnable(GL_LIGHTING);     
	glEnable(GL_TEXTURE_2D);
	glPopAttrib();
	glPopMatrix();
}
void CGLFont:: Printftext (int x, int y, LPCTSTR lpszText,HFONT hFont)
{ 
	//CBitmap bitmap;
	BITMAP bm;
	SIZE size; 
	HDC MDC = ::CreateCompatibleDC(0);
	SelectObject(MDC,hFont); 
	::GetTextExtentPoint32(MDC,lpszText,_tcslen(lpszText),&size);
	//bitmap.CreateBitmap(size.cx, size.cy, 1, 1, NULL);
	HBITMAP hbm = CreateBitmap(
		size.cx,         // bitmap width, in pixels
		size.cy,        // bitmap height, in pixels
		1,       // number of color planes
		1,   // number of bits to identify color
		NULL // color data array
		);
	HBITMAP oldBmp=(HBITMAP)SelectObject(MDC,hbm/*bitmap*/);
	SetBkColor  (MDC, RGB(0,     0,   0));
	SetTextColor(MDC, RGB(255, 255, 255));
	TextOut(MDC, 0, 0, lpszText, _tcslen(lpszText));
	//bitmap.GetBitmap(&bm);
	::GetObject(hbm, sizeof(BITMAP), &bm);

	//��ͼƬ��С��Ϊ2���ݴη�
	size.cx = (bm.bmWidth + 31) & (~31);

	int bufsize =size.cy * size.cx;
	struct {  BITMAPINFOHEADER bih;
	RGBQUAD col[2];
	}bic; 
	BITMAPINFO *binf = (BITMAPINFO *)&bic; 
	binf->bmiHeader.biSize     = sizeof(binf->bmiHeader);
	binf->bmiHeader.biWidth    = bm.bmWidth;
	binf->bmiHeader.biHeight   = bm.bmHeight;
	binf->bmiHeader.biPlanes   = 1;   
	binf->bmiHeader.biBitCount = 1;
	binf->bmiHeader.biCompression = BI_RGB;
	binf->bmiHeader.biSizeImage   = bufsize;

	UCHAR* Bits = new UCHAR[bufsize]; 
	::GetDIBits(MDC,hbm,0,bm.bmHeight,Bits,binf,DIB_RGB_COLORS); 

	glPixelStorei(GL_UNPACK_ALIGNMENT ,1);
	glRasterPos2i(x,y); 
	glBitmap(size.cx,size.cy,0,0,0,0,Bits); 
	delete[] Bits;    
	SelectObject(MDC, oldBmp);  
	::DeleteDC(MDC);
	DeleteObject(hbm);
}

void CGLFont::drawtext3d(float x,float y, float z,LPCTSTR lpszText,HFONT hFont,float r,float g,float b,float thickness)
{
	HDC hdc = wglGetCurrentDC();
	::SelectObject(hdc,hFont);
	size_t nnum=_tcslen(lpszText)*2;
	int   nListNum;  
	DWORD dwChar;
	UCHAR * pChar=(UCHAR*)lpszText;
	GLYPHMETRICSFLOAT pgmf[1];
	glTranslatef(x,y,z);
	//glRasterPos3f(x,y,z);
	glPushMatrix();
	glScalef(0.08f, 0.15f, 0.15f); 
	glColor3f(r,g,b);
		
	for(int i = 0; i <(int)nnum; i++)
	{ 
		if(IsDBCSLeadByte((BYTE)pChar[i]))
		{
			dwChar=(DWORD)((pChar[i]<<8)|pChar[i+1]);
			i++;
		}
		else dwChar = pChar[i];
		nListNum = glGenLists(1);  
		wglUseFontOutlines(hdc,	dwChar,1,nListNum,0.0f,thickness,WGL_FONT_POLYGONS,pgmf);
		glCallList(nListNum);  
		glDeleteLists(nListNum, 1);   
	}
	glPopMatrix();
	glTranslatef(-x,-y,-z);
	::DeleteObject(hFont);
}

/////////һ���Ӣ�������ӡ
void CGLFont::BuildGLFont(int fontHeight)
{
	HDC   hDC =::GetDC(HWND_DESKTOP);    //////������������˰���

	int tFontHeight = -1 * fontHeight;

	GLuint NormalFontBase = glGenLists(96);                         // Storage For 96 Characters
	HFONT font = CreateFont( -tFontHeight,                   // Height Of Font
		0,                            // Width Of Font
		0,                            // Angle Of Escapement
		0,                            // Orientation Angle
		FW_BOLD,                      // Font Weight
		TRUE,                         // Italic
		FALSE,                        // Underline
		FALSE,                        // Strikeout
		ANSI_CHARSET,                 // Character Set Identifier
		OUT_TT_PRECIS,                // Output Precision
		CLIP_DEFAULT_PRECIS,          // Clipping Precision
		ANTIALIASED_QUALITY,          // Output Quality
		FF_DONTCARE|DEFAULT_PITCH,    // Family And Pitch
		_T("Georgia"));                   // Font Name

	HFONT oldfont = (HFONT)SelectObject(hDC, font);         // Selects The Font We Want

	wglUseFontBitmaps(hDC, 32, 96, NormalFontBase);         // Builds 96 Characters Starting At Character 32

	SelectObject(hDC, oldfont);                         // Selects The Font We Want to return to
	DeleteObject(font);                                 // Delete The Font

	SetBkMode(hDC,TRANSPARENT);   

	bool NormalFont = true; 
}

void CGLFont::PrintGLText(GLint x, GLint y, const char *string) 
{
	char  text[256];
	va_list pArguments;

	if (string == NULL)
		return;
	GLuint NormalFontBase = glGenLists(96);
	va_start(pArguments, string);
	vsprintf(text, string, pArguments);
	va_end(pArguments);


	glPushAttrib(GL_LIST_BIT | GL_CURRENT_BIT |  GL_ENABLE_BIT | GL_LIGHTING_BIT);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	//glColor4f(mColor[0], mColor[1], mColor[2], mColor[3]);

	//glWindowPos2i(x, y);

	glListBase(NormalFontBase - 32);
	glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);
	glPopAttrib();
}


GLvoid CGLFont::glBuild3DFont(GLvoid)
{
	HFONT	font;						// ������
	HFONT	oldfont;						// �ɵ�������
	HDC hDC = wglGetCurrentDC(); 

	base = glGenLists(256);						// ����256����ʾ�б�	
	font = CreateFont(	-24,					// ����߶�
		0,				// ������
		0,				// �������ת�Ƕ� Angle Of Escapement
		0,				// ������ߵ���ת�Ƕ�Orientation Angle
		FW_BOLD,				// ���������
		FALSE,				// �Ƿ�ʹ��б��
		FALSE,				// �Ƿ�ʹ���»���
		FALSE,				// �Ƿ�ʹ��ɾ����
		ANSI_CHARSET,			// �����ַ���
		OUT_TT_PRECIS,			// �������
		CLIP_DEFAULT_PRECIS,		// �ü�����
		ANTIALIASED_QUALITY,		// �������
		FF_DONTCARE|DEFAULT_PITCH,		// Family And Pitch
		_T("Courier New"));			// ��������
	oldfont = (HFONT)SelectObject(hDC, font);			// ѡ��������Ҫ������
	wglUseFontOutlines(	hDC,					// ���õ�ǰ�����豸������ľ��
		0,				// ���ڴ�����ʾ�б�����ĵ�һ���ַ���ASCIIֵ
		255,				// �ַ���
		base,				// ��һ����ʾ�б������
		0.0f,				// ����Ĺ⻬�ȣ�ԽСԽ�⻬��0.0Ϊ��⻬��״̬
		0.2f,				// ��z����ͻ���ľ���
		WGL_FONT_POLYGONS,			// ʹ�ö�����������ַ���ÿ��������ж����ķ���
		gmf);				//һ���������ζ������ݵ�����ĵ�ַ��ÿ������Ԫ��������Ӧ����ʾ�б��ַ���������� 
	SelectObject(hDC, oldfont);					// ѡ��ԭ��������
	DeleteObject(font);					// ɾ������
}

GLvoid CGLFont::Kill3DFont(GLvoid)						// ɾ����ʾ�б�
{
	glDeleteLists(base, 256);					// ɾ��256����ʾ�б�
}

GLvoid CGLFont::gl3DPrint(const char *fmt, ...)					// �Զ���GL������庯��
{
	float		length=0;					// ��ѯ�ַ����ĳ���
	char		text[256];				// ����������Ҫ�����ִ�
	va_list		ap;					// ָ��һ�������б��ָ��

	if (fmt == NULL)						// ����������򷵻�
		return;					
	va_start(ap, fmt);						// �����ɱ����
	vsprintf(text, fmt, ap);				// �Ѳ���ֵд���ַ���
	va_end(ap);						// ��������
	for (unsigned int loop=0;loop<(strlen(text));loop++)	// ���������ַ����ĳ���
	{
		length+=gmf[text[loop]].gmfCellIncX;		
	}
	glTranslatef(-length/2,0.0f,0.0f);			// ���ַ������������
	glPushAttrib(GL_LIST_BIT);				// ����ʾ�б�����ѹ�����Զ�ջ
	glListBase(base);					// ������ʾ�б�Ļ���ֵΪ0

	glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);	// ������ʾ�б�����ַ���
	glPopAttrib();					// �������Զ�ջ
}
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
	Printfc3d(TEXT("立体汉字"),hFont,z);
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

	//--坐标变换--begin
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
	//--坐标变换--end

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

	//把图片大小变为2的幂次方
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

/////////一般的英语字体打印
void CGLFont::BuildGLFont(int fontHeight)
{
	HDC   hDC =::GetDC(HWND_DESKTOP);    //////就是这里搞晕了半晚

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
	HFONT	font;						// 字体句柄
	HFONT	oldfont;						// 旧的字体句柄
	HDC hDC = wglGetCurrentDC(); 

	base = glGenLists(256);						// 创建256个显示列表	
	font = CreateFont(	-24,					// 字体高度
		0,				// 字体宽度
		0,				// 字体的旋转角度 Angle Of Escapement
		0,				// 字体底线的旋转角度Orientation Angle
		FW_BOLD,				// 字体的重量
		FALSE,				// 是否使用斜体
		FALSE,				// 是否使用下划线
		FALSE,				// 是否使用删除线
		ANSI_CHARSET,			// 设置字符集
		OUT_TT_PRECIS,			// 输出精度
		CLIP_DEFAULT_PRECIS,		// 裁剪精度
		ANTIALIASED_QUALITY,		// 输出质量
		FF_DONTCARE|DEFAULT_PITCH,		// Family And Pitch
		_T("Courier New"));			// 字体名称
	oldfont = (HFONT)SelectObject(hDC, font);			// 选择我们需要的字体
	wglUseFontOutlines(	hDC,					// 设置当前窗口设备描述表的句柄
		0,				// 用于创建显示列表字体的第一个字符的ASCII值
		255,				// 字符数
		base,				// 第一个显示列表的名称
		0.0f,				// 字体的光滑度，越小越光滑，0.0为最光滑的状态
		0.2f,				// 在z方向突出的距离
		WGL_FONT_POLYGONS,			// 使用多边形来生成字符，每个顶点具有独立的法线
		gmf);				//一个接收字形度量数据的数组的地址，每个数组元素用它对应的显示列表字符的数据填充 
	SelectObject(hDC, oldfont);					// 选择原来的字体
	DeleteObject(font);					// 删除字体
}

GLvoid CGLFont::Kill3DFont(GLvoid)						// 删除显示列表
{
	glDeleteLists(base, 256);					// 删除256个显示列表
}

GLvoid CGLFont::gl3DPrint(const char *fmt, ...)					// 自定义GL输出字体函数
{
	float		length=0;					// 查询字符串的长度
	char		text[256];				// 保存我们想要的文字串
	va_list		ap;					// 指向一个变量列表的指针

	if (fmt == NULL)						// 如果无输入则返回
		return;					
	va_start(ap, fmt);						// 分析可变参数
	vsprintf(text, fmt, ap);				// 把参数值写入字符串
	va_end(ap);						// 结束分析
	for (unsigned int loop=0;loop<(strlen(text));loop++)	// 查找整个字符串的长度
	{
		length+=gmf[text[loop]].gmfCellIncX;		
	}
	glTranslatef(-length/2,0.0f,0.0f);			// 把字符串置于最左边
	glPushAttrib(GL_LIST_BIT);				// 把显示列表属性压入属性堆栈
	glListBase(base);					// 设置显示列表的基础值为0

	glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);	// 调用显示列表绘制字符串
	glPopAttrib();					// 弹出属性堆栈
}
// lengnan.h : lengnan Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// ClengnanApp:
// �йش����ʵ�֣������ lengnan.cpp
//

class ClengnanApp : public CWinApp
{
public:
	ClengnanApp();


// ��д
public:
	virtual BOOL InitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern ClengnanApp theApp;
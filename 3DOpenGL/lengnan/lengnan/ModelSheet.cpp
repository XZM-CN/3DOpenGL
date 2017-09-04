// ModelSheet.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "lengnan.h"
#include "ModelSheet.h"
#include "myheader.h"


// CModelSheet

IMPLEMENT_DYNAMIC(CModelSheet, CPropertySheet)

CModelSheet::CModelSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{

	// �����������ҳ�����Ա�    
	AddPage(&mymodelset);
	AddPage(&myxlayout);
	AddPage(&myylayout);
	AddPage(&myzlayout);
	AddPage(&mysensornum);
	for (int x=0;x<SENSORLAYNUM;x++)
	{
		for (int y=0;y<SENSORLAYNUM;y++)
		{
			for (int z=0;z<SENSORLAYNUM;z++)
			{
				sensor_pos[x][y][z]=0;
			}
		}
	}
}

CModelSheet::CModelSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{

	// �����������ҳ�����Ա�    
	AddPage(&mymodelset); 
	AddPage(&myxlayout);
	AddPage(&myylayout);
	AddPage(&myzlayout); 
	AddPage(&mysensornum);
	for (int x=0;x<SENSORLAYNUM;x++)
	{
		for (int y=0;y<SENSORLAYNUM;y++)
		{
			for (int z=0;z<SENSORLAYNUM;z++)
			{
				sensor_pos[x][y][z]=0;
			}
		}
	}
}

CModelSheet::~CModelSheet()
{
}


BEGIN_MESSAGE_MAP(CModelSheet, CPropertySheet)
END_MESSAGE_MAP()


// CModelSheet ��Ϣ�������

// lengnanDoc.cpp : ClengnanDoc ���ʵ��
//

#include "stdafx.h"
#include "lengnan.h"

#include "lengnanDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ClengnanDoc

IMPLEMENT_DYNCREATE(ClengnanDoc, CDocument)

BEGIN_MESSAGE_MAP(ClengnanDoc, CDocument)
END_MESSAGE_MAP()


// ClengnanDoc ����/����

ClengnanDoc::ClengnanDoc()
{
	// TODO: �ڴ����һ���Թ������

}

ClengnanDoc::~ClengnanDoc()
{
}

BOOL ClengnanDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// ClengnanDoc ���л�

void ClengnanDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
	}
}


// ClengnanDoc ���

#ifdef _DEBUG
void ClengnanDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void ClengnanDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// ClengnanDoc ����

// lengnanDoc.cpp : ClengnanDoc 类的实现
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


// ClengnanDoc 构造/析构

ClengnanDoc::ClengnanDoc()
{
	// TODO: 在此添加一次性构造代码

}

ClengnanDoc::~ClengnanDoc()
{
}

BOOL ClengnanDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// ClengnanDoc 序列化

void ClengnanDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}


// ClengnanDoc 诊断

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


// ClengnanDoc 命令

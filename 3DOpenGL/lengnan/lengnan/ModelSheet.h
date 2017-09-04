#pragma once
#include "MOdelSet.h"
#include "Cxlayout.h"
#include "Cylayout.h"
#include "Sensornum.h"
#include "Czlayout.h"
#include "myheader.h"

// CModelSheet

class CModelSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CModelSheet)

public:
	CModelSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CModelSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CModelSheet();

protected:
	DECLARE_MESSAGE_MAP()
public:
	CMOdelSet mymodelset;
	Cxlayout myxlayout;
	Cylayout myylayout;
	Czlayout myzlayout;
	CSensornum mysensornum;
};



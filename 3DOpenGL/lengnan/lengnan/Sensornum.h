#pragma once
#include "myheader.h"
#include "afxcmn.h"

// CSensornum �Ի���

class CSensornum : public CPropertyPage
{
	DECLARE_DYNAMIC(CSensornum)

public:
	CSensornum();
	virtual ~CSensornum();

// �Ի�������
	enum { IDD = IDD_DSENORNUM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnSetActive();
	afx_msg void OnPaint();
	// X�򴫸�������
	int myxnum;
	// Y�򴫸�������
	int myynum;
	// Z�����
	int myznum;
	//�ػ���±�־
	bool redrawflag;
	virtual BOOL OnInitDialog();
	// ����������б�
	CListCtrl mylist;
	afx_msg void OnNMDblclkLsensornum(NMHDR *pNMHDR, LRESULT *pResult);
	virtual LRESULT OnWizardBack();
	virtual BOOL OnWizardFinish();
	//����ɱ�־λ
	bool finishflag;
};

#pragma once
#include "myheader.h"

// CMOdelSet �Ի���

class CMOdelSet : public CPropertyPage
{
	DECLARE_DYNAMIC(CMOdelSet)

public:
	CMOdelSet();
	virtual ~CMOdelSet();

// �Ի�������
	enum { IDD = IDD_MODELPARAM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnSetActive();
	// ģ�ͳ���
	double mymodellongth;
	// ģ�Ϳ��
	double mymodelwith;
	// ģ�͸߶�
	double mymodelheigh;
	// �ɼ�������
	int mycardnum;
	virtual LRESULT OnWizardNext();
	// �����ļ�����
	CString myfilename;
	//�����ļ�����λ��
	CString pathname;
	afx_msg void OnBnClickedButton1();
};

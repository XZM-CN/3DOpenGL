#pragma once
#include "afxwin.h"
#include "myheader.h"

// Cselectnum �Ի���

class Cselectnum : public CDialogEx
{
	DECLARE_DYNAMIC(Cselectnum)

public:
	Cselectnum(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~Cselectnum();

// �Ի�������
	enum { IDD = IDD_DIALOG5 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	// �����������б�
	CComboBox mycombox;
	virtual BOOL OnInitDialog();
///ѡ��Ŀռ��ķ�λ����
	unsigned int Xposnum;
	unsigned int Yposnum;
	unsigned int Zposnum;
	unsigned int SelectNum;
	afx_msg void OnBnClickedOk();
};

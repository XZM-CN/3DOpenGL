#pragma once


// CPointNow �Ի���

class CPointNow : public CDialogEx
{
	DECLARE_DYNAMIC(CPointNow)

public:
	CPointNow(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPointNow();

// �Ի�������
	enum { IDD = IDD_DIALOG3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	float m_point_x;
	float m_point_y;
	float m_point_z;
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};

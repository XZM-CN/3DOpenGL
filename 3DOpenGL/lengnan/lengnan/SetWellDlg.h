#pragma once


// CSetWellDlg �Ի���

class CSetWellDlg : public CDialog
{
	DECLARE_DYNAMIC(CSetWellDlg)

public:
	CSetWellDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSetWellDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_WELL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnWellRadio(UINT ucmdID);
	afx_msg void OnWellRadio2(UINT ucmdID);
	// ��ʼ����
	float m_start;
	// ��ֹ����
	float m_end;
	int m_nwellmode;
	int m_nwellorientations;
	int m_wellflag;
	virtual BOOL OnInitDialog();
	// x���������
	double x_start;
	// Y���������
	double y_start;
	// Z���������
	double Z_start;
};

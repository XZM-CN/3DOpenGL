#pragma once


// CHistoty �Ի���

class CHistoty : public CDialogEx
{
	DECLARE_DYNAMIC(CHistoty)

public:
	CHistoty(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CHistoty();
	//�򿪵������ļ�����
	CString mypathfilename;
	//ͼƬ����Ƶ���ݱ���λ��
	CString mypictpath;
// �Ի�������
	enum { IDD = IDD_DIALOG7 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedOk();
};

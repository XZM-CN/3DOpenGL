#pragma once


// CHistoty 对话框

class CHistoty : public CDialogEx
{
	DECLARE_DYNAMIC(CHistoty)

public:
	CHistoty(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CHistoty();
	//打开的数据文件名称
	CString mypathfilename;
	//图片和视频数据保存位置
	CString mypictpath;
// 对话框数据
	enum { IDD = IDD_DIALOG7 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedOk();
};

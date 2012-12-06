#pragma once


// RGBToGrey 对话框

class RGBToGrey : public CDialog
{
	DECLARE_DYNAMIC(RGBToGrey)

public:
	RGBToGrey(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~RGBToGrey();

// 对话框数据
	enum { IDD = IDD_DIALOG_RTG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_path_source;
	afx_msg void OnBnClickedButtonRtgOpen();
	afx_msg void OnBnClickedButtonRtgStart();
	//add by yqc
	int GtoR(CString source,CString des);
};

#pragma once


// RGBToGrey �Ի���

class RGBToGrey : public CDialog
{
	DECLARE_DYNAMIC(RGBToGrey)

public:
	RGBToGrey(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~RGBToGrey();

// �Ի�������
	enum { IDD = IDD_DIALOG_RTG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_path_source;
	afx_msg void OnBnClickedButtonRtgOpen();
	afx_msg void OnBnClickedButtonRtgStart();
	//add by yqc
	int GtoR(CString source,CString des);
};

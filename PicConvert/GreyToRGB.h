#pragma once


// GreyToRGB �Ի���

class GreyToRGB : public CDialog
{
	DECLARE_DYNAMIC(GreyToRGB)

public:
	GreyToRGB(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~GreyToRGB();

// �Ի�������
	enum { IDD = IDD_DIALOG_GTR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_source_path;
	afx_msg void OnBnClickedButtonGtrOpen();
	afx_msg void OnBnClickedButtonGtrStart();
	//add by yqc
	bool saveBmp24(CString bmpName, unsigned char *imgBuf, int width, int height, int biBitCount);
};

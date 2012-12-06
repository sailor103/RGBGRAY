
// PicConvertView.h : CPicConvertView ��Ľӿ�
//

#pragma once


class CPicConvertView : public CView
{
protected: // �������л�����
	CPicConvertView();
	DECLARE_DYNCREATE(CPicConvertView)

// ����
public:
	CPicConvertDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CPicConvertView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnRgbtogrey();
	afx_msg void OnGreytorgb();
};

#ifndef _DEBUG  // PicConvertView.cpp �еĵ��԰汾
inline CPicConvertDoc* CPicConvertView::GetDocument() const
   { return reinterpret_cast<CPicConvertDoc*>(m_pDocument); }
#endif



// PicConvertView.cpp : CPicConvertView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "PicConvert.h"
#endif

#include "PicConvertDoc.h"
#include "PicConvertView.h"
#include "RGBToGrey.h"
#include "GreyToRGB.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPicConvertView

IMPLEMENT_DYNCREATE(CPicConvertView, CView)

BEGIN_MESSAGE_MAP(CPicConvertView, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CPicConvertView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_RGBTOGREY, &CPicConvertView::OnRgbtogrey)
	ON_COMMAND(ID_GREYTORGB, &CPicConvertView::OnGreytorgb)
END_MESSAGE_MAP()

// CPicConvertView ����/����

CPicConvertView::CPicConvertView()
{
	// TODO: �ڴ˴���ӹ������

}

CPicConvertView::~CPicConvertView()
{
}

BOOL CPicConvertView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CPicConvertView ����

void CPicConvertView::OnDraw(CDC* /*pDC*/)
{
	CPicConvertDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


// CPicConvertView ��ӡ


void CPicConvertView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CPicConvertView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CPicConvertView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CPicConvertView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}

void CPicConvertView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CPicConvertView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CPicConvertView ���

#ifdef _DEBUG
void CPicConvertView::AssertValid() const
{
	CView::AssertValid();
}

void CPicConvertView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CPicConvertDoc* CPicConvertView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPicConvertDoc)));
	return (CPicConvertDoc*)m_pDocument;
}
#endif //_DEBUG


// CPicConvertView ��Ϣ�������


void CPicConvertView::OnRgbtogrey()
{
	// TODO: RGBģʽת���Ҷ�ģʽ
	RGBToGrey dlg;
	dlg.DoModal();
}


void CPicConvertView::OnGreytorgb()
{
	// TODO: �Ҷ�ģʽת��RGBģʽ
	GreyToRGB dlg;
	dlg.DoModal();
}

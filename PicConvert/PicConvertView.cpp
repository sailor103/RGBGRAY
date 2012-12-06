
// PicConvertView.cpp : CPicConvertView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
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
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CPicConvertView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_RGBTOGREY, &CPicConvertView::OnRgbtogrey)
	ON_COMMAND(ID_GREYTORGB, &CPicConvertView::OnGreytorgb)
END_MESSAGE_MAP()

// CPicConvertView 构造/析构

CPicConvertView::CPicConvertView()
{
	// TODO: 在此处添加构造代码

}

CPicConvertView::~CPicConvertView()
{
}

BOOL CPicConvertView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CPicConvertView 绘制

void CPicConvertView::OnDraw(CDC* /*pDC*/)
{
	CPicConvertDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CPicConvertView 打印


void CPicConvertView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CPicConvertView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CPicConvertView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CPicConvertView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
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


// CPicConvertView 诊断

#ifdef _DEBUG
void CPicConvertView::AssertValid() const
{
	CView::AssertValid();
}

void CPicConvertView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CPicConvertDoc* CPicConvertView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPicConvertDoc)));
	return (CPicConvertDoc*)m_pDocument;
}
#endif //_DEBUG


// CPicConvertView 消息处理程序


void CPicConvertView::OnRgbtogrey()
{
	// TODO: RGB模式转换灰度模式
	RGBToGrey dlg;
	dlg.DoModal();
}


void CPicConvertView::OnGreytorgb()
{
	// TODO: 灰度模式转换RGB模式
	GreyToRGB dlg;
	dlg.DoModal();
}

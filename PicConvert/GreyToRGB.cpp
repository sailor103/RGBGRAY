// GreyToRGB.cpp : 实现文件
//

#include "stdafx.h"
#include "PicConvert.h"
#include "GreyToRGB.h"
#include "afxdialogex.h"
#include "ImageCenterDib.h"
#include <math.h>
// GreyToRGB 对话框

#define PI 3.1415926
#define B 0
#define G 1
#define R 2
int ColorTranTable[3][256];
unsigned char *pBmpBuf24;

IMPLEMENT_DYNAMIC(GreyToRGB, CDialog)

GreyToRGB::GreyToRGB(CWnd* pParent /*=NULL*/)
	: CDialog(GreyToRGB::IDD, pParent)
	, m_source_path(_T(""))
{

}

GreyToRGB::~GreyToRGB()
{
}

//保存函数
bool GreyToRGB::saveBmp24(CString bmpName, unsigned char *imgBuf, int width, int height, int biBitCount)
{
	//如果位图数据指针为0,则没有数据传入,函数返回
	if(!imgBuf)
		return 0;
	biBitCount = 24;

	//以二进制写的方式打开文件
	FILE *fp=fopen(bmpName,"wb");
	if(fp==0) return 0;	

	//申请位图文件头结构变量，填写文件头信息
	BITMAPFILEHEADER fileHead;
	fileHead.bfType = 0x4D42;//bmp类型

	//待存储图像数据每行字节数为4的倍数
	int lineByte=(width*biBitCount/8+3)/4*4;

	//bfSize是图像文件4个组成部分之和
	fileHead.bfSize= sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + lineByte*height;
	fileHead.bfReserved1 = 0;
	fileHead.bfReserved2 = 0;	

	//bfOffBits是图像文件前三个部分所需空间之和
	fileHead.bfOffBits=54;
	//写文件头进文件
	fwrite(&fileHead, sizeof(BITMAPFILEHEADER),1, fp);	
	//申请位图信息头结构变量，填写信息头信息
	BITMAPINFOHEADER head; 
	head.biBitCount=biBitCount;
	head.biClrImportant=0;
	head.biClrUsed=0;
	head.biCompression=0;
	head.biHeight=height;
	head.biPlanes=1;
	head.biSize=40;
	head.biSizeImage=lineByte*height;
	head.biWidth=width;
	head.biXPelsPerMeter=0;
	head.biYPelsPerMeter=0;
	//写位图信息头进内存
	fwrite(&head, sizeof(BITMAPINFOHEADER),1, fp);			
	fwrite(imgBuf, height*lineByte, 1, fp);	
	//关闭文件
	fclose(fp);	
	return 1;
}

void GreyToRGB::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_GTR, m_source_path);
}


BEGIN_MESSAGE_MAP(GreyToRGB, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_GTR_OPEN, &GreyToRGB::OnBnClickedButtonGtrOpen)
	ON_BN_CLICKED(IDC_BUTTON_GTR_START, &GreyToRGB::OnBnClickedButtonGtrStart)
END_MESSAGE_MAP()


// GreyToRGB 消息处理程序



void GreyToRGB::OnBnClickedButtonGtrOpen()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog fileDialog(TRUE,_T("Bmp Files(*.bmp)|*.bmp"),
		NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T( "Bmp Files(*.bmp)|*.bmp||"));

	//下面调用了打开文件的对话框
	if(fileDialog.DoModal()==IDOK)
	{
		m_source_path=fileDialog.GetPathName();//得到路径
		this->UpdateData(FALSE);
	}
}


void GreyToRGB::OnBnClickedButtonGtrStart()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_source_path!="")
	{
		CString tempsave;
		CFileDialog fileDialog(FALSE,_T("Bmp Files(*.bmp)|*.bmp"),
			NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T( "Bmp Files(*.bmp)|*.bmp||"));

		if(fileDialog.DoModal()==IDOK)
		{
			tempsave=fileDialog.GetPathName();//得到路径
		}
		//start------
		ImgCenterDib m_dib;
		m_dib.Read(m_source_path);//读取文件
		//获取ImgCenterDib类对象的指针，访问打开文件的数据
		ImgCenterDib *pDib=&m_dib;
		//位图数据的指针
		unsigned char *imgData=pDib->m_pImgData;

		//位图阵列的大小
		CSize imgSize=pDib->GetDimensions();

		//每像素的位数
		int nBitCount=pDib->m_nBitCount;
		double degree;
		//循环变量，图像坐标
		int i,j;

		//每像素占字节数
		int bytePerPixel=nBitCount/8;

		//每行像素所占字节数，必须是4的倍数
		int lineByte=(imgSize.cx*nBitCount/8+3)/4*4;
		int lineByte24=(imgSize.cx*3*nBitCount/8+3)/4*4;
		if(nBitCount==8)
		{
			for(i=0;i<=255;i++)
			{
				//将1-255的图像信息编程PI，进行正弦和余弦函数的计算
				degree=i*3*PI*(1.0/255)/2;
				if(degree<=PI/2&&degree>=0)
				{
					ColorTranTable[B][i]=cos(degree)*255;
					ColorTranTable[G][i]=sin(degree)*255;
					ColorTranTable[R][i]=0;
				}
				else if(degree>PI/2 && degree<=PI)
				{
					ColorTranTable[B][i]=0;
					ColorTranTable[G][i]=sin(degree)*255;
					ColorTranTable[R][i]=-cos(degree)*255;
				}
				else if(degree>PI && degree<=PI*3/2)
				{
					ColorTranTable[B][i]=-sin(degree)*255;
					ColorTranTable[G][i]=0;
					ColorTranTable[R][i]=-cos(degree)*255;
				}
			}
		}
		pBmpBuf24=new unsigned char[lineByte24 * imgSize.cy];

		//得到24位的数据
		for(i=0;i<imgSize.cy;i++)
		{
			for(j=0;j<imgSize.cx;j++)
			{
				*(pBmpBuf24+i*lineByte24+j*3+0)=ColorTranTable[B][*(imgData+i*lineByte+j)];//blue
				*(pBmpBuf24+i*lineByte24+j*3+1)=ColorTranTable[G][*(imgData+i*lineByte+j)];//green
				*(pBmpBuf24+i*lineByte24+j*3+2)=ColorTranTable[R][*(imgData+i*lineByte+j)];//red
			}
		}

		//保存伪彩色数据
		saveBmp24(tempsave, pBmpBuf24, imgSize.cx, imgSize.cy, 24);
		MessageBox(_T("转换成功"));
		//end--------
	}
	else
	{
		MessageBox(_T("请先打开文件"));
	}
}

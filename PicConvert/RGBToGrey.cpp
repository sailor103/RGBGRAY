// RGBToGrey.cpp : 实现文件
//

#include "stdafx.h"
#include "PicConvert.h"
#include "RGBToGrey.h"
#include "afxdialogex.h"


// RGBToGrey 对话框

IMPLEMENT_DYNAMIC(RGBToGrey, CDialog)

RGBToGrey::RGBToGrey(CWnd* pParent /*=NULL*/)
	: CDialog(RGBToGrey::IDD, pParent)
	, m_path_source(_T(""))
{

}

RGBToGrey::~RGBToGrey()
{
}

int RGBToGrey::GtoR(CString source,CString des)
{
	//灰度转换代码
	FILE *sourcefile,*targetfile;


	//位图文件头和信息头
	BITMAPFILEHEADER sourcefileheader,targetfileheader;
	BITMAPINFOHEADER sourceinfoheader,targetinfoheader;
	memset(&targetfileheader,0,sizeof(BITMAPFILEHEADER));
	memset(&targetinfoheader,0,sizeof(BITMAPINFOHEADER));

	sourcefile=fopen(source,"rb"); //以二进制形式读文件
	fread((void*)&sourcefileheader,1,sizeof(BITMAPFILEHEADER),sourcefile);//提取原图文件头
	if(sourcefileheader.bfType!=0x4d42)
	{ 
		fclose(sourcefile);
		MessageBox(_T("原图象不为BMP图象！"));
		return -1;
	}
	fread((void*)&sourceinfoheader,1,sizeof(BITMAPINFOHEADER),sourcefile);//提取文件信息头
	if(sourceinfoheader.biBitCount!=24)
	{
		fclose(sourcefile);
		MessageBox(_T("原图象不为24位真彩色！"));
		return -1;
	}
	if(sourceinfoheader.biCompression!=BI_RGB)
	{
		fclose(sourcefile);
		MessageBox(_T("原图象为压缩后的图象，本程序不处理压缩过的图象！"));
		return -1;
	}

	//构造灰度图的文件头
	targetfileheader.bfOffBits=54+sizeof(RGBQUAD)*256;
	targetfileheader.bfSize=targetfileheader.bfOffBits+sourceinfoheader.biSizeImage/3;
	targetfileheader.bfReserved1=0;
	targetfileheader.bfReserved2=0;
	targetfileheader.bfType=0x4d42;

	//构造灰度图的信息头
	targetinfoheader.biBitCount=8;
	targetinfoheader.biSize=40;
	targetinfoheader.biHeight=sourceinfoheader.biHeight;
	targetinfoheader.biWidth=sourceinfoheader.biWidth;
	targetinfoheader.biPlanes=1;
	targetinfoheader.biCompression=BI_RGB;
	targetinfoheader.biSizeImage=sourceinfoheader.biSizeImage/3;
	targetinfoheader.biXPelsPerMeter=sourceinfoheader.biXPelsPerMeter;
	targetinfoheader.biYPelsPerMeter=sourceinfoheader.biYPelsPerMeter;
	targetinfoheader.biClrImportant=0;
	targetinfoheader.biClrUsed=256;

	//构造灰度图的调色板

	RGBQUAD rgbquad[256];
	int i,j,k;
	for(i=0;i<256;i++)
	{
		rgbquad[i].rgbBlue=i;
		rgbquad[i].rgbGreen=i;
		rgbquad[i].rgbRed=i;
		rgbquad[i].rgbReserved=0;
	}
	targetfile=fopen(des,"wb");

	//写入灰度图的文件头,信息头和调色板信息
	fwrite((void*)&targetfileheader,1,sizeof(BITMAPFILEHEADER),targetfile);
	fwrite((void*)&targetinfoheader,1,sizeof(BITMAPINFOHEADER),targetfile);
	fwrite((void*)&rgbquad,1,sizeof(RGBQUAD)*256,targetfile);
	BYTE* sourcebuf;
	BYTE* targetbuf;

	//获取原图像的宽度、高度和颜色深度
	int bmpWidth=sourceinfoheader.biWidth;
	int bmpHeight=sourceinfoheader.biHeight;
	int bmpBitCount=sourceinfoheader.biBitCount;
	//定义变量，计算图像每行像素所占的字节数（必须是4的倍数）
	int lineByte=(bmpWidth*bmpBitCount/8+3)/4*4;
	//定义变量，计算灰度图像每行像素所占的字节数（必须是4的倍数）
	int grayLineByte=(bmpWidth*bmpBitCount/8/3+3)/4*4;

	sourcebuf=new BYTE[lineByte*bmpHeight];

	//读取原图的颜色矩阵信息
	fread(sourcebuf,1,lineByte*bmpHeight,sourcefile);
	fclose(sourcefile);
	targetbuf=new BYTE[grayLineByte*bmpHeight];
	BYTE color[3];   

	//通过原图颜色矩阵信息得到灰度图的矩阵信息
	for(i=0;i<bmpHeight;i++)
	{
		for(j=0;j<bmpWidth;j++)
		{  
			for(k=0; k<3; k++)   
				color[k]=sourcebuf[i*lineByte+j*3+k];

			targetbuf[(i*grayLineByte)+j]=(unsigned char)(color[0]*0.114+color[1]*0.587+color[2]*0.299);   
			if(targetbuf[(i*grayLineByte)+j]>255)
				targetbuf[(i*grayLineByte)+j]=255;
		}
	}  

	fwrite((void*)targetbuf,1,grayLineByte*bmpHeight,targetfile);
	fclose(targetfile);

	delete []sourcebuf;
	delete []targetbuf;
	return 1;
}

void RGBToGrey::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_RTG, m_path_source);
}


BEGIN_MESSAGE_MAP(RGBToGrey, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_RTG_OPEN, &RGBToGrey::OnBnClickedButtonRtgOpen)
	ON_BN_CLICKED(IDC_BUTTON_RTG_START, &RGBToGrey::OnBnClickedButtonRtgStart)
END_MESSAGE_MAP()


// RGBToGrey 消息处理程序


void RGBToGrey::OnBnClickedButtonRtgOpen()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog fileDialog(TRUE,_T("Bmp Files(*.bmp)|*.bmp"),
		NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T( "Bmp Files(*.bmp)|*.bmp||"));

	//下面调用了打开文件的对话框
	if(fileDialog.DoModal()==IDOK)
	{
		m_path_source=fileDialog.GetPathName();//得到路径
		this->UpdateData(FALSE);
	}
}


void RGBToGrey::OnBnClickedButtonRtgStart()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_path_source!="")
	{
		CString tempsave;
		CFileDialog fileDialog(FALSE,_T("Bmp Files(*.bmp)|*.bmp"),
			NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T( "Bmp Files(*.bmp)|*.bmp||"));

		if(fileDialog.DoModal()==IDOK)
		{
			tempsave=fileDialog.GetPathName();//得到路径
		}
		if (GtoR(m_path_source,tempsave)==1)
		{
			MessageBox(_T("转换成功"));
		}
		else
		{
			MessageBox(_T("转换失败"));
		}
	}
	else
	{
		MessageBox(_T("请先打开文件"));
	}
}


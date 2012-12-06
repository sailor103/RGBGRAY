// GreyToRGB.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PicConvert.h"
#include "GreyToRGB.h"
#include "afxdialogex.h"
#include "ImageCenterDib.h"
#include <math.h>
// GreyToRGB �Ի���

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

//���溯��
bool GreyToRGB::saveBmp24(CString bmpName, unsigned char *imgBuf, int width, int height, int biBitCount)
{
	//���λͼ����ָ��Ϊ0,��û�����ݴ���,��������
	if(!imgBuf)
		return 0;
	biBitCount = 24;

	//�Զ�����д�ķ�ʽ���ļ�
	FILE *fp=fopen(bmpName,"wb");
	if(fp==0) return 0;	

	//����λͼ�ļ�ͷ�ṹ��������д�ļ�ͷ��Ϣ
	BITMAPFILEHEADER fileHead;
	fileHead.bfType = 0x4D42;//bmp����

	//���洢ͼ������ÿ���ֽ���Ϊ4�ı���
	int lineByte=(width*biBitCount/8+3)/4*4;

	//bfSize��ͼ���ļ�4����ɲ���֮��
	fileHead.bfSize= sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + lineByte*height;
	fileHead.bfReserved1 = 0;
	fileHead.bfReserved2 = 0;	

	//bfOffBits��ͼ���ļ�ǰ������������ռ�֮��
	fileHead.bfOffBits=54;
	//д�ļ�ͷ���ļ�
	fwrite(&fileHead, sizeof(BITMAPFILEHEADER),1, fp);	
	//����λͼ��Ϣͷ�ṹ��������д��Ϣͷ��Ϣ
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
	//дλͼ��Ϣͷ���ڴ�
	fwrite(&head, sizeof(BITMAPINFOHEADER),1, fp);			
	fwrite(imgBuf, height*lineByte, 1, fp);	
	//�ر��ļ�
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


// GreyToRGB ��Ϣ�������



void GreyToRGB::OnBnClickedButtonGtrOpen()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFileDialog fileDialog(TRUE,_T("Bmp Files(*.bmp)|*.bmp"),
		NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T( "Bmp Files(*.bmp)|*.bmp||"));

	//��������˴��ļ��ĶԻ���
	if(fileDialog.DoModal()==IDOK)
	{
		m_source_path=fileDialog.GetPathName();//�õ�·��
		this->UpdateData(FALSE);
	}
}


void GreyToRGB::OnBnClickedButtonGtrStart()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_source_path!="")
	{
		CString tempsave;
		CFileDialog fileDialog(FALSE,_T("Bmp Files(*.bmp)|*.bmp"),
			NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T( "Bmp Files(*.bmp)|*.bmp||"));

		if(fileDialog.DoModal()==IDOK)
		{
			tempsave=fileDialog.GetPathName();//�õ�·��
		}
		//start------
		ImgCenterDib m_dib;
		m_dib.Read(m_source_path);//��ȡ�ļ�
		//��ȡImgCenterDib������ָ�룬���ʴ��ļ�������
		ImgCenterDib *pDib=&m_dib;
		//λͼ���ݵ�ָ��
		unsigned char *imgData=pDib->m_pImgData;

		//λͼ���еĴ�С
		CSize imgSize=pDib->GetDimensions();

		//ÿ���ص�λ��
		int nBitCount=pDib->m_nBitCount;
		double degree;
		//ѭ��������ͼ������
		int i,j;

		//ÿ����ռ�ֽ���
		int bytePerPixel=nBitCount/8;

		//ÿ��������ռ�ֽ�����������4�ı���
		int lineByte=(imgSize.cx*nBitCount/8+3)/4*4;
		int lineByte24=(imgSize.cx*3*nBitCount/8+3)/4*4;
		if(nBitCount==8)
		{
			for(i=0;i<=255;i++)
			{
				//��1-255��ͼ����Ϣ���PI���������Һ����Һ����ļ���
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

		//�õ�24λ������
		for(i=0;i<imgSize.cy;i++)
		{
			for(j=0;j<imgSize.cx;j++)
			{
				*(pBmpBuf24+i*lineByte24+j*3+0)=ColorTranTable[B][*(imgData+i*lineByte+j)];//blue
				*(pBmpBuf24+i*lineByte24+j*3+1)=ColorTranTable[G][*(imgData+i*lineByte+j)];//green
				*(pBmpBuf24+i*lineByte24+j*3+2)=ColorTranTable[R][*(imgData+i*lineByte+j)];//red
			}
		}

		//����α��ɫ����
		saveBmp24(tempsave, pBmpBuf24, imgSize.cx, imgSize.cy, 24);
		MessageBox(_T("ת���ɹ�"));
		//end--------
	}
	else
	{
		MessageBox(_T("���ȴ��ļ�"));
	}
}

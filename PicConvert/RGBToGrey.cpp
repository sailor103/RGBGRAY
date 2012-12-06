// RGBToGrey.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PicConvert.h"
#include "RGBToGrey.h"
#include "afxdialogex.h"


// RGBToGrey �Ի���

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
	//�Ҷ�ת������
	FILE *sourcefile,*targetfile;


	//λͼ�ļ�ͷ����Ϣͷ
	BITMAPFILEHEADER sourcefileheader,targetfileheader;
	BITMAPINFOHEADER sourceinfoheader,targetinfoheader;
	memset(&targetfileheader,0,sizeof(BITMAPFILEHEADER));
	memset(&targetinfoheader,0,sizeof(BITMAPINFOHEADER));

	sourcefile=fopen(source,"rb"); //�Զ�������ʽ���ļ�
	fread((void*)&sourcefileheader,1,sizeof(BITMAPFILEHEADER),sourcefile);//��ȡԭͼ�ļ�ͷ
	if(sourcefileheader.bfType!=0x4d42)
	{ 
		fclose(sourcefile);
		MessageBox(_T("ԭͼ��ΪBMPͼ��"));
		return -1;
	}
	fread((void*)&sourceinfoheader,1,sizeof(BITMAPINFOHEADER),sourcefile);//��ȡ�ļ���Ϣͷ
	if(sourceinfoheader.biBitCount!=24)
	{
		fclose(sourcefile);
		MessageBox(_T("ԭͼ��Ϊ24λ���ɫ��"));
		return -1;
	}
	if(sourceinfoheader.biCompression!=BI_RGB)
	{
		fclose(sourcefile);
		MessageBox(_T("ԭͼ��Ϊѹ�����ͼ�󣬱����򲻴���ѹ������ͼ��"));
		return -1;
	}

	//����Ҷ�ͼ���ļ�ͷ
	targetfileheader.bfOffBits=54+sizeof(RGBQUAD)*256;
	targetfileheader.bfSize=targetfileheader.bfOffBits+sourceinfoheader.biSizeImage/3;
	targetfileheader.bfReserved1=0;
	targetfileheader.bfReserved2=0;
	targetfileheader.bfType=0x4d42;

	//����Ҷ�ͼ����Ϣͷ
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

	//����Ҷ�ͼ�ĵ�ɫ��

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

	//д��Ҷ�ͼ���ļ�ͷ,��Ϣͷ�͵�ɫ����Ϣ
	fwrite((void*)&targetfileheader,1,sizeof(BITMAPFILEHEADER),targetfile);
	fwrite((void*)&targetinfoheader,1,sizeof(BITMAPINFOHEADER),targetfile);
	fwrite((void*)&rgbquad,1,sizeof(RGBQUAD)*256,targetfile);
	BYTE* sourcebuf;
	BYTE* targetbuf;

	//��ȡԭͼ��Ŀ�ȡ��߶Ⱥ���ɫ���
	int bmpWidth=sourceinfoheader.biWidth;
	int bmpHeight=sourceinfoheader.biHeight;
	int bmpBitCount=sourceinfoheader.biBitCount;
	//�������������ͼ��ÿ��������ռ���ֽ�����������4�ı�����
	int lineByte=(bmpWidth*bmpBitCount/8+3)/4*4;
	//�������������Ҷ�ͼ��ÿ��������ռ���ֽ�����������4�ı�����
	int grayLineByte=(bmpWidth*bmpBitCount/8/3+3)/4*4;

	sourcebuf=new BYTE[lineByte*bmpHeight];

	//��ȡԭͼ����ɫ������Ϣ
	fread(sourcebuf,1,lineByte*bmpHeight,sourcefile);
	fclose(sourcefile);
	targetbuf=new BYTE[grayLineByte*bmpHeight];
	BYTE color[3];   

	//ͨ��ԭͼ��ɫ������Ϣ�õ��Ҷ�ͼ�ľ�����Ϣ
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


// RGBToGrey ��Ϣ�������


void RGBToGrey::OnBnClickedButtonRtgOpen()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFileDialog fileDialog(TRUE,_T("Bmp Files(*.bmp)|*.bmp"),
		NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T( "Bmp Files(*.bmp)|*.bmp||"));

	//��������˴��ļ��ĶԻ���
	if(fileDialog.DoModal()==IDOK)
	{
		m_path_source=fileDialog.GetPathName();//�õ�·��
		this->UpdateData(FALSE);
	}
}


void RGBToGrey::OnBnClickedButtonRtgStart()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	if (m_path_source!="")
	{
		CString tempsave;
		CFileDialog fileDialog(FALSE,_T("Bmp Files(*.bmp)|*.bmp"),
			NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T( "Bmp Files(*.bmp)|*.bmp||"));

		if(fileDialog.DoModal()==IDOK)
		{
			tempsave=fileDialog.GetPathName();//�õ�·��
		}
		if (GtoR(m_path_source,tempsave)==1)
		{
			MessageBox(_T("ת���ɹ�"));
		}
		else
		{
			MessageBox(_T("ת��ʧ��"));
		}
	}
	else
	{
		MessageBox(_T("���ȴ��ļ�"));
	}
}


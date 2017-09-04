#include "StdAfx.h"
#include "ToAvi.h"
#include "Vfw.h"

#pragma comment(lib,"Vfw32.lib")


void ImageToAVI( LPCTSTR pAviName, LPCTSTR pDir, int videoWidth, int videoHeight, int bpp );
bool AviToImage( LPCTSTR pAviName, LPCTSTR pDir);

ToAvi::ToAvi(void)
{
}

ToAvi::~ToAvi(void)
{
}

//======================================================================================
//
// ���� : ��Ŀ¼�е�ͼ������ó�AVI��Ƶ
// ���� : videoWidth��videoHeight�����������Ƶ�Ŀ�Ⱥ͸߶�
//        pAviName �����������Ƶ���ļ���
//		  pDir������ͼ���ļ���·��. ���ļ����µ�ͼ���ļ��ĳߴ���Ҫһ��
//		  bpp ����ͼ����ÿ����λ��
// ���� : ����
//======================================================================================
void ImageToAVI( LPCTSTR pAviName, LPCTSTR pDir, int videoWidth, int videoHeight, int bpp )
{
	if ( NULL == pAviName || NULL == pDir )
		return;

	// ��ʼ��AVI ��
	AVIFileInit();

	//
	// Create AVI file
	//
	PAVIFILE pAviFile = NULL;
	HRESULT hr = AVIFileOpen( &pAviFile, pAviName, OF_WRITE | OF_CREATE, NULL );
	if ( 0 != hr )
	{
		ASSERT( FALSE );
		return;
	}

	//
	// Create AVI video stream
	//
	AVISTREAMINFO strhdr;
	memset( &strhdr, 0, sizeof( strhdr ) );

	strhdr.fccType = streamtypeVIDEO;
	strhdr.fccHandler = 0;
	strhdr.dwScale = 1;
	strhdr.dwRate = 10;

	// Calc image size
	UINT pitch = ( videoWidth * bpp + 31 ) / 32 * 4;
	UINT biSizeImage = ( videoWidth * bpp + 31 ) / 32 * 4 * videoHeight;
	strhdr.dwSuggestedBufferSize = biSizeImage;

	SetRect( &strhdr.rcFrame, 0, 0, videoWidth, videoHeight );

	// And create the stream;
	PAVISTREAM pAviStream = NULL;
	hr = AVIFileCreateStream( pAviFile, &pAviStream, &strhdr );
	//PAVISTREAM ps;//��Ҫѹ���洢�Ļ���ѹ����ָ��
	///////////////�������ѹ�����������漸��//����һ��ѹ�������ýṹ��
	//AVICOMPRESSOPTIONS avico;//������ȥ��ֻΪ��������
	//AVICOMPRESSOPTIONS FAR * avicof[1];
	//avicof[0] = &avico;
	//memset(&avico,0,sizeof(avico));
	////����ᵯ��һ��ѹ����ʽѡ��Ի���һ��ѡ���һ��
	//AVISaveOptions(NULL,0,1,&ps,(LPAVICOMPRESSOPTIONS FAR *) &avicof);//����ѹ�����ṹ����Ϣ
	//AVIMakeCompressedStream(&pAviStream,ps,&avico,NULL); DWORD nFrame(0);
	if ( 0 != hr )
	{
		ASSERT( FALSE );

		AVIFileRelease( pAviFile );
		pAviFile = NULL;

		return;
	}

	//
	// Set stream format
	//
	BITMAPINFOHEADER bih;
	memset( &bih, 0, sizeof( BITMAPINFOHEADER ) );

	bih.biBitCount = bpp;
	bih.biClrImportant = 0;
	bih.biClrUsed = 0;
	bih.biCompression = BI_RGB;
	bih.biPlanes = 1;
	bih.biSize = 40;
	bih.biXPelsPerMeter = 0;
	bih.biYPelsPerMeter = 0;
	bih.biWidth = videoWidth;
	bih.biHeight = videoHeight;
	bih.biSizeImage = biSizeImage;

	hr = AVIStreamSetFormat( pAviStream, 0, &bih, sizeof( bih ) );
	if ( 0 != hr )
	{
		ASSERT( FALSE );

		AVIStreamClose( pAviStream );
		pAviStream = NULL;

		AVIFileRelease( pAviFile );
		pAviFile = NULL;
	}

	//
	// ����ÿһ��ͼ����������Ƶ�е�һ֡֡ͼ��
	//
	//TCHAR imageDir[ MAX_PATH ];
	//memset( imageDir, 0, sizeof( imageDir ) );
	//sprintf( imageDir, "%s\\*.*", pDir );
	CString imageDir;
	imageDir=(CString)pDir+_T("\\*.*");

	CFileFind finder;
	BOOL bFind = finder.FindFile( imageDir );
	int nFrames =0;

	// ͼ�����ݻ�����
	BYTE * pData = new BYTE[ biSizeImage ];
	if ( pData )
	{
		memset( pData, 0, biSizeImage );
	}

	while ( bFind )
	{
		bFind = finder.FindNextFile();

		if ( !finder.IsDots() && !finder.IsDirectory() )
		{
			// ��ȡͼ���ļ�����·��
			CString str = finder.GetFilePath();

			//
			// Open Image and get image info & data
			//
			CImage img;
			if ( FAILED( img.Load( str ) ) )
			{
				TRACE( "Warning : fail to load file %s!!!\n", str );
				continue;
			}

			//
			// ��ȡͼ�����ݣ�����AVI��Ƶ֡
			//
			if ( pData )
			{
				int w = min( videoWidth, img.GetWidth() );
				int h = min( videoHeight, img.GetHeight() );

				for ( int i =0; i < h; i++ )
					for ( int j = 0; j <w; j++ )
					{
						COLORREF clr = img.GetPixel( j, h-i-1 );

						pData[ i * pitch + j * ( bpp / 8 ) + 0 ] = GetBValue( clr );
						pData[ i * pitch + j * ( bpp / 8 ) + 1 ] = GetGValue( clr );
						pData[ i * pitch + j * ( bpp / 8 ) + 2 ] = GetRValue( clr );
					}

					hr = AVIStreamWrite( pAviStream, nFrames, 1, pData, biSizeImage, AVIIF_KEYFRAME, NULL, NULL );
					ASSERT( 0 == hr );
			}

			nFrames ++;
		}
	}

	if ( pData )
	{
		delete [] pData;
		pData = NULL;
	}

	if ( pAviStream )
	{
		AVIStreamClose( pAviStream );
		pAviStream = NULL;
	}

	if ( pAviFile )
	{
		AVIFileRelease( pAviFile );
		pAviFile = NULL;
	}

	AVIFileExit();
}
//======================================================================================
// ���� :	AVI������ʽ�ļ�ת��Ϊһϵ��BMP�ļ�
// ���� :	pDir ����bmp�ļ�Ŀ¼
//			pAviName ����AVI��Ƶ������·���ļ���
// ���� :	sejarce
//======================================================================================
bool AviToImage( LPCTSTR pAviName, LPCTSTR pDir)
{
	return true;
}

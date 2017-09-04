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
// 功能 : 将目录中的图像件制用成AVI视频
// 参数 : videoWidth、videoHeight代表输出的视频的宽度和高度
//        pAviName 代表输出的视频的文件名
//		  pDir代表存放图像文件的路径. 此文件夹下的图像文件的尺寸需要一致
//		  bpp 代表图像中每像素位数
// 作者 : 敖建
//======================================================================================
void ImageToAVI( LPCTSTR pAviName, LPCTSTR pDir, int videoWidth, int videoHeight, int bpp )
{
	if ( NULL == pAviName || NULL == pDir )
		return;

	// 初始化AVI 库
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
	//PAVISTREAM ps;//需要压缩存储的话，压缩流指针
	///////////////如果进行压缩，则做下面几步//声明一个压缩流配置结构体
	//AVICOMPRESSOPTIONS avico;//导来导去，只为传参数用
	//AVICOMPRESSOPTIONS FAR * avicof[1];
	//avicof[0] = &avico;
	//memset(&avico,0,sizeof(avico));
	////这里会弹出一个压缩格式选择对话框，一般选择第一个
	//AVISaveOptions(NULL,0,1,&ps,(LPAVICOMPRESSOPTIONS FAR *) &avicof);//配置压缩流结构体信息
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
	// 遍历每一个图像，制作成视频中的一帧帧图像
	//
	//TCHAR imageDir[ MAX_PATH ];
	//memset( imageDir, 0, sizeof( imageDir ) );
	//sprintf( imageDir, "%s\\*.*", pDir );
	CString imageDir;
	imageDir=(CString)pDir+_T("\\*.*");

	CFileFind finder;
	BOOL bFind = finder.FindFile( imageDir );
	int nFrames =0;

	// 图像数据缓冲区
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
			// 获取图像文件绝对路径
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
			// 读取图像数据，更新AVI视频帧
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
// 功能 :	AVI视屏格式文件转化为一系列BMP文件
// 参数 :	pDir 代表bmp文件目录
//			pAviName 代表AVI视频的完整路径文件名
// 作者 :	sejarce
//======================================================================================
bool AviToImage( LPCTSTR pAviName, LPCTSTR pDir)
{
	return true;
}

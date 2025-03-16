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
		//printf( "[!] CreateFileA Failed With Error: %d \n", GetLastError() );
		goto _END_OF_FUNC;
	}

	if ( !WriteFile( hFile, pFileBuffer, dwFileSize, &dwNumberOfBytesWritten, NULL ) || dwFileSize !=
		dwNumberOfBytesWritten )
	{
		//printf( "[!] WriteFile Failed With Error: %d \n[i] Wrote %d Of %d Bytes \n", GetLastError(),
		//dwNumberOfBytesWritten, dwFileSize );
	}

_END_OF_FUNC:
	if ( hFile != INVALID_HANDLE_VALUE )
		CloseHandle( hFile );
	return ( dwNumberOfBytesWritten == dwFileSize ) ? TRUE : FALSE;
}

PVOID CopyMemoryEx( _Inout_ PVOID Destination, _In_ CONST PVOID Source, _In_ SIZE_T Length )
{
	PBYTE D = Destination;
	PBYTE S = Source;

	while ( Length-- )
		*D++ = *S++;

	return Destination;
}

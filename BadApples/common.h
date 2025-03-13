#pragma once

/* ---------------------------------------------------------------------------------------------------------------------------------------- */

#define U_PTR( x ) (UINT_PTR) x
#define C_PTR( x ) (PVOID) x

BOOL ScreenshotBmp(
	_Out_ PVOID* Buffer,
	_Out_ PULONG Length
);

BOOL WriteFileToDiskA( IN LPCSTR cFileName, IN PBYTE pFileBuffer, OUT DWORD dwFileSize );

/* ---------------------------------------------------------------------------------------------------------------------------------------- */

#define REG_ERR( cApiName, ... ) printf("(-) %s FAILED: %lx\n", cApiName)

#define LOG_ERR( cApiName, ... ) printf("(-) %s FAILED: %d\n", cApiName, GetLastError())

/* ---------------------------------------------------------------------------------------------------------------------------------------- */

BOOL ActivatePersistance();

BOOL ReadLocalFile( OUT PVOID* pFileBuff, OUT DWORD* pdwSize );

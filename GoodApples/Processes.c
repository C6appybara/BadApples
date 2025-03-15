#include <windows.h>
#include <stdio.h>
#include "../BadApples/common.h"
#include "typedef.h"

BOOL TerminateRunningProcess( IN DWORD dwPid )
{
	HANDLE hProcess = INVALID_HANDLE_VALUE;

	/* open a handle to the payload process */
	hProcess = OpenProcess( PROCESS_ALL_ACCESS, FALSE, dwPid );
	if ( hProcess == INVALID_HANDLE_VALUE )
	{
		LOG_ERR( "OpenProcess" );
		return FALSE;
	}

	/* terminate the payload process */
	if ( !TerminateProcess( hProcess, 0 ) )
	{
		LOG_ERR( "TerminateProcess" );
		return FALSE;
	}
	printf( "(+) Terminated Process...\n" );

	/* close the handle */
	CloseHandle( hProcess );

	return TRUE;
}

BOOL CheckProcesses( IN LPCWSTR lpProcessName, OUT DWORD* dwPid )
{
	PSYSTEM_PROCESS_INFORMATION SysInfo = {0};
	ULONG uReturnLen1 = 0x00,
	      uReturnLen2 = 0x00;
	PVOID pValueToFree = NULL;
	NTSTATUS status = 0x00;
	HANDLE hProcess = NULL;

	/* get address of NtQuerySystemInformation */
	fnNtQuerySystemInformation fNtQuerySystemInformation = ( fnNtQuerySystemInformation )GetProcAddress(
		GetModuleHandle( TEXT( "ntdll.dll" ) ), "NtQuerySystemInformation" );
	if ( fNtQuerySystemInformation == NULL )
	{
		LOG_ERR( "GetProcAddress" );
		return FALSE;
	}
	printf( "(+) \"NtQuerySystemInformation\" Found @ 0x%p\n", fNtQuerySystemInformation );

	/* run NtQuerySystemInformation to get the sizeof to allocate for SysInfo. */
	fNtQuerySystemInformation( SystemProcessInformation, NULL, NULL, &uReturnLen1 );

	/* allocate enough space for the 2nd NtQuerySystemInformation call */
	if ( !( SysInfo = ( PSYSTEM_PROCESS_INFORMATION )HeapAlloc( GetProcessHeap(), HEAP_ZERO_MEMORY, uReturnLen1 ) ) )
	{
		LOG_ERR( "HeapAlloc" );
		return FALSE;
	}

	pValueToFree = SysInfo;

	/* fill SysInfo with the correct information */
	status = fNtQuerySystemInformation( SystemProcessInformation, SysInfo, uReturnLen1, &uReturnLen2 );
	if ( !NT_SUCCESS( status ) )
	{
		printf( "(-) NtQuerySystemInformation FAILED: %lx\n", status );
		return FALSE;
	}

	/* skip 'null' entry in the list */
	SysInfo = ( PSYSTEM_PROCESS_INFORMATION )( ( PBYTE )SysInfo + SysInfo->NextEntryOffset );

	/* loop through the list until we find the process */
	while ( TRUE )
	{
		if ( SysInfo->ImageName.Length && wcscmp( SysInfo->ImageName.Buffer, lpProcessName ) == 0 )
		{
			printf( "(i) Found Process...\n" );
			*dwPid = HandleToULong( SysInfo->UniqueProcessId );
			break;
		}

		if ( !SysInfo->NextEntryOffset )
			break;

		/* go to next entry */
		SysInfo = ( PSYSTEM_PROCESS_INFORMATION )( ( PBYTE )SysInfo + SysInfo->NextEntryOffset );
	}

	return TRUE;
}

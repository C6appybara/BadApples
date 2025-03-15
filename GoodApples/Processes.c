#include <windows.h>
#include <stdio.h>
#include "../BadApples/common.h"
#include "typedef.h"

BOOL TerminateRunningProcess( IN DWORD dwPid )
{
	HANDLE hProcess = INVALID_HANDLE_VALUE;

	hProcess = OpenProcess( PROCESS_ALL_ACCESS, FALSE, dwPid );
	if ( hProcess == INVALID_HANDLE_VALUE )
	{
		LOG_ERR( "OpenProcess" );
		return FALSE;
	}

	if ( !TerminateProcess( hProcess, 0 ) )
	{
		LOG_ERR( "TerminateProcess" );
		return FALSE;
	}
	printf( "(+) Terminated Process...\n" );

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

	fnNtQuerySystemInformation fNtQuerySystemInformation = ( fnNtQuerySystemInformation )GetProcAddress(
		GetModuleHandle( TEXT( "ntdll.dll" ) ), "NtQuerySystemInformation" );
	if ( fNtQuerySystemInformation == NULL )
	{
		LOG_ERR( "GetProcAddress" );
		return FALSE;
	}
	printf( "(+) \"NtQuerySystemInformation\" Found @ 0x%p\n", fNtQuerySystemInformation );

	fNtQuerySystemInformation( SystemProcessInformation, NULL, NULL, &uReturnLen1 );

	if ( !( SysInfo = ( PSYSTEM_PROCESS_INFORMATION )HeapAlloc( GetProcessHeap(), HEAP_ZERO_MEMORY, uReturnLen1 ) ) )
	{
		LOG_ERR( "HeapAlloc" );
		return FALSE;
	}

	pValueToFree = SysInfo;

	status = fNtQuerySystemInformation( SystemProcessInformation, SysInfo, uReturnLen1, &uReturnLen2 );
	if ( !NT_SUCCESS( status ) )
	{
		printf( "(-) NtQuerySystemInformation FAILED: %lx\n", status );
		return FALSE;
	}

	SysInfo = ( PSYSTEM_PROCESS_INFORMATION )( ( PBYTE )SysInfo + SysInfo->NextEntryOffset );

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

		SysInfo = ( PSYSTEM_PROCESS_INFORMATION )( ( PBYTE )SysInfo + SysInfo->NextEntryOffset );
	}

	return TRUE;
}

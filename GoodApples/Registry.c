#include <windows.h>
#include <stdio.h>
#include "../BadApples/common.h"

BOOL DeletePersistanceReg()
{
	HKEY hKey = NULL;
	LRESULT lResult = 0x00;

	if ( ( lResult = RegOpenKeyW( HKEY_CURRENT_USER, REGISTRY_SUBKEY, &hKey ) ) != ERROR_SUCCESS )
	{
		REG_ERR( "RegOpenKeyW", lResult );
		return FALSE;
	}


	lResult = RegDeleteValueW( hKey, L"load" );
	if ( lResult != ERROR_SUCCESS )
	{
		REG_ERR( "RegDeleteValueW", lResult );
		RegCloseKey( hKey );
		return FALSE;
	}
	printf( "(+) Deleted The Persistance Registry Key...\n" );

	RegCloseKey( hKey );

	return TRUE;
}

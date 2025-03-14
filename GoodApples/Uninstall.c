#include <Windows.h>
#include <stdio.h>
#include <Shlwapi.h>
#include "..\BadApples\common.h"

#pragma comment(lib, "Shlwapi.lib")

///
///	Removing Payload File From Disk...
///
BOOL RemovePayloadFromDisk() {
	 
	LPCWSTR lpTempPath[MAX_PATH];
	BOOL bExists = FALSE;

	// creating full payload path
	if (!GetTempPathW(MAX_PATH, lpTempPath)) {
		LOG_ERR("GetTempPathW");
		return FALSE;
	}

	StrCatW(lpTempPath, PAYLOAD_NAME);

	/* checking if the file exists */
	bExists = PathFileExistsW(lpTempPath);
	if (bExists == FALSE) {
		LOG_ERR("PathFileExistsW");
		return FALSE;
	}
	
	wprintf(L"(+) Found Payload File @ %ls\n", lpTempPath);

	if (!DeleteFileW(lpTempPath)) {
		LOG_ERR("DeleteFileW");
		return FALSE;
	}
	printf("(+) Payload Has Been Removed...\n");
	


	return TRUE;
}

BOOL DeletePersistanceReg() {

	HKEY hKey = NULL;
	LRESULT lResult = 0x00;

	if ((lResult = RegOpenKeyW(HKEY_CURRENT_USER, REGISTRY_SUBKEY, &hKey)) != ERROR_SUCCESS) {
		REG_ERR("RegOpenKeyW", lResult);
		return FALSE;
	}
	wprintf(L"($) Registry key before StrCatW: %ls\n", REGISTRY_SUBKEY);

	lResult = RegDeleteValueW(hKey, L"load");
	if (lResult != ERROR_SUCCESS) {
		REG_ERR("RegDeleteValueW", lResult);
		RegCloseKey(hKey);
		return FALSE;
	}
	printf("(+) Deleted The Persistance Registry Key...\n");

	RegCloseKey(hKey);

	return TRUE;
}

int main() {

	RemovePayloadFromDisk();

	DeletePersistanceReg();


	return 0;
}
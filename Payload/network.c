#include <windows.h>
#include <wininet.h>
#include <stdio.h>
#include "../BadApples/common.h"

#pragma comment(lib, "wininet.lib")

BOOL SendOverHttp( IN PVOID pAddress, IN DWORD size )
{
	HINTERNET hInternet = NULL;
	HINTERNET hConnect = NULL;
	HINTERNET hRequest = NULL;

	hInternet = InternetOpen(
		L"WinINet Example",
		INTERNET_OPEN_TYPE_DIRECT,
		NULL,
		NULL,
		0
	);
	if ( !hInternet )
	{
		return FALSE;
	}

	/* connect to the server */
	hConnect = InternetConnect(
		hInternet,
		L"localhost",
		8080,
		NULL,
		NULL,
		INTERNET_SERVICE_HTTP,
		0,
		0
	);
	if ( !hConnect )
	{
		InternetCloseHandle( hInternet );
		return FALSE;
	}

	/* create an HTTP request */
	hRequest = HttpOpenRequest(
		hConnect,
		L"POST",
		L"/api/endpoint",
		NULL,
		NULL,
		NULL,
		INTERNET_FLAG_RELOAD,
		0
	);
	if ( !hRequest )
	{
		InternetCloseHandle( hConnect );
		InternetCloseHandle( hInternet );
		return FALSE;
	}

	BOOL result = HttpSendRequest(
		hRequest,
		NULL,
		0,
		pAddress,
		size );

	// ----------------------------------------------------------------------------------------------------------------------------------------------

	// Clean up
	if ( hRequest ) InternetCloseHandle( hRequest );
	if ( hConnect ) InternetCloseHandle( hConnect );
	if ( hInternet ) InternetCloseHandle( hInternet );

	return TRUE;
}

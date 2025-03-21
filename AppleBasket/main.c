#include <Windows.h>
#include <stdio.h>
#include <wininet.h>
#include "../BadApples/common.h"

#pragma comment(lib, "wininet.lib")

int main(void) {

	HINTERNET hInternet = NULL,
		hConnect = NULL,
		hRequest = NULL;

	hInternet = InternetOpen(L"BadApples", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
	if (hInternet == NULL) {
		printf("(-) InternetOpen FAILED: %d\n", GetLastError());
		goto RETURN;
	}
	
	hConnect = InternetConnect(hInternet, REMOTE_IP, CONNECTION_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
	if (hConnect == NULL) {
		printf("(-) InternetConnect FAILED: %d\n", GetLastError());
		goto RETURN;
	}
	
	hRequest = HttpOpenRequestW(hConnect, L"GET", L"/api/endpoint", NULL, NULL, NULL, INTERNET_FLAG_RELOAD, 0);
	if (hRequest == NULL) {
		printf("(-) HttpOpenRequestW FAILED: %d\n", GetLastError());
		goto RETURN;
	}

RETURN:
	if (hInternet != NULL && hConnect != NULL) {
		InternetCloseHandle(hInternet);
		InternetCloseHandle(hConnect);
		InternetCloseHandle(hRequest);
	}
	

	return -1;
}
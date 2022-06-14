#include <Windows.h>
#include <iostream>
#include <WinInet.h>

namespace Hooks
{
	extern int m_iStage;

	BOOL __stdcall hkDllMain( HMODULE hModule, DWORD ulReason, LPVOID lpReserved );

	HANDLE __stdcall hkCreateSnapshot( DWORD ulFlags, DWORD ulProcessID );
	HANDLE __stdcall hkCreateThread( void* pSecurity, DWORD ulStackSize, void* pThreadStart, void* pReserved, DWORD ulFlags, LPDWORD pTID );
	PVOID __stdcall hkAddVectoredExceptionHandler( DWORD, void* );

	using InternetOpenA_t = HINTERNET ( __stdcall* ) ( LPCSTR, DWORD, LPCSTR, LPCSTR, DWORD );
	using InternetConnectA_t = HINTERNET ( __stdcall* ) ( HINTERNET, LPCSTR, INTERNET_PORT, LPCSTR, LPCSTR, DWORD, DWORD, DWORD_PTR );
	using HttpOpenRequestA_t = HINTERNET ( __stdcall* ) ( HINTERNET, LPCSTR, LPCSTR, LPCSTR, LPCSTR, LPCSTR*, DWORD, DWORD_PTR );
	using HttpSendRequestA_t = BOOL ( __stdcall* ) ( HINTERNET, LPCSTR, DWORD, LPVOID, DWORD );
	using InternetReadFile_t = BOOL ( __stdcall* ) ( HINTERNET, LPVOID, DWORD, LPDWORD );
	using InternetCloseHandle_t = BOOL ( __stdcall* ) ( HINTERNET );

	inline InternetOpenA_t o_InternetOpenA;
	inline InternetConnectA_t o_InternetConnectA;
	inline HttpOpenRequestA_t o_HttpOpenRequestA;
	inline HttpSendRequestA_t o_HttpSendRequestA;
	inline InternetReadFile_t o_InternetReadFile;
	inline InternetCloseHandle_t o_InternetCloseHandle;

	HINTERNET __stdcall hkInternetOpenA( LPCSTR lpszAgent, DWORD dwAccessType, LPCSTR lpszProxy, LPCSTR lpszProxyBypass, DWORD dwFlags );
	HINTERNET __stdcall hkInternetConnectA( HINTERNET hInternet, LPCSTR lpszServerName, INTERNET_PORT nServerPort, LPCSTR lpszUserName,
		LPCSTR lpszPassword, DWORD dwService, DWORD dwFlags, DWORD_PTR dwContext );
	HINTERNET __stdcall hkHttpOpenRequestA( HINTERNET hInternet, LPCSTR lpszVerb, LPCSTR lpszObjectName, LPCSTR lpszVersion,
		LPCSTR lpszReferrer, LPCSTR* lplpszAcceptTypes, DWORD dwFlags, DWORD_PTR dwContext );
	BOOL __stdcall hkHttpSendRequestA( HINTERNET hRequest, LPCSTR lpszHeaders, DWORD dwHeadersLength, LPVOID lpOptional, DWORD dwOptionalLength );
	BOOL __stdcall hkInternetReadFile( HINTERNET hFile, LPVOID lpBuffer, DWORD dwNumberOfBytesToRead, LPDWORD lpdwNumberOfBytesRead );
	BOOL __stdcall hkInternetCloseHandle( HINTERNET hInternet );
}
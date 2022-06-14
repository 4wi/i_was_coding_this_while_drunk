#include "../Hooks.hpp"

BOOL __stdcall Hooks::Http::hkHttpSendRequestA( HINTERNET hRequest, LPCSTR lpszHeaders, DWORD dwHeadersLength, LPVOID lpOptional, DWORD dwOptionalLength )
{
	printf( "attempted to send request lol\n" );
	return 1;
}
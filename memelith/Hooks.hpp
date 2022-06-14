#include <Windows.h>
#include <iostream>
#include <wininet.h>
#include <vector>

#include "Helpers/Json.hpp"

inline int g_stage = 0;

namespace Hooks
{
	namespace Cheat
	{
		inline int ( __thiscall* o_GetNetVar ) ( void*, uint32_t, uint32_t );

		BOOL __stdcall hkDllMain( HMODULE hModule, DWORD ulReason, LPVOID lpReserved );

		int __cdecl hkRand( );
		int __fastcall hkGetNetVar( void* thisptr, int, uint32_t iClassHash, uint32_t iVarHash );
		void __fastcall hkInitFonts( void* thisptr, int, int pData );

		void __cdecl hkRunInitializer( void* thisptr, void* pVtable );
		void __fastcall hkSendRequest( void* pRequestMgr, int, std::string pResponse, uint32_t prolly_size, bool );
	}

	namespace Http
	{
		BOOL __stdcall hkHttpSendRequestA( HINTERNET hRequest, LPCSTR lpszHeaders, DWORD dwHeadersLength, LPVOID lpOptional, DWORD dwOptionalLength );
	}
}
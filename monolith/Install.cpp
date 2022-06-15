#include <WinSock2.h>
#include <WS2tcpip.h>
#include <sstream>
#include <TlHelp32.h>
#include <DbgHelp.h>
#include <unordered_map>

#include "Install.hpp"
#include "Hooks.hpp"
#include "Json.hpp"
#include "Globals.hpp"

#include "Helpers/Utils.hpp"
#include "Binaries/Imports.hpp"
#include "MinHook/minhook.hpp"

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "dbghelp.lib")

void* o_findpattern = nullptr;
void* o_createhook = nullptr;

std::unordered_map< std::string, std::string > pattern_redirs = // changes outdated pattern to a new ones
{
	std::make_pair( "84 C0 75 04 B0 01 5F", "84 C0 75 05 B0 01 5F" ),
	std::make_pair( "55 8B EC 83 E4 F8 83 EC 78 56 57 8B 3D ? ? ? ? 89 4C 24 18", "55 8B EC 83 E4 F8 83 EC 78 56 89" ),
	std::make_pair( "55 8B EC 83 EC 18 56 8B 35 ? ? ? ? 57 8B 7D 08", "55 8B EC 83 EC 18 53 56 8B D9 8B 4D 04 57" ),
	std::make_pair( "55 8B EC 83 E4 F8 81 EC ? ? ? ? 53 56 57 8B 7D 08 89 4C 24 14", "55 8B EC 83 E4 F8 81 EC  0C 04 00 00 53 56 57 8B 7D 08 8B D9 89 5C 24 14" ),
	std::make_pair( "E8 ? ? ? ? 8B 45 F4 8B 55 F0 47", "E8 ? ? ? ? 8B 45 F4 8B 4D F0 47" ),
	std::make_pair( "56 8B 35 ? ? ? ? 57 85 F6 74 56", "55 8B EC 8B 4D 04 56 57 E8 ? ? ? ? 8B 35 ? ? ? ? 85 F6 74 57 8B" ),
	std::make_pair( "55 8B EC A1 ? ? ? ? 56 8B 75 08 83 FE FF", "55 8B EC 8B 4D 04 56 8B  C1 83 C0 08 8B 75 08 A1" ),
};

std::unordered_map< uint8_t*, std::pair< std::string, std::string > > address_records = { };

uint8_t* __fastcall hk_findpattern( uint32_t thisptr, int, std::string module_name, std::string pattern, bool is_relative, int rel_offset )
{
	using findpattern_t = uint8_t* ( __thiscall* ) ( uint32_t, std::string, std::string, bool, int );

	auto result = ( ( findpattern_t ) o_findpattern )( thisptr, module_name, pattern, is_relative, rel_offset );
	
	if ( pattern_redirs.find( pattern ) != pattern_redirs.end( ) )
		result = g_Utils.FindPattern( module_name, pattern_redirs.at( pattern ) );

	if ( !result )
		printf( "[failed] %s %s (%d 0x%X)\n", module_name.c_str( ), pattern.c_str( ), is_relative, rel_offset );
	
	if ( pattern_redirs.find( pattern ) != pattern_redirs.end( ) )
		pattern = pattern_redirs.at( pattern );

	address_records.emplace( std::make_pair( result, std::make_pair( module_name, pattern ) ) );

	return result;
}

uint8_t* __fastcall hk_createhook( uint32_t thisptr, int, uint32_t target, uint32_t detour )
{
	using createhook_t = uint8_t* ( __thiscall* ) ( uint32_t, uint32_t, uint32_t );

	auto result = ( ( createhook_t ) o_createhook )( thisptr, target, detour );

	std::pair< std::string, std::string > hook_data = { "unk", "unk" };
	if ( address_records.find( ( uint8_t* ) target ) != address_records.end( ) )
		hook_data = address_records.at( ( uint8_t* ) target );

	//printf( "[hook] 0x%X 0x%X -> 0x%X [%s >> %s] <0x%p>\n", target, detour, *( uint32_t* )( ( uint32_t ) _ReturnAddress( ) + 0x1 ), 
		//hook_data.first.c_str( ), hook_data.second.c_str( ), _ReturnAddress( ) );

	return result;
}

void CInstall::Init( )
{
	g_pGlobals = new CGlobals( );

	for ( const auto& CurrentImport : g_aImports )
	{
		HMODULE hModule = LoadLibraryA( std::get< 1 >( CurrentImport ).c_str( ) );
		if ( !hModule )
			continue;

		uint32_t pFunction = ( uint32_t ) GetProcAddress( hModule, std::get< 2 >( CurrentImport ).c_str( ) );
		if ( !pFunction )
			continue;

		*reinterpret_cast< uint32_t* >( std::get< 0 >( CurrentImport ) ) = pFunction;
	}

	std::vector< uint8_t > aOrigBytes =
	{
		0x55, 0x8B, 0xEC, 0x6A, 0xFF
	};

	memcpy( reinterpret_cast< void* >( 0x224633d0 ), aOrigBytes.data( ), aOrigBytes.size( ) );
	memcpy( reinterpret_cast< void* >( 0x225a8710 ), aOrigBytes.data( ), aOrigBytes.size( ) );

	SetupHooks( );
	Connect( );

	memset( reinterpret_cast< void* >( 0x228FE3B4 ), 0x00, 0x2B4 );
	*reinterpret_cast< uint32_t* >( 0x22857B46 ) = 0x228FE3BC; 

	memset( reinterpret_cast< void* >( 0x2247D6AF ), 0x90, 0x7 );
	memset( reinterpret_cast< void* >( 0x2272A5CD ), 0x90, 0x7 );
	memset( reinterpret_cast< void* >( 0x238821B8 ), 0x90, 0x2 );
	memset( reinterpret_cast< void* >( 0x225ECDDE ), 0x90, 0x2 );

	using DllEntry_t = BOOL ( __stdcall* ) ( void*, DWORD, void* );
	( reinterpret_cast< DllEntry_t >( 0x22857DB8 ) )( reinterpret_cast< void* >( 0x221B0000 ), 
		DLL_PROCESS_ATTACH, 0 );
}

void* orig_json_dump = nullptr;
void* orig_json_sstream = nullptr;

using json_stream_t = void ( __thiscall* ) ( void* string, const bool strict, nlohmann::json* pResult );

using json_dump_t = std::string* ( __thiscall* ) (
	void* pMeme,
	void* pResult,
	const int indent, 
	const char indent_char,
    const bool ensure_ascii,
    const nlohmann::json::error_handler_t error_handler );

int SendBinary(SOCKET s, char* buf, int* len)
{
	int total = 0;
	int bytesleft = *len;
	int n;

	while (total < *len)
	{
		n = send(s, buf + total, bytesleft, 0);
		if (n == -1) { break; }
		total += n;
		bytesleft -= n;
	}

	*len = total;

	return n == -1 ? -1 : 0;
}

std::string* __fastcall hooked_dump( nlohmann::json* pMeme, 
	int, 
	void* pResult,
	const int indent = -1,
    const char indent_char = ' ',
    const bool ensure_ascii = false,
    const nlohmann::json::error_handler_t error_handler = nlohmann::json::error_handler_t::strict )
{
	const std::lock_guard< std::mutex > LockGuard( g_pGlobals->m_mtxServer );
	auto result = ( ( json_dump_t ) orig_json_dump )( pMeme, pResult, indent, indent_char, ensure_ascii, error_handler );
	
	static bool is_inited = false;
	
	if ( is_inited )
		Hooks::m_iStage++;

	if ( !is_inited )
		is_inited = true;

	nlohmann::json jsonRequest;
	jsonRequest[ "type" ] = 0;

	if ( !( ( *pMeme )[ "serverName" ].is_null( ) ) )
	{
		jsonRequest.clear( );
		jsonRequest = *pMeme;
		jsonRequest[ "type" ] = 3;
	}
	else if ( ( !( ( *pMeme )[ "action" ].is_null( ) ) ) )
	{
		if ( ( ( *pMeme )[ "action" ].get< std::string >( ) ) == "Get" )
		{
			jsonRequest[ "type" ] = 1;
		}
		else if ( ( ( *pMeme )[ "action" ].get< std::string >( ) ) == "GetID" )
		{
			jsonRequest[ "type" ] = 2;
			jsonRequest[ "item_id" ] = ( *pMeme )[ "id" ].get< int >( );
		}
		else if ( ( ( *pMeme )[ "action" ].get< std::string >( ) ) == "Update" )
		{
			jsonRequest = *pMeme;
			jsonRequest[ "type" ] = 4;
		}
		else if ( ( ( *pMeme )[ "action" ].get< std::string >( ) ) == "Create" )
		{
			jsonRequest = *pMeme;
			jsonRequest[ "type" ] = 5;
		}
		else if ( ( ( *pMeme )[ "action" ].get< std::string >( ) ) == "Delete" )
		{
			jsonRequest[ "type" ] = 6;
			jsonRequest[ "item_id" ] = ( *pMeme )[ "id" ].get< int >( );
		}
		//else
			//printf( "unk memes %s\n", ( *pMeme ).dump( ).c_str( ) );
	}

	if ( jsonRequest[ "type" ].get< uint32_t >( ) == 0 )
		return result;

	// TODO: implement wintcp requests here!

	return result;
}

void __fastcall hooked_parse( void* string, int, const bool strict, nlohmann::json* pResult )
{
	const std::lock_guard< std::mutex > LockGuard( g_pGlobals->m_mtxServer );

	( ( json_stream_t ) orig_json_sstream )( string, strict, pResult );

	nlohmann::json jsonResult;

	static bool bParsedUser = false;
	if ( !bParsedUser )
	{
		jsonResult[ "success" ] = true;
		jsonResult[ "secondary_group_ids" ] = "5";
		jsonResult[ "user_id" ] = g_pGlobals->m_iUserID;
		jsonResult[ "username" ] = "gay";
		jsonResult[ "beta" ] = true;

		*pResult = jsonResult;

		bParsedUser = true;
		return;
	}

	if ( !g_pGlobals->m_aJsonResponses.size( ) )
		return;

	std::string sData;
	sData.resize( g_pGlobals->m_aJsonResponses.front( ).size( ) );
	memcpy( sData.data( ), g_pGlobals->m_aJsonResponses.front( ).data( ), g_pGlobals->m_aJsonResponses.front( ).size( ) );
	g_pGlobals->m_aJsonResponses.pop_front( );

	jsonResult.clear( );
	try
	{
		std::stringstream( sData ) >> jsonResult;
	}
	catch ( ... )
	{
		printf( "parse failed\n" );
		return;
	}

	//printf( "json resp %s\n", jsonResult.dump( ).c_str( ) );

	*pResult = jsonResult;
}

__forceinline void CInstall::Connect( )
{
	WSADATA WSAData;
	if ( WSAStartup( MAKEWORD( 2, 2 ), &WSAData ) )
		return;

	struct addrinfo* result = NULL;
	struct addrinfo* ptr = NULL;
	struct addrinfo hints = { 0 };

	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	if ( getaddrinfo( "127.0.0.1", "1488", &hints, &result ) )
		return;

	for ( ptr = result; ptr != NULL; ptr = ptr->ai_next )
	{
		g_pGlobals->m_iSocket = socket( ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol );
		if ( g_pGlobals->m_iSocket == INVALID_SOCKET )
		{
			WSACleanup( );
			return;
		}

		if ( connect( g_pGlobals->m_iSocket, ptr->ai_addr, ptr->ai_addrlen ) == SOCKET_ERROR )
		{
			printf( "failed to connect to server\n" );
			return;
		}
	}

	freeaddrinfo( result );

	if ( g_pGlobals->m_iSocket == INVALID_SOCKET )
	{
		printf( "connection failure" );
		return;
	}

	unsigned long lMode = 0;
	if ( ioctlsocket( g_pGlobals->m_iSocket, FIONBIO, &lMode ) == SOCKET_ERROR )
	{
		closesocket( g_pGlobals->m_iSocket );
		WSACleanup( );
		return;
	}

	char value = 1;
	setsockopt( g_pGlobals->m_iSocket, IPPROTO_TCP, TCP_NODELAY, &value, sizeof( value ) );

	printf( "successfully connected\n" );
}

__forceinline void CInstall::SetupHooks( )
{
	*reinterpret_cast< uint32_t* >( 0x228fe138 ) = ( uint32_t ) Hooks::hkCreateSnapshot;
	*reinterpret_cast< uint32_t* >( 0x228fe090 ) = ( uint32_t ) Hooks::hkCreateThread;
	*reinterpret_cast< uint32_t* >( 0x228fe0e8 ) = ( uint32_t ) Hooks::hkAddVectoredExceptionHandler;

	*reinterpret_cast< uint32_t* >( 0x2270DE31 ) = ( uint32_t ) Hooks::hkDllMain - 0x2270DE35;

	if ( MH_Initialize( ) != MH_OK )
		return;

	MH_CreateHook( reinterpret_cast< void* >( 0x224633d0 ), hooked_dump, &orig_json_dump );
	MH_CreateHook( reinterpret_cast< void* >( 0x225a8710 ), hooked_parse, &orig_json_sstream );
	MH_CreateHook( reinterpret_cast< void* >( 0x2280BED0 ), hk_findpattern, &o_findpattern );
	MH_CreateHook( reinterpret_cast< void* >( 0x226EBE80 ), hk_createhook, &o_createhook );

	if ( MH_CreateHookApi( ( L"Wininet.dll" ), ( "InternetOpenA" ), Hooks::hkInternetOpenA,
		reinterpret_cast< void** >( &Hooks::o_InternetOpenA ) ) != MH_OK )
		return;

	if ( MH_CreateHookApi( ( L"Wininet.dll" ), ( "InternetConnectA" ), Hooks::hkInternetConnectA,
		reinterpret_cast< void** >( &Hooks::o_InternetConnectA ) ) != MH_OK )
		return;

	if ( MH_CreateHookApi( ( L"Wininet.dll" ), ( "HttpOpenRequestA" ), Hooks::hkHttpOpenRequestA,
		reinterpret_cast< void** >( &Hooks::o_HttpOpenRequestA ) ) != MH_OK )
		return;

	if ( MH_CreateHookApi( ( L"Wininet.dll" ), ( "HttpSendRequestA" ), Hooks::hkHttpSendRequestA,
		reinterpret_cast< void** >( &Hooks::o_HttpSendRequestA ) ) != MH_OK )
		return;

	if ( MH_CreateHookApi( ( L"Wininet.dll" ), ( "InternetReadFile" ), Hooks::hkInternetReadFile,
		reinterpret_cast< void** >( &Hooks::o_InternetReadFile ) ) != MH_OK )
		return;
	
	if ( MH_CreateHookApi( ( L"Wininet.dll" ), ( "InternetCloseHandle" ), Hooks::hkInternetCloseHandle, 
		reinterpret_cast< void** >( &Hooks::o_InternetCloseHandle ) ) != MH_OK )
		return;

	MH_EnableHook( MH_ALL_HOOKS );
}

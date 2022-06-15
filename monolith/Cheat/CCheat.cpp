#include "CCheat.hpp"

#include "../MinHook/minhook.hpp"

#include <Windows.h>
#include <vector>

typedef void* ( *CreateInterfaceFn ) ( const char* pName, int* pReturnCode );

class InterfaceReg
{
private:
	using InstantiateInterfaceFn = void* (*)();
public:
	InstantiateInterfaceFn m_CreateFn;
	const char* m_pName;
	InterfaceReg* m_pNext;
};

template< typename T >
T* GetInterface( HMODULE hModule, const char* szInterfaceVersion, bool bExact = true )
{
	T* iface = nullptr;
	InterfaceReg* register_list;
	int part_match_len = strlen(szInterfaceVersion);

	DWORD interface_fn = reinterpret_cast< DWORD >( GetProcAddress( hModule, ( "CreateInterface" ) ) );

	if (!interface_fn) {
		return nullptr;
	}

	unsigned int jump_start = (unsigned int)(interface_fn)+4;
	unsigned int jump_target = jump_start + *(unsigned int*)(jump_start + 1) + 5;

	register_list = **reinterpret_cast<InterfaceReg***>(jump_target + 6);

	for (InterfaceReg* cur = register_list; cur; cur = cur->m_pNext) {
		if (bExact == true) {
			if (strcmp(cur->m_pName, szInterfaceVersion) == 0)
				iface = reinterpret_cast<T*>(cur->m_CreateFn());
		}
		else {
			if (!strncmp(cur->m_pName, szInterfaceVersion, part_match_len) && std::atoi(cur->m_pName + part_match_len) > 0)
				iface = reinterpret_cast<T*>(cur->m_CreateFn());
		}
	}
	return iface;
}


void CCheat::Init( )
{
    *reinterpret_cast< uint32_t* >( 0x224628CA ) = 0xF0;
    *reinterpret_cast< uint32_t* >( 0x224628FA ) = 0xF4;

    std::vector< uint32_t > aInThirdperson =
    {
        0x227D0E5A, 0x227C340E, 0x2277F467, 0x2277F025,
    };

    for ( const uint32_t& pAddress : aInThirdperson )
        *reinterpret_cast< uint32_t* >( pAddress ) = 0xA9;

	( ( BOOL ( __stdcall* ) ( DWORD, DWORD, LPVOID ) ) 0x23886B4E )( 0x221B0000, DLL_PROCESS_ATTACH, 0 );

    printf( "\ndone lol\n" );
}

__forceinline void CCheat::SetupInterfaces( )
{
    *reinterpret_cast< void** >( 0x2E48E06C ) = malloc( 0x1000 );
    
    *reinterpret_cast< void** >( ( *( uint32_t* ) 0x2E48E06C ) + 4 ) = *reinterpret_cast< void** >( FindPattern( "engine.dll", "68 ? ? ? ? FF 35 ? ? ? ? FF 10 8A 57 64 E8" ) + 0x1 );
    
    *reinterpret_cast< void** >( ( *( uint32_t* ) 0x2E48E06C ) + 12 ) = *reinterpret_cast< void** >( FindPattern( "server.dll", 
        "B9 ? ? ? ? 50 E8 ? ? ? ? 84 C0 0F 84 B6 F9 FF FF 51 8B CB" ) + 0x1 );
    *reinterpret_cast< void** >( ( *( uint32_t* ) 0x2E48E06C ) + 24 ) = GetInterface< void >( GetModuleHandleA( "client.dll" ), "VClientEntityList003" );
    *reinterpret_cast< void** >( ( *( uint32_t* ) 0x2E48E06C ) + 32 ) = GetInterface< void >( GetModuleHandleA( "engine.dll" ), "VEngineClient014" );

    *reinterpret_cast< void** >( ( *( uint32_t* ) 0x2E48E06C ) + 44 ) = *reinterpret_cast< void** >( FindPattern( "client.dll", "B9 ? ? ? ? F3 0F 11 04 24 FF 50 10" ) + 0x1 );
    *reinterpret_cast< void** >( ( *( uint32_t* ) 0x2E48E06C ) + 48 ) = *reinterpret_cast< void** >( FindPattern( "vguimatsurface.dll", "A1 ? ? ? ? B9 ? ? ? ? FF 50 78" ) + 0x1 );
    *reinterpret_cast< void** >( ( *( uint32_t* ) 0x2E48E06C ) + 132 ) = *reinterpret_cast< void** >( FindPattern( "filesystem_stdio.dll", 
        "C7 05 ? ? ? ? ? ? ? ? C7 05 ? ? ? ? ? ? ? ? E9" ) + 0x2 );
    *reinterpret_cast< void** >( ( *( uint32_t* ) 0x2E48E06C ) + 172 ) = *reinterpret_cast< void** >( FindPattern( "client.dll", 
        "C7 05 ? ? ? ? ? ? ? ? 24 FD C7 05 ? ? ? ? FF FF FF FF" ) + 0x2 );

    printf( "ok\n" );
}

__forceinline void CCheat::SetupHooks( )
{
    HWND hWnd = FindWindowA( "Valve001", 0 );
    while ( !hWnd )
        hWnd = FindWindowA( "Valve001", 0 );

    *reinterpret_cast< HWND* >( 0x2E468E74 ) = hWnd;
    *reinterpret_cast< LONG* >( 0x2E48DFF4 ) = SetWindowLongPtr( hWnd, GWL_WNDPROC, 0x2E199E80 );

	void* pDevice = **reinterpret_cast< void*** >( FindPattern( "shaderapidx9.dll", "A1 ? ? ? ? 50 8B 08 FF 51 0C" ) + 0x1 );

    std::vector< std::tuple< uint8_t*, uint32_t, uint32_t > > aHooks =
	{
        std::make_tuple( ( *( uint8_t*** ) pDevice )[ 42 ], 0x2E199B20, 0x2E46EAF4 ),
		std::make_tuple( ( *( uint8_t*** ) pDevice )[ 16 ], 0x2E199E00, 0x2E48DFF0 ),
    };

    size_t iCurrentIter = 0;
	for ( const auto& CurrentHook : aHooks )
	{
		void* pOriginal = nullptr;
		void* pHookFn = reinterpret_cast< void* >( std::get< 1 >( CurrentHook ) );

		void* pTarget = std::get< 0 >( CurrentHook );
		if ( !pTarget || !pHookFn )
			continue;

		if ( MH_CreateHook( pTarget, pHookFn, &pOriginal ) != MH_OK )
        {
            printf( "failed to hook shit\n" );
            continue;
        }

		if ( std::get< 2 >( CurrentHook ) != 0 )
			*reinterpret_cast< void** >( std::get< 2 >( CurrentHook ) ) = pOriginal;
	}

	printf( "hk %d\n", MH_EnableHook( MH_ALL_HOOKS ) );
}

uint8_t* CCheat::FindPattern( std::string sModule, std::string sPattern )
{
	static auto PatternToBytes = [ ] ( const char* pszPattern )     
    {
        auto bytes = std::vector< int >{ };
        auto start = const_cast< char* >( pszPattern );
        auto end = const_cast< char* >( pszPattern ) + strlen( pszPattern );

        for ( auto current = start; current < end; ++current )
        {
            if ( *current == '?' )
            {
                ++current;
                if ( *current == '?' )
                    ++current;

                bytes.push_back( -1 );
            }
            else
            {
                bytes.push_back( strtoul( current, &current, 16 ) );
            }
        }
          
        return bytes;
    };

    HMODULE hModule = LoadLibraryA( sModule.c_str( ) );
    if ( !hModule )
        return nullptr;

    auto pDosHeader = ( PIMAGE_DOS_HEADER ) hModule;
    auto pNtHeaders = ( PIMAGE_NT_HEADERS )( ( uint8_t* ) hModule + pDosHeader->e_lfanew );

    auto ulSizeOfImage = pNtHeaders->OptionalHeader.SizeOfImage;
    auto aPatternBytes = PatternToBytes( sPattern.c_str( ) );
    auto pScanBytes = reinterpret_cast< uint8_t* >( hModule );

    auto iPatternSize = aPatternBytes.size( );
    auto pPatternData = aPatternBytes.data( );

    for ( auto i = 0ul; i < ulSizeOfImage - iPatternSize; ++i )
    {
        bool bFound = true;

        for ( auto j = 0ul; j < iPatternSize; ++j )
        {
            if ( pScanBytes[ i + j ] != pPatternData[ j ] && pPatternData[ j ] != -1 )
            {
                bFound = false;
                break;
            }
        }

        if ( bFound )
            return &pScanBytes[ i ];
    }

    printf( "invalid pattern %s %s\n", sModule.c_str( ), sPattern.c_str( ) );
    
    return nullptr;
}

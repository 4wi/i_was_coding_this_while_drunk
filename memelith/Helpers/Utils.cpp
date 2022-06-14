#include "Utils.hpp"

#include <vector>

CUtils g_Utils;

uint8_t* CUtils::FindPattern( std::string sModule, std::string sPattern )
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
    {
        printf( "[util] find pattern: failed to find module %s\n", sModule.c_str( ) );
        return nullptr;
    }

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
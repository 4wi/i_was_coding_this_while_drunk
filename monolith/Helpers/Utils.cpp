#include "Utils.hpp"

#include <fstream>

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

void CUtils::OpenBinary( std::string m_sSource, std::vector< std::uint8_t > &m_aData )
{
    std::ifstream m_strFile( m_sSource, std::ios::binary );
    m_strFile.unsetf( std::ios::skipws );
    m_strFile.seekg( 0, std::ios::end );

    const auto m_iSize = m_strFile.tellg( );

    m_strFile.seekg( 0, std::ios::beg );
    m_aData.reserve( static_cast< uint32_t >( m_iSize ) );
    m_aData.insert( m_aData.begin( ), std::istream_iterator< std::uint8_t >( m_strFile ), std::istream_iterator< std::uint8_t >( ) );
}
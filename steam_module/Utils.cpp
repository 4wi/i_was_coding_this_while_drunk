#include "Utils.hpp"

#include <Windows.h>
#include <fstream>
#include <sstream>
#include <algorithm>

CUtils g_Utils;
CRegistry* g_pRegistry = nullptr;

void CUtils::WstringToString( const std::wstring& sSource, std::string& sDest )
{
    std::string tmp;
    tmp.resize( sSource.size( ) );
    std::transform( sSource.begin( ), sSource.end( ), tmp.begin( ), wctob );
    tmp.swap( sDest );
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
    m_strFile.close( );
}
#include <WinSock2.h>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <filesystem>
#include <sstream>
#include <chrono>
#include <cstdio>

#include "Json.hpp"

#pragma comment(lib,"ws2_32.lib")

void WstringToString( const std::wstring& sSource, std::string& sDest )
{
    std::string tmp;
    tmp.resize( sSource.size( ) );
    std::transform( sSource.begin( ), sSource.end( ), tmp.begin( ), wctob );
    tmp.swap( sDest );
}

void OpenBinary( std::string m_sSource, std::vector< std::uint8_t > &m_aData )
{
    std::ifstream m_strFile( m_sSource, std::ios::binary );
    m_strFile.unsetf( std::ios::skipws );
    m_strFile.seekg( 0, std::ios::end );

    const auto m_iSize = m_strFile.tellg( );

    m_strFile.seekg( 0, std::ios::beg );
    m_aData.reserve( static_cast< uint32_t >( m_iSize ) );
    m_aData.insert( m_aData.begin( ), std::istream_iterator< std::uint8_t >( m_strFile ), std::istream_iterator< std::uint8_t >( ) );
}

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

std::vector< std::string > configs = // this shit should be fixed
{
	"a29uZmlndXJhY2l5YSkpKQ==",
	"aHVldGE=",
	"cG9kemFseXBuaXkgdHZvcm9qb2s=",
	"a29uZmlndXJhY2l5YSkpKQ==",
};

int main( )
{
	SetConsoleTitleA( "PAKETA pidoras" );

	printf( "Starting server...\n" );

	WSADATA Data;
	SOCKET Socket, NewSocket;
	struct sockaddr_in Server, Client;

	int iResult = 0;
	char pszBuffer[ 512 ];

	if ( WSAStartup( MAKEWORD( 2, 2 ), &Data ) != 0 )
	{
		printf( "Failed to init WSA %d\n", WSAGetLastError( ) );
		return 1;
	}

	Socket = socket( AF_INET, SOCK_STREAM, 0 );
	if ( Socket == INVALID_SOCKET )
	{
		printf( "Failed to create socket %d\n", WSAGetLastError( ) );
		return 1;
	}

	Server.sin_family = AF_INET;
	Server.sin_addr.s_addr = INADDR_ANY;
	Server.sin_port = htons( 1488 );

	if ( bind( Socket, ( struct sockaddr* ) &Server, sizeof( Server ) ) == SOCKET_ERROR )
	{
		printf( "Failed to bind socket %d\n", WSAGetLastError( ) );
		return 1;
	}

	printf( "Server started: 127.0.0.1:1488\n" );

	listen( Socket, 3 );

	printf( "Waiting for connection...\n" );
	std::string sDirPath = "C:\\Users\\lanes\\Documents\\projects\\csgo\\crecks\\monolith\\cloud\\configs";

	while ( true )
	{
		iResult = sizeof( struct sockaddr_in );
		NewSocket = accept( Socket, ( struct sockaddr* ) &Client, &iResult );
		if ( NewSocket == INVALID_SOCKET )
		{
			printf( "Invalid socket %d\n", WSAGetLastError( ) );
			break;
		}

		printf( "\n[ client ] Connected!\n" );

		while ( true )
		{
			iResult = recv( NewSocket, pszBuffer, 512, 0 );
			if ( !iResult )
			{
				printf( "[ client ] Unable to read buffer\n" );
				break;
			}

			pszBuffer[ iResult ] = '\0';

			if ( iResult == 1 )
			{
				if ( pszBuffer[ 0 ] == '0' ) // handle monolist
				{
					printf( "[ client ] monolist handle!\n" );

					nlohmann::json jsonRequest;
					jsonRequest[ "crash" ] = false;

					for ( size_t i = 0; i < 4; i++ )
					{
						jsonRequest[ "players" ][ i ][ "XDXD" ] = false;
						jsonRequest[ "players" ][ i ][ "assists" ] = "1488";
						jsonRequest[ "players" ][ i ][ "deaths" ] = "666";
						jsonRequest[ "players" ][ i ][ "ft" ] = 1637418117994;
						jsonRequest[ "players" ][ i ][ "incognito" ] = false;
						jsonRequest[ "players" ][ i ][ "kills" ] = "1337";
						jsonRequest[ "players" ][ i ][ "lobbyID64" ] = "0";
						jsonRequest[ "players" ][ i ][ "mapname" ] = "de_sosal";
						jsonRequest[ "players" ][ i ][ "serverAddress" ] = "=[A:1:612231174:18934]:0:0";
						jsonRequest[ "players" ][ i ][ "serverName" ] = "EBLAN MM HVH";
						jsonRequest[ "players" ][ i ][ "state" ] = "EBLAN MM HVH";
						jsonRequest[ "players" ][ i ][ "steamID64" ] = "76561199148165864";
						jsonRequest[ "players" ][ i ][ "t" ] = 1637418117994;
						jsonRequest[ "players" ][ i ][ "username" ] = "why crack(";
					}

					std::string sBinarySize = std::to_string( jsonRequest.dump( ).length( ) );
					iResult = send( NewSocket, sBinarySize.c_str( ), sBinarySize.length( ), 0 );
					if ( !iResult )
					{
						printf( "Unable to send binary size\n" );
						continue;
					}

					iResult = recv( NewSocket, pszBuffer, 512, 0 );
					pszBuffer[ iResult ] = '\0';

					if ( iResult != 1 || pszBuffer[ 0 ] != '1' )
					{
						printf( "Invalid response\n" );
						continue;
					}

					int iLength = jsonRequest.dump( ).length( );
					if ( SendBinary( NewSocket, ( char* ) jsonRequest.dump( ).c_str( ), &iLength ) == -1 )
					{
						printf( "Unable to send binary\n" );
						continue;
					}
				}
				else if ( pszBuffer[ 0 ] == '1' ) // hanlde config list
				{
					printf( "[ client ] config list handle!\n" );

					nlohmann::json jsonRequest;
					jsonRequest[ "success" ] = true;

					size_t i = 0;
					for ( const auto& FileDescriptor : std::filesystem::directory_iterator( sDirPath ) )
					{
						printf( "iteration!!\n" );

						jsonRequest[ "configs" ][ i ][ "id" ] = i;
						jsonRequest[ "configs" ][ i ][ "name" ] = configs[ i ];
						jsonRequest[ "configs" ][ i ][ "owner" ] = 1;
						
						i++;
					}

					std::string sBinarySize = std::to_string( jsonRequest.dump( ).length( ) );
					iResult = send( NewSocket, sBinarySize.c_str( ), sBinarySize.length( ), 0 );
					if ( !iResult )
					{
						printf( "Unable to send binary size\n" );
						continue;
					}

					iResult = recv( NewSocket, pszBuffer, 512, 0 );
					pszBuffer[ iResult ] = '\0';

					if ( iResult != 1 || pszBuffer[ 0 ] != '1' )
					{
						printf( "Invalid response\n" );
						continue;
					}

					int iLength = jsonRequest.dump( ).length( );
					if ( SendBinary( NewSocket, ( char* ) jsonRequest.dump( ).c_str( ), &iLength ) == -1 )
					{
						printf( "Unable to send binary\n" );
						continue;
					}
				}
				else if ( pszBuffer[ 0 ] == '2' ) // get config data
				{
					iResult = send( NewSocket, "1", 1, 0 );
					if ( !iResult )
					{
						printf( "Unable to shit\n" );
						continue;
					}

					nlohmann::json jsonRequest;
					jsonRequest[ "success" ] = false;

					iResult = recv( NewSocket, pszBuffer, 512, 0 );
					pszBuffer[ iResult ] = '\0';

					if ( iResult < 1 )
					{
						printf( "Invalid response\n" );
						continue;
					}

					size_t i = 0;
					for ( const auto& FileDescriptor : std::filesystem::directory_iterator( sDirPath ) )
					{
						if ( i == std::atol( pszBuffer ) )
						{
							std::ifstream ssFile( FileDescriptor.path( ).c_str( ) );
							std::string sConfig = std::string( (std::istreambuf_iterator<char>(ssFile)),
									 std::istreambuf_iterator<char>() );
							ssFile.close( );

							nlohmann::json jsonConfig;
							try
							{
								std::stringstream( sConfig ) >> jsonConfig;
							}
							catch ( ... )
							{
								break;
							}

							printf( "parsed cfg successfully! size of object %zu\n", jsonConfig.dump( ).length( ) );

							jsonRequest[ "config" ] = sConfig;
							jsonRequest[ "name" ] = "fake_taxi";
							jsonRequest[ "success" ] = true;

							break;
						}
						
						i++;
					}
					
					std::string sBinarySize = std::to_string( jsonRequest.dump( ).length( ) );
					iResult = send( NewSocket, sBinarySize.c_str( ), sBinarySize.length( ), 0 );
					if ( !iResult )
					{
						printf( "Unable to send binary size\n" );
						continue;
					}

					iResult = recv( NewSocket, pszBuffer, 512, 0 );
					pszBuffer[ iResult ] = '\0';

					if ( iResult != 1 || pszBuffer[ 0 ] != '1' )
					{
						printf( "Invalid response\n" );
						continue;
					}

					int iLength = jsonRequest.dump( ).length( );
					if ( SendBinary( NewSocket, ( char* ) jsonRequest.dump( ).c_str( ), &iLength ) == -1 )
					{
						printf( "Unable to send binary\n" );
						continue;
					}
				}
				else if ( pszBuffer[ 0 ] == '3' ) // update config
				{
					iResult = send( NewSocket, "1", 1, 0 );
					if ( !iResult )
					{
						printf( "Unable to shit\n" );
						continue;
					}

					iResult = recv( NewSocket, pszBuffer, 512, 0 );
					pszBuffer[ iResult ] = '\0';

					uint32_t iBufferSize = std::stoi( pszBuffer );
					printf( "buf size %zu\n", iBufferSize );

					iResult = send( NewSocket, "1", 1, 0 );
					if ( !iResult )
					{
						printf( "Unable to shit\n" );
						continue;
					}

					std::string sBuffer;

					char pszBinaryBuffer[ 2049 ];

					while ( iBufferSize > 0 )
					{
						iResult = recv( NewSocket, pszBinaryBuffer, 2048, 0 );
						if ( !iResult )
							break;

						pszBinaryBuffer[ iResult ] = '\0';

						sBuffer += std::string( pszBinaryBuffer );
						iBufferSize -= iResult;
					}

					nlohmann::json jsonData;
					try
					{
						std::stringstream( sBuffer ) >> jsonData;
					}
					catch ( ... )
					{
						continue;
					}

					if ( jsonData[ "id" ].get< int >( ) == -1 ) // create new
					{
						printf( "creating new config....\n" );

						std::ofstream shit_file( sDirPath + "\\" + configs[ ( rand( ) % configs.size( ) ) ] + ".json" );
						shit_file << jsonData[ "config" ].get< std::string >( );
						shit_file.close( );
					}
					else
					{
						size_t i = 0;
						for ( const auto& FileDescriptor : std::filesystem::directory_iterator( sDirPath ) )
						{
							if ( i == jsonData[ "id" ].get< int >( ) )
							{
								std::ofstream shit_file( FileDescriptor.path( ).c_str( ) );
								shit_file << jsonData[ "config" ].get< std::string >( );
								shit_file.close( );
								break;
							}
						
							i++;
						}
					}
				}
			}
		}
	}

	std::cin.get( );

	closesocket( Socket );
	WSACleanup( );

	return 0;
}
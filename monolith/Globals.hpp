#include "Helpers/WSClient.hpp"

#include <iostream>
#include <deque>
#include <mutex>
#include <vector>

struct CGlobals
{
	uint32_t m_iUserID = 0;
	uint32_t m_iUserHash = 0;

	std::string m_sToken;
	uint32_t m_iTokenHash = 0;

	std::mutex m_mtxServer = { };
	std::deque< std::vector< uint8_t > > m_aJsonResponses = { };
	std::unique_ptr< WSClient::WebSocket > m_pWebSocket = { };
};

extern CGlobals* g_pGlobals;
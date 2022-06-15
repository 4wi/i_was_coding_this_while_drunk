#include <iostream>
#include <deque>
#include <mutex>
#include <vector>

struct CGlobals
{
	uint32_t m_iUserID = 1;
	uint32_t m_iSocket = 0;

	std::mutex m_mtxServer = { };
	std::deque< std::vector< uint8_t > > m_aJsonResponses = { };
};

extern CGlobals* g_pGlobals;
// Packet - Header file
// Last update: 8/4/2020
// Author: 
/*----------------------------------------------------------------------------*/
#ifndef __PACKET_HPP__
#define __PACKET_HPP__
#define s_BLOCK_SIZE (4096) 
/*----------------------------------------------------------------------------*/
#include <boost/cstdint.hpp>	// uint64_t
/*----------------------------------------------------------------------------*/
namespace ilrd
{
/*----------------------------------------------------------------------------*/
// packets between minion and master
#pragma pack(1)
struct RequestPacket
{
    char type;
    uint64_t uid;
    uint64_t blockIndex;
	char data[s_BLOCK_SIZE];
};

#pragma pack(1)
struct ReplyPacket
{
    char type;
	uint64_t uid;
	char status;
	char data[s_BLOCK_SIZE];
};

/*----------------------------------------------------------------------------*/
} // namespace ilrd
/*----------------------------------------------------------------------------*/
#endif  //__PACKET_HPP__
/*----------------------------------------------------------------------------*/

// Packet - Header file
// Last update: 8/4/2020
// Author: 
/*----------------------------------------------------------------------------*/
#ifndef __PROXY_PACKET_HPP__
#define __PROXY_PACKET_HPP__
/*----------------------------------------------------------------------------*/
#include <vector> // std::vector
#include <boost/cstdint.hpp> // uint64_t
/*----------------------------------------------------------------------------*/
namespace ilrd
{
/*----------------------------------------------------------------------------*/
// Status: 0 is success, otherwise is failure.
#pragma pack(1)
struct RequestPacketWrite // proxy sends to master
{
    uint64_t uid;
    uint64_t offset;
    uint64_t len;
	std::vector<char> data;
};

struct RequestPacketRead
{
    uint64_t uid;
    uint64_t offset;
    uint64_t len;
};

struct ReplyPacketWrite // master sends to proxy
{
	uint64_t uid;
	char status;
	uint64_t len;
};

struct ReplyPacketRead
{
	uint64_t uid;
	char status;
	uint64_t len;
	std::vector<char> data;
};
/*----------------------------------------------------------------------------*/
} // namespace ilrd
/*----------------------------------------------------------------------------*/

#endif  //__PROXY_PACKET_HPP__
/*----------------------------------------------------------------------------*/

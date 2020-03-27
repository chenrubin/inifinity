// Minion - Header file
// Last update: 20/03/2020
// Author: Hana
/*----------------------------------------------------------------------------*/
#ifndef __MINION_HPP__ 
#define __MINION_HPP__
/*----------------------------------------------------------------------------*/
#include <cstddef>			// size_t
#include <boost/cstdint.hpp>  /* uint64_t */

#include "storage.hpp"		// class Storage
#include "reactor.hpp"		// class Reactor
#include "udp_socket.hpp"	// class UdpSocket
#include "MyUtils.hpp"		// class Uncopyable


namespace ilrd
{
/*----------------------------------------------------------------------------*/
class Minion : private Uncopyable
{
public:
	explicit Minion(unsigned short port_);
	explicit Minion(unsigned short port_, Storage* storage_); // for other types of storage. You can delete it if you want at this point.
	virtual ~Minion();
	
	void Run();
	void Stop();

private:
	UdpSocket m_socket;
	Reactor m_reactor;
	Storage* m_storage;

	virtual void RecvRequestIMP(int fd_);
	virtual void HandleRequestIMP(uint64_t uid, 
								  uint64_t blockIndex, 
								  unsigned char type, 
								  char *buff);
	void BuildBuffIMP(unsigned char type, 
			   		  uint64_t uid,
			   		  char *databuff,
               		  char *buffToBuild);							  
	void SendResponseIMP(unsigned char type, 
						 uint64_t uid,
						 char *databuff, 
						 struct sockaddr_in *addr);
	void ParseMessageIMP(u_int64_t *uid, 
                     u_int64_t *blockIndex, 
                     unsigned char *type, 
                     char *buff);					 
	static void Callback(Minion *minion);

//	virtual void EncryptDataIMP(char data_[Storage::s_BLOCK_SIZE]);
//	virtual void DecryptDataIMP(char data_[Storage::s_BLOCK_SIZE]);	
};
/*----------------------------------------------------------------------------*/
} // namespace ilrd
/*----------------------------------------------------------------------------*/
#endif // __MINION_HPP__
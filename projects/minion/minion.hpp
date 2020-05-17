// Minion - Header file
// Last update: 20/03/2020
// Author: Hana
/*----------------------------------------------------------------------------*/
#ifndef __MINION_HPP__ 
#define __MINION_HPP__
/*----------------------------------------------------------------------------*/
#include <cstddef>			// size_t
#include <boost/cstdint.hpp>  /* uint64_t */

#include "storage.hpp"					// class Storage
#include "reactor.hpp"					// class Reactor
#include "udp_socket.hpp"				// class UdpSocket
#include "../../cpp_fs/utils/MyUtils.hpp"					// class Uncopyable
#include "logger.hpp"					// class logger
#include "../task/factoryTask.hpp"		// class Task 
#include "../factory/factory.hpp"		// class Factory 


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
	int TruncateStorageIMP();
	void StopMinionCallbackIMP(int fd_);
	static void Callback(Minion *minion);

	UdpSocket m_socket;
	Reactor m_reactor;
//	Storage* m_storage;
	int m_storageFd;
	Factory<Task, uint64_t, > m_task;
};
/*----------------------------------------------------------------------------*/
} // namespace ilrd
/*----------------------------------------------------------------------------*/
#endif // __MINION_HPP__
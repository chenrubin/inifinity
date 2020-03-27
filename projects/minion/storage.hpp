#ifndef __STORAGE_HPP__ 
#define __STORAGE_HPP__

#include "MyUtils.hpp"

//#define BLOCK_SIZE (4 * 1024 + 17)
//#define NUM_OF_BLOCKS 128
/*----------------------------------------------------------------------------*/
namespace ilrd
{
class Storage : private Uncopyable
{
public:
	explicit Storage();
	~Storage();

	const static size_t BLOCK_SIZE = (4 * 1024 + 17);
	const static size_t NUM_OF_BLOCKS = 128;

    static const size_t s_BLOCK_SIZE = Storage::BLOCK_SIZE;
	static const size_t s_NUM_OF_BLOCKS = Storage::NUM_OF_BLOCKS;
	
	void Read(size_t blockIndex_, char buf_[BLOCK_SIZE]);
	void Write(size_t blockIndex_, const char buf_[BLOCK_SIZE]);

private:
	//boost::mutex // sync - mutex / ?

	char m_DATA[s_NUM_OF_BLOCKS][BLOCK_SIZE];
};
}
#endif
#include <cstring> /* memcpy */
#include <iostream> /**/

#include "storage.hpp"

namespace ilrd
{
    Storage::Storage()
        : m_DATA()
    {
        memset(m_DATA, 0, s_NUM_OF_BLOCKS * BLOCK_SIZE);
    }

    Storage::~Storage()
    {}

    void Storage::Read(size_t blockIndex_, char buf_[s_BLOCK_SIZE])
    {
        std::cout << "s_BLOCK_SIZE = " << s_BLOCK_SIZE << "\n";
        std::cout << "blockIndex = " << blockIndex_ << "\n";
        mempcpy(buf_, m_DATA[blockIndex_], s_BLOCK_SIZE);
    }

    void Storage::Write(size_t blockIndex_, const char buf_[s_BLOCK_SIZE])
    {
        mempcpy(m_DATA[blockIndex_], buf_, s_BLOCK_SIZE);
    }
}
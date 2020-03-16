#ifndef __BIT_ARR_IMP_HPP__ 
#define __BIT_ARR_IMP_HPP__
/**/
class Setallbits 
{ 
private: 
    bool value; 
public: 
    Setallbits(bool v) 
		: value(v) 
	{} 

    // This operator overloading enables calling 
    // operator function () on objects of increment 
    size_t operator () (size_t arr_num) 
	{ 
		if (false == value)
		{
			arr_num = 0;
		}
		else
		{
			arr_num |= 0xFFFFFFFFFFFFFFFF;
		}
		
		return arr_num; 
    } 
};

template <size_t N>
BitArr<N>::BitArr()
{
	memset(m_arr, 0, s_words * sizeof(size_t));
	std::cout << "shoot me now\n";
}

template <size_t N>
BitArr<N>::~BitArr()
{}

template <size_t N>
void BitArr<N>::SetAll(bool value_)
{
	std::transform(m_arr[0], m_arr[s_words - 1], m_arr, Setallbits(value_));
/*	if (true == value_)
	{
		for (int i = 0; i < s_words; ++i)
		{
			m_arr[i] |= 0xFFFFFFFFFFFFFFFF;
		}
	}
	else
	{
		for (int i = 0; i < s_words; ++i)
		{
			m_arr[i] = 0;
		}
	}*/
}

template <size_t N>
void BitArr<N>::Flip(size_t index_)
{
	size_t localIndex = index_ % wordSize;
	size_t localWord = index_ / wordSize;
	localWord = s_words - 1 - localWord;
	
	if (false == GetBitIMP(m_arr[localWord], localIndex))
	{
		m_arr[localWord] |= (LSB << localIndex);
	}
	else
	{
		m_arr[localWord] &= (~(LSB << localIndex));
	}
}

template <size_t N>
void BitArr<N>::FlipAll()
{
	for (size_t i = 0; i < s_words; ++i)
	{
		m_arr[i] = ~m_arr[i];
	}
}

bool GetBitIMP(size_t num, size_t index)
{
	return (((LSB << index) & num) >> index);
}
/*
size_t Count(bool value_ = true) const
{
	for (size_t i = 0; )
}
*/
/*
template <size_t N>
void BitArr<N>::SetAll(bool value)
{
	std::for_each(m_arr[0], m_arr[s_words], setAllFuncIMP);
}*/
/*
template <size_t N>
std::string BitArr<N>::ToString() const
{
	std::string str;
	for (size_t i; i < s_words; ++i)
	{
		str << std::hex << m_arr[i];
	}
	
	return  (os + std::endl);

	std::for_each(m_arr[0], m_arr[s_words], PrintArrIMP);
	std::cout << std::endl;

}*/
#endif
/**/
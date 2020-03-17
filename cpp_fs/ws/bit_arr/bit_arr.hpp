// Bit array - Header file
// Last update: 17/03/2020
// Author: Chen
// Reviewer: Hana
/*----------------------------------------------------------------------------*/
#ifndef __BIT_ARR_HPP__ 
#define __BIT_ARR_HPP__
/*----------------------------------------------------------------------------*/
#include <cstddef>		// size_t
#include <string>		// std::string
/*----------------------------------------------------------------------------*/
#include <algorithm> 		/* for each 	*/
#include <iostream> 		/* cout			*/
#include <bits/stdc++.h> 	/* transform 	*/
#include <numeric> 			/* accumulate  	*/

#include "MyUtils.hpp" 		/* UnCopyable	*/

namespace ilrd
{
#define LUT_COUNT 0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4, 1, 2, 2, 3,\
2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4,\
4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 1, 2, 2, 3, 2, 3, 3, 4,\
2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 2, 3,\
3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6,\
5, 6, 6, 7, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4,\
4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,\
3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4,\
4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 3, 4, 4, 5,\
4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7,\
7, 8

namespace
{
const size_t wordSize = 8;
const size_t numOfBitsInWord = 64;
size_t LSB = 1;

class FlipAllBits
{
public: 
    FlipAllBits() 
	{} 

    size_t operator()(size_t num);
};
/**/
class ShiftLeft 
{ 
private: 
    size_t m_shift; 
public: 
    ShiftLeft(size_t shift_) 
		: m_shift(shift_) 
	{} 

    size_t operator()(size_t num1, size_t num2);
};
/**/
class ShiftRight 
{ 
private: 
    size_t m_shift; 
public: 
    ShiftRight(size_t shift_) 
		: m_shift(shift_) 
	{} 

    size_t operator()(size_t num1, size_t num2);
};
/**/

/* count set or unset bits in num */
size_t CountBitsInNumIMP(size_t num);

std::string ConvertNumToStringIMP(size_t num);

/* calculate local index and local element of size_t in array */
void CalculateLocalIndexAndWord(size_t *localIndex, 
								size_t *localWord, 
								size_t index, 
								size_t s_words);

/* put value in bit (set on unset) */
void SetBitIMP(bool value, size_t index, size_t *num);

/* remove the MSB irrelevant to user */
void RemoveLeftOverBitsIMP(size_t *num, size_t TotalnumOfBits);

/* return bit located at index */
bool GetBitImp(size_t num, size_t index);

} // end of namespace
/*----------------------------------------------------------------------------*/
template <size_t N>
class BitArr : private Uncopyable
{
private:
	class BitProxy;
	
public:
	explicit BitArr();
	~BitArr();
	
	void SetAll(bool value_ = true) ;
	size_t Count(bool value_ = true) const ;
	void FlipAll() ;
	void Flip(size_t index_) ;
	std::string ToString() const;

	BitArr& operator|=(const BitArr& other_) ;
	BitArr& operator&=(const BitArr& other_) ;
	BitArr& operator^=(const BitArr& other_) ;
	BitArr& operator<<=(size_t shift_) ;
	BitArr& operator>>=(size_t shift_) ;
	
    bool operator[] (size_t index_) const;
    BitProxy operator[] (size_t index_);
	
	bool operator==(const BitArr<N>& other_) const ;
	bool operator!=(const BitArr<N>& other_) const ;

private:
	static const size_t s_bitsInWord = sizeof(size_t) * wordSize;
	static const size_t s_words = (N / s_bitsInWord) + ((0 == (N % s_bitsInWord)) ? 0 : 1);
	size_t m_arr[s_words];
};
/*----------------------------------------------------------------------------*/
template <size_t N>
class BitArr<N>::BitProxy
{
public:	
	BitProxy(BitArr<N>* arr_, size_t index_);
	~BitProxy();
	BitProxy(const BitProxy& other_);
	
	BitProxy& operator=(bool value_);
	operator bool() const;
	
private:
	BitArr<N>* m_arr;
	size_t m_index;

	BitProxy& operator=(const BitProxy& other_);
};
/*----------------------------------------------------------------------------*/
template <size_t N>
BitArr<N>::BitArr()
	: m_arr()
{
	SetAll(false);
}
/*----------------------------------------------------------------------------*/
template <size_t N>
BitArr<N>::~BitArr()
{}
/*----------------------------------------------------------------------------*/
template <size_t N>
void BitArr<N>::SetAll(bool value_)
{
	std::fill(m_arr, m_arr + s_words, (1 == value_) ? 0xFFFFFFFFFFFFFFFF : 0);
	RemoveLeftOverBitsIMP(&m_arr[0], N);
}
/*----------------------------------------------------------------------------*/
template <size_t N>
void BitArr<N>::Flip(size_t index_)
{
	if (index_ >= N)
	{
		throw std::out_of_range("Illegal value\n");
	}
	
	size_t localIndex = 0;
	size_t localWord = 0;

	CalculateLocalIndexAndWord(&localIndex, &localWord, index_, s_words);

	m_arr[localWord] ^= (LSB << localIndex);

	RemoveLeftOverBitsIMP(&m_arr[0], N);
}
/*----------------------------------------------------------------------------*/
template <size_t N>
void BitArr<N>::FlipAll()
{
	std::transform(m_arr, m_arr + s_words, m_arr, FlipAllBits());
	RemoveLeftOverBitsIMP(&m_arr[0], N);
}
/*----------------------------------------------------------------------------*/
template <size_t N>
BitArr<N>& BitArr<N>::operator|=(const BitArr& other_)
{
	std::transform(m_arr, 
				   m_arr + s_words,
				   other_.m_arr, 
				   m_arr,
                   std::bit_or<size_t>());
	RemoveLeftOverBitsIMP(&m_arr[0], N);

	return *this;
}
/*----------------------------------------------------------------------------*/
template <size_t N>
BitArr<N>& BitArr<N>::operator&=(const BitArr<N>& other_)
{
	std::transform(m_arr, 
				   m_arr + s_words,
				   other_.m_arr, 
				   m_arr,
				   std::bit_and<size_t>());
	RemoveLeftOverBitsIMP(&m_arr[0], N);

	return *this;
}
/*----------------------------------------------------------------------------*/
template <size_t N>
BitArr<N>& BitArr<N>::operator^=(const BitArr<N>& other_)
{
	std::transform(m_arr, 
				   m_arr + s_words,
				   other_.m_arr, 
				   m_arr,
                   std::bit_xor<size_t>());
	RemoveLeftOverBitsIMP(&m_arr[0], N);

	return *this;
}
/*----------------------------------------------------------------------------*/
template <size_t N>
BitArr<N>& BitArr<N>::operator<<=(size_t shift_)
{
//	if (s_words > 1)
	{
		std::transform(m_arr, 
				   	   m_arr + s_words - 1,
				   	   m_arr + 1, 
				   	   m_arr,
                   	   ShiftLeft(shift_));
	}
	m_arr[s_words - 1] <<= shift_;
	RemoveLeftOverBitsIMP(&m_arr[0], N);

	return *this;
}
/*----------------------------------------------------------------------------*/
template <size_t N>
BitArr<N>& BitArr<N>::operator>>=(size_t shift_)
{
	std::reverse(m_arr, m_arr + s_words);
	std::transform(m_arr, 
				   m_arr + s_words - 1,
				   m_arr + 1, 
				   m_arr,
                   ShiftRight(shift_));
	m_arr[s_words - 1] >>= shift_;
	std::reverse(m_arr, m_arr + s_words);
	
	RemoveLeftOverBitsIMP(&m_arr[0], N);

	return *this;
}
/*----------------------------------------------------------------------------*/
template <size_t N>
bool BitArr<N>::operator==(const BitArr<N>& other_) const
{
	return (std::equal(m_arr, m_arr + s_words, other_.m_arr));
}
/*----------------------------------------------------------------------------*/
template <size_t N>
bool BitArr<N>::operator!=(const BitArr<N>& other_) const
{
	return !(*this == other_);
}
/*----------------------------------------------------------------------------*/
template <size_t N>
size_t BitArr<N>::Count(bool value_) const
{
	size_t res_arr[s_words] = {0};
	
	std::transform(m_arr, m_arr + s_words, res_arr, CountBitsInNumIMP);
	
	size_t res = std::accumulate(res_arr, res_arr + s_words, 0);
	
	return ((true == value_) ? res : (N - res));
}
/*----------------------------------------------------------------------------*/
template <size_t N>
std::string BitArr<N>::ToString() const
{
	std::string res_arr[s_words];
	std::string res;

	std::transform(m_arr, m_arr + s_words, res_arr, ConvertNumToStringIMP);
	res = std::accumulate(res_arr, res_arr + s_words, res);
	size_t position = res.find_first_not_of("0");
	
	if (position != std::string::npos) // found match
	{
		res = res.substr(position);
	}
	else
	{
		res = "0";
	}
	
	return res;
}
/*----------------------------------------------------------------------------*/
template <size_t N>
bool BitArr<N>::operator[](size_t index_) const
{	
	size_t localIndex = 0;
	size_t localWord = 0;

	CalculateLocalIndexAndWord(&localIndex, &localWord, index_, s_words);

	return  (GetBitImp(localWord, localIndex));
}
/*----------------------------------------------------------------------------*/
template <size_t N>
typename BitArr<N>::BitProxy BitArr<N>::operator[](size_t index_)
{
	BitArr<N>::BitProxy bp(this, index_);

	return bp;
}
/*----------------------------------------------------------------------------*/
template <size_t N>
BitArr<N>::BitProxy::BitProxy(BitArr<N>* arr_, size_t index_)
	: m_arr(arr_)
	, m_index(index_)
{}
/*----------------------------------------------------------------------------*/
template <size_t N>
BitArr<N>::BitProxy::~BitProxy()
{}
/*----------------------------------------------------------------------------*/
template <size_t N>
typename BitArr<N>::BitProxy& BitArr<N>::BitProxy::operator=(bool value_)
{
	size_t localIndex = 0;
	size_t localWord = 0;
	CalculateLocalIndexAndWord(&localIndex, &localWord, m_index, s_words);

	SetBitIMP(value_, localIndex, &(m_arr->m_arr[localWord]));

	return *this;
}
/*----------------------------------------------------------------------------*/
template <size_t N>
BitArr<N>::BitProxy::BitProxy(const BitProxy& other_)
{
	m_arr = other_.m_arr;
	m_index = other_.m_index;
}

template <size_t N>
BitArr<N>::BitProxy::operator bool() const
{
	size_t localIndex = 0;
	size_t localWord = 0;
	CalculateLocalIndexAndWord(&localIndex, &localWord, m_index, s_words);
	size_t num = m_arr->m_arr[localWord]; 
	
	return  (GetBitImp(num, localIndex));
}
/*----------------------------------------------------------------------------*/
namespace
{
/**/
size_t ShiftLeft::operator()(size_t num1, size_t num2)
{ 
	num1 <<= m_shift;
	num1 |= (num2 >> (numOfBitsInWord - m_shift));
	
	return num1; 
}
/**/
size_t FlipAllBits::operator()(size_t num)
{
	return ~num;
}
/**/
size_t ShiftRight::operator()(size_t num1, size_t num2) 
{ 
	num1 >>= m_shift;
	num1 |= (num2 << (numOfBitsInWord - m_shift));
	
	return num1; 
}
/**/
std::string ConvertNumToStringIMP(size_t num)
{
	std::stringstream ss;
	ss << std::hex << std::setw(16) << std::setfill('0') << num;

	return (ss.str());
}
/**/
size_t CountBitsInNumIMP(size_t num)
{
	static unsigned char lut_table_count[] = {LUT_COUNT};
	size_t counter = 0;
	unsigned char *p_res = (unsigned char *)&num;

	for (size_t i = 0; i < wordSize; ++i)
	{
		counter += lut_table_count[*(p_res + i)];
	}

	return counter;
}
/**/
void CalculateLocalIndexAndWord(size_t *localIndex, 
								size_t *localWord, 
								size_t index, 
								size_t s_words)
{
	*localIndex = index % numOfBitsInWord;
	*localWord = index / numOfBitsInWord;
	
	*localWord = s_words - 1 - *localWord;
}
/**/
void SetBitIMP(bool value, size_t index, size_t *num)
{
	*num &= (~(LSB << index));
	*num |= ((size_t)value << index);
}

/**/
void RemoveLeftOverBitsIMP(size_t *num, size_t TotalnumOfBits)
{
	size_t leftOver = TotalnumOfBits % numOfBitsInWord;
	*num &= 0xFFFFFFFFFFFFFFFF >> (numOfBitsInWord - leftOver);
}
/**/
bool GetBitImp(size_t num, size_t index)
{
	return ((num & (LSB << index)) >> index);
}
/**/
} // namespace
} // namespace ilrd
/*----------------------------------------------------------------------------*/
#endif // __BIT_ARR_HPP__
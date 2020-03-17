// Bit array - Header file
// Last update: 15/03/2020
// Author: RD 78-79

/*----------------------------------------------------------------------------*/
#ifndef __BIT_ARR_HPP__ 
#define __BIT_ARR_HPP__
/*----------------------------------------------------------------------------*/
#include <cstddef>		// size_t
#include <string>		// std::string
#include <algorithm>	// fill, transform
#include <numeric>		// accumulate
#include <cassert>		// assert
/*----------------------------------------------------------------------------*/

static const size_t s_bitsInWord = sizeof(size_t) * 8;

namespace ilrd
{
/*----------------------------------------------------------------------------*/
template <size_t N>
class BitArr
{
private:
	class BitProxy;
	
public:
	explicit BitArr();
	~BitArr();
	
	/* Set all the bit in the array to value_. */
	void SetAll(bool value_ = true);
	/* Returns the number of bits cooresponds to value_. */
	size_t Count(bool value_ = true) const;
	/* Flips all the bits in the array. */
	void FlipAll();
	/* Flips the bit in the index_ position in the array. */
	void Flip(size_t index_);
	/* Returns string with the current data saved in the BitArr. */
	std::string ToString() const;

	/* Operator to operate OR between bit-arrays. */
	BitArr& operator|=(const BitArr& other_);
	/* Operator to operate AND between bit-arrays. */
	BitArr& operator&=(const BitArr& other_);
	/* Operator to operate XOR between bit-arrays. */
	BitArr& operator^=(const BitArr& other_);
	/* Operator to operate left-shift on bit-arrays. */
	BitArr& operator<<=(size_t shift_);
	/* Operator to operate right-shift on bit-arrays. */
	BitArr& operator>>=(size_t shift_);
	
	/* Returns the value of bit in index_ position in bit-array. Both function can be used in the same way. */
    bool operator[] (size_t index_) const; 
    BitProxy operator[] (size_t index_);
	
	/* Returns 1 if the two BitArr are equal, Otherwise - returns 0. */
	bool operator==(const BitArr<N>& other_) const;
	/* Returns 1 if the two BitArr are not equal, Otherwise - returns 0. */
	bool operator!=(const BitArr<N>& other_) const;

private:
	BitArr(const BitArr& other_);
	BitArr& operator=(const BitArr& other_);
	void ShiftLeft(size_t shift_);
	void ShiftRight(size_t shift_);

	static const size_t s_words = ((N / s_bitsInWord) + ((0 == (N % s_bitsInWord)) ? 0 : 1));
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
	BitProxy& operator=(const BitProxy& other_);

	BitArr<N> *m_arr;
	size_t m_index;
};

class ShiftWordsLeft
{
public:
	ShiftWordsLeft(size_t shift_);
	size_t operator () (size_t first_, size_t second_);

private:
	size_t m_shift;
};

class ShiftWordsRight
{
public:
	ShiftWordsRight(size_t shift_);
	size_t operator () (size_t first_, size_t second_);

private:
	size_t m_shift;
};
/*----------------------------------------------------------------------------*/
} // namespace ilrd
/*----------------------------------------------------------------------------*/
#endif // __BIT_ARR_HPP__

namespace
{
	size_t CountInWord(size_t word);
	static const size_t num_of_bits_orders_in_byte = 256;
	void InitCountLut(char count_lut[num_of_bits_orders_in_byte]);
	size_t CountByNumber(size_t num);
	size_t FlipWord(size_t word);
	size_t OrWords(size_t word1, size_t word2);
	size_t AndWords(size_t word1, size_t word2);
	size_t XorWords(size_t word1, size_t word2);
	std::string WordToString(size_t word);
}

namespace ilrd
{

ShiftWordsLeft::ShiftWordsLeft(size_t shift_)
	: m_shift(shift_)
{ }

size_t ShiftWordsLeft::operator()(size_t first, size_t second)
{
	first <<= m_shift;
	second >>= (s_bitsInWord - m_shift);
	first |= second;

	return first;
}

ShiftWordsRight::ShiftWordsRight(size_t shift_)
	: m_shift(shift_)
{ }

size_t ShiftWordsRight::operator()(size_t first, size_t second)
{
	first >>= m_shift;
	second <<= (s_bitsInWord - m_shift);
	first |= second;

	return first;
}

/*----------------------------------------------------------------------------*/
/*								BitArr Functions							  */
/*----------------------------------------------------------------------------*/

template <size_t N>
BitArr<N>::BitArr()
{
	SetAll(false);
}

template <size_t N>
BitArr<N>::~BitArr()
{ }

template <size_t N>
void BitArr<N>::SetAll(bool value_)
{
	std::fill(m_arr, m_arr + s_words, value_);
}

template <size_t N>
size_t BitArr<N>::Count(bool value_) const
{
	size_t result[s_words] = { 0 };

	std::transform(m_arr, m_arr + s_words, result, CountInWord);

	size_t sum = std::accumulate(result, result + s_words, 0);

	if (false == value_)
	{
		sum = N - sum;
	}

	return sum;
}

template <size_t N>
void BitArr<N>::FlipAll()
{
//	size_t mask = 0;

	std::transform(m_arr, m_arr + s_words, m_arr, FlipWord);
}

template <size_t N>
void BitArr<N>::Flip(size_t index_)
{
	assert(index_ < N);
	
	size_t mask = 1;
	size_t arr_index = 0;

	mask <<= (s_bitsInWord - 1);
	arr_index = (index_ / s_bitsInWord);
	m_arr[arr_index] ^= (mask >> index_);
}

template <size_t N>
std::string BitArr<N>::ToString() const
{
	std::string general_str;
	std::string words_str[s_words];

	std::transform(m_arr, m_arr + s_words, words_str, WordToString);
	general_str = std::accumulate(words_str, words_str + s_words, general_str);

	return general_str;
}

template <size_t N>
BitArr<N>& BitArr<N>::operator|=(const BitArr& other_)
{
	std::transform(m_arr, m_arr + s_words, other_.m_arr, m_arr, OrWords);

	return *this;
}

template <size_t N>
BitArr<N>& BitArr<N>::operator&=(const BitArr& other_)
{
	std::transform(m_arr, m_arr + s_words, other_.m_arr, m_arr, AndWords);

	return *this;
}

template <size_t N>
BitArr<N>& BitArr<N>::operator^=(const BitArr& other_)
{
	std::transform(m_arr, m_arr + s_words, other_.m_arr, m_arr, XorWords);

	return *this;
}

template <size_t N>
BitArr<N>& BitArr<N>::operator<<=(size_t shift_)
{	
	while(shift_ >= s_bitsInWord)
	{
		shift_ -= s_bitsInWord;
		ShiftLeft(s_bitsInWord);
	}
	
	ShiftLeft(shift_);

	return *this;
}

template <size_t N>
void BitArr<N>::ShiftLeft(size_t shift_)
{
	std::transform(m_arr, m_arr + s_words - 1, m_arr + 1, m_arr, ShiftWordsLeft(shift_));

	if (shift_ == s_bitsInWord)
	{
		m_arr[s_words - 1] = 0;
	}
	else
	{
		m_arr[s_words - 1] <<= shift_;
	}
	
}

template <size_t N>
BitArr<N>& BitArr<N>::operator>>=(size_t shift_)
{
	while(shift_ >= s_bitsInWord)
	{
		shift_ -= s_bitsInWord;
		ShiftRight(s_bitsInWord);
	}
	
	ShiftRight(shift_);

	return *this;
}

template <size_t N>
void BitArr<N>::ShiftRight(size_t shift_)
{
	std::reverse(m_arr, m_arr + s_words);
	std::transform(m_arr, m_arr + s_words - 1, m_arr + 1, m_arr, ShiftWordsRight(shift_));

	if (shift_ == s_bitsInWord)
	{
		m_arr[s_words - 1] = 0;
	}
	else
	{
		m_arr[s_words - 1] >>= shift_;
	}

	std::reverse(m_arr, m_arr + s_words);
}

template <size_t N>
bool BitArr<N>::operator[] (size_t index_) const
{
	size_t arr_index = (index_ / s_bitsInWord);
	index_ -= (arr_index * s_bitsInWord);

	size_t mask = 0;
	mask <<= index_;

	return (bool)((m_arr[arr_index] | mask) << index_);
}

template <size_t N>
typename BitArr<N>::BitProxy BitArr<N>::operator[] (size_t index_)
{
	BitProxy bp(this, index_);

	return bp;
}

template <size_t N>
bool BitArr<N>::operator==(const BitArr<N>& other_) const
{
	return std::equal(m_arr, m_arr +s_words, other_.m_arr);
}

template <size_t N>
bool BitArr<N>::operator!=(const BitArr<N>& other_) const
{
	return !(std::equal(m_arr, m_arr +s_words, other_.m_arr));
}

/*----------------------------------------------------------------------------*/
/*								Proxy Functions								  */
/*----------------------------------------------------------------------------*/

template <size_t N>
BitArr<N>::BitProxy::BitProxy(BitArr<N>* arr_, size_t index_)
	: m_arr(arr_)
	, m_index(index_)
{ }

template <size_t N>
BitArr<N>::BitProxy::~BitProxy()
{ }

template <size_t N>
BitArr<N>::BitProxy::BitProxy(const BitProxy& other_)
{
	m_arr = other_.m_arr;
	m_index = other_.m_index;
}

template <size_t N>
typename BitArr<N>::BitProxy& BitArr<N>::BitProxy::operator=(bool value_)
{
	if (value_ != (*m_arr)[m_index])
	{
		m_arr->Flip(m_index);
	}

	return *this;
}

template <size_t N>
BitArr<N>::BitProxy::operator bool() const
{
	return (*static_cast<const BitArr*>(m_arr))[m_index];
}

}	// namespcae ilrd

/*----------------------------------------------------------------------------*/
/*								Auxiliary Functions							  */
/*----------------------------------------------------------------------------*/

namespace
{

size_t CountInWord(size_t word)
{
	static int flag = 0;
	static char count_lut[num_of_bits_orders_in_byte] = { 0 };
	unsigned char* byte_from_word = (unsigned char *)&word;
	size_t result = 0;

	if (flag == 0)
	{
		InitCountLut(count_lut);
		flag = 1;
	}

	for (size_t i = 0; i < 8; ++i)
	{
		result += count_lut[*byte_from_word];
		++byte_from_word;
	}

	return result;
}

void InitCountLut(char count_lut[num_of_bits_orders_in_byte])
{
	for (size_t i = 0; i < num_of_bits_orders_in_byte; ++i)
	{
		count_lut[i] = (char)CountByNumber(i);
	}
}

size_t CountByNumber(size_t num)
{
	num = (num & 0x5555555555555555) + ((num >> 1) & 0x5555555555555555);
	num = (num & 0x3333333333333333) + ((num >> 2) & 0x3333333333333333);
	num = (num & 0x0f0f0f0f0f0f0f0f) + ((num >> 4) & 0x0f0f0f0f0f0f0f0f);
	num = (num & 0x00ff00ff00ff00ff) + ((num >> 8) & 0x00ff00ff00ff00ff);
	num = (num & 0x0000ffff0000ffff) + ((num >> 16) & 0x0000ffff0000ffff);
	num = (num & 0x00000000ffffffff) + ((num >> 32) & 0x00000000ffffffff);
	
	return (size_t)num;
}

size_t FlipWord(size_t word)
{
	return ~word;
}

size_t OrWords(size_t word1, size_t word2)
{
	return (word1 | word2);
}

size_t AndWords(size_t word1, size_t word2)
{
	return (word1 & word2);
}

size_t XorWords(size_t word1, size_t word2)
{
	return (word1 ^ word2);
}

std::string WordToString(size_t word)
{
	size_t mask = 1;
	std::string str;
//	size_t i = 0;

	for (size_t i = 0; i < s_bitsInWord; ++i)
	{
		str.push_back((word & mask) + '0');
		word >>= 1;
	}

	std::reverse(str.begin(), str.end());
	str += '\n';

	return str;
}

}	// anonymous namespace

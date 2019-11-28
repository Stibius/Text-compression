#pragma once

#include "BlockCoder.h"

#include <vector>

/**
*   Encoder/Decoder using the BWT transform
*/
class BWTCoder : public BlockCoder
{
private:

	class StringPermutation
	{
	private:

		const unsigned char* m_first = nullptr; //!< points to the first character of this permutation in the string
		uint32_t m_startIndex = 0;              //!< index of the first character of this permutation in the string
		uint32_t m_length = 0;                  //!< length of the string

	public:

		StringPermutation() = default;

		StringPermutation(const std::string& str, uint32_t startIndex);

		StringPermutation(const StringPermutation& other) = default;

		StringPermutation(StringPermutation&& other) = default;

		StringPermutation& operator=(const StringPermutation& other) = default;

		StringPermutation& operator=(StringPermutation&& other) = default;

		~StringPermutation() = default;

		void set(const std::string& str, uint32_t startIndex);

		//lexicographical comparisons of permutations

		bool operator==(const StringPermutation& other) const;

		bool operator!=(const StringPermutation& other) const;

		bool operator<(const StringPermutation& other) const;

		bool operator<=(const StringPermutation& other) const;

		bool operator>(const StringPermutation& other) const;

		bool operator>=(const StringPermutation& other) const;
	};

	/**
	*   Sorts elements in the range [first,last) in ascending order using Merge Sort algorithm
	*   The range [tFirst,tLast) must have the same length as the range [first,last), it's elements are sorted in the same order
	*   Each element in the range [tFirst,tLast) must contain the value of the index of the element before sorting,
	*   so after sorting we get a vector where t[new  index] = old index
	*   \param first iterator to the first element of the range to be sorted
	*   \param last iterator to the element after the last element of the range to be sorted
	*   \param tFirst iterator to the first element of the range where the mapping will be created
	*   \param tLast iterator to the element after the last element of the range where the mapping will be created
	*/
	template
	<typename RandomAccessIterator>
	void sortRange(RandomAccessIterator first, RandomAccessIterator last, std::vector<uint32_t>::iterator tFirst, std::vector<uint32_t>::iterator tLast) const;

	/**
	*   \brief Sorts elements in the range [first,last) in ascending order using Merge Sort algorithm
	*   \param first iterator to the first element of the range to be sorted
	*   \param last iterator to the element after the last element of the range to be sorted
	*   \return vector t where t[new  index] = old index
	*/
	template
	<typename RandomAccessIterator>
	std::vector<uint32_t> mergeSort(RandomAccessIterator first, RandomAccessIterator last) const;

	/**
	*   \brief Determines the lexicographical order of all permutations of the input string
	*   \param str string whose permutatios will be sorted
	*   \return vector t where t[lexicographical order of given permutation] = index of the first character of given permutation in the string
	*/
	std::vector<uint32_t> sortPermutations(const std::string& str) const;

public:

	/**
	*   size of encoded BWT index for a single block in bytes 
	*   value of the index cannot be greater than the value of the size of a block
	*/
	static constexpr int INDEX_SIZE = sizeof(uint32_t);

	/**
	*   \brief Encodes the input string using BWT transform
	*   \param input input string (uncoded)
	*   \return encoded string
	*/
	std::string encode(const std::string& input) const override;

	/**
	*   \brief Decodes the input string using BWT transform
	*   \param input input string (encoded)
	*   \return decoded string
	*/
	std::string decode(const std::string& input) const override;
};

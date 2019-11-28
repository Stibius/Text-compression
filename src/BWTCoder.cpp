#include "BWTCoder.h"

#include <algorithm>
#include <vector>

template<typename RandomAccessIterator>
void BWTCoder::sortRange(RandomAccessIterator first, RandomAccessIterator last, std::vector<uint32_t>::iterator tFirst, std::vector<uint32_t>::iterator tLast) const
{
	if (first < last - 1) //if the given range contains at least 2 elements
	{
		auto elementCount = last - first;

		//element which follows after the last element of the first half of the range
		RandomAccessIterator midLast = first + (elementCount / 2);
		std::vector<uint32_t>::iterator tMidLast = tFirst + (elementCount / 2);

		//first element of the second half of the range
		RandomAccessIterator midFirst = midLast;
		std::vector<uint32_t>::iterator tMidFirst = tMidLast;

		sortRange(first, midLast, tFirst, tMidLast); //recursively sort the first half of the range
		sortRange(midFirst, last, tMidFirst, tLast); //recursively sort the second half of the range

		//sort the entire range knowing that both of it's halves are already sorted

		using value_type = typename std::iterator_traits<RandomAccessIterator>::value_type; //data type of the sorted values

		//allocate temporary vectors for the sorted values
		std::vector<value_type> sorted(elementCount);
		std::vector<uint32_t> tSorted(elementCount);

		//iterators which assign values to the sorted vectors
		typename std::vector<value_type>::iterator sortedIt = sorted.begin();
		std::vector<uint32_t>::iterator sortedTIt = tSorted.begin();

		RandomAccessIterator it1 = first;    //iterates over the first half of the range
		RandomAccessIterator it2 = midFirst; //iterates over the second half of the range

		bool done = false; //will be set to true when the entire range is sorted

		while (!done) //while the entire range isn't sorted
		{
			//if the current value in the first half of the range is lesser or equal than the current value in the second half of the range
			if (*it1 <= *it2)
			{
				//assign it to the current position in the sorted vectors
				*sortedIt = *it1;
				*sortedTIt = *(tFirst + (it1 - first));

				//move to the next position in sorted vectors
				sortedIt++;
				sortedTIt++;

				if (it1 < midLast - 1) //we are not at the end of the first half of the range yet
				{
					it1++; //move to the next position in the first half of the range
				}
				else //we are at the end of the first half of the range
				{
					//add the remainder of the second half of the range to the end of the sorted vectors
					while (it2 < last)
					{
						//assign the current value from the second half of the range to the current position in the sorted vectors
						*sortedIt = *it2;
						*sortedTIt = *(tFirst + (it2 - first));

						//move to the next position in sorted vectors
						sortedIt++;
						sortedTIt++;

						it2++; //move to the next position in the second half of the range
					}

					done = true; //the entire range is sorted
				}
			}
			else //if the current value in the second half of the range is lesser than the current value in the first half of the range
			{
				//assign it to the current position in the sorted vectors
				*sortedIt = *it2;
				*sortedTIt = *(tFirst + (it2 - first));

				//move to the next position in sorted vectors
				sortedIt++;
				sortedTIt++;

				if (it2 < last - 1) //we are not at the end of the second half of the range yet
				{
					it2++; //move to the next position in the second half of the range
				}
				else //we are at the end of the second half of the range
				{
					//add the remainder of the first half of the range to the end of the sorted vectors
					while (it1 < midLast)
					{
						//assign the current value from the first half of the range to the current position in the sorted vectors
						*sortedIt = *it1;
						*sortedTIt = *(tFirst + (it1 - first));

						//move to the next position in sorted vectors
						sortedIt++;
						sortedTIt++;

						it1++; //move to the next position in the first half of the range
					}

					done = true; //the entire range is sorted
				}
			}
		}

		//overwrite the original ranges with the sorted ranges
		std::copy(sorted.begin(), sorted.end(), first);
		std::copy(tSorted.begin(), tSorted.end(), tFirst);
	}
}

template<typename RandomAccessIterator>
std::vector<uint32_t> BWTCoder::mergeSort(RandomAccessIterator first, RandomAccessIterator last) const
{
	auto elementCount = last - first;

	//allocate space for the result
	std::vector<uint32_t> t;
	t.reserve(elementCount);

	//each element of the result will have the value of it's unsorted index
	for (int i = 0; i < elementCount; ++i)
	{
		t.push_back(i);
	}

	//sort
	sortRange(first, last, t.begin(), t.end());

	return t;
}

std::vector<uint32_t> BWTCoder::sortPermutations(const std::string& str) const
{
	//create vector of all permutations 
	std::vector<StringPermutation> permutations;
	permutations.reserve(str.size());

	for (uint32_t i = 0; i < str.size(); ++i)
	{
		permutations.push_back(StringPermutation(str, i));
	}

	//sort permutations
	std::vector<uint32_t> t = mergeSort(permutations.begin(), permutations.end());

	return t;
}

std::string BWTCoder::encode(const std::string& input) const
{
	//allocate space for the result
	std::string output;
	output.reserve(input.size() + INDEX_SIZE);

	//get vector t where t[lexicographical order of given permutation] = index of the first character of given permutation in the string
	std::vector<uint32_t> t = sortPermutations(input);

	uint32_t index; //BWT index

	//iterate over all permutatins in sorted order
	for (uint32_t i = 0; i < t.size(); ++i)
	{
		//take the last character of current permutation
		output += input[(t[i] + input.size() - 1) % input.size()];
		//the value of the BWT index is a position where t contains 0
		if (t[i] == 0)
		{
			index = i;
		}
	}

	//save the BWT index at the beginning of output
	output.insert(0, encodeNumber(index, INDEX_SIZE));

	return output;
}

std::string BWTCoder::decode(const std::string& input) const
{
	std::string last = input.substr(INDEX_SIZE);  //last column of sorted permutation matrix
	std::string first = input.substr(INDEX_SIZE); //here will be the first column of sorted permutation matrix

	//get first by sorting last
	//also get vector t where t[new  index] = old index
	std::vector<uint32_t> t = mergeSort(first.begin(), first.end());

	//we need a mapping from the old ordering to the new
	std::vector<uint32_t> tRev(t.size());
	for (uint32_t i = 0; i < tRev.size(); ++i)
	{
		tRev[t[i]] = i;
	}

	//std::string output(last.size(), '0'); //allocate string for output
	std::string output;
	output.reserve(last.size());

	uint32_t index = decodeNumber(input.substr(0, INDEX_SIZE)); //decode BWT index from the beginning of input

	//decode the rest
	for (uint32_t i = 0; i < last.size(); ++i)
	{
		uint32_t prevIndex;

		if (i == 0)
		{
			output += last[index];
			prevIndex = index;
		}
		else
		{
			output += last[tRev[prevIndex]];
			prevIndex = tRev[prevIndex];
		}
	}

	std::reverse(output.begin(), output.end());

	return output;
}

BWTCoder::StringPermutation::StringPermutation(const std::string& str, uint32_t startIndex)
{
	set(str, startIndex);
}

void BWTCoder::StringPermutation::set(const std::string& str, uint32_t startIndex)
{
	m_first = reinterpret_cast<const unsigned char*>(str.data() + startIndex);
	m_startIndex = startIndex;
	m_length = str.length();
}

bool BWTCoder::StringPermutation::operator==(const StringPermutation& other) const
{
	uint32_t index1 = m_startIndex; //current index in first permutation
	uint32_t index2 = other.m_startIndex; //current index in second permutation
	const unsigned char* c1 = m_first;   //points to the value at the current index in first permutation
	const unsigned char* c2 = other.m_first;   //points to the value at the current index in second permutation

	//perform as many comparisons as there are characters in the string
	for (uint32_t i = 0; i < m_length; ++i)
	{
		//if one of the indices passed the end of the string return it back to the beginning
		if (index1 >= m_length)
		{
			index1 = 0;
			c1 -= m_length;
		}
		if (index2 >= m_length)
		{
			index2 = 0;
			c2 -= m_length;
		}

		//compare characters at the current indices
		if (*c1 != *c2) return false;

		//if both characters are equal, continue
		index1++;
		index2++;
		c1++;
		c2++;
	}

	//both permutations are the same
	return true;
}

bool BWTCoder::StringPermutation::operator!=(const StringPermutation& other) const
{
	return !(*this == other);
}

bool BWTCoder::StringPermutation::operator<(const StringPermutation& other) const
{
	uint32_t index1 = m_startIndex; //current index in first permutation
	uint32_t index2 = other.m_startIndex; //current index in second permutation
	const unsigned char* c1 = m_first;   //points to the value at the current index in first permutation
	const unsigned char* c2 = other.m_first;   //points to the value at the current index in second permutation

	//perform as many comparisons as there are characters in the string
	for (uint32_t i = 0; i < m_length; ++i)
	{
		//if one of the indices passed the end of the string return it back to the beginning
		if (index1 >= m_length)
		{
			index1 = 0;
			c1 -= m_length;
		}
		if (index2 >= m_length)
		{
			index2 = 0;
			c2 -= m_length;
		}

		//compare characters at the current indices
		if (*c1 < *c2) return true;
		if (*c1 > *c2) return false;

		//if both characters are equal, continue
		index1++;
		index2++;
		c1++;
		c2++;
	}

	//both permutations are the same
	return false;
}

bool BWTCoder::StringPermutation::operator<=(const StringPermutation& other) const
{
	uint32_t index1 = m_startIndex; //current index in first permutation
	uint32_t index2 = other.m_startIndex; //current index in second permutation
	const unsigned char* c1 = m_first;   //points to the value at the current index in first permutation
	const unsigned char* c2 = other.m_first;   //points to the value at the current index in second permutation

	//perform as many comparisons as there are characters in the string
	for (uint32_t i = 0; i < m_length; ++i)
	{
		//if one of the indices passed the end of the string return it back to the beginning
		if (index1 >= m_length)
		{
			index1 = 0;
			c1 -= m_length;
		}
		if (index2 >= m_length)
		{
			index2 = 0;
			c2 -= m_length;
		}

		//compare characters at the current indices
		if (*c1 < *c2) return true;
		if (*c1 > *c2) return false;

		//if both characters are equal, continue
		index1++;
		index2++;
		c1++;
		c2++;
	}

	//both permutations are the same
	return true;
}

bool BWTCoder::StringPermutation::operator>(const StringPermutation& other) const
{
	return !(*this <= other);
}

bool BWTCoder::StringPermutation::operator>=(const StringPermutation& other) const
{
	return !(*this < other);
}

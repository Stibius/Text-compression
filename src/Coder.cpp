#include "Coder.h"

#include <algorithm>
#include <climits>

std::string Coder::encodeNumber(uint64_t number, int byteCount) const
{
	//allocate space for result
	std::string result;
	result.reserve(byteCount);

	//go through the bytes of the input value from the right
	for (int i = 0; i < byteCount; ++i)
	{
		//erase the bytes to the left of current byte
		uint64_t byte = number << (sizeof(uint64_t) - 1 - i) * CHAR_BIT;
		//erase the bytes to the right of current byte
		byte = byte >> (sizeof(uint64_t) - 1) * CHAR_BIT;
		//insert current byte to the start of the output
		//*(result.end() - 1 - i) = static_cast<unsigned char>(byte);
		result += byte;
	}

	std::reverse(result.begin(), result.end());

	return result;
}

uint64_t Coder::decodeNumber(const std::string& str) const
{
	uint64_t result = 0;

	//go through the bytes of the input string from the left
	for (int i = 0; i < str.size(); ++i)
	{
		//insert current byte to the end of the output
		result += static_cast<unsigned char>(str[i]) << (CHAR_BIT * ((str.size() - 1) - i));
	}

	return result;
}

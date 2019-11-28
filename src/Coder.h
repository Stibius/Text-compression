#pragma once

#include <cstdint>
#include <string>

/**
*   Base class for Encoders/Decoders. Contains common functions.
*/
class Coder
{
protected:

	/**
	*   \brief Encodes given 64bit unsigned integer as a string of bytes
	*   \param value value to encode
	*   \param byteCount number of rightmost bytes of the input value to encode
	*   \return input value encoded as a string of bytes
	*/
	std::string encodeNumber(uint64_t value, int byteCount) const;

	/**
	*   \brief Decodes given string of bytes as a 64bit unsigned integer
	*   \param str string of bytes to decode
	*   \return 64bit unsigned integer corresponding to the input string of bytes
	*/
	uint64_t decodeNumber(const std::string& str) const;
};

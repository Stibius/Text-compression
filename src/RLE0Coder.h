#pragma once

#include "BlockCoder.h"

/**
*   Encoder/Decoder using RLE0 coding
*/
class RLE0Coder : public BlockCoder
{
public:

	/**
	*   \brief Encodes the input string using RLE0 coding
	*   \param input input string (uncoded)
	*   \return encoded string
	*/
	std::string encode(const std::string& input) const override;

	/**
	*   \brief Decodes the input string using RLE0 coding
	*   \param input input string (encoded)
	*   \return decoded string
	*/
	std::string decode(const std::string& input) const override;
};

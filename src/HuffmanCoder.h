#pragma once

#include "BlockCoder.h"
#include "HuffmanTree.h"

/**
*   Encoder/Decoder using static Huffman coding
*/
class HuffmanCoder : public BlockCoder
{
private:

	/**
	*   \brief Computes histogram of characters in the input string
	*   \param str input string
	*   \return vector t where t[character] = number of occurences of character in the input string,
	*	        vector has values for all possible values of unsigned char type
	*/
	std::array<uint32_t, UCHAR_MAX + 1> computeHistogram(const std::string& str) const;

public:

	/**
	*   \brief Encodes the input string using static Huffman coding
	*   \param input input string (uncoded)
	*   \return encoded string
	*/
	std::string encode(const std::string& input) const override;

	/**
	*   \brief Decodes the input string using static Huffman coding
	*   \param input input string (encoded)
	*   \return decoded string
	*/
	std::string decode(const std::string& input) const override;
};

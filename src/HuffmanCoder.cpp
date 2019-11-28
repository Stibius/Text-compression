#include "HuffmanCoder.h"

#include <climits>

std::array<uint32_t, UCHAR_MAX + 1> HuffmanCoder::computeHistogram(const std::string& str) const
{
	//allocate space for histogram
	std::array<uint32_t, UCHAR_MAX + 1> hist = {};

	//compute histogram
	for (unsigned char ch : str)
	{
		hist[ch]++;
	}

	return hist;
}

std::string HuffmanCoder::encode(const std::string& input) const
{
	std::string output;

	//compute histogram from input string
	std::array<uint32_t, UCHAR_MAX + 1> hist = computeHistogram(input);

	//encode the histogram at the beginning of output
	for (uint32_t histValue : hist)
	{
		output += encodeNumber(histValue, sizeof(uint32_t));
	}

	//encode the size of input string
	output += encodeNumber(input.size(), sizeof(uint32_t));

	//build Huffman tree from the histogram
	HuffmanTree huffmanTree = HuffmanTree(hist);

	//create mapping of characters to Huffman codes
	const std::unordered_map<unsigned char, HuffmanCode>& codeMap = huffmanTree.getCodes();

	//encode the input using the mapping

	//index of bit in the last byte of the output where we will continue to write more data, 0 if another byte needs to be added
	int index = 0;

	//for all input values
	for (unsigned char inputValue : input)
	{
		if (index == 0) //add new byte at the end of output
		{
			output += codeMap.at(inputValue).m_code;
			index = codeMap.at(inputValue).m_bitCount % CHAR_BIT;
		}
		else
		{
			int remainingBits = codeMap.at(inputValue).m_bitCount; //number of bits that need to be written to output

			//for all bytes of Huffman code
			for (unsigned char code : codeMap.at(inputValue).m_code)
			{
				//add bits at the end of the last byte of output
				output.back() += code >> index;

				remainingBits -= CHAR_BIT - index;
				if (remainingBits < 0) remainingBits = 0;

				if (remainingBits > 0) //still need to write some bits
				{
					//write rest of bits to new byte
					output += code << CHAR_BIT - index;

					remainingBits -= index;
					if (remainingBits < 0) remainingBits = 0;
				}

				index = (index + codeMap.at(inputValue).m_bitCount - remainingBits) % CHAR_BIT;
			}
		}
	}

	return output;
}

std::string HuffmanCoder::decode(const std::string& input) const
{
	//get histogram from the beginning of input
	//allocate space for histogram
	std::array<uint32_t, UCHAR_MAX + 1> hist;
	for (int i = 0; i <= UCHAR_MAX; ++i)
	{
		hist[i] = decodeNumber(input.substr(i * sizeof(uint32_t), sizeof(uint32_t)));
	}

	//build Huffman tree from the histogram
	HuffmanTree huffmanTree = HuffmanTree(hist);

	//read the number of bytes of uncoded data
	uint32_t size = decodeNumber(input.substr((UCHAR_MAX + 1) * sizeof(uint32_t), sizeof(uint32_t)));

	//decode the output using Huffman tree

	std::string output;
	output.reserve(size);

	int globalBit = ((UCHAR_MAX + 2) * sizeof(uint32_t)) * CHAR_BIT; //order of the current bit in the entire input
	const HuffmanTreeNode* node = huffmanTree.getRoot(); //current Huffman tree node
	while (output.size() < size) //while all values haven't been decoded
	{
		int byte = globalBit / CHAR_BIT; //order of the current byte of input
		int localBit = globalBit % CHAR_BIT; //order of the current bit in the current byte

		//determine if current bit is 1
		bool one = (static_cast<unsigned char>(input[byte]) & (1 << (CHAR_BIT - 1 - localBit)) && 1);

		//if current bit is 0 and current Huffman tree node has corresponding child
		if (!one && node->m_zero != nullptr)
		{
			node = node->m_zero.get(); //move to the child
		}
		//if current bit is 1 and current Huffman tree node has corresponding child
		else if (one && node->m_one != nullptr)
		{
			node = node->m_one.get(); //move to the child
		}

		//we reached a leaf node of the Huffman tree
		if (node->m_zero == nullptr && node->m_one == nullptr)
		{
			output += *node->m_value; //send the value stored in the leaf node to output
			node = huffmanTree.getRoot(); //go back to the root of Huffman tree
		}

		globalBit++; //go to the next bit of input
	}

	return output;
}

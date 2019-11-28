
#pragma once

#include <fstream>

#include "StreamCoder.h"
#include "HuffmanCoder.h"
#include "BWTCoder.h"
#include "MTFCoder.h"
#include "RLE0Coder.h"

/**
*   Encoder/Decoder which uses a combination of other coders/decoders:
*   Encoding: BWT -> MTF -> RLE -> Huffman
*   Decoding: Huffman -> RLE -> MTF -> BWT
*/
class BWT_MTF_RLE_Huffman_Coder : public StreamCoder
{
private:

	HuffmanCoder m_huffmanCoder;
	BWTCoder m_BWTCoder;
	MTFCoder m_MTFCoder;
	RLE0Coder m_RLE0Coder;

public:

	/**
    *   \brief Encodes the input stream using this sequence of encoders: BWT -> MTF -> RLE -> Huffman
    *   \param log log of the encoding process gets saved here
	*   \param inputStream input stream (uncoded)
	*   \param outputStream output stream (encoded)
	*   \return 0 on success, -1 on error
    */
	bool encode(Log& log, std::istream& inputStream, std::ostream& outputStream) const override;

	/**
	*   \brief Decodes the input stream using this sequence of decoders: Huffman -> RLE -> MTF -> BWT
	*   \param log log of the decoding process gets saved here
	*   \param inputStream input stream (encoded)
	*   \param outputStream output stream (decoded)
	*   \return 0 on success, -1 on error
	*/
	bool decode(Log& bwted, std::istream& inputStream, std::ostream& outputStream) const override;

};

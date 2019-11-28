

#include "BWT_MTF_RLE_Huffman_Coder.h"

#include <algorithm>
#include <cmath>

bool BWT_MTF_RLE_Huffman_Coder::encode(Log& log, std::istream& inputStream, std::ostream& outputStream) const
{
	std::string buffer(m_blockSize, '0'); //auxiliary buffer for reading blocks of data from input stream
    std::string block;                    //input blocks of data will then be copied here and all other processing will be done on this string
    
	//initialize the log values
    log.m_uncodedSize = 0;
    log.m_codedSize = 0;
    
	//for all blocks of input data
    do
    {
		//read one block of input data
		inputStream.read(buffer.data(), m_blockSize);
		std::streamsize count = inputStream.gcount();
		//check for errors during reading
        if (!inputStream && !inputStream.eof()) return false;
		//if nothing was read, end
		if (count == 0) break;
		//update uncoded data size
        log.m_uncodedSize += count;
		//copy input block to string
        block.assign(buffer.data(), count);

		//perform all the encoding steps on the block
        block = m_BWTCoder.encode(block); 
        block = m_MTFCoder.encode(block);
        block = m_RLE0Coder.encode(block);
		block = m_huffmanCoder.encode(block);
		//encode the size of the encoded block before the encoded block
		block = encodeNumber(block.size(), sizeof(uint32_t)) + block;

		//update the size of encoded data in the log
        log.m_codedSize += block.size();  
		//write encoded block to output stream
		outputStream.write(block.c_str(), block.size());

		//check for errors during writing
        if (!outputStream) return false;

    } while (!inputStream.eof()); //check for end of file
    
    return true;
}

bool BWT_MTF_RLE_Huffman_Coder::decode(Log& log, std::istream& inputStream, std::ostream& outputStream) const
{
	//auxiliary buffer for reading blocks of data from input stream
	//maximum possible size of encoded block (without the information about block size):
	//(UCHAR_MAX + 1) * sizeof(u_int32_t) = size of histogram
	//sizeof(u_int32_t) = size of the value of the number of values encoded by Huffman
	//+ size of BWT index and size of uncoded block
	std::string buffer(((UCHAR_MAX + 1) * sizeof(uint32_t)) + sizeof(uint32_t) + BWTCoder::INDEX_SIZE + m_blockSize, '0');
    std::string block;           //input blocks of data will then be copied here and all other processing will be done on this string
	std::string blockSizeString; //size of encoded block will be in here
    
	//initialize the log values
    log.m_uncodedSize = 0;
    log.m_codedSize = 0;

	//for all blocks of input data
    do
    {
		//read the size of encoded block 
		inputStream.read(buffer.data(), sizeof(uint32_t));
		std::streamsize count = inputStream.gcount();
		//check for errors during reading
        if (!inputStream) return false;
		//if nothing was read, end
		if (count == 0) break;
		//update encoded data size
        log.m_codedSize += count;
		//copy size of encoded block to string
		blockSizeString.assign(buffer.data(), count);
		//decode size of encoded block
		uint64_t blockSize = decodeNumber(blockSizeString);

		//read the encoded block
		inputStream.read(buffer.data(), blockSize);
		count = inputStream.gcount();
		//check for errors during reading
		if (!inputStream || count == 0) return false;
		//update encoded data size
		log.m_codedSize += count;
		//copy encoded block to string
		block.assign(buffer.data(), count);

		//perform all the decoding steps on the block
		block = m_huffmanCoder.decode(block);
        block = m_RLE0Coder.decode(block);
        block = m_MTFCoder.decode(block);
        block = m_BWTCoder.decode(block);

		//update decoded data size
        log.m_uncodedSize += block.size();   
		//write decoded block to output stream
		outputStream.write(block.c_str(), block.size());

		//check for errors during writing
        if (!outputStream) return false;

    } while (!inputStream.eof()); //check for end of file
	
    return true;
}
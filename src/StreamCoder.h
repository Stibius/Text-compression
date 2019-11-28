#pragma once

#include "Coder.h"

#include <iostream>

/**
*   log of the encoding/decoding process
*/
struct Log
{
	int64_t m_uncodedSize = 0; //!< size of uncoded data in bytes
	int64_t m_codedSize = 0;   //!< size of encoded data in bytes
};

/**
*   Base class for Coders/Decoders which operate on a stream of data
*/
class StreamCoder : public Coder
{
protected:

	int m_blockSize = 500000; //!< maximum number of bytes to encode/decode at once in a single iteration

public:

	/**
	*   \brief Sets the maximum number of bytes to encode/decode at once in a single iteration
	*   \param blockSize maximum number of bytes to encode/decode at once in a single iteration
	*/
	void setBlockSize(int blockSize);

	/**
	*   \brief Gets the maximum number of bytes to encode/decode at once in a single iteration
	*   \return maximum number of bytes to encode/decode at once in a single iteration
	*/
	int getBlockSize() const;

	/**
	*   \brief Encodes a stream of data
	*   \param log log of the encoding process gets saved here
	*   \param inputStream input stream (uncoded)
	*   \param outputStream output stream (encoded)
	*   \return 0 on success, -1 on error
	*/
	virtual bool encode(Log& log, std::istream& inputStream, std::ostream& outputStream) const = 0;

	/**
	*   \brief Decodes a stream of data
	*   \param log log of the decoding process gets saved here
	*   \param inputStream input stream (encoded)
	*   \param outputStream output stream (decoded)
	*   \return 0 on success, -1 on error
	*/
	virtual bool decode(Log& log, std::istream& inputStream, std::ostream& outputStream) const = 0;
};

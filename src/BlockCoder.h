#pragma once

#include "Coder.h"

#include <string>

/**
*   Base class for Encoders/Decoders which operate on a block of data. 
*/
class BlockCoder : public Coder
{
public:

	/**
	*   \brief Encodes input string
	*   \param input input string (uncoded)
	*   \return encoded string
	*/
	virtual std::string encode(const std::string& input) const = 0;

	/**
	*   \brief Decodes input string
	*   \param input input string (coded)
	*   \return decoded string
	*/
	virtual std::string decode(const std::string& input) const = 0;
};

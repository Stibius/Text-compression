#include "RLE0Coder.h"

#include <cmath>

std::string RLE0Coder::encode(const std::string& input) const
{
	std::string output;

	//for all input characters
	for (uint32_t i = 0; i < input.size(); ++i)
	{
		//if it is a special symbol
		if (input[i] == '@')
		{
			output += input[i]; //copy to output
			output += '\0';     //indicates that this isn't the beginning of encoded sequence of 0s
		}
		//if it is 0
		else if (input[i] == '\0')
		{
			//find out the number of repeated 0s

			uint32_t repeatCount = 1;

			uint32_t j = 0;

			for (j = i + 1; j < input.size(); j++)
			{
				if (input[j] != input[i])
				{
					break;
				}

				repeatCount++;
			}

			//it is worth to encode only if there are at least 6 repeats
			if (repeatCount > 5)
			{
				uint32_t bitCount = floor(log2(repeatCount + 1));    //number of bits needed to encode the sequence
				uint32_t number = repeatCount + 1 - (1 << bitCount); //value which will get encoded by bitCount of bits

				output += '@';  //special symbol which indicates possible beginning of encoded sequence
				output += '\1'; //indicates that this is the beginning of encoded sequence

				//convert number to a binary number with bitCount bits
				//encode every 0 bit as '\0' and every 1 bit as '\1'
				for (uint32_t k = 0; k < bitCount; ++k)
				{
					if (number >> (bitCount - 1 - k) & 1)
					{
						output += '\1';
					}
					else
					{
						output += '\0';
					}
				}

				output += '@'; //special symbol which indicates the end of encoded sequence
			}
			else
			{
				//don't encode, just copy input to output
				output.append(repeatCount, input[i]);
			}

			i = j - 1; //continue by the next character after the sequence of 0s
		}
		else
		{
			//don't encode, just copy input to output
			output += input[i];
		}
	}

	return output;
}

std::string RLE0Coder::decode(const std::string& input) const
{
	std::string output;

	//for all input characters
	for (uint32_t i = 0; i < input.size(); ++i)
	{
		//if it is a special symbol
		if (input[i] == '@')
		{
			//if the next value is '\0' then it isn't the beginning of encoded sequence of 0s but just a single '@' value
			if (input[i + 1] == '\0')
			{
				output += input[i]; //just copy input to output
				i++; //skip the '\0' indicator
			}
			//if the next value is '\1' then it is the beginning of encoded sequence of 0s
			else
			{
				uint32_t number = 0;   //value which is encoded by bitCount of bits
				uint32_t bitCount = 0; //number of bits which encode the sequence

				//read values until the special symbol which denotes the end of the sequence, decode the value and count number of bits
				for (uint32_t j = i + 2; j < input.size(); ++j)
				{
					if (input[j] == '@')
					{
						break;
					}
					else if (input[j] == '\0')
					{
						number = number << 1;
					}
					else if (input[j] == '\1')
					{
						number = number << 1;
						number += 1;
					}

					bitCount++;
				}

				//compute the length of the sequence of 0s from the decoded value
				uint32_t repeatCount = (1 << bitCount) + number - 1;

				//add the computed number of 0s to the output
				output.append(repeatCount, '\0');

				//continue by the next character after the sequence of 0s
				i += 1 + bitCount + 1;
			}
		}
		else
		{
			//don't encode, just copy input to output
			output += input[i];
		}
	}

	return output;
}

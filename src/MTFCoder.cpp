#include "MTFCoder.h"

#include <algorithm>
#include <climits>

std::string MTFCoder::encode(const std::string& input) const
{
	//allocate space for alphabet
	std::string alphabet;
	alphabet.reserve(UCHAR_MAX + 1);

	//the alphabet contains all possible characters of unsigned char type
	for (int i = 0; i <= UCHAR_MAX; ++i)
	{
		alphabet += i;
	}

	//allocate space for output
	std::string output;
	output.reserve(input.size());

	//for all input characters
	for (uint32_t i = 0; i < input.size(); ++i)
	{
		//find current input character in the alphabet
		std::string::iterator it = std::find(alphabet.begin(), alphabet.end(), input[i]);

		output += it - alphabet.begin(); //output the number of characters which precede the current input character in the alphabet

		//move the current input character to the beginning of the alphabet
		char c = *it;
		alphabet.erase(it);
		alphabet.insert(alphabet.begin(), c);
	}

	return output;
}

std::string MTFCoder::decode(const std::string& input) const
{
	//allocate space for alphabet
	std::string alphabet;
	alphabet.reserve(UCHAR_MAX + 1);

	//the alphabet contains all possible characters of unsigned char type
	for (int i = 0; i <= UCHAR_MAX; ++i)
	{
		alphabet += i;
	}

	//allocate space for output
	std::string output;
	output.reserve(input.size());

	//for all input characters
	for (uint32_t i = 0; i < input.size(); ++i)
	{
		//the input value is the number of characters which precede the corresponding decoded character in the alphabet
		unsigned char index = static_cast<unsigned char>(input[i]);
		output += alphabet[index];

		//move the decoded character to the beginning of the alphabet
		alphabet.erase(index, 1);
		alphabet.insert(alphabet.begin(), output[i]);
	}

	return output;
}

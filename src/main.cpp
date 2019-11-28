
#include <iostream>
#include <fstream>

#include "BWT_MTF_RLE_Huffman_Coder.h"

int main(int argc, char *argv[])
{
	char action = 0; //action to perform ('c' - code, 'x' - decode, 'h' - help)
	std::ifstream inputStream; 
	std::ofstream outputStream; 
	std::ofstream logStream; 
	BWT_MTF_RLE_Huffman_Coder coder;
	Log log; 
	log.m_uncodedSize = 0;
	log.m_codedSize = 0;
	bool success = true;
    
    for (int i = 1; i < argc; ++i)
	{
		std::string arg = argv[i];

		if (arg == "-i" && i < argc - 1) //name of input file follows, attempt to open it
		{
			inputStream.open(argv[i + 1], std::ifstream::in | std::ifstream::binary);
			if (!inputStream.is_open())
			{
				//unable to open given input file, close everything, print error and exit program
				if (outputStream.is_open()) outputStream.close();
				if (logStream.is_open()) logStream.close();
				std::cout << "The specified input file \"" << argv[i + 1] << "\" couldn't be opened!\n";
				return -1;
			}
		}
		else if (arg == "-o" && i < argc - 1) //name of output file follows, open it
		{
			outputStream.open(argv[i + 1], std::ofstream::out | std::ifstream::binary);
		}
		else if (arg == "-l" && i < argc - 1) //name of log file follows, open it
		{
			logStream.open(argv[i + 1]);
		}
		else if (arg == "-c" || arg == "-x" || arg == "-h") //encode, decode or print help
		{
			action = arg[1]; 
		}
    }

	switch (action) //action to perform 
	{
	case 'c': //encode
        //input and output file are specified
		if (inputStream.is_open() && outputStream.is_open())
		{
	        success = coder.encode(log, inputStream, outputStream);
		}
        //output file isn't specified, use standard output
		else if (inputStream.is_open())
		{
			success = coder.encode(log, inputStream, std::cout);
		}
		//input file isn't specified, use standard input
		else if (outputStream.is_open())
	    {
			success = coder.encode(log, std::cin, outputStream);
		}
		//input and output file aren't specified, use standard input and output
		else
		{
			success = coder.encode(log, std::cin, std::cout);
		}
		break;
	case 'x': //decode
		//input and output file are specified
		if (inputStream.is_open() && outputStream.is_open())
		{
			success = coder.decode(log, inputStream, outputStream);
		}
		//output file isn't specified, use standard output
		else if (inputStream.is_open())
		{
			success = coder.decode(log, inputStream, std::cout);
		}
		//input file isn't specified, use standard input
		else if (outputStream.is_open())
		{
			success = coder.decode(log, std::cin, outputStream);
		}
		//input and output file aren't specified, use standard input and output
		else
		{
			success = coder.decode(log, std::cin, std::cout);
		}
		break;
	case 'h': //print help
		std::cout << "app_name [-i <ifile>] [-o <ofile>] [-l <logFile>] {-c | -x | -h}\n";
        std::cout << "-i <ifile>: input file name <ifile>. If not specified, standard input is used.\n";
		std::cout << "-o <ofile>: output file name <ofile>. If not specified, standard output is used.\n";
		std::cout << "-l <logfile>: log file name <logfile>. If not specified, log is not generated.\n";
		std::cout << "-c: encode the input file.\n";
		std::cout << "-x: decode the input file.\n";
		std::cout << "-h: print help information on the standard output.\n";
		break;
	default:
		break;
	}

    //generate log file if it is required and then close it
	if (logStream.is_open())
	{
		logStream << "uncodedSize = " << log.m_uncodedSize << "\n";
	    logStream << "codedSize = " << log.m_codedSize << "\n";
		
		logStream.close();
	}

    //close input and output file if they are open
	if (inputStream.is_open()) inputStream.close();
	if (outputStream.is_open()) outputStream.close();
    
	if (success == false)
	{
		return -1;
	}
	else
	{
		return 0;
	}
}
#include "StreamCoder.h"

void StreamCoder::setBlockSize(int blockSize)
{
	m_blockSize = blockSize;
}

int StreamCoder::getBlockSize() const
{
	return m_blockSize;
}

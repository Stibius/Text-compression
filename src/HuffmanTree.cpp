#include "HuffmanTree.h"

#include <algorithm>
#include <climits>

bool operator==(const HuffmanTreeNode& lhs, const HuffmanTreeNode& rhs)
{
	return (lhs.m_count == rhs.m_count);
}

bool operator!=(const HuffmanTreeNode& lhs, const HuffmanTreeNode& rhs)
{
	return (lhs.m_count != rhs.m_count);
}

bool operator<(const HuffmanTreeNode& lhs, const HuffmanTreeNode& rhs)
{
	return (lhs.m_count < rhs.m_count);
}

bool operator<=(const HuffmanTreeNode& lhs, const HuffmanTreeNode& rhs)
{
	return (lhs.m_count <= rhs.m_count);
}

bool operator>(const HuffmanTreeNode& lhs, const HuffmanTreeNode& rhs)
{
	return (lhs.m_count > rhs.m_count);
}

bool operator>=(const HuffmanTreeNode& lhs, const HuffmanTreeNode& rhs)
{
	return (lhs.m_count >= rhs.m_count);
}

std::unique_ptr<HuffmanTreeNode> HuffmanTree::copyTree(const std::unique_ptr<HuffmanTreeNode>& root) const
{
	if (root == nullptr)
	{
		return nullptr;
	}

	std::unique_ptr<HuffmanTreeNode> newRoot = std::make_unique<HuffmanTreeNode>();
	newRoot->m_value = root->m_value;
	newRoot->m_count = root->m_count;
	newRoot->m_zero = copyTree(root->m_zero);
	newRoot->m_one = copyTree(root->m_one);

	return newRoot;
}

void HuffmanTree::computeHuffmanCodes(const std::unique_ptr<HuffmanTreeNode>& root, HuffmanCode currentCode) 
{
	if (root == nullptr)
	{
		return;
	}

	//we are not in a leaf node
	if (root->m_zero != nullptr && root->m_one != nullptr)
	{
		//position of current bit in the last byte of currentCode (from the left), 0 if new byte needs to be added
		int pos = currentCode.m_bitCount % CHAR_BIT;

		//add 0
		HuffmanCode zeroCode = currentCode;
		if (pos == 0) //new byte needs to be added (all 0s)
		{
			zeroCode.m_code.push_back(0);
		}
		zeroCode.m_bitCount++;

		//continue in the zero subtree
		computeHuffmanCodes(root->m_zero, zeroCode);

		//add 1
		HuffmanCode oneCode = currentCode;
		if (pos == 0) //new byte needs to be added (with 1 on the left)
		{
			oneCode.m_code.push_back(1 << (CHAR_BIT - 1));
		}
		else //add 1 to the end
		{
			oneCode.m_code.back() += (1 << (CHAR_BIT - 1 - pos));
		}
		oneCode.m_bitCount++;

		//continue in the one subtree
		computeHuffmanCodes(root->m_one, oneCode);
	}
	//we are in a leaf node
	else
	{
		//save the resulting Huffman code 
		m_codes[*root->m_value] = currentCode;
	}
}

HuffmanTree::HuffmanTree(const std::array<uint32_t, UCHAR_MAX + 1>& histogram)
{
	build(histogram);
}

HuffmanTree::HuffmanTree(const HuffmanTree& other)
	: m_root(copyTree(other.m_root))
	, m_codes(other.m_codes)
{
}

HuffmanTree& HuffmanTree::operator=(const HuffmanTree& other)
{
	if (this == &other)
	{
		return *this;
	}

	m_root = copyTree(other.m_root);
	m_codes = other.m_codes;

	return *this;
}

void HuffmanTree::build(const std::array<uint32_t, UCHAR_MAX + 1>& histogram)
{
	std::vector<std::unique_ptr<HuffmanTreeNode>> nodes;  //vector of nodes which haven't been connected yet
	nodes.reserve(UCHAR_MAX + 1);

	//create leaf nodes for all possible characters of unsigned char type
	for (int i = 0; i <= UCHAR_MAX; ++i)
	{
		if (histogram[i] == 0) continue;  //don't create leaf nodes for characters which have 0 occurences

		//create a leaf node for a given character
		std::unique_ptr<HuffmanTreeNode> newNode = std::make_unique<HuffmanTreeNode>();
		newNode->m_zero = nullptr;
		newNode->m_one = nullptr;
		newNode->m_value = i;
		newNode->m_count = histogram[i];
		nodes.push_back(std::move(newNode));
	}

	//sort the leaf nodes in ascending order according to the number of occurences of each character
	std::sort(nodes.begin(), nodes.end(), [](const std::unique_ptr<HuffmanTreeNode>& node1, const std::unique_ptr<HuffmanTreeNode>& node2) 
	{
		return *node1 < *node2;
	});

	//while all nodes haven't been joined
	while (nodes.size() > 1)
	{
		//create a new node, it's children will be the first two nodes from the vector (they have the smallest number of occurences of their characters)
		std::unique_ptr<HuffmanTreeNode> newNode = std::make_unique<HuffmanTreeNode>();
		newNode->m_zero = std::move(nodes[0]);
		newNode->m_one = std::move(nodes[1]);
		//the new node will have a number of occurences equal to the sum of numbers of occurences of it's children
		newNode->m_count = newNode->m_zero->m_count + newNode->m_one->m_count;
		//erase these two children from the vector
		nodes.erase(nodes.begin(), nodes.begin() + 2);

		//insert the new node to it's proper place in the vector (according to it's number of occurences)
		auto it = std::find_if(nodes.begin(), nodes.end(), [&newNode](const std::unique_ptr<HuffmanTreeNode>& node)
		{
			return *node >= *newNode;
		});
		nodes.insert(it, std::move(newNode));
	}

	//the last remaining node in the vector is the root of the Huffman tree
	m_root = std::move(nodes[0]);

	//compute the mapping of characters to Huffman codes 
	computeHuffmanCodes();
}

const std::unordered_map<unsigned char, HuffmanCode>& HuffmanTree::getCodes() const
{
	return m_codes;
}

const HuffmanTreeNode* HuffmanTree::getRoot() const
{
	return m_root.get();
}

void HuffmanTree::computeHuffmanCodes() 
{
	m_codes.clear();

	//compute the mapping of characters to Huffman codes 
	HuffmanCode startCode;
	computeHuffmanCodes(m_root, startCode);
}



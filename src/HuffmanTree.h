#pragma once

#include <array>
#include <cstdint>
#include <unordered_map>
#include <memory>
#include <optional>
#include <string>
#include <vector>

struct HuffmanCode
{
	std::string m_code; //!< string of bytes
	int m_bitCount = 0; //!< number of starting bits which constitute Huffman code, rest are 0
};

struct HuffmanTreeNode
{
	std::unique_ptr<HuffmanTreeNode> m_zero; //!< pointer to a child along the 0 edge
	std::unique_ptr<HuffmanTreeNode> m_one;  //!< pointer to a child along the 1 edge
	std::optional<unsigned char> m_value;    //!< if it is a leaf node, character is stored here
	uint32_t m_count = 0;                    //!< sum of the numbers of occurences of characters which are stored in the leaf nodes of the subtree rooted in this node
};

//comparisons of Huffman tree nodes based on their count values

bool operator==(const HuffmanTreeNode& lhs, const HuffmanTreeNode& rhs);

bool operator!=(const HuffmanTreeNode& lhs, const HuffmanTreeNode& rhs);

bool operator<(const HuffmanTreeNode& lhs, const HuffmanTreeNode& rhs);

bool operator<=(const HuffmanTreeNode& lhs, const HuffmanTreeNode& rhs);

bool operator>(const HuffmanTreeNode& lhs, const HuffmanTreeNode& rhs);

bool operator>=(const HuffmanTreeNode& lhs, const HuffmanTreeNode& rhs);

class HuffmanTree
{
public:

	HuffmanTree() = default;

	/**
	*   \brief Constructs Huffman tree from histogram
	*   \param histogram vector t where t[character] = number of occurences of character, 
	*		             vector has elements for all possible values of unsigned char type
	*/
	explicit HuffmanTree(const std::array<uint32_t, UCHAR_MAX + 1>& histogram);

	HuffmanTree(const HuffmanTree& other);

	HuffmanTree(HuffmanTree&& other) = default;

	HuffmanTree& operator=(const HuffmanTree& other);

	HuffmanTree& operator=(HuffmanTree&& other) = default;

	~HuffmanTree() = default;

	/**
	*   \brief Constructs Huffman tree from histogram
	*   \param histogram vector t where t[character] = number of occurences of character,
	*		             vector has elements for all possible values of unsigned char type
	*/
	void build(const std::array<uint32_t, UCHAR_MAX + 1>& histogram);

	/**
	*   \brief Returns the mapping of characters to Huffman codes
	*   \return mapping of characters to Huffman codes
	*/
	const std::unordered_map<unsigned char, HuffmanCode>& getCodes() const;

	/**
	*   \brief Returns a const pointer to the root of the tree or nullptr if tree is empty
	*   \return const pointer to the root of the tree or nullptr if tree is empty
	*/
	const HuffmanTreeNode* getRoot() const;

private:

	std::unique_ptr<HuffmanTreeNode> m_root;                     //!< pointer to the root node of this Huffman tree
	std::unordered_map<unsigned char, HuffmanCode> m_codes;      //!< mapping of characters to Huffman codes

	/**
	*   \brief Performs a deep copy of a Huffman tree
	*   \param root pointer to the root node of the Huffman tree which is to be copied
	*   \return pointer to the root node of the newly created copy
	*/
	std::unique_ptr<HuffmanTreeNode> copyTree(const std::unique_ptr<HuffmanTreeNode>& root) const;

	/**
	*   \brief Computes the mapping of characters to Huffman codes by recursively traversing this Huffman tree
	*/
	void computeHuffmanCodes();

	/**
	*   \brief Computes the mapping of characters to Huffman codes by recursively traversing a Huffman subtree
	*   \param root pointer to the root node of a Huffman subtree
	*   \param currentCode the Huffman code of the current path is created here during recursive traversal of a Huffman subtree
	*/
	void computeHuffmanCodes(const std::unique_ptr<HuffmanTreeNode>& root, HuffmanCode currentCode);
};

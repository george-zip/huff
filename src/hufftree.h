#ifndef HUFF_TREE_H
#define HUFF_TREE_H

#include <unordered_map>

namespace huffman {

typedef int character;
typedef std::unordered_map<character, std::string> codedMap;
typedef std::unordered_map<character, unsigned> frequencyMap;

class TreeNode {
public:
	TreeNode(unsigned count, TreeNode* l, TreeNode* r, int c = 0)
		: _count(count), _zero(l), _one(r), _c(c)
	{}
	virtual ~TreeNode() {}
	unsigned count() const {
		return _count;
	}
	TreeNode* zero() {
		return _zero;
	}
	TreeNode* one() {
		return _one;
	}
	int letter() const {
		return _c;
	}
private:
	unsigned _count;
	TreeNode* _zero;
	TreeNode* _one;
	int _c;
};

class HuffTree {
public:

	explicit HuffTree(const frequencyMap& fMap);
	~HuffTree();
	TreeNode* getRoot() {
		return _root;
	}
	void toCodeMap(codedMap& cm);

private:

	HuffTree(const HuffTree&);
	HuffTree& operator=(const HuffTree&);
	TreeNode* _root;

};

}

#endif
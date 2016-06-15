#ifndef HUFF_TREE_H
#define HUFF_TREE_H

#include <unordered_map>

using std::auto_ptr;

namespace huffman {

typedef int huff_char_type;
typedef std::unordered_map<huff_char_type, std::string> codedMap;
typedef std::unordered_map<huff_char_type, unsigned> frequencyMap;
typedef std::unordered_map<std::string, huff_char_type> codeToCharMap;

class TreeNode {
public:
	TreeNode(unsigned count, TreeNode* l, TreeNode* r, int c = 0)
		: _count(count), _zero(l), _one(r), _c(c)
	{}
	~TreeNode() {
		delete _zero;
		delete _one;
	}
	unsigned count() const {
		return _count;
	}
	TreeNode* zero() const {
		return _zero;
	}
	TreeNode* one() const {
		return _one;
	}
	void setZero(TreeNode* n) {
		_zero = n;
	}
	void setOne(TreeNode* n) {
		_one = n;
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
	explicit HuffTree(const codeToCharMap& cMap);

	~HuffTree();
	TreeNode* getRoot() const {
		return _root;
	}
	void toCodeMap(codedMap& cm);
	int depth() const;
	huff_char_type getLetter(const std::string& binString) const; 

private:

	HuffTree(const HuffTree&);
	HuffTree& operator=(const HuffTree&);
	TreeNode* _root;

};

}

#endif
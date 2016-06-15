#include "hufftree.h"

#include <queue>
#include <string>
#include <cassert>
#include <algorithm>

#include <iostream>

using std::cout;
using std::endl;
using std::string;


namespace huffman {

	int treedepth(TreeNode* n) {
		if(n == NULL) {
			return 0;
		}
		else {
			return 1 + std::max(treedepth(n->one()), treedepth(n->zero()));	
		}
	}

	struct NodeComp {
		bool operator()(const TreeNode* l, const TreeNode* r) {
			if(l->count() == r->count()) {
				return l->letter() > r->letter();
			}
			else {
				return l->count() > r->count();
			}
		}
	};

	void toCodeMapF(codedMap& cm, string bitString, TreeNode* node) {
		if(node == NULL)
			return;

		if(node->zero() == NULL && node->one() == NULL) {
			// leaf 
			cm.insert(std::make_pair(node->letter(), bitString));
		}
		else {
			string bitStringCopy = bitString;
			bitString += '0';
			toCodeMapF(cm, bitString, node->zero());
			bitStringCopy += '1';
			toCodeMapF(cm, bitStringCopy, node->one());
		}
	}

	int HuffTree::depth() const {
		return treedepth(_root);
	}

	HuffTree::HuffTree(const frequencyMap& fMap) : _root(NULL) {
		frequencyMap::const_iterator itr = fMap.begin();
        frequencyMap::const_iterator end = fMap.end();
        std::priority_queue<TreeNode*, std::vector<TreeNode*>, NodeComp> q;
        for(; itr != end; ++itr) {
          	TreeNode* tmp = new TreeNode(itr->second, NULL, NULL, itr->first);
          	q.push(tmp);
        }
        while(q.size() > 1) {
        	TreeNode* tmp1 = q.top();
        	q.pop();
        	TreeNode* tmp2 = q.top();
        	q.pop();
        	TreeNode* parent = new TreeNode(tmp1->count() + tmp2->count(), tmp1, tmp2);
        	q.push(parent);
        }
        if(!q.empty()) {
	        _root = q.top();
        	q.pop();
        }
	}

	HuffTree::HuffTree(const codeToCharMap& cMap) : _root(NULL) {
		if(!cMap.empty()) {
			_root = new TreeNode(0, NULL, NULL);
			codeToCharMap::const_iterator itr = cMap.begin();
			for(; itr != cMap.end(); ++itr) {
				const string bitString = itr->first;
				TreeNode* curr = _root;
				string::const_iterator sItr = bitString.begin();
				for(; sItr != bitString.end(); ++sItr) {
					int payload = 0;
					if(sItr == bitString.end() - 1) {
						payload = itr->second;
					}
					if(*sItr == '1') {
						if(curr->one() == NULL) {
							curr->setOne(new TreeNode(0, NULL, NULL, payload));
						}
						assert(curr->one() != NULL);
						curr = curr->one();
					}
					else {
						if(curr->zero() == NULL) {
							curr->setZero(new TreeNode(0, NULL, NULL, payload));
						}
						assert(curr->zero() != NULL);
						curr = curr->zero();						
					}
				}
				assert(curr->zero() == NULL);
				assert(curr->one() == NULL);
				assert(curr->letter() == itr->second);
			}
		}
	}

	HuffTree::~HuffTree() {
		delete _root;
	}

	void HuffTree::toCodeMap(codedMap& cm) {
		toCodeMapF(cm, string(), _root);
	}

	huff_char_type HuffTree::getLetter(const std::string& binString) const {
  		TreeNode* curr = _root;
  		std::string::const_iterator itr = binString.begin();
  		for(; itr != binString.end() && curr != NULL; ++itr) {
    		if(*itr == '1') {
      			curr = curr->one();
    		}
    		else {
      			curr = curr->zero();
    		}
    		if(curr == NULL)
      			throw std::runtime_error("Binary string not found");
    		if(curr->one() == NULL && curr->zero() == NULL) {
      			return curr->letter();
    		}
  		}
  		assert(false);
  		return -1;
	}

}

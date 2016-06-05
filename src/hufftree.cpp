#include "hufftree.h"

#include <queue>
#include <string>

#include <iostream>

using std::cout;
using std::endl;

namespace huffman {

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

	void destroy(TreeNode* p) {
		if(p != NULL) {
			if(p->zero() != NULL)
				destroy(p->zero());
			if(p->one() != NULL)
				destroy(p->one());
			delete p;
		}
	}

	void toCodeMapF(codedMap& cm, std::string bitString, TreeNode* node) {
		if(node == NULL)
			return;

		if(node->zero() == NULL && node->one() == NULL) {
			// leaf 
			cm.insert(std::make_pair(node->letter(), bitString));
		}
		else {
			std::string bitStringCopy = bitString;
			bitString += '0';
			toCodeMapF(cm, bitString, node->zero());
			bitStringCopy += '1';
			toCodeMapF(cm, bitStringCopy, node->one());
		}
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

	HuffTree::~HuffTree() {
		destroy(_root);
	}

	void HuffTree::toCodeMap(codedMap& cm) {
		toCodeMapF(cm, std::string(), _root);
	}
}

#ifndef HUFFUTIL_H
#define HUFFUTIL_H

#include "hufftree.h"
#include "globals.h"

#include <unordered_map>
#include <string>
#include <cassert>

class ibstream;
class obstream;

namespace huffman {

static const int EOF_CHAR = -1;
static const int MAGIC_NUMBER = 0x1F9D;
static const int MAGIC_NUMBER_BITS = 16;

typedef std::unordered_map<int, unsigned> frequencyMap;
typedef std::unordered_map<int, std::string> codedMap;
typedef std::unordered_map<std::string, int> codeToCharMap;

template<typename stream_type>
void getCharCount(stream_type& infile, std::unordered_map<int, unsigned>& result) {
	assert(result.empty());
  	int inbits;
  	while(infile.readbits(BITS_PER_WORD, inbits)) {
    	std::unordered_map<int, unsigned>::iterator itr = result.find(inbits);
    	if(itr == result.end()) {
      		result.insert(std::make_pair(inbits, 1));
    	}
    	else {
      		++(itr->second);
    	}
  	}
  	result.insert(std::make_pair(EOF_CHAR, 1));
}

template<typename stream_type>
void writeMagicNumber(stream_type& outfile) {
  outfile.writebits(MAGIC_NUMBER_BITS, MAGIC_NUMBER);
}

template<typename stream_type>
void writeMap(stream_type& outfile, const codedMap& cm) {
  assert(!cm.empty());
  codedMap::const_iterator itr = cm.begin();
  codedMap::const_iterator end = cm.end();
  outfile.writebits(BITS_PER_WORD, cm.size()); 
  for(; itr != end; ++itr) {
    std::string bitString = itr->second;
    outfile.writebits(BITS_PER_WORD, bitString.size());
    for(int i = 0; i < bitString.size(); ++i) {
      if(bitString.at(i) == '1')
        outfile.writebits(1, 1);
      else
        outfile.writebits(1, 0);      
    }
    // character last because when we unhuff we want to map bits to character
    outfile.writebits(8, itr->first); 
  }
}

template<typename stream_type>
void writeFile(stream_type& outfile, const std::string& bitString, 
  const codedMap& cm) {
	if(!bitString.empty()) {
		assert(!cm.empty());
  		writeMagicNumber(outfile);
  		writeMap(outfile, cm);
  		std::string::const_iterator itr = bitString.begin();
  		std::string::const_iterator end = bitString.end();
  		for( ; itr != end; ++itr) {
    		if(*itr == '1') {
      			outfile.writebits(1, 1);
    		}
    		else {
      			outfile.writebits(1, 0);      
    		}
  		}
  		outfile.writebits(BITS_PER_WORD, EOF_CHAR);
	}
}

template<typename stream_type>
void createBitString(stream_type& infile, const codedMap& cm, std::string& output) {
	assert(!cm.empty());
  	int inbits;
  	while(infile.readbits(BITS_PER_WORD, inbits)) {
    	codedMap::const_iterator itr = cm.find(inbits);
    	if(itr != cm.end()) {
      		output += itr->second;
    	} 
    	else {
      		std::string errorMsg("Unable to find bit code for ");
      		errorMsg += static_cast<char>(inbits);
      		throw std::runtime_error(errorMsg);
    	}
	}
	assert(!output.empty());
}

template<typename stream_type>
void readMap(stream_type& infile, codeToCharMap& cm) {
  assert(cm.empty());
  int inbits;
  if(infile.readbits(BITS_PER_WORD, inbits)) {
    int mapSize = inbits;
    for(int i = 0; i < mapSize; ++i) {
      if(infile.readbits(BITS_PER_WORD, inbits)) {
        int bitStringSize = inbits;
        std::string bitString;
        int j = 0;
        for(int j = 0; j < bitStringSize; ++j) {
          if(infile.readbits(1, inbits)) {
            if(inbits == 1)
              bitString += '1';
            else 
              bitString += '0';
          }
        }
        if(infile.readbits(BITS_PER_WORD, inbits)) {
          cm.insert(std::make_pair(bitString, inbits));
        }
      }
    }
  }
  assert(!cm.empty());
}

template<typename stream_type>
int getNextLetter(stream_type& infile, const HuffTree& ht) {
  int retVal = -1;
  int inbits;
  TreeNode* curr = ht.getRoot();
  assert(curr != NULL);
  while(infile.readbits(1, inbits) && curr != NULL) {
    if(inbits == 1) {
      curr = curr->one();
    }
    else {
      curr = curr->zero();
    }
    if(curr == NULL)
      break;
    if(curr->one() == NULL && curr->zero() == NULL) {
      retVal = curr->letter();
      break;
    }
  }
  return retVal == EOF_CHAR? -1 : retVal;
}

size_t targetFileBytes(const codedMap&);

size_t filesize(const std::string& );	

bool isHuffCompressed(ibstream&);

}

#endif
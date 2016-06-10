#include "huffutil.h"
#include "globals.h"
#include "bitops.h"

#include <unordered_map>
#include <utility>
#include <stdexcept>
#include <bitset>
#include <cmath>
#include <fstream>

namespace huffman {

static const int MAGIC_NUMBER = 0x1F9D;
static const int MAGIC_NUMBER_BITS = 16;

void writeMagicNumber(obstream& outfile) {
  outfile.writebits(MAGIC_NUMBER_BITS, MAGIC_NUMBER);
}

size_t filesize(const std::string& filename) {
    std::ifstream in(filename, std::ios::binary | std::ios::ate);
    return in.tellg(); 
}

size_t targetFileBytes(const codedMap& cm) {
  size_t payloadBits = 0;
  codedMap::const_iterator itr = cm.begin();
  for(; itr != cm.end(); ++itr) {
    payloadBits += BITS_PER_WORD;
    payloadBits += itr->second.length();
  }
  return ceil((payloadBits + MAGIC_NUMBER_BITS + BITS_PER_WORD) 
    / BITS_PER_WORD);
}


bool isHuffCompressed(ibstream& infile) {
  bool retVal = false;
  int inbits;
  if(infile.readbits(MAGIC_NUMBER_BITS, inbits)) {
    retVal = inbits == MAGIC_NUMBER;
  }
  return retVal;
}

int getNextLetter(ibstream& infile, const HuffTree& ht) {
  int retVal = -1;
  int inbits;
  TreeNode* curr = ht.getRoot();
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

void readMap(ibstream& infile, codeToCharMap& cm) {
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
}

void writeMap(obstream& outfile, const codedMap& cm) {
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

void getCharCount(ibstream& infile, std::unordered_map<int, unsigned>& result) {
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

void writeFile(obstream& outfile, const std::string& bitString, const codedMap& cm) {
  writeMagicNumber(outfile);
  writeMap(outfile, cm);
  //writeMap(outfile)
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

void createBitString(ibstream& infile, const codedMap& cm, std::string& output) {
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
}

}

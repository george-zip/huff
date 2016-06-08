#include "huffutil.h"
#include "globals.h"
#include "bitops.h"

#include <unordered_map>
#include <utility>
#include <stdexcept>
#include <bitset>

namespace huffman {

static const int MAGIC_NUMBER = 0x1F9D;
static const int MAGIC_NUMBER_BITS = 16;

void writeMagicNumber(obstream& outfile) {
  outfile.writebits(MAGIC_NUMBER_BITS, MAGIC_NUMBER);
}

bool isHuffCompressed(ibstream& infile) {
  bool retVal = false;
  int inbits;
  if(infile.readbits(MAGIC_NUMBER_BITS, inbits)) {
    retVal = inbits == MAGIC_NUMBER;
  }
  return retVal;
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
    if(bitString.size() > BITS_PER_WORD)
      throw std::runtime_error(bitString + " too long");
    else
      outfile.writebits(BITS_PER_WORD, bitString.size());
    int i = 0;
    for( ; i < bitString.size(); ++i) {
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

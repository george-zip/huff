#include "huffutil.h"
#include "globals.h"
#include "bitops.h"

#include <unordered_map>
#include <utility>
#include <stdexcept>

namespace huffman {


void writeMagicNumber(obstream& outfile) {
  static const int MAGIC_NUMBER = 0x1F9D;
  outfile.writebits(16, MAGIC_NUMBER);
}

void writeMap(obstream& outfile, const codedMap& cm) {
  codedMap::const_iterator itr = cm.begin();
  codedMap::const_iterator end = cm.end();
  outfile.writebits(8, cm.size()); 
  for(; itr != end; ++itr) {
    outfile.writebits(8, itr->first); // character
    std::string bitString = itr->second;
    if(bitString.size() > 8)
      throw std::runtime_error(bitString + " too long");
    int i = 0;
    for( ; i < bitString.size(); ++i) {
      if(bitString.at(i) == '1')
        outfile.writebits(1, 1);
      else
        outfile.writebits(1, 0);      
    }
    // fill in remaining bits in byte
    for(int j = i; j < 8; ++j) {
        outfile.writebits(1, 0);            
    }
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

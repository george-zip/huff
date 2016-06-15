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
  assert(!filename.empty());
  std::ifstream in(filename, std::ios::binary | std::ios::ate);
  // TODO: assert that file is opened
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

void writeMap(obstream& outfile, const codedMap& cm) {
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

}

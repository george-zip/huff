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

}

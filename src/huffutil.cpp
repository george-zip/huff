#include "huffutil.h"
#include "globals.h"
#include "bitops.h"

#include <unordered_map>
#include <utility>

namespace huffman {

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

}

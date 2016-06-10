#include "bitops.h"
#include "globals.h"
#include "huffutil.h"
#include "hufftree.h"

#include <iostream>
#include <string>
#include <unordered_map>

using namespace huffman;

void printCM(const codedMap& cm) {
      codedMap::const_iterator itr = cm.begin();
      codedMap::const_iterator end = cm.end();
      for(; itr != end; ++itr) {
        std::cout << static_cast<char>(itr->first) << " : " << itr->second << std::endl;
      }  
}

int main(int argc, char * argv[]) {

  if (argc > 1) {
    ibstream infile;
    if (infile.open(argv[1])) {
      frequencyMap result;
      getCharCount(infile, result);
      HuffTree tree(result);
      codedMap cm;
      tree.toCodeMap(cm);
      if(targetFileBytes(cm) > filesize(argv[1])) {
        std::cout 
        << "No compression occurred: Target file size is larger than original."
        << std::endl;
        return -1;
      }
      std::string bitString;
      infile.rewind();
      createBitString(infile, cm, bitString);
      if(argc > 2) {
        // std::cout << bitString << std::endl;
        obstream outfile;
        if(outfile.open(argv[2])) {
          writeFile(outfile, bitString, cm);
        }
        else {
          // TODO
        }
      }
    }
    else {
      std::cerr << "Unable to open file name [" << argv[1]
                << "]" << std::endl;
    }
  }
  return 0;
}

#include <iostream>
#include <string>
#include <unordered_map>

#include "bitops.h"
#include "globals.h"
#include "huffutil.h"
#include "hufftree.h"

using namespace huffman;

int main(int argc, char * argv[]) {

  if (argc > 1) {
    ibstream infile;
    if (infile.open(argv[1])) {
      frequencyMap result;
      getCharCount(infile, result);
      HuffTree tree(result);
      codedMap cm;
      tree.toCodeMap(cm);
      codedMap::const_iterator itr = cm.begin();
      codedMap::const_iterator end = cm.end();
      for (; itr != end; ++itr) {
        std::cout << "Char [" << static_cast<char>(itr->first) << "] Code ["
                  << itr->second << "]" << std::endl;
      }
    }
    else {
      std::cerr << "Unable to open file name [" << argv[1]
                << "]" << std::endl;
    }
  }
}

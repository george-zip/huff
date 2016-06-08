#include "bitops.h"
#include "globals.h"
#include "huffutil.h"

#include <iostream>

using namespace huffman;

void reportFileOpenErr(const std::string& fileName) {
	std::cerr << "Unable to open file name [" << fileName << "]" << std::endl;
}

void printCM(const codeToCharMap& cm) {
      codeToCharMap::const_iterator itr = cm.begin();
      codeToCharMap::const_iterator end = cm.end();
      for(; itr != end; ++itr) {
        std::cout << itr->first << " : " << static_cast<char>(itr->second) << std::endl;
      }  
}
  
int main(int argc, char * argv[]) {

	if(argc > 1) {
		// open stream provided as argument
		ibstream infile;
		if(!infile.open(argv[1])) {
			reportFileOpenErr(argv[1]);
			return -1;
		}
		if(isHuffCompressed(infile)) {
			codeToCharMap cm;
			readMap(infile, cm);
			//printCM(cm);

			// decode remainder of bitstream and write to file
		}
		else {
			std::cerr << "Not huff compressed" << std::endl;		
			return -1;
		}
	}
	return 0;
}
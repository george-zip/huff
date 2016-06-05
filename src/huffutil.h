#ifndef HUFFUTIL_H
#define HUFFUTIL_H

#include <unordered_map>

class ibstream;

namespace huffman {

typedef std::unordered_map<int, unsigned> frequencyMap;

void getCharCount(ibstream&, frequencyMap&);

}

#endif
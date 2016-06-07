#ifndef HUFFUTIL_H
#define HUFFUTIL_H

#include <unordered_map>
#include <string>

class ibstream;
class obstream;

namespace huffman {

typedef std::unordered_map<int, unsigned> frequencyMap;
typedef std::unordered_map<int, std::string> codedMap;

void getCharCount(ibstream&, frequencyMap&);

void writeFile(obstream&, const std::string& bitString, const codedMap& cm);

void createBitString(ibstream&, const codedMap&, std::string&);

void writeMagicNumber(obstream&);

void writeMap(obstream&, const codedMap&);

}

#endif
#ifndef HUFFUTIL_H
#define HUFFUTIL_H

#include "hufftree.h"

#include <unordered_map>
#include <string>

class ibstream;
class obstream;

namespace huffman {

static int EOF_CHAR = -1;

typedef std::unordered_map<int, unsigned> frequencyMap;
typedef std::unordered_map<int, std::string> codedMap;
typedef std::unordered_map<std::string, int> codeToCharMap;

void getCharCount(ibstream&, frequencyMap&);

void writeFile(obstream&, const std::string& bitString, const codedMap& cm);

void createBitString(ibstream&, const codedMap&, std::string&);

void writeMagicNumber(obstream&);

void writeMap(obstream&, const codedMap&);

void readMap(ibstream&, codeToCharMap&);

bool isHuffCompressed(ibstream&);

int getNextLetter(ibstream&, const HuffTree&);

size_t targetFileBytes(const codedMap&);

size_t filesize(const std::string& );	

}

#endif
#include <gtest/gtest.h>
#include <gmock/gmock.H>
#include <string>

namespace huffman {

class stream_type_mock {
public:
	MOCK_METHOD1(open, bool(const std::string &));
    MOCK_METHOD2(readbits, bool(int, int &));
    MOCK_METHOD2(writebits, void(int, int));
    MOCK_METHOD0(resetbits, void());
    MOCK_METHOD0(rewind, void());
    MOCK_METHOD0(close, void()); 
};

}
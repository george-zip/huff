#include "../src/lib/huffutil.h"
#include "huffutil_mock.h"
#include <gtest/gtest.h>

#include <iostream>

namespace huffman {

static int ASCII_OFFSET = 48;

using ::testing::Return;                            
using ::testing::_;                            
using ::testing::SetArgReferee;                            
using ::testing::InSequence;
using ::testing::DoAll;

class StreamMock : public ::testing::Test {
public:

  StreamMock() {
	InSequence dummy;

  	EXPECT_CALL(mock, readbits(_, _))                  
      .Times(1)
      .WillOnce(DoAll(SetArgReferee<1>(97), Return(true)));

    EXPECT_CALL(mock, readbits(_, _))                  
      .Times(1)
      .WillOnce(DoAll(SetArgReferee<1>(98), Return(true)));

  	EXPECT_CALL(mock, readbits(_, _))                  
      .Times(1)
      .WillOnce(DoAll(SetArgReferee<1>(99), Return(true)));

  	EXPECT_CALL(mock, readbits(_, _))                  
      .Times(1)
      .WillOnce(DoAll(SetArgReferee<1>(97), Return(true)));

  	EXPECT_CALL(mock, readbits(_, _))                  
      .Times(1)
      .WillOnce(DoAll(SetArgReferee<1>(98), Return(true)));

  	EXPECT_CALL(mock, readbits(_, _))                  
      .Times(1)
      .WillOnce(Return(false));
  }

	stream_type_mock mock;
};


TEST_F(StreamMock, getCharCount) {
    std::unordered_map<int, unsigned> res;
    getCharCount(mock, res);

    // three characters plus EOF marker
    EXPECT_EQ(res.size(), 4);
    EXPECT_EQ(res.at(97), 2);
    EXPECT_EQ(res.at(98), 2);
    EXPECT_EQ(res.at(99), 1);
}

TEST_F(StreamMock, createBitString) {
	codedMap cm;
	cm.insert(std::make_pair(97, "01"));
	cm.insert(std::make_pair(98, "10"));
	cm.insert(std::make_pair(99, "110"));
	std::string output;
	createBitString(mock, cm, output);
	EXPECT_EQ(output, "01101100110");
}

TEST(HuffUtil, createBitStringThrow) {
	stream_type_mock mock;

	EXPECT_CALL(mock, readbits(_, _))                  
      .Times(1)
      .WillOnce(DoAll(SetArgReferee<1>(97), Return(true)));

	codedMap cm;
	cm.insert(std::make_pair(10, "01"));
	std::string output;
	EXPECT_THROW(createBitString(mock, cm, output), std::runtime_error);
}

void addBitStringToMock(const std::string& bitString, InSequence& dummy, 
	stream_type_mock& mock) {
	for(char c : bitString) {
		EXPECT_CALL(mock, readbits(_, _))                  
      	.Times(1)
      	.WillOnce(DoAll(SetArgReferee<1>(c - ASCII_OFFSET), Return(true)));
	}
}

void addMapEntrytoMock(const std::string& bitString, huff_char_type letter, InSequence& dummy, 
	stream_type_mock& mock) {
	// bitstring size 
	EXPECT_CALL(mock, readbits(_, _))                  
    .Times(1)
    .WillOnce(DoAll(SetArgReferee<1>(bitString.size()), Return(true)));

    addBitStringToMock(bitString, dummy, mock);

	EXPECT_CALL(mock, readbits(_, _))                  
     .Times(1)
     .WillOnce(DoAll(SetArgReferee<1>(letter), Return(true)));
}

TEST(HuffUtil, readMap) {
	stream_type_mock mock;
	InSequence dummy;

	static const int MAP_SIZE = 3;
	EXPECT_CALL(mock, readbits(_, _))                  
      .Times(1)
      .WillOnce(DoAll(SetArgReferee<1>(MAP_SIZE), Return(true)));

    addMapEntrytoMock("01", 97, dummy, mock);
    addMapEntrytoMock("10", 98, dummy, mock);
    addMapEntrytoMock("110", 99, dummy, mock);

    codeToCharMap cm;
    readMap(mock, cm); 
    EXPECT_EQ(cm.at("01"), 97);
    EXPECT_EQ(cm.at("10"), 98);
    EXPECT_EQ(cm.at("110"), 99);
}

TEST(HuffUtil, getNextLetter) {
	stream_type_mock mock;
	InSequence dummy;

	const std::string BIT_STRING = "011101011100";
	addBitStringToMock(BIT_STRING, dummy, mock);

	codeToCharMap ccm;
	const int MY_CHAR = 97;
	ccm.insert(std::make_pair(BIT_STRING, MY_CHAR));
	HuffTree ht(ccm);

	EXPECT_EQ(getNextLetter(mock, ht), MY_CHAR);	
}

}
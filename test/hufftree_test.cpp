#include "../src/lib/huffutil.h"
#include "../src/lib/hufftree.h"

#include <gtest/gtest.h>

#include <utility>
#include <string>
#include <iostream>

using namespace std;
using namespace huffman;

class HuffTreeTest : public ::testing::Test {
public:
  HuffTreeTest( ) {
  }

  void SetUp( ) {
    fMap_.insert(std::make_pair('a', 4));
    fMap_.insert(std::make_pair('l', 3));
    fMap_.insert(std::make_pair('p', 2));
    fMap_.insert(std::make_pair('h', 1));
    pHuffTree_ = new HuffTree(fMap_);
  }

  ~HuffTreeTest( )  {
    delete pHuffTree_;
  }

  void reset(const frequencyMap& input) {
    delete pHuffTree_;
    pHuffTree_ = new HuffTree(input);
  }

  frequencyMap fMap_;
  HuffTree* pHuffTree_;
};

class HuffTreeTestEmpty : public ::testing::Test {
public:
  HuffTreeTestEmpty() : pHuffTree_(new HuffTree(frequencyMap())) {
  }

  HuffTree* pHuffTree_;
};


TEST_F (HuffTreeTest, UnitTest1) {
  TreeNode* root = pHuffTree_->getRoot();
  EXPECT_EQ(root->zero()->letter(), 'a')
      << static_cast<char>(root->zero()->letter());
  EXPECT_EQ(root->count(), 10);
}

TEST_F (HuffTreeTest, UnitTest2) {
  frequencyMap fMap;
  fMap.insert(std::make_pair('a', 1));
  fMap.insert(std::make_pair('b', 1));
  reset(fMap);
  EXPECT_EQ(static_cast<char>(pHuffTree_->getRoot()->zero()->letter()), 'a');
  EXPECT_EQ(static_cast<char>(pHuffTree_->getRoot()->one()->letter()), 'b');
  EXPECT_EQ(pHuffTree_->getRoot()->count(), 2);
  codedMap cm;
  pHuffTree_->toCodeMap(cm);
  EXPECT_EQ(cm.size(), 2);
  EXPECT_EQ(cm.at(static_cast<int>('a')), "0");
  EXPECT_EQ(cm.at(static_cast<int>('b')), "1");
}

TEST_F (HuffTreeTest, UnitTest3) {
  frequencyMap fMap;
  fMap.insert(std::make_pair('g', 3));
  fMap.insert(std::make_pair('o', 3));
  fMap.insert(std::make_pair('p', 1));
  fMap.insert(std::make_pair('h', 1));
  fMap.insert(std::make_pair('e', 1));
  fMap.insert(std::make_pair('r', 1));
  fMap.insert(std::make_pair('s', 1));
  fMap.insert(std::make_pair(' ', 2));
  reset(fMap);
  EXPECT_EQ(pHuffTree_->getRoot()->count(), 13);
  codedMap cm;
  pHuffTree_->toCodeMap(cm);
  EXPECT_EQ(cm.size(), 8);
  EXPECT_EQ(cm.at('g').size(), 2);
  EXPECT_EQ(cm.at('o').size(), 2);
  EXPECT_EQ(cm.at(' ').size(), 3);
  EXPECT_EQ(cm.at('e').size(), 4);
}

TEST_F(HuffTreeTestEmpty, UnitTest4) {
  codedMap cm;
  pHuffTree_->toCodeMap(cm);
  EXPECT_EQ(cm.size(), 0);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
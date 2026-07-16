#include "gwen/container/trie.hpp"
#include <gtest/gtest.h>
#include <string>

using namespace gwen;

TEST(TrieTest, BasicOperations) {
    LowerAlphaTrie trie;
    
    trie.insert(std::string("hello"));
    trie.insert(std::string("world"));
    trie.insert(std::string("hell"));
    trie.insert(std::string("helloworld"));
    trie.insert(std::string("hello"));
    
    EXPECT_TRUE(trie.contains(std::string("hello")));
    EXPECT_EQ(trie.count(std::string("hello")), 2);
    
    EXPECT_TRUE(trie.contains(std::string("hell")));
    EXPECT_EQ(trie.count(std::string("hell")), 1);
    
    EXPECT_TRUE(trie.contains(std::string("world")));
    EXPECT_EQ(trie.count(std::string("world")), 1);
    
    EXPECT_FALSE(trie.contains(std::string("he")));
    EXPECT_EQ(trie.count(std::string("he")), 0);
    
    EXPECT_EQ(trie.match_count(std::string("hell")), 4); // "hell", "hello" x2, "helloworld"
    
    trie.erase(std::string("hello"), 1);
    EXPECT_EQ(trie.count(std::string("hello")), 1);
    EXPECT_EQ(trie.match_count(std::string("hell")), 3);
    
    trie.erase(std::string("hello"), 0); // erase all
    EXPECT_EQ(trie.count(std::string("hello")), 0);
    EXPECT_FALSE(trie.contains(std::string("hello")));
    EXPECT_EQ(trie.match_count(std::string("hell")), 2); // "hell", "helloworld"
}

TEST(TrieTest, Path) {
    LowerAlphaTrie trie;
    trie.insert(std::string("abc"));
    
    auto p = trie.path(std::string("abcd"));
    // root, a, b, c, d(NIL)
    // path size should be 4 (root, a, b, c) -> actually "abcd" will insert 0,1,2, then 3 is NIL so break.
    // wait, path() stops when cur == NIL.
    // "abc" nodes: root(0), a(1), b(2), c(3).
    // for "abcd", it visits root, a(1), b(2), c(3), then 'd' goes to NIL.
    // So ret contains: root, 1, 2, 3, NIL -> size 5.
    EXPECT_EQ(p.size(), 5);
    EXPECT_EQ(p.back(), LowerAlphaTrie::NIL);
    EXPECT_EQ(trie.get_node(std::string("abc")).cnt, 1);
}

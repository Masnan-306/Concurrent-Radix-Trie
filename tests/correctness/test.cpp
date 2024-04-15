#include <gtest/gtest.h>
#include "PatriciaTree.h"

class PatriciaTreeTest : public ::testing::Test {
protected:
    PatriciaTree tree;

    void SetUp() override {
        tree = PatriciaTree();
    }

    void TearDown() override {
        // Cleanup code here, if needed.
    }
};

TEST_F(PatriciaTreeTest, TestInsertAndSearch) {
    // Test insertion of strings.
    tree.insert("hello");
    tree.insert("world");
    tree.insert("helium");

    // Test searching of strings.
    EXPECT_TRUE(tree.search("hello"));
    EXPECT_TRUE(tree.search("world"));
    EXPECT_FALSE(tree.search("help"));  // "help" is not inserted, should return false
    EXPECT_TRUE(tree.search("helium"));
}

TEST_F(PatriciaTreeTest, TestSearchNonExistentString) {
    // Test searching for a string that has not been inserted.
    EXPECT_FALSE(tree.search("nonexistent"));
}

TEST_F(PatriciaTreeTest, TestEmptyTree) {
    // Test searching in an empty tree.
    EXPECT_FALSE(tree.search(""));
    EXPECT_FALSE(tree.search("anything"));
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

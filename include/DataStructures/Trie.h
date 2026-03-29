#pragma once
#include <string>
using namespace std;

class TrieNode {
public:
    TrieNode* children[256];
    bool isEndOfWord;

    TrieNode();
    ~TrieNode();
};

class Trie {
private:
    TrieNode* root;
    bool deleteHelper(TrieNode* current, const std::string& word, int index);

public:
    Trie();
    ~Trie();

    void insert(const std::string& word);
    bool search(const std::string& word) const;
    void remove(const std::string& word);
    void update(const std::string& oldWord, const std::string& newWord);
};

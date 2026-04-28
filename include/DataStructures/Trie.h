#pragma once
#include <string>
#include <iostream>

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
    void clearHelper(TrieNode* node);

public:
    Trie();
    ~Trie();

    TrieNode* getRoot() const { return root; }
    void insert(const std::string& word);
    bool search(const std::string& word) const;
    void remove(const std::string& word);
    void update(const std::string& oldWord, const std::string& newWord);
    void clear(); 
};


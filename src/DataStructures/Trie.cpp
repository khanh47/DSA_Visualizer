#include "Trie.h"
using namespace std;
///ending of the node
TrieNode::TrieNode() : isEndOfWord(false) {
    for (int i = 0; i < 256; ++i) {
        children[i] = nullptr;
    }
}
///sdjskavjlflvn
TrieNode::~TrieNode() {
    for (int i = 0; i < 256; ++i) {
        if (children[i] != nullptr) {
            delete children[i];
        }
    }
}

Trie::Trie() {
    root = new TrieNode();
}

Trie::~Trie() {
    delete root;
}

void Trie::insert(const string& word) {
    TrieNode* current = root;
    for (char ch : word) {
        unsigned char index = static_cast<unsigned char>(ch);
        if (current->children[index] == nullptr) {
            current->children[index] = new TrieNode();
        }
        current = current->children[index];
    }
    current->isEndOfWord = true;
}
///hehe ahahah
bool Trie::search(const string& word) const {
    TrieNode* current = root;
    for (char ch : word) {
        unsigned char index = static_cast<unsigned char>(ch);
        if (current->children[index] == nullptr) {
            return false;
        }
        current = current->children[index];
    }
    return current != nullptr && current->isEndOfWord;
}

bool Trie::deleteHelper(TrieNode* current, const string& word, int index) {
    if (index == word.length()) {
        if (!current->isEndOfWord) {
            return false;
        }
        current->isEndOfWord = false;
        
        for (int i = 0; i < 256; ++i) {
            if (current->children[i] != nullptr) {
                return false;
            }
        }
        return true;
    }

    unsigned char ch = static_cast<unsigned char>(word[index]);
    if (current->children[ch] == nullptr) {
        return false;
    }

    bool shouldDeleteCurrentNode = deleteHelper(current->children[ch], word, index + 1);
/// do mixi
    if (shouldDeleteCurrentNode) {
        delete current->children[ch];
        current->children[ch] = nullptr;
        
        if (current->isEndOfWord) {
            return false;
        }
        
        for (int i = 0; i < 256; ++i) {
            if (current->children[i] != nullptr) {
                return false;
            }
        }
        return true;
    }

    return false;
}
///skib idi
void Trie::remove(const string& word) {
    deleteHelper(root, word, 0);
}
///phung thanh do
void Trie::update(const string& oldWord, const string& newWord) {
    if (search(oldWord)) {
        remove(oldWord);
        insert(newWord);
    }
}

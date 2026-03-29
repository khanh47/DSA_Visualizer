#include "ChainingHashTable.h"

ChainingHashTable::ChainingHashTable(int cap) : capacity(cap), size(0) {
    table.resize(capacity);
}

int ChainingHashTable::hashFunction(const std::string& key) const {
    int sum = 0;
    for (char c : key) sum += static_cast<int>(c);
    return sum % capacity;
}

void ChainingHashTable::clear() {
    for (auto& list : table) list.clear();
    size = 0;
}

void ChainingHashTable::resetHighlights() {
    for (auto& list : table) {
        for (auto& node : list) node.isHighlighted = false;
    }
}

bool ChainingHashTable::insert(const std::string& key, const std::string& value) {
    if (key.empty()) return false;
    int index = hashFunction(key);
    
    for (auto& node : table[index]) {
        if (node.key == key) return false; // Key exists
    }
    
    table[index].push_back({key, value, true});
    size++;
    return true;
}

bool ChainingHashTable::remove(const std::string& key) {
    int index = hashFunction(key);
    auto& list = table[index];
    
    for (auto it = list.begin(); it != list.end(); ++it) {
        if (it->key == key) {
            list.erase(it);
            size--;
            return true;
        }
    }
    return false;
}

HashNode* ChainingHashTable::search(const std::string& key) {
    resetHighlights();
    int index = hashFunction(key);
    for (auto& node : table[index]) {
        if (node.key == key) {
            node.isHighlighted = true;
            return &node;
        }
    }
    return nullptr;
}

bool ChainingHashTable::update(const std::string& key, const std::string& value) {
    HashNode* node = search(key);
    if (node) {
        node->value = value;
        return true;
    }
    return false;
}
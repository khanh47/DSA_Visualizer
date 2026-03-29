#pragma once
#include "IHashTable.h"
#include <vector>
#include <list>

class ChainingHashTable : public IHashTable {
private:
    int capacity;
    int size;
    std::vector<std::list<HashNode>> table;

    int hashFunction(const std::string& key) const;

public:
    ChainingHashTable(int cap = 7);

    void clear() override;
    bool insert(const std::string& key, const std::string& value) override;
    bool remove(const std::string& key) override;
    HashNode* search(const std::string& key) override;
    bool update(const std::string& key, const std::string& value) override;
    
    HashMode getMode() const override { return HashMode::CHAINING; }
    int getCapacity() const override { return capacity; }
    void resetHighlights() override;

    // Specific to chaining (Visualizer will cast to this class to get the 2D data)
    const std::vector<std::list<HashNode>>& getTable() const { return table; }
};
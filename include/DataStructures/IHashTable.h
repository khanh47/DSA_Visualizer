#pragma once
#include <string>

// Enum for easy switching later
enum class HashMode {
    CHAINING,
    LINEAR_PROBING,
    QUADRATIC_PROBING,
    DOUBLE_HASHING
};

struct HashNode {
    std::string key;
    std::string value;
    bool isHighlighted = false;
};

// Abstract Base Class (Interface)
class IHashTable {
public:
    virtual ~IHashTable() = default;

    virtual void clear() = 0;
    virtual bool insert(const std::string& key, const std::string& value) = 0;
    virtual bool remove(const std::string& key) = 0;
    virtual HashNode* search(const std::string& key) = 0;
    virtual bool update(const std::string& key, const std::string& value) = 0;
    
    virtual HashMode getMode() const = 0;
    virtual int getCapacity() const = 0;
    virtual void resetHighlights() = 0;
};
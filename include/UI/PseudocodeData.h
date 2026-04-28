#pragma once

#include <string>
#include <vector>

// ── Pseudocode data for all data structure operations ──────────────────
// Each operation is a pair of { title, lines[] }.
// Lines are 0-indexed; visualizers reference line indices when recording steps.

namespace Pseudocode {

// ── Linked List ─────────────────────────────────────────────────────────

inline const std::string kLinkedListInsertHeadTitle = "INSERT AT HEAD";
inline const std::vector<std::string> kLinkedListInsertHead = {
    "Node newNode = new Node(value)", // 0
    "newNode->next = head",           // 1
    "head = newNode",                 // 2
};

inline const std::string kLinkedListInsertTailTitle = "INSERT AT TAIL";
inline const std::vector<std::string> kLinkedListInsertTail = {
    "Node newNode = new Node(value)", // 0
    "if head == null:",               // 1
    "    head = newNode",             // 2
    "    return",                     // 3
    "cur = head",                     // 4
    "while cur->next != null:",       // 5
    "    cur = cur->next",            // 6
    "cur->next = newNode",            // 7
};

inline const std::string kLinkedListInsertIndexTitle = "INSERT AT INDEX";
inline const std::vector<std::string> kLinkedListInsertIndex = {
    "Node newNode = new Node(value)", // 0
    "if index == 1:",                 // 1
    "    newNode->next = head",       // 2
    "    head = newNode",             // 3
    "    return",                     // 4
    "cur = head",                     // 5
    "for i = 1 to index - 2:",        // 6
    "    cur = cur->next",            // 7
    "newNode->next = cur->next",      // 8
    "cur->next = newNode",            // 9
};

inline const std::string kLinkedListSearchTitle = "SEARCH";
inline const std::vector<std::string> kLinkedListSearch = {
    "cur = head",                  // 0
    "while cur != null",           // 1
    "    if cur->value == target", // 2
    "        return cur",          // 3
    "    cur = cur->next",         // 4
    "return NOT_FOUND",            // 5
};

inline const std::string kLinkedListDeleteTitle = "DELETE";
inline const std::vector<std::string> kLinkedListDelete = {
    "if index == 1",               // 0
    "    head = head->next",       // 1
    "    return",                   // 2
    "cur = head",                  // 3
    "for i = 1 to index - 2",     // 4
    "    cur = cur->next",         // 5
    "cur->next = cur->next->next", // 6
};

inline const std::string kLinkedListUpdateTitle = "UPDATE";
inline const std::vector<std::string> kLinkedListUpdate = {
    "cur = head",                  // 0
    "for i = 1 to index - 1",     // 1
    "    cur = cur->next",         // 2
    "cur->value = newVal",         // 3
};

// ── Kruskal ─────────────────────────────────────────────────────────────

inline const std::string kKruskalTitle = "KRUSKAL MST";
inline const std::vector<std::string> kKruskalMain = {
    "sort edges by weight",               // 0
    "init DSU for all vertices",          // 1
    "for each edge (u, v, w) in edges",   // 2
    "    if find(u) != find(v)",          // 3
    "        union(u, v); add edge",      // 4
    "if mst.size == n - 1: return mst",   // 5
};

// ── Trie ─────────────────────────────────────────────────────────────────

inline const std::string kTrieInsertTitle = "INSERT";
inline const std::vector<std::string> kTrieInsert = {
    "cur = root",                          // 0
    "for each char c in word",             // 1
    "    if cur.children[c] == null",      // 2
    "        cur.children[c] = new Node",  // 3
    "    cur = cur.children[c]",           // 4
    "cur.isEndOfWord = true",              // 5
};

inline const std::string kTrieSearchTitle = "SEARCH";
inline const std::vector<std::string> kTrieSearch = {
    "cur = root",                          // 0
    "for each char c in word",             // 1
    "    if cur.children[c] == null",      // 2
    "        return NOT_FOUND",            // 3
    "    cur = cur.children[c]",           // 4
    "return cur.isEndOfWord",              // 5
};

inline const std::string kTrieDeleteTitle = "DELETE";
inline const std::vector<std::string> kTrieDelete = {
    "cur = root",                          // 0
    "for each char c in word",             // 1
    "    if cur.children[c] == null",      // 2
    "        return (word not found)",     // 3
    "    cur = cur.children[c]",           // 4
    "cur.isEndOfWord = false",             // 5
    "prune unused nodes if any",           // 6


};

// ── Hash Table (Chaining) ───────────────────────────────────────────────

inline const std::string kHashInsertTitle = "INSERT (CHAINING)";
inline const std::vector<std::string> kHashInsert = {
    "index = hash(key)",                     // 0
    "for each node in table[index]:",        // 1
    "    if node.key == key: return false",  // 2 
    "table[index].push_back({key, value})",  // 3 
    "return true",                           // 4
};

inline const std::string kHashSearchTitle = "SEARCH (CHAINING)";
inline const std::vector<std::string> kHashSearch = {
    "index = hash(key)",                     // 0
    "for each node in table[index]:",        // 1
    "    if node.key == key: return true",   // 2 
    "return false",                          // 3 
};

inline const std::string kHashDeleteTitle = "DELETE (CHAINING)";
inline const std::vector<std::string> kHashDelete = {
    "index = hash(key)",                     // 0
    "for each node in table[index]:",        // 1
    "    if node.key == key:",               // 2 
    "        remove node; return true",      // 3 (Đã xóa)
    "return false",                          // 4
};

inline const std::string kHashUpdateTitle = "UPDATE (CHAINING)";
inline const std::vector<std::string> kHashUpdate = {
    "index = hash(key)",                     // 0
    "for each node in table[index]:",        // 1
    "    if node.key == key:",               // 2
    "        node.value = newValue",         // 3 
    "        return true",                   // 4
    "return false",                          // 5
};

} // namespace Pseudocode

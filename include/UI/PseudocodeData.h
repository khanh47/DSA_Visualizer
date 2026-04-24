#pragma once

#include <string>
#include <vector>

// ── Pseudocode data for all data structure operations ──────────────────
// Each operation is a pair of { title, lines[] }.
// Lines are 0-indexed; visualizers reference line indices when recording steps.

namespace Pseudocode {

// ── Linked List ─────────────────────────────────────────────────────────

inline const std::string kLinkedListInsertTailTitle = "INSERT AT TAIL";
inline const std::vector<std::string> kLinkedListInsertTail = {
    "node = new Node(value)",      // 0
    "if head == null",             // 1
    "    head = node; return",     // 2
    "cur = head",                  // 3
    "while cur->next != null",     // 4
    "    cur = cur->next",         // 5
    "cur->next = node",            // 6
};

inline const std::string kLinkedListInsertHeadTitle = "INSERT AT HEAD";
inline const std::vector<std::string> kLinkedListInsertHead = {
    "node = new Node(value)",      // 0
    "node->next = head",           // 1
    "head = node",                 // 2
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
    "if index == 0",               // 0
    "    head = head->next",       // 1
    "    return",                   // 2
    "cur = head",                  // 3
    "for i = 0 to index - 2",     // 4
    "    cur = cur->next",         // 5
    "cur->next = cur->next->next", // 6
};

inline const std::string kLinkedListUpdateTitle = "UPDATE";
inline const std::vector<std::string> kLinkedListUpdate = {
    "cur = head",                  // 0
    "for i = 0 to index - 1",     // 1
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
    "if mst.size == n - 1: stop",         // 5
    "return mst",                         // 6
};

} // namespace Pseudocode

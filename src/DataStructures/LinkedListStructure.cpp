#include "LinkedListStructure.h"

LinkedList::LinkedList() : head(nullptr) {}

void LinkedList::insert(int value) {
    Node* newNode = new Node{value, nullptr};
    if (!head) {
        head = newNode;
        return;
    }

    Node* cur = head;
    while (cur->next) cur = cur->next;
    cur->next = newNode;
}
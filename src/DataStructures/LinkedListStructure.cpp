#include "LinkedListStructure.h"

LinkedList::LinkedList() : head(nullptr) {}

LinkedList::~LinkedList() {
    Node* cur = head;
    while (cur) {
        Node* next = cur->next;
        delete cur;
        cur = next;
    }
    head = nullptr;
}

Node* LinkedList::getHead() const {
    return head;
}

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

void LinkedList::remove(int value) {
    if (!head) return;

    // Case 1: Head node is the target
    if (head->value == value) {
        Node* temp = head;
        head = head->next;
        delete temp;
        return;
    }

    // Case 2: Traverse to find the node preceding the target
    Node* cur = head;
    while (cur->next && cur->next->value != value) {
        cur = cur->next;
    }

    // If target node is found, unlink and delete it
    if (cur->next) {
        Node* temp = cur->next;
        cur->next = temp->next;
        delete temp;
    }
}

bool LinkedList::search(int value) {
    Node* cur = head;
    while (cur) {
        if (cur->value == value) {
            return true;
        }
        cur = cur->next;
    }
    return false;
}

void LinkedList::update(int oldVal, int newVal) {
    Node* cur = head;
    while (cur) {
        if (cur->value == oldVal) {
            cur->value = newVal;
            return;
        }
        cur = cur->next;
    }
}
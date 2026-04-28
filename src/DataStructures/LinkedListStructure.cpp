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

int LinkedList::getSize() const {
    int size = 0;
    Node* cur = head;
    while (cur) {
        size++;
        cur = cur->next;
    }
    return size;
}

void LinkedList::insertAtIndex(int value, int index) {
    if (index <= 1 || !head) {
        head = new Node{value, head};
        return;
    }

    Node* cur = head;
    int currentIndex = 1;
    while (cur->next && currentIndex < index - 1) {
        cur = cur->next;
        currentIndex++;
    }

    Node* newNode = new Node{value, cur->next};
    cur->next = newNode;
}

void LinkedList::remove(int index) {
    if (!head || index < 1) return;

    // Case 1: Head node is the target
    if (index == 1) {
        Node* temp = head;
        head = head->next;
        delete temp;
        return;
    }

    // Case 2: Traverse to find the node preceding the target
    Node* cur = head;
    int currentIndex = 1;
    while (cur->next && currentIndex < index - 1) {
        cur = cur->next;
        currentIndex++;
    }

    // If target node is found, unlink and delete it
    if (cur->next) {
        Node* temp = cur->next;
        cur->next = temp->next;
        delete temp;
    }
}

void LinkedList::update(int index, int newVal) {
    if (index < 1) return;
    Node* cur = head;
    int currentIndex = 1;
    while (cur) {
        if (currentIndex == index) {
            cur->value = newVal;
            return;
        }
        cur = cur->next;
        currentIndex++;
    }
}
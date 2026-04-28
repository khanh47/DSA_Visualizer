#pragma once

struct Node {
    int value;
    Node* next;
};

class LinkedList {
private:
    Node* head;

public:
    LinkedList();
    ~LinkedList();

    void insertAtIndex(int value, int index);
    void remove(int index);
    void update(int index, int newVal);

    Node* getHead() const;
    int getSize() const;
};
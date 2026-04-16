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

    void insert(int value);
    bool search(int value);
    void remove(int index);
    void update(int index, int newVal);

    Node* getHead() const;
};
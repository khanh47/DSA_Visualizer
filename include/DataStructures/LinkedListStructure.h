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
    void remove(int value);
    void update(int oldVal, int newVal);

    Node* getHead() const;
};
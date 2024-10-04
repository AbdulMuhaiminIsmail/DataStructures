#include <iostream>
#include "List.cpp"

using namespace std;

template <typename T> class Stack {
private:
    LinkedList<T> stack;
    int size;

public:
    Stack() : size(0) {}

    bool isEmpty() {
        return stack.isEmpty();
    }

    int getSize() {
        return size;
    }

    T getHead() {
        return stack.getHead();
    }

    T pop() {
        T head = stack.getHead();
        stack.deleteStart();
        size--;
        return head;
    }

    void push(T& element) {
        stack.insertAtStart(element);
        size++;
    }

    void print() {
        stack.printList();
    }
};
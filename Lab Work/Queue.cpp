#include <iostream>
#include <conio.h>
#include "List.cpp"
using namespace std;

template  <class T>
class Queue {
private:
	LinkedList<T> queue;
	int maxSize;
	int currElements;

public:
	Queue(int maxSize) : maxSize(maxSize), currElements(0) {}

	void print() 
	{
		queue.printList();
	}

	void dequeue()
	{
		if (currElements == 0) {
			cout << "Cant dequeue empty queue";
			return;
		}

		queue.deleteStart();
		currElements--;
	}

	T front() const
	{
		return queue.getHeadValue();
	}

	T rear() const
	{
		return queue.getTailValue();
	}

	void enqueue(T const element)
	{
		if (currElements == maxSize) {
			cout << "Cant add more elements";
			return;
		}

		queue.insertAtEnd(element);
		currElements++;
	}

	bool isEmpty() const
	{
		return currElements == 0;
	}

	bool isFull()
	{
		return currElements == maxSize;
	}

	int size()
	{
		return currElements;
	}

	void clear() {
		int size = currElements;
		for (int i = 0; i < size; i++) {
			dequeue();
		}
	}

	LinkedList<T>& getQueue() {
		return queue;
	}

	void arrange() {
		if (currElements % 2 == 1) return;

		auto ptr1 = queue.getHead();
		auto ptr2 = queue.accessNode(currElements / 2);
		Queue<T> temp(currElements);

		for (int i = 0; i < currElements; i++) {
			if (i % 2 == 0) {
				temp.enqueue(ptr1->value);
				ptr1 = ptr1->next;
			}
			else {
				temp.enqueue(ptr2->value);
				ptr2 = ptr2->next;
			}
		}

		clear();

		for (auto iter = temp.getQueue().getHead(); iter; iter = iter->next) {
			enqueue(iter->value);
		}
	}

};

int main()
{
	Queue<int> q(8);

	q.enqueue(3);
	q.enqueue(2);
	q.enqueue(5);
	q.enqueue(6);
	q.enqueue(7);
	q.enqueue(9);
	q.enqueue(8);
	q.enqueue(4);

	q.print();

	q.arrange();

	q.print();

	return 0;
}
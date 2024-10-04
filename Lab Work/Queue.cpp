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

	void print() {
		queue.printList();
	}

	void dequeue()
	{
		if (currElements == 0) {
			cout << "Cant dequeue empty queue";
			return;
		}

		stack.deleteEnd();
		currElements--;
	}

	T front() const
	{
		return queue.getHead();
	}

	T rear() const
	{
		return queue.getTail();
	}

	void enqueue(T const element)
	{
		if (currElements == maxSize) {
			cout << "Cant add more elements";
			return;
		}

		queue.insertAtStart(element);
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
		for (int i = 0; i < currElements; i++) {
			dequeue();
		}
	}

	LinkedList<T> getQueue() {
		return queue;
	}

	void arrange() {
		if (currElements%2 == 0) return;

		T ptr1 = queue.getHead();
		T ptr2 = queue.accessNode(currElements / 2)->value;
		Queue<T> temp(currElements);

		for (int i = 0; i < currElements; i++) {
			if (i % 2 == 0) {
				temp.enqueue(ptr1);
			}
			else {
				temp.enqueue(ptr2);
			}
		}

		clear();

		for (int i = 0; i < currElements; i++) {
			enqueue(temp.getQueue().accessNode(i)->value);
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

	_getch();
	return 0;
}


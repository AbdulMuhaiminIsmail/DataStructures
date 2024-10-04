#include <iostream>
using namespace std;

struct Book {
	char* title;
	int id;

	Book(const char* title, int id) : id(id) {
		this->title = new char[strlen(title)+1];
		for (int i = 0; i < strlen(title); i++) {
			this->title[i] = title[i];
		}
		this->title[strlen(title)] = '\0';
	}
	~Book() {
		if (title != nullptr) {
			delete[] title;
			title = nullptr;
		}
	}
};

class Library {
	Book** books;
	int capacity;
	int size;

public:
	Library(int capacity) : capacity(capacity), size(0) {
		books = new Book * [capacity];
	}

	~Library() {
		if (books != nullptr) {
			for (int i = 0; i < size; i++) {
				delete books[i];
				books[i] = nullptr;
			}
			delete[] books;
			books = nullptr;
		}
	}

	void addBook(int id, const char* title) {
		if (size == capacity) {
			cout << "No more capacity in library" << endl;
			return;
		}
		
		Book* book = new Book(title, id);
		books[size++] = book;
	}

	void displayBooks() const {
		if (size == 0) {
			cout << "No books in the library." << endl;
			return;
		}

		for (int i = 0; i < size; i++) {
			cout << "Name: \t" << books[i]->title << "\n ID: \t" << books[i]->id << endl;
		}
	}

	friend ostream& operator << (ostream& cout, Library* library) {
		library->displayBooks();
		return cout;
	}

};

int main() {
	Library* library = new Library(5);
	cout << library;
	library->addBook(1, "Harry Potter");
	library->addBook(2, "Harry Potter");
	library->addBook(3, "Harry Potter");
	library->addBook(4, "Harry Potter");
	library->addBook(5, "Harry Potter");
	library->displayBooks();
	library->addBook(6, "Harry Potter");
	library->displayBooks();

	delete library;
}

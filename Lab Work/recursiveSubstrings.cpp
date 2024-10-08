#include <iostream>
using namespace std;

void printSubstrings(string str, string curr, int index) {
	if (index >= str.length()) {
		if (curr == "") {
			cout << "{}";
			return;
		}

		cout << curr << ", ";
		return;
	}

	printSubstrings(str, curr + str[index], index + 1);
	printSubstrings(str, curr, index + 1);
}

int main() {
	string str;
	cout << "Enter string: " << endl;
	cin >> str;
	cout << "All possible substrings: ";
	printSubstrings(str, "", 0);
	cout << endl << "Total Count: " << pow(2, str.length());
}
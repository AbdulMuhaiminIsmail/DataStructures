#include <iostream>
#include <string>
#include "Stack.cpp"

using namespace std;

bool isCorrectlyParanthesized(string str) {
	Stack<char> parantheses;

	if (parantheses.isEmpty() && (str[0] == ')' || str[0] == '}' || str[0] == ']')) return false;

	if (str.length() % 2 == 1) return false;

	for (int i = 0; i < str.length(); i++) {
		parantheses.push(str[i]);
	}

	while (!parantheses.isEmpty()) {
		char x = parantheses.pop();
		char y = parantheses.pop();

		if (x == '(' || x == '{' || x == '[') return false;
		if (x == ')' && y != '(' || x == '}' && y != '{' || x == ']' && y != '[') return false;
	}
	
	return true;
}

int main() {
	while (1) {
		string str;
		cout << "Enter string: ";
		getline(cin, str);
		isCorrectlyParanthesized(str) ? cout << "Correct\n" : cout << "Incorrect\n";
	}
}
#include <iostream>
#include "DLL.cpp"
#include "AVL.cpp"
#include "User.cpp"

// ===========================
//       UserList Class
// ===========================

class UserList {
private:
	DLL<User> users;
	AVLTree<int> indexID;

public:
	UserList() : indexID(), users() {}

	void addUser(User user) {
		users.addUserAndUpdateIndexID(user, indexID);
	}
};

// static data
int User::currentUserID = 1;

int main() {
	UserList userList;

	string name = "Muhaimin";
	string email = "max@gmail.com";
	string country = "Pakistan";
	Type type = Platinum;

	User user(name, email, country, type);
	userList.addUser(user);

	return 0;
}
#pragma once
#include <iostream>
using namespace std;

enum Type { New = 1, Regular, Silver, Gold, Platinum };

// ===========================
//         User Class
// ===========================

class User {
    int userID;
    string userName;
    string userEmail;
    string userCountry;
    Type userType;
    static int currentUserID;

public:
    User(string userName, string userEmail, string userCountry, Type userType)
        : userID(++currentUserID), userName(userName), userEmail(userEmail), userCountry(userCountry), userType(userType) {}

    // Getters
    int getUserID() const {
        return userID;
    }

    string getUserName() const {
        return userName;
    }

    string getUserEmail() const {
        return userEmail;
    }

    string getUserCountry() const {
        return userCountry;
    }

    Type getUserType() const {
        return userType;
    }
};

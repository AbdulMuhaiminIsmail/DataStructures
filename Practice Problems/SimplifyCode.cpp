// This program takes a LinkedList as input, which has some numbers and returns a LinkedList in which 
// the list is modified such that next of every node tells its continous instances except for those
// numbers which only appeared once

#include <iostream>
#include <list>
#include <map>

using namespace std;

list<int> simplifyCode(auto start, auto end) {

    // Initialize variables for keeping track of current element and count and starting from second element
    int curr = *start;
    int count = 1;
    start = ++start;
    list<int> newList;

    // Iterate for the first n-1 terms and update the map
    for (auto it = start; it != end; ++it) {
        if (curr == *it) count++;
        else {
            newList.push_back(curr);
            if (count > 1) newList.push_back(count);
            curr = *it;
            count = 1;
        }
    }
    
    // Adding the last pair as well
    newList.push_back(curr);
    if (count > 1) newList.push_back(count);
    
    return newList;
}   

int main()
{
    list<int> l;
    l.push_back(1);
    l.push_back(1);
    l.push_back(1);
    l.push_back(1);
    l.push_back(1);
    l.push_back(2);
    l.push_back(1);

    for (auto it = l.begin(); it != l.end(); ++it) {
        cout << *it << " ";
    }     
    cout << endl;           

    list<int> l2 = simplifyCode(l.begin(), l.end());

    for (auto it = l2.begin(); it != l2.end(); ++it) {
        cout << *it << "->";
    }

    return 0;
}
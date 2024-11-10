// This program takes a LinkedList and X as input, which has some numbers and returns a LinkedList in which 
// the list is modified such that the numbers have the order <X, X, >X the relative order of the numbers is preserved
// We have to implement it in O(N)

#include <iostream>
#include <list>

using namespace std;

list<int> partitionList(auto start, auto end, int X) { 
    bool foundX = false;
    list<int> newList;

    // Iterate for the <X nums
    for (auto it = start; it != end; ++it) {
        if (*it < X) newList.push_back(*it);
        if (*it == X) foundX = true;
    }

    // Append X if found
    if (foundX) newList.push_back(X);

    // Iterate for the >X nums
    for (auto it = start; it != end; ++it) {
        if (*it > X) newList.push_back(*it);
    }
    
    return newList;
}   

int main()
{
    list<int> l;
    l.push_back(2);
    l.push_back(9);
    l.push_back(67);
    l.push_back(4);
    l.push_back(6);
    l.push_back(1);
    l.push_back(3);
    l.push_back(12);

    for (auto it = l.begin(); it != l.end(); ++it) {
        cout << *it << ">";
    }     
    cout << endl;           

    list<int> l2 = partitionList(l.begin(), l.end(), 4);

    for (auto it = l2.begin(); it != l2.end(); ++it) {
        cout << *it << "->";
    }

    return 0;
}
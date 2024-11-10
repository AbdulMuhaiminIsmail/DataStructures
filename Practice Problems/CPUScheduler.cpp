// This program aims to mimic a CPU Scheduler, which takes clock speed, number of processes and 
// the number of instructions in each as arguments and processes them just like a CPU.

#include <iostream>
#include <queue>

using namespace std;

void scheduler(queue<int>& queue, int clockSpeed, int processes) { 
    int i = 1;
    while(!queue.empty()) {
        int instr = queue.front();
        instr -= clockSpeed;

        cout << "Process " << i << " has " << max(0, instr) << " instructions left." << endl;
        if (instr <= 0) cout << "Process " << i << " has finished." << endl;
        
        if (++i > queue.size()) i = 1;

        queue.pop();
        if (instr > 0) queue.push(instr);
    }
}   

int main()
{
    queue<int> queue;
    queue.push(7);
    queue.push(6);
    queue.push(5);

    scheduler(queue, 3, 3);

    return 0;
}
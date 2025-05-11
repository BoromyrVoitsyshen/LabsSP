#include <iostream>
#include <unistd.h>
#include <cstdint>
#include <iomanip>
using namespace std;
volatile int shared_data = 0;
int main()
{
    int temp;
    cout << "Process PID: " << getpid() << "\n";
    cout << "Address of shared_data: " << std::showbase << std::hex << reinterpret_cast<uintptr_t>(&shared_data) << "\n";
    while (true)
    {
        cout << "Enter a number: ";
        cin >> temp;
        shared_data = temp;
    }
    return 0;
}

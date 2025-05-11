#include <iostream>
#include <fstream>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>
#include <cstdlib>
#include <cstdint>
using namespace std;
int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        cout << "Error: wrong format" << "\n";
        cout << "To execture the program, type: " << argv[0] << " <Process ID> <Data Adress>" << "\n";
        return 1;
    }
    pid_t pid = atoi(argv[1]);
    uintptr_t addr = strtoull(argv[2], nullptr, 0);
    std::string mem_path = "/proc/" + std::to_string(pid) + "/mem";
    int mem_fd = open(mem_path.c_str(), O_RDONLY);
    if (mem_fd == -1)
    {
        perror("open");
        return 1;
    }
    int prev_value = 0;
    while (true)
    {
        lseek(mem_fd, addr, SEEK_SET);
        int value;
        if (read(mem_fd, &value, sizeof(value)) == sizeof(value))
        {
            if (value != prev_value)
            {
                std::cout << "Value changed: " << value << "\n";
                prev_value = value;
            }
        }
        else
        {
            perror("read");
            break;
        }
        usleep(500000);
    }
    close(mem_fd);
    return 0;
}

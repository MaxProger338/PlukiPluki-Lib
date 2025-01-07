#include "PlukiPluki.h"

#include <iostream>
#include <exception>
#include <fstream>
#include <vector>

using namespace PlukiPlukiLib;

int main(int argc, char** argv)
{
    try
    {
        PlukiPluki fb("data/data.txt", std::ios::in);

        std::vector<std::string> myVec{ fb.indexAll() };

        for (auto i : myVec)
        {
            std::cout << i << '\t';
        }
        std::cout << '\n';
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    catch(...)
    {
        std::cerr << "FUCK YOU!!! (said by MEMORY)" << '\n';
    }

    return 0;
}
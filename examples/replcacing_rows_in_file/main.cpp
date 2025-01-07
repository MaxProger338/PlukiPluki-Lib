#include "PlukiPluki.h"

#include <iostream>
#include <exception>

using namespace PlukiPlukiLib;

int main(int argc, char** argv)
{
    try
    {
        PlukiPluki file("data/data.txt", std::ios::in);

        std::cout << file.getAmountRows() << '\n';

        file.reopen(std::ios::app);

        file.setRow(1, "lol!");

        file.reopen(std::ios::in);

        std::cout << file[1] << '\n';
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
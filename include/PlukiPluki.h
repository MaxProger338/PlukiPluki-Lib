#pragma once

#include "../src/core/FileBase.h"

#include <fstream>
#include <exception>
#include <string>

//! #define DEBUG_MODE

#ifdef DEBUG_MODE

#include <iostream>
#define LOG(msg) std::cout << msg;

#endif

namespace PlukiPlukiLib
{
    typedef unsigned int __amountRows;
    typedef unsigned int __amountSymbols;

    class PlukiPluki : public FileBase
    {
        private:
            __amountRows    _countRowsInFile  (std::fstream* file)                     const;

            std::string     _getRowByIndex    (std::fstream* file, __amountRows index) const;

        public:
            PlukiPluki(std::string path, const std::_Ios_Openmode& mode);

            __amountRows getAmountRows()                                               const;

            std::string  getRowByIndex(__amountRows index)                             const;

            std::string  operator[](__amountRows index)                                const;
    };
};
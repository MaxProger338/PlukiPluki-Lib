#pragma once

#include "../src/core/FileBase.h"

#include <fstream>
#include <exception>
#include <vector>
#include <string>
#include <iterator>
#include <algorithm>

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
            enum FILE_ERRORS
            {
                WRONG_MODE           = 6,
                INDEX_OUT_OF_RANGE   = 7,
            };

            std::string  _getErrorMsgByStatus(FILE_ERRORS error)                                  const;

        protected:
            bool         _compareModeByRead (const std::_Ios_Openmode& currentMode)               const;

            bool         _compareModeByWrite(const std::_Ios_Openmode& currentMode)               const;

            __amountRows _countRowsInFile   (            std::fstream* file)                      const;

            std::string  _getRowByIndex     (            std::fstream* file,  __amountRows index) const;

            void         _setRowByIndex     (            std::fstream*& file, __amountRows index, std::string newStr);

            std::vector<std::string>* _indexAllFile(     std::fstream* file)                      const;

            std::vector<std::string>* _checkIndexAllFile(std::fstream* file)                      const;

        public:
            PlukiPluki(std::string path, const std::_Ios_Openmode& mode);

            __amountRows getAmountRows()                                        const;

            std::string  getRowByIndex(__amountRows index)                      const;

            void         setRowByIndex(__amountRows index, std::string newStr);

            std::vector<std::string> indexAllFile()                             const;

            std::string  operator[]   (__amountRows index)                      const;

            // Call setRowByIndex
            void         operator()   (__amountRows index, std::string newStr);

            // Call getAmountRows
            __amountRows operator()()                                           const;
    };
};
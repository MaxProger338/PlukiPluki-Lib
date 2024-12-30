#pragma once

#include <fstream>
#include <exception>
#include <string>
#include <filesystem>

//! #define DEBUG_MODE

#ifdef DEBUG_MODE

#include <iostream>
#define LOG(msg) std::cout << msg;

#endif

namespace PlukiPlukiLib
{
    typedef unsigned int __amountRows;

    class PlukiPluki
    {
        private:
            std::string   _path;
            std::fstream* _file;

            short         _openFile(std::fstream*& file, std::string path);
            
            void          _checkOpenFile(std::fstream*& file, std::string path);

            short         _closeFile(std::fstream*& file);

            void          _checkCloseFile(std::fstream*& file);

            __amountRows  _countRowsInFile(std::fstream* file) const;

            std::string   _getRowByIndex(std::fstream* file, __amountRows index) const;

        public:
            PlukiPluki(std::string path);

            /*
             * TODO: Поразмышлять на тему удаления конструкторов и операторов присвоения
            */
            PlukiPluki(const PlukiPluki& plukiPluki)           = delete;

            PlukiPluki(PlukiPluki&& plukiPluki)                = delete;

            PlukiPluki operator=(const PlukiPluki& plukiPluki) = delete;

            PlukiPluki operator=(PlukiPluki&& plukiPluki)      = delete;

            ~PlukiPluki();

            __amountRows getAmountRows() const;

            std::string getRowByIndex(__amountRows index) const;

            static bool isExistsFile(std::string);

            std::string operator[](__amountRows index) const;
    };
};
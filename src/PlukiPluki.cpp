#include "../include/PlukiPluki.h"

using namespace PlukiPlukiLib;

PlukiPluki::
    PlukiPluki(std::string path):
        FileBase(path) {}


__amountRows PlukiPluki::
    _countRowsInFile(std::fstream* file) const
    { 
        __amountRows count = 0;

        /*
         * Я ещё глупенький
         * TODO: Найти какой нибудь другой способ
         */
        std::string tmp;
        while(*file >> tmp)
        {
            count++;
        }

        file->clear();
        file->seekg(0);

        return count;
    }

__amountRows PlukiPluki::
    getAmountRows() const
    {
        /*
         * Какая-то защита ...
         */

        __amountRows amount = _countRowsInFile(_file);

        return amount;
    }

std::string PlukiPluki::
    _getRowByIndex(std::fstream* file, __amountRows index) const
    {
        __amountRows row = 0;
        std::string  res;

        std::string tmp;
        while(*file >> tmp)
        {
            if (row == index)
            {
                res = tmp;
                break;
            }

            row++;
        }

        file->clear();
        file->seekg(0);

        return res;
    }

std::string PlukiPluki::
    getRowByIndex(__amountRows index) const
    {
        /*
         * Warning: Здесь всё защита в плане существования файла 
         *          проверяется в методе getAmountRows
         */

        __amountRows amountRows = getAmountRows();
        if (index >= amountRows)
        {
            throw std::runtime_error("Index out of range!");
        }

        std::string row = _getRowByIndex(_file, index);

        return row;
    }

std::string PlukiPluki::
    operator[](__amountRows index) const
    {
        return getRowByIndex(index);
    }
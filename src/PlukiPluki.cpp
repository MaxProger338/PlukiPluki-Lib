#include "../include/PlukiPluki.h"

using namespace PlukiPlukiLib;

PlukiPluki::
    PlukiPluki(std::string path, const std::_Ios_Openmode& mode):
        FileBase(path, mode) {}

std::string PlukiPluki::
    _getErrorMsgByStatus(PlukiPluki::FILE_ERRORS error) const
    {
        switch (error)
        {
            // case SUCCESS - должно проверяться перед вызовом функции!
            case WRONG_MODE:          return "Inccorect file mode!";
            case INDEX_OUT_OF_RANGE:  return "Index out of range!";

            default:                  return "Unknow file error!";
        }
    }

bool PlukiPluki::
    _compareModeByRead(const std::_Ios_Openmode& currentMode) const
    {
        switch (currentMode)
        {
            case std::ios::in: return true;

            default:           return false;
        }
    }

bool PlukiPluki::
    _compareModeByWrite(const std::_Ios_Openmode& currentMode) const
    {
        /* switch (currentMode) ... */

        return !_compareModeByRead(currentMode);
    }

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
        if (!_compareModeByRead(getMode()))
        {
            throw std::runtime_error(_getErrorMsgByStatus(WRONG_MODE));
        }

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

void PlukiPluki::
    _setRowByIndex(std::fstream*& file, __amountRows index, std::string newStr)
    {
        _reopen(file, std::ios::in);

        std::vector<std::string> buf;

        std::string tmp;
        while (*file >> tmp)
        {
            buf.push_back(tmp);
        }

        buf[index] = newStr;

        _reopen(file, std::ios::out);

        std::copy(buf.begin(), buf.end(), std::ostream_iterator<std::string>(*file, "\n"));
    }

std::string PlukiPluki::
    getRowByIndex(__amountRows index) const
    {
        /*
         * Warning: Здесь всё защита в плане существования файла 
         *          проверяется в методе getAmountRows
         */
        if (!_compareModeByRead(getMode()))
        {
            throw std::runtime_error(_getErrorMsgByStatus(WRONG_MODE));
        }

        __amountRows amountRows = getAmountRows();
        if (index >= amountRows)
        {
            throw std::runtime_error(_getErrorMsgByStatus(INDEX_OUT_OF_RANGE));
        }

        std::string row = _getRowByIndex(_file, index);

        return row;
    }

void PlukiPluki::
    setRowByIndex(__amountRows index, std::string newStr)
    {
        if (!_compareModeByWrite(getMode()))
        {
            throw std::runtime_error(_getErrorMsgByStatus(WRONG_MODE));
        }

        reopen(std::ios::in);

        __amountRows amountRows = getAmountRows();
        if (index >= amountRows)
        {
            throw std::runtime_error(_getErrorMsgByStatus(INDEX_OUT_OF_RANGE));
        }

        reopen(std::ios::app);

        _setRowByIndex(_file, index, newStr);
    }

std::string PlukiPluki::
    operator[](__amountRows index) const
    {
        return getRowByIndex(index);
    }

void PlukiPluki::
    operator()(__amountRows index, std::string newStr)
    {
        setRowByIndex(index, newStr);
    }

__amountRows PlukiPluki::
    operator()() const
    {
        return getAmountRows();
    }
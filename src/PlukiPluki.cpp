#include "../include/PlukiPluki.h"

using namespace PlukiPlukiLib;

PlukiPluki::
    PlukiPluki(std::string path, const std::_Ios_Openmode& mode):
        FileBase(path, mode) {}

std::string PlukiPluki::
    _getErrorMsgByStatus(PlukiPluki::ERROR_STATUS error) const noexcept
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
    _compareModeByIsRead(const std::_Ios_Openmode& currentMode) const noexcept
    {
        switch (currentMode)
        {
            case std::ios::in: return true;

            default:           return false;
        }
    }

bool PlukiPluki::
    _compareModeByIsWrite(const std::_Ios_Openmode& currentMode) const noexcept
    {
        /* switch (currentMode) ... */

        return !_compareModeByIsRead(currentMode);
    }

void PlukiPluki::
    _reopenToPreviosMode(
        std::fstream*&     file,
        const std::string& path,
        const __IOS_MODE&  currentMode
    ) noexcept
    {
         // TODO: исправь эту хрень!!!
        if (currentMode != std::ios::out)
        {
            _reopenImpl(file, path, currentMode);
        }
        else
        {
            _reopenImpl(file, path, std::ios::app);
        }
    }

// Get amount rows
__amountRows PlukiPluki::
    _getAmountRows(std::fstream* file, const __IOS_MODE& currentMode) const
    {
        if (!_compareModeByIsRead(currentMode))
        {
            throw std::runtime_error(_getErrorMsgByStatus(WRONG_MODE));
        }

        return _getAmountRowsImpl(file);
    }

__amountRows PlukiPluki::
    _getAmountRowsImpl(std::fstream* file) const noexcept
    { 
        __amountRows count = 0;

        /*
         * Я ещё глупенький
         * TODO: Найти какой-нибудь другой способ
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

// End amount rows

// Get row by index
std::string PlukiPluki::
    _getRow(
        std::fstream*     file, 
        const __IOS_MODE& currentMode,
        __amountRows      index
    ) const
    {
        if (!_compareModeByIsRead(currentMode))
        {
            throw std::runtime_error(_getErrorMsgByStatus(WRONG_MODE));
        }

        __amountRows amountRows = getAmountRows();
        if (index >= amountRows)
        {
            throw std::runtime_error(_getErrorMsgByStatus(INDEX_OUT_OF_RANGE));
        }

        std::string row = _getRowImpl(_file, index);

        return row;
    }

std::string PlukiPluki::
    _getRowImpl(std::fstream* file, __amountRows index) const noexcept
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

// End Get row by index

// Set row by index
void PlukiPluki::
    _setRow(
        std::fstream*&     file, 
        const std::string& path, 
        const __IOS_MODE&  currentMode,
        __amountRows       index, 
        std::string        newRow
    ) {
        ERROR_STATUS setErrorStatus = _checkIsRowSetted(file, path, currentMode, index, newRow);

        if (setErrorStatus != SUCCESS)
        {
            throw std::runtime_error(_getErrorMsgByStatus(setErrorStatus));
        }
    }

PlukiPluki::ERROR_STATUS PlukiPluki::
    _checkIsRowSetted(
        std::fstream*&     file, 
        const std::string& path, 
        const __IOS_MODE&  currentMode,
        __amountRows       index, 
        std::string        newRow
    ) noexcept
    {
        if (!_compareModeByIsWrite(currentMode))
        {
            return WRONG_MODE;
        }

        _reopenImpl(file, path, std::ios::in);

        __amountRows amountRows = _getAmountRowsImpl(file);
        if (index >= amountRows)
        {
            return INDEX_OUT_OF_RANGE;
        }

        _reopenImpl(file, path, std::ios::app);

        _setRowImpl(file, path, index, newRow);

        // ------ Reopen to prev mode ------
        _reopenToPreviosMode(file, path, currentMode);

        return SUCCESS;
    }

void PlukiPluki::
    _setRowImpl(
        std::fstream*&     file, 
        const std::string& path, 
        __amountRows       index, 
        std::string        newRow
    ) noexcept
    {
        // ------ Read ------
        _reopenImpl(file, path, std::ios::in);

        std::vector<std::string> buf;

        std::string tmp;
        while (*file >> tmp)
        {
            buf.push_back(tmp);
        }

        buf[index] = newRow;

        // ------ Clear ------
        _reopenImpl(file, path, std::ios::out);

        // ------ Write ------
        std::copy(buf.begin(), buf.end(), std::ostream_iterator<std::string>(*file, "\n"));
    }

// End Set row by index

// Index all file
std::vector<std::string> PlukiPluki::
    _indexAll(std::fstream* file, const __IOS_MODE& currentMode) const
    {
        if (!_compareModeByIsRead(currentMode))
        {
            throw std::runtime_error(_getErrorMsgByStatus(WRONG_MODE));
        }

        return _indexAllImpl(file);
    }

std::vector<std::string> PlukiPluki::
    _indexAllImpl(std::fstream* file) const noexcept
    {
        __amountRows amountRows = _getAmountRowsImpl(file);

        std::vector<std::string> rows(amountRows);

        for (size_t i = 0; i < amountRows; i++)
        {
            std::string currentRow = _getRowImpl(file, i);
            
            rows[i] = currentRow;
        }

        return rows;
    }

// Clear
void PlukiPluki::
    _clear(
        std::fstream*&            file, 
        const std::string&        path, 
        const std::_Ios_Openmode& currentMode
    ) {
        ERROR_STATUS clearErrorStatus = _checkIfHasCleared(file, path, currentMode);

        if (clearErrorStatus != SUCCESS)
        {
            throw std::runtime_error(_getErrorMsgByStatus(clearErrorStatus));
        }
    }

PlukiPluki::ERROR_STATUS PlukiPluki::
    _checkIfHasCleared(
        std::fstream*&            file, 
        const std::string&        path, 
        const std::_Ios_Openmode& currentMode
    ) noexcept
    {
        if (!_compareModeByIsWrite(currentMode))
        {
            return WRONG_MODE;
        }

        _clearImpl(file, path);

        // TODO: Обдумать, надо ли перезаписовать если std::ios::app
        // Это востановит прежний режим (ААА!!! ПАСХААЛКА, ПАСХААЛКА!!!)
        if (currentMode != std::ios::out)
        {
            _reopen(file, path, currentMode);
        }
        else
        {
            _reopen(file, path, std::ios::app);
        }

        return SUCCESS;
    }

void PlukiPluki::
    _clearImpl(std::fstream*& file, const std::string& path) noexcept
    {
        // Этот переоткрытие нам почистит файл
        _reopenImpl(file, path, std::ios::out);
    }

// End clear

// Insert row
void PlukiPluki::
    _insertRow(
        std::fstream*&     file, 
        const std::string& path, 
        const __IOS_MODE&  currentMode,
        __amountRows       index, 
        std::string        newRow
    ) {
        ERROR_STATUS insertErrorStatus = _checkIfHasRowInserted(file, path, currentMode, index, newRow);

        if (insertErrorStatus != SUCCESS)
        {
            throw std::runtime_error(_getErrorMsgByStatus(insertErrorStatus));
        }
    }

PlukiPluki::ERROR_STATUS PlukiPluki::
    _checkIfHasRowInserted (
            std::fstream*&     file, 
            const std::string& path, 
            const __IOS_MODE&  currentMode,
            __amountRows       index, 
            std::string        newRow
    ) noexcept
    {
        if (!_compareModeByIsWrite(currentMode))
        {
            return WRONG_MODE;
        }

        _reopenImpl(file, path, std::ios::in);

        __amountRows amountRows = _getAmountRowsImpl(file);
        if (index >= amountRows)
        {
            return INDEX_OUT_OF_RANGE;
        }

        _reopenImpl(file, path, std::ios::in);

        _insertRowImpl(file, path, index, newRow);

        // ------ Reopen to prev mode ------
        _reopenToPreviosMode(file, path, currentMode);

        return SUCCESS;
    }

void PlukiPluki::
    _insertRowImpl(
        std::fstream*&     file, 
        const std::string& path, 
        __amountRows       index, 
        std::string        newRow
    ) noexcept
    {
        std::vector<std::string> buf = const_cast<PlukiPluki*>(this)->_indexAllImpl(file);

        buf.insert(buf.begin() + index, newRow);

        _clearImpl(file, path);
        
        std::copy(buf.begin(), buf.end(), std::ostream_iterator<std::string>(*file, "\n"));
    }

// End Insert row

// Delete row
void PlukiPluki::
    _deleteRow(
        std::fstream*&     file,
        const std::string& path, 
        const __IOS_MODE&  currentMode,
        __amountRows       index   
    ) {
        ERROR_STATUS deleteErrorStatus = _checkIfHasRowDeleted(file, path, currentMode, index);

        if (deleteErrorStatus != SUCCESS)
        {
            throw std::runtime_error(_getErrorMsgByStatus(deleteErrorStatus));
        }
    }

PlukiPluki::ERROR_STATUS PlukiPluki::
    _checkIfHasRowDeleted (
        std::fstream*&     file,
        const std::string& path, 
        const __IOS_MODE&  currentMode,
        __amountRows       index
    ) noexcept
    {
        if (!_compareModeByIsWrite(currentMode))
        {
            return WRONG_MODE;
        }

        _reopenImpl(file, path, std::ios::in);

        __amountRows amountRows = _getAmountRowsImpl(file);
        if (index >= amountRows)
        {
            return INDEX_OUT_OF_RANGE;
        }

        _reopenImpl(file, path, std::ios::in);

        _deleteRowImpl(file, path, index);

        // ------ Reopen to prev mode ------
        _reopenToPreviosMode(file, path, currentMode);

        return SUCCESS;
    }

void PlukiPluki::
    _deleteRowImpl(
        std::fstream*&     file,
        const std::string& path,  
        __amountRows       index
    ) noexcept
    {
        std::vector<std::string> buf = const_cast<PlukiPluki*>(this)->_indexAllImpl(file);

        buf.erase(buf.begin() + index);

        _clearImpl(file, path);
        
        std::copy(buf.begin(), buf.end(), std::ostream_iterator<std::string>(*file, "\n"));
    }

// End delete row

__amountRows PlukiPluki::
    getAmountRows() const
    {
        const __IOS_MODE& currentMode = _getMode();

        return _getAmountRows(_file, currentMode);
    }

std::string PlukiPluki::
    getRow(__amountRows index) const
    {
        const __IOS_MODE& currentMode = _getMode();

        return _getRow(_file, currentMode, index);
    }

void PlukiPluki::
    setRow(__amountRows index, std::string newRow)
    {
        const __IOS_MODE& currentMode = _getMode();

        _setRow(_file, _path, currentMode, index, newRow);
    }

std::vector<std::string> PlukiPluki::
    indexAll() const
    {
        const __IOS_MODE& currentMode = _getMode();

        return _indexAll(_file, currentMode);
    }

void PlukiPluki::
    clearFile()
    {
        const __IOS_MODE& currentMode = _getMode();

        _clear(_file, _path, currentMode);
    }

void PlukiPluki::
    insertRow(__amountRows index, std::string newRow)
    {
        const __IOS_MODE& currentMode = _getMode();

        _insertRow(_file, _path, currentMode, index, newRow);
    }

void PlukiPluki::
    deleteRow(__amountRows index)
    {
        const __IOS_MODE& currentMode = _getMode();

        _deleteRow(_file, _path, currentMode, index);
    }

std::string PlukiPluki::
    operator[](__amountRows index) const
    {
        return getRow(index);
    }

void PlukiPluki::
    operator()(__amountRows index, std::string newStr)
    {
        setRow(index, newStr);
    }

void PlukiPluki::
    operator()(__amountRows index)
    {
        deleteRow(index);
    }

__amountRows PlukiPluki::
    operator()() const
    {
        return getAmountRows();
    }
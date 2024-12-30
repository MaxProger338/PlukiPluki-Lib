#include "../include/PlukiPluki.h"

using namespace PlukiPlukiLib;

/*
 * TODO: Решить вопросы с inline 
*/

PlukiPluki::
    PlukiPluki(std::string path):
        _path{ "" }, _file{ nullptr }
    {
        _checkOpenFile(_file, path);

        _path = path;
    }

PlukiPluki::
    ~PlukiPluki()
    {
        /*
         * Эта функция выбрасывает исключения, а в деструкторе это вроде как не принято
         TODO: Разобраться как правильно
        */
        // _checkCloseFile(_file); 

        if (_closeFile(_file) != 0)
        {
            /* 
             * Что-то пошло не так 
             * Надо что-то обработать
            */
           delete _file;
        }

        _path = "";
    }

short PlukiPluki::
    _openFile(std::fstream*& file, std::string path)
    {
        if (!isExistsFile(path))
        {
            return 1;
        }

        if (file != nullptr)
        {
            if (file->is_open())
            {
                return 2;
            }
        }

        file = new std::fstream;
        file->open(path);

        return 0;
    }

void PlukiPluki::
    _checkOpenFile(std::fstream*& file, std::string path)
    {
        short status = _openFile(file, path);

        switch (status)
        {
            case 0:
                /* Success */
                break;

            case 1:
                throw std::runtime_error("File not exists!");    
                break;

            case 2:
                throw std::runtime_error("File already is open!");
                break;

            default:
                throw std::runtime_error("Unknow file error!");
        }
    }

short PlukiPluki::
    _closeFile(std::fstream*& file)
    {
        /*
         * WARNING!: ИЗ-ЗА ЭТОГО УСЛОВИЯ ПРИ ДЕФОЛТНОМ КОПИРОВАНИЕ ОБЪЕКТА
         *           (ЗА СЧЁТ КОНСТРУКТОРА КОПИРОВАНИЯ) ОНО ОТРАБОТАЕТ БЕЗ ОШИБОК,
         *           ПОТОМУ ЧТО КОГДА ПЕРВЫЙ ДЕСТРУКТОР СРАБОТАЕТ ОН УДАЛИТ И ПРЕРВНЯЕТ К 
         *           NULLPTR, А ВТОРОЙ ПРИ УДАЛЕНИИ НЕ ВЫДАСТ ОШИБКУ, ПОТОМУ ЧТО ОТРАБОТАЕТ 
         *           ЭТО УСЛОВИЕ.
         *           ИЗ-ЗА ЭТОГО МОЖЕТ ПОКАЗАТЬСЯ, ЧТО КОНСТРУКТОР КОПИРОВАНИЯ РАБОТАЕТ ПРАВИЛЬНО
         *           И ЕГО МОЖНО ОСТАВИТЬ, НО ЭТО НЕ ТАК, ПОТОМУ ЭТОТ КОНСТРУКТОР ПОМЕЧЕН КАК = DELETED
         *           БУДЬТЕ С ЭТИМ ВНИМАТЕЛЬНЫ!!!
         * 
         *              |
         *              ↓
         * 
        */
        if (file == nullptr)   
        {
            return 3;    
        }

        if (!file->is_open())
        {
            return 1;
        }

        file->close();

        delete file;

        return 0;
    }

void PlukiPluki::
    _checkCloseFile(std::fstream*& file)
    {
        short status = _closeFile(file);

        switch (status)
        {
            case 0:
                /* Success */
                break;

            case 1:
                throw std::runtime_error("File not opened!");    
                break;

            case 3:
                throw std::runtime_error("File is equals nullptr!");    
                break;

            default:
                throw std::runtime_error("Unknow file error!");
        }
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

bool PlukiPluki::
    isExistsFile(std::string path)
    {
        return std::filesystem::exists(path);
    }

std::string PlukiPluki::
    operator[](__amountRows index) const
    {
        return getRowByIndex(index);
    }
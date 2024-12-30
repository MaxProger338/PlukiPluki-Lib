#include "FileBase.h"

using namespace PlukiPlukiLib;

/*
 * TODO: Решить вопросы с inline 
*/

FileBase::
    FileBase(std::string path):
        _path{ "" }, _file{ nullptr }
    {
        _checkOpenFile(_file, path);

        _path = path;
    }

FileBase::
    ~FileBase()
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

short FileBase::
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

void FileBase::
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

short FileBase::
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

void FileBase::
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

bool FileBase::
    isExistsFile(std::string path)
    {
        return std::filesystem::exists(path);
    }
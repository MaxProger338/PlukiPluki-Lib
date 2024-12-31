#include "FileBase.h"

using namespace PlukiPlukiLib;

/*
 * TODO: Решить вопросы с inline 
*/

FileBase::
    FileBase(std::string path, const std::_Ios_Openmode& mode):
        _path{ "" }, _file{ nullptr }
    {
        _checkOpenFile(_file, path, mode);

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
            _file = nullptr;
        }

        _path = "";
    }

short FileBase::
    _openFile(std::fstream*& file, std::string path, const std::_Ios_Openmode& mode)
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

            delete file;
        }

        file = new std::fstream;
        file->open(path, mode);

        return 0;
    }

void FileBase::
    _checkOpenFile(std::fstream*& file, std::string path, const std::_Ios_Openmode& mode)
    {
        short status = _openFile(file, path, mode);

        switch (status)
        {
            case 0:
                /* Success */
                break;

            case 1:
                throw std::runtime_error("File not exists!");    
                break;

            case 2:
                throw std::runtime_error("File already is opened!");
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
            return 4;
        }

        file->close();

        delete file;
        file = nullptr;

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

            case 3:
                throw std::runtime_error("File is equals nullptr!");    
                break;

            case 4:
                throw std::runtime_error("File already is closed!");    
                break;

            default:
                throw std::runtime_error("Unknow file error!");
        }
    }

short FileBase::
    _reopen(std::fstream*& file, const std::_Ios_Openmode& mode)
    {
        short closeStatus = _closeFile(file);

        if (closeStatus != 0)
        {
            return closeStatus;
        }

        short openStatus = _openFile(file, _path, mode);

        return openStatus;
    }

void  FileBase::
    _checkReopenFile(std::fstream*& file, const std::_Ios_Openmode& mode)
    {
        short reopenStatus = _reopen(file, mode);

        switch (reopenStatus)
        {
            case 0:
                /* Success */
                break;

            case 1:
                throw std::runtime_error("File not exists!");    
                break;

            case 2:
                throw std::runtime_error("File already is opened!");
                break;

            case 3:
                throw std::runtime_error("File is equals nullptr!");    
                break;

            case 4:
                throw std::runtime_error("File already is closed!");    
                break;

            default:
                throw std::runtime_error("Unknow file error!");
        }
    }

void FileBase::
    reopen(const std::_Ios_Openmode& mode)
    {
        _checkReopenFile(_file, mode);
    }

std::string FileBase::
    getPath() const
    {
        return _path;
    }

std::fstream* FileBase::
    getFile() const
    {
        return _file;
    }

bool FileBase::
    isExistsFile(std::string path)
    {
        return std::filesystem::exists(path);
    }
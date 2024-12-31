#include "FileBase.h"

using namespace PlukiPlukiLib;

/*
 * TODO: Решить вопросы с inline 
*/

FileBase::
    FileBase(std::string path, const std::_Ios_Openmode& mode):
        _path{ "" }, _mode{ nullptr }, _file{ nullptr }
    {
        _checkOpenFile(_file, path, mode);

        _path = path;
        _mode = new std::_Ios_Openmode{ mode };
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

        delete _mode;
        _mode = nullptr;

        _path = "";
    }

std::string FileBase::
    _getErrorMsgByStatus(__fileStatus status) const
    {
        switch (status)
        {
            case 0: /* Success */
            case 1: return "File not exists!";
            case 2: return "File already is opened!";
            case 3: return "File is equals nullptr!";
            case 4: return "File already is closed!";
            case 5: return "Mode is undefined, maybe because file closed!";

            default: return "Unknow file error!";
        }
    }

__fileStatus FileBase::
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
        __fileStatus openStatus = _openFile(file, path, mode);

        if (openStatus != 0)
        {
            throw std::runtime_error(_getErrorMsgByStatus(openStatus));
        }
    }

__fileStatus FileBase::
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
        __fileStatus closeStatus = _closeFile(file);

        if (closeStatus != 0)
        {
            throw std::runtime_error(_getErrorMsgByStatus(closeStatus));
        }
    }

__fileStatus FileBase::
    _reopen(std::fstream*& file, const std::_Ios_Openmode& mode)
    {
        __fileStatus closeStatus = _closeFile(file);

        if (closeStatus != 0)
        {
            return closeStatus;
        }

        __fileStatus openStatus = _openFile(file, _path, mode);

        return openStatus;
    }

void  FileBase::
    _checkReopenFile(std::fstream*& file, const std::_Ios_Openmode& mode)
    {
        __fileStatus reopenStatus = _reopen(file, mode);

        if (reopenStatus != 0)
        {
            throw std::runtime_error(_getErrorMsgByStatus(reopenStatus));
        }
    }

void FileBase::
    reopen(const std::_Ios_Openmode& mode)
    {
        _checkReopenFile(_file, mode);

        *_mode = mode;
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

std::_Ios_Openmode FileBase::
    getMode() const
    {
        if (_mode == nullptr)
        {
            throw _getErrorMsgByStatus(5);
        }

        return *_mode;
    }

bool FileBase::
    isExistsFile(std::string path)
    {
        return std::filesystem::exists(path);
    }
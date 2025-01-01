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
    _getErrorMsgByStatus(FileBase::FILE_ERRORS error) const
    {
        switch (error)
        {
            // case SUCCESS - должно проверяться перед вызовом функции!
            case FILE_NOT_EXISTS:     return "File not exists!";
            case FILE_ALREADY_OPENED: return "File already is opened!";
            case FILE_EQUALS_NULLPTR: return "File is equals nullptr!";
            case FILE_ALREADY_CLOSED: return "File already is closed!";
            case MODE_UNDEFINED:      return "Mode is undefined, maybe because file closed!";
            case WRONG_MODE:          return "Inccorect file mode!";

            default:                  return "Unknow file error!";
        }
    }

FileBase::FILE_ERRORS FileBase::
    _openFile(std::fstream*& file, std::string path, const std::_Ios_Openmode& mode)
    {
        if (!isExistsFile(path))
        {
            return FILE_NOT_EXISTS;
        }

        if (file != nullptr)
        {
            if (file->is_open())
            {
                return FILE_ALREADY_OPENED;
            }

            delete file;
        }

        file = new std::fstream;
        file->open(path, mode);

        return SUCCESS;
    }

void FileBase::
    _checkOpenFile(std::fstream*& file, std::string path, const std::_Ios_Openmode& mode)
    {
        FileBase::FILE_ERRORS openError = _openFile(file, path, mode);

        if (openError != SUCCESS)
        {
            throw std::runtime_error(_getErrorMsgByStatus(openError));
        }
    }

FileBase::FILE_ERRORS FileBase::
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
            return FILE_EQUALS_NULLPTR;    
        }

        if (!file->is_open())
        {
            return FILE_ALREADY_CLOSED;
        }

        file->close();

        delete file;
        file = nullptr;

        return SUCCESS;
    }

void FileBase::
    _checkCloseFile(std::fstream*& file)
    {
        FileBase::FILE_ERRORS closeError = _closeFile(file);

        if (closeError != SUCCESS)
        {
            throw std::runtime_error(_getErrorMsgByStatus(closeError));
        }
    }

FileBase::FILE_ERRORS FileBase::
    _reopen(std::fstream*& file, const std::_Ios_Openmode& mode)
    {
        FileBase::FILE_ERRORS closeError = _closeFile(file);

        if (closeError != SUCCESS)
        {
            return closeError;
        }

        FileBase::FILE_ERRORS openError = _openFile(file, _path, mode);

        return openError;
    }

void  FileBase::
    _checkReopenFile(std::fstream*& file, const std::_Ios_Openmode& mode)
    {
        FileBase::FILE_ERRORS reopenError = _reopen(file, mode);

        if (reopenError != SUCCESS)
        {
            throw std::runtime_error(_getErrorMsgByStatus(reopenError));
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
            throw _getErrorMsgByStatus(MODE_UNDEFINED);
        }

        return *_mode;
    }

bool FileBase::
    isExistsFile(std::string path)
    {
        return std::filesystem::exists(path);
    }
#include "FileBase.h"

using namespace PlukiPlukiLib;

/*
 * TODO: Решить вопросы с inline 
*/

FileBase::
    FileBase(const std::string& path, const __IOS_MODE& mode)
    {
        // Если что пробрасывает исключение
        _open(_file, path, mode);

        _path = path;
        _mode = new __IOS_MODE{ mode };
    }

FileBase::
    ~FileBase()
    {
        /*
         * Эта функция выбрасывает исключения, а в деструкторе это вроде как не принято
         TODO: Разобраться как правильно
         */
        // _checkCloseFile(_file); 

        if (_checkIfHasClosed(_file) != SUCCESS)
        {
            /* 
             * Что-то пошло не так 
             * Надо что-то обработать
             */
            _closeImpl(_file);
        }

        delete _mode;
        _mode = nullptr;

        _path = "";
    }

__IOS_MODE FileBase::
    _getMode() const
    {
        if (_mode == nullptr)
        {
            throw _getErrorMsgByStatus(MODE_UNDEFINED);
        }

        return *_mode;
    }

std::string FileBase::
    _getErrorMsgByStatus(FileBase::ERROR_STATUS error) const noexcept
    {
        switch (error)
        {
            // case SUCCESS - должно проверяться перед вызовом функции!
            case FILE_NOT_EXISTS:     return "File not exists!";
            case FILE_ALREADY_OPENED: return "File already is opened!";
            case FILE_EQUALS_NULLPTR: return "File is equals nullptr!";
            case FILE_ALREADY_CLOSED: return "File already is closed!";
            case MODE_UNDEFINED:      return "Mode is undefined, maybe because file closed!";

            default:                  return "Unknow file error!";
        }
    }

// Open
void FileBase::
    _open(std::fstream*& file, const std::string& path, const __IOS_MODE& mode)
    {
        FileBase::ERROR_STATUS openErrorStatus = _checkIfHasOpened(file, path, mode);

        if (openErrorStatus != SUCCESS)
        {
            throw std::runtime_error(_getErrorMsgByStatus(openErrorStatus));
        }
    }

FileBase::ERROR_STATUS FileBase::
    _checkIfHasOpened(
        std::fstream*&     file, 
        const std::string& path, 
        const __IOS_MODE&  mode
    ) noexcept
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

            return FILE_NOT_EQUALS_NULLPTR;
        }

        _openImpl(file, path, mode);

        return SUCCESS;
    }

void FileBase::
    _openImpl(
        std::fstream*&     file, 
        const std::string& path, 
        const __IOS_MODE&  mode
    ) noexcept
    {
        file = new std::fstream;
        file->open(path, mode);
    }

// End open

// Close
void FileBase::
    _close(std::fstream*& file)
    {
        FileBase::ERROR_STATUS closeErrorStatus = _checkIfHasClosed(file);

        if (closeErrorStatus != SUCCESS)
        {
            throw std::runtime_error(_getErrorMsgByStatus(closeErrorStatus));
        }
    }

FileBase::ERROR_STATUS FileBase::
    _checkIfHasClosed(std::fstream*& file) noexcept
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

        _closeImpl(file);

        return SUCCESS;
    }

void FileBase::
    _closeImpl(std::fstream*& file) noexcept
    {
        file->close();

        delete file;
        file = nullptr;
    }

// End close

// Reopen
void FileBase::
    _reopen(
        std::fstream*&     file, 
        const std::string& path, 
        const __IOS_MODE&  mode
    ) {
        FileBase::ERROR_STATUS reopenErrorStatus = _checkIfHasReopened(file, path, mode);

        if (reopenErrorStatus != SUCCESS)
        {
            throw std::runtime_error(_getErrorMsgByStatus(reopenErrorStatus));
        }
    }

FileBase::ERROR_STATUS FileBase::
    _checkIfHasReopened(
        std::fstream*&     file, 
        const std::string& path, 
        const __IOS_MODE&  mode
    ) noexcept
    {
        if (!isExistsFile(path))
        {
            return FILE_NOT_EXISTS;
        }

        if (file == nullptr)   
        {
            return FILE_EQUALS_NULLPTR;    
        }

        if (!file->is_open())
        {
            return FILE_ALREADY_CLOSED;
        }

        _reopenImpl(file, path, mode);

        return SUCCESS;
    }

void FileBase::
    _reopenImpl(
        std::fstream*&     file, 
        const std::string& path, 
        const __IOS_MODE&  mode
    ) noexcept
    {
        _closeImpl(file);

        _openImpl(file, path, mode);
    }

// End reopen

void FileBase::
    reopen(const __IOS_MODE& mode)
    {
        _reopen(_file, _path, mode);

        *_mode = mode;
    }

std::string FileBase::
    getPath() const noexcept
    {
        return _path;
    }

std::fstream* FileBase::
    getFile() const noexcept
    {
        return _file;
    }

__IOS_MODE FileBase::
    getMode() const
    {
        return _getMode();
    }

bool FileBase::
    isExistsFile(std::string path) noexcept
    {
        return std::filesystem::exists(path);
    }
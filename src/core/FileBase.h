#pragma once

#include <string>
#include <fstream>
#include <exception>
#include <filesystem>
#include <iostream>

namespace PlukiPlukiLib
{
    class FileBase
    {
        protected:
            enum FILE_ERRORS
            {
                SUCCESS              = 0,
                FILE_NOT_EXISTS      = 1,
                FILE_ALREADY_OPENED  = 2,
                FILE_EQUALS_NULLPTR  = 3,
                FILE_ALREADY_CLOSED  = 4,
                MODE_UNDEFINED       = 5,
                WRONG_MODE           = 6,
            };

            std::string         _path;
            std::_Ios_Openmode* _mode;
            std::fstream*       _file;

            std::string _getErrorMsgByStatus(FILE_ERRORS status) const;

            FILE_ERRORS _openFile(       std::fstream*& file, std::string path, const std::_Ios_Openmode& mode);
            
            void        _checkOpenFile(  std::fstream*& file, std::string path, const std::_Ios_Openmode& mode);

            FILE_ERRORS _closeFile(      std::fstream*& file);

            void        _checkCloseFile( std::fstream*& file);

            FILE_ERRORS _reopen(         std::fstream*& file, const std::_Ios_Openmode& mode);

            void        _checkReopenFile(std::fstream*& file, const std::_Ios_Openmode& mode);

        public:
            FileBase(std::string path, const std::_Ios_Openmode& mode);

            /*
             * TODO: Поразмышлять на тему удаления конструкторов и операторов присвоения
             */
            FileBase(const FileBase& fileBase)           = delete;

            FileBase(FileBase&& fileBase)                = delete;

            FileBase operator=(const FileBase& fileBase) = delete;

            FileBase operator=(FileBase&& fileBase)      = delete;

            ~FileBase();

            void reopen(const std::_Ios_Openmode& mode);

            std::string getPath()   const;

            std::fstream* getFile() const;

            std::_Ios_Openmode getMode() const;

            static bool isExistsFile(std::string path);
    };
};
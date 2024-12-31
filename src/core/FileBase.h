#pragma once

#include <string>
#include <fstream>
#include <exception>
#include <filesystem>
#include <iostream>

namespace PlukiPlukiLib
{
    typedef short __fileStatus;

    class FileBase
    {
        protected:
            std::string   _path;
            std::fstream* _file;

            std::string _getErrorMsgByStatus(__fileStatus status) const;

            __fileStatus _openFile(std::fstream*& file, std::string path, const std::_Ios_Openmode& mode);
            
            void  _checkOpenFile(std::fstream*& file, std::string path, const std::_Ios_Openmode& mode);

            __fileStatus _closeFile(std::fstream*& file);

            void  _checkCloseFile(std::fstream*& file);

            __fileStatus _reopen(std::fstream*& file, const std::_Ios_Openmode& mode);

            void  _checkReopenFile(std::fstream*& file, const std::_Ios_Openmode& mode);

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

            std::string getPath() const;

            std::fstream* getFile() const;

            static bool isExistsFile(std::string path);
    };
};
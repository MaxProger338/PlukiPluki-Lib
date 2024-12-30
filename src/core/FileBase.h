#pragma once

#include <string>
#include <fstream>
#include <exception>
#include <filesystem>

namespace PlukiPlukiLib
{
    class FileBase
    {
        protected:
            std::string   _path;
            std::fstream* _file;

            short _openFile(std::fstream*& file, std::string path);
            
            void  _checkOpenFile(std::fstream*& file, std::string path);

            short _closeFile(std::fstream*& file);

            void  _checkCloseFile(std::fstream*& file);

        public:
            FileBase(std::string path);

            /*
             * TODO: Поразмышлять на тему удаления конструкторов и операторов присвоения
             */
            FileBase(const FileBase& fileBase)           = delete;

            FileBase(FileBase&& fileBase)                = delete;

            FileBase operator=(const FileBase& fileBase) = delete;

            FileBase operator=(FileBase&& fileBase)      = delete;

            ~FileBase();

            std::string getPath() const;

            std::fstream* getFile() const;

            static bool isExistsFile(std::string path);
    };
};
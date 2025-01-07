#pragma once

#include <string>
#include <fstream>
#include <exception>
#include <filesystem>
#include <iostream>


namespace PlukiPlukiLib
{
    typedef std::_Ios_Openmode __IOS_MODE;

    class FileBase
    {

        private:
            enum ERROR_STATUS
            {
                SUCCESS                  = 0,
                FILE_NOT_EXISTS          = 1,
                FILE_ALREADY_OPENED      = 2,
                FILE_EQUALS_NULLPTR      = 3,
                FILE_ALREADY_CLOSED      = 4,
                FILE_NOT_EQUALS_NULLPTR  = 5,
                MODE_UNDEFINED           = 6,
            };

            std::string _getErrorMsgByStatus(ERROR_STATUS error) const noexcept;

        protected:
            std::string   _path = "";
            __IOS_MODE*   _mode = nullptr;
            std::fstream* _file = nullptr;

            __IOS_MODE  _getMode()     const;

            // Open 
            void         _open               (
                                                std::fstream*&     file, 
                                                const std::string& path, 
                                                const __IOS_MODE&  mode
                                             );

            ERROR_STATUS _checkIfHasOpened   (
                                                std::fstream*&     file, 
                                                const std::string& path, 
                                                const __IOS_MODE&  mode
                                             ) noexcept;
            
            void         _openImpl           (
                                                std::fstream*&     file, 
                                                const std::string& path, 
                                                const __IOS_MODE&  mode
                                             ) noexcept;

            // Close
            void         _close              (std::fstream*& file);

            ERROR_STATUS _checkIfHasClosed   (std::fstream*& file) 
                                                noexcept;

            void         _closeImpl          (std::fstream*& file) 
                                                noexcept;

            // Reopen
            void         _reopen             (
                                                std::fstream*&     file, 
                                                const std::string& path, 
                                                const __IOS_MODE&  mode
                                             );

            ERROR_STATUS _checkIfHasReopened (
                                                std::fstream*&     file, 
                                                const std::string& path, 
                                                const __IOS_MODE&  mode
                                             ) noexcept;

            void         _reopenImpl         (
                                                std::fstream*& file, 
                                                const std::string& path, 
                                                const __IOS_MODE& mode
                                             ) noexcept;

            // Get mode
            

        public:
            FileBase (const std::string& path, const __IOS_MODE& mode);

            /*
             * TODO: Поразмышлять на тему удаления конструкторов и операторов присвоения
             */
            FileBase          (const FileBase& fileBase) = delete;

            FileBase          (FileBase&&      fileBase) = delete;

            FileBase operator=(const FileBase& fileBase) = delete;

            FileBase operator=(FileBase&&      fileBase) = delete;

            ~FileBase();

            void reopen(const __IOS_MODE& mode);

            std::string        getPath() const          noexcept;

            std::fstream*      getFile() const          noexcept;

            __IOS_MODE         getMode() const;

            static bool isExistsFile (std::string path) noexcept;
    };
};
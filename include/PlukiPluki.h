#pragma once

#include "../src/core/FileBase.h"

#include <fstream>
#include <exception>
#include <vector>
#include <string>
#include <iterator>
#include <algorithm>

//! #define DEBUG_MODE

#ifdef DEBUG_MODE

#include <iostream>
#define LOG(msg) std::cout << msg;

#endif

namespace PlukiPlukiLib
{
    typedef unsigned int __amountRows;
    typedef unsigned int __amountSymbols;

    class PlukiPluki : public FileBase
    {
        private:
            enum ERROR_STATUS
            {
                SUCCESS              = 0,
                WRONG_MODE           = 1,
                INDEX_OUT_OF_RANGE   = 2,
            };

            std::string              _getErrorMsgByStatus    (
                                                                ERROR_STATUS error
                                                             ) const noexcept;

        protected:
            bool                     _compareModeByIsRead    (
                                                                const __IOS_MODE& currentMode
                                                             ) const noexcept;

            bool                     _compareModeByIsWrite   (
                                                                const __IOS_MODE& currentMode
                                                             ) const noexcept;

            void                     _reopenToPreviosMode    (
                                                                std::fstream*&      file,
                                                                const std::string& path,
                                                                const __IOS_MODE&  currentMode
                                                             ) noexcept;

            // Get amount rows
            __amountRows             _getAmountRows          (
                                                                std::fstream*     file,
                                                                const __IOS_MODE& currentMode
                                                             ) const;

            __amountRows             _getAmountRowsImpl      (
                                                                std::fstream*     file
                                                             ) const noexcept;

            // Get row by index
            std::string              _getRow                 (
                                                                std::fstream*     file,
                                                                const __IOS_MODE& currentMode, 
                                                                __amountRows      index
                                                             ) const;

            std::string              _getRowImpl             (
                                                                std::fstream* file, 
                                                                __amountRows  index
                                                             ) const noexcept;

            // Set row by index
            void                     _setRow                 (
                                                                std::fstream*&     file, 
                                                                const std::string& path, 
                                                                const __IOS_MODE&  currentMode,
                                                                __amountRows       index, 
                                                                std::string        newStr
                                                             );

            ERROR_STATUS             _checkIsRowSetted       (
                                                                std::fstream*&     file, 
                                                                const std::string& path, 
                                                                const __IOS_MODE&  currentMode,
                                                                __amountRows       index, 
                                                                std::string        newStr
                                                             ) noexcept;

            void                     _setRowImpl             (
                                                                std::fstream*&     file, 
                                                                const std::string& path, 
                                                                __amountRows       index, 
                                                                std::string        newStr
                                                             ) noexcept;

            // Index all file
            std::vector<std::string> _indexAll               (
                                                                std::fstream*     file, 
                                                                const __IOS_MODE& currentMode
                                                             ) const;

            std::vector<std::string> _indexAllImpl           (std::fstream* file) 
                                                                const noexcept;

            // Clear
            void                      _clear                 (
                                                                std::fstream*&     file,
                                                                const std::string& path, 
                                                                const __IOS_MODE&  currentMode
                                                             );

            ERROR_STATUS              _checkIfHasCleared     (
                                                                std::fstream*&     file,
                                                                const std::string& path, 
                                                                const __IOS_MODE&  currentMode
                                                             ) noexcept;

            void                      _clearImpl             (
                                                                std::fstream*&     file, 
                                                                const std::string& path
                                                             ) noexcept;

            // Insert row
            void                      _insertRow             (
                                                                std::fstream*&     file, 
                                                                const std::string& path, 
                                                                const __IOS_MODE&  currentMode,
                                                                __amountRows       index, 
                                                                std::string        newRow
                                                             );

            ERROR_STATUS              _checkIfHasRowInserted (
                                                                std::fstream*&     file, 
                                                                const std::string& path, 
                                                                const __IOS_MODE&  currentMode,
                                                                __amountRows       index, 
                                                                std::string        newRow
                                                             ) noexcept;

            void                      _insertRowImpl         (
                                                                std::fstream*&     file, 
                                                                const std::string& path,
                                                                __amountRows       index, 
                                                                std::string        newRow
                                                             ) noexcept;

            // Delete row
            void                      _deleteRow             (
                                                                std::fstream*&     file,
                                                                const std::string& path, 
                                                                const __IOS_MODE&  currentMode,
                                                                __amountRows       index   
                                                             );

            ERROR_STATUS              _checkIfHasRowDeleted  (
                                                                std::fstream*&     file,
                                                                const std::string& path, 
                                                                const __IOS_MODE&  currentMode,
                                                                __amountRows       index
                                                             ) noexcept;

            void                      _deleteRowImpl         (
                                                                std::fstream*&     file,
                                                                const std::string& path, 
                                                                __amountRows       index
                                                             ) noexcept;

        public:
            PlukiPluki                        (std::string path, const __IOS_MODE& mode);

            PlukiPluki                        (const PlukiPluki& plukiPluki) 
                                                = delete;
         
            PlukiPluki                        (PlukiPluki&&      plukiPluki) 
                                                = delete;

            PlukiPluki operator=              (const PlukiPluki& plukiPluki) 
                                                = delete;

            PlukiPluki operator=              (PlukiPluki&&      plukiPluki) 
                                                = delete;

            __amountRows getAmountRows        ()             
                                                const;

            std::string  getRow               (__amountRows index)                      
                                                const;

            void         setRow               (__amountRows index, std::string newRow);

            std::vector<std::string> indexAll ()
                                                const;

            void clearFile                    ();

            void insertRow                    (__amountRows index, std::string newRow);

            void deleteRow                    (__amountRows index);

            // Call getRow
            std::string  operator[]           (__amountRows index)
                                                const;

            // Call setRow
            void         operator()           (__amountRows index, std::string newStr);

            // Call deleteRow
            void         operator()           (__amountRows index);

            // Call getAmountRows
            __amountRows operator()           ()
                                                const;
    };
};
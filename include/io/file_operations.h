#ifndef FILE_OPERATIONS_H
#define FILE_OPERATIONS_H

#include <stdbool.h>
#include "core/document.h"

/**
 * @file file_operations.h
 * @brief File I/O operations interface - handles reading/writing documents
 * 
 * This module follows the Single Responsibility Principle by handling only
 * file system operations related to document persistence.
 */

/**
 * @brief Result codes for file operations
 */
typedef enum {
    FILE_OP_SUCCESS = 0,
    FILE_OP_ERROR_OPEN,
    FILE_OP_ERROR_READ,
    FILE_OP_ERROR_WRITE,
    FILE_OP_ERROR_MEMORY,
    FILE_OP_ERROR_INVALID_PATH,
    FILE_OP_ERROR_PERMISSION
} FileOperationResult;

/**
 * @brief Callback for error reporting
 * @param error_code The error code
 * @param message Error message
 * @param user_data User-provided data
 */
typedef void (*FileErrorCallback)(FileOperationResult error_code, 
                                   const char* message, 
                                   void* user_data);

/**
 * @brief Reads a file and loads its content into a document
 * @param path Path to the file to read
 * @param doc Document to load content into
 * @return Result code indicating success or failure
 */
FileOperationResult file_operations_read(const char* path, Document* doc);

/**
 * @brief Writes document content to a file
 * @param path Path to the file to write
 * @param doc Document to save
 * @return Result code indicating success or failure
 */
FileOperationResult file_operations_write(const char* path, const Document* doc);

/**
 * @brief Checks if a file exists
 * @param path Path to check
 * @return true if file exists, false otherwise
 */
bool file_operations_exists(const char* path);

/**
 * @brief Checks if a file is readable
 * @param path Path to check
 * @return true if file is readable, false otherwise
 */
bool file_operations_is_readable(const char* path);

/**
 * @brief Checks if a file is writable
 * @param path Path to check
 * @return true if file is writable, false otherwise
 */
bool file_operations_is_writable(const char* path);

/**
 * @brief Gets a human-readable error message for a result code
 * @param result The result code
 * @return Error message string
 */
const char* file_operations_get_error_message(FileOperationResult result);

#endif /* FILE_OPERATIONS_H */
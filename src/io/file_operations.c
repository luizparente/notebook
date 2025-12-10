#define _POSIX_C_SOURCE 200809L
#include "io/file_operations.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>

/**
 * @brief Error messages for each result code
 */
static const char* error_messages[] = {
    [FILE_OP_SUCCESS] = "Operation successful",
    [FILE_OP_ERROR_OPEN] = "Failed to open file",
    [FILE_OP_ERROR_READ] = "Failed to read file",
    [FILE_OP_ERROR_WRITE] = "Failed to write file",
    [FILE_OP_ERROR_MEMORY] = "Memory allocation failed",
    [FILE_OP_ERROR_INVALID_PATH] = "Invalid file path",
    [FILE_OP_ERROR_PERMISSION] = "Permission denied"
};

FileOperationResult file_operations_read(const char* path, Document* doc) {
    if (!path || !doc) {
        return FILE_OP_ERROR_INVALID_PATH;
    }
    
    FILE* file = fopen(path, "rb");
    if (!file) {
        if (errno == EACCES) {
            return FILE_OP_ERROR_PERMISSION;
        }
        return FILE_OP_ERROR_OPEN;
    }
    
    // Get file size
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    if (file_size < 0) {
        fclose(file);
        return FILE_OP_ERROR_READ;
    }
    
    // Allocate buffer
    char* buffer = (char*)malloc(file_size + 1);
    if (!buffer) {
        fclose(file);
        return FILE_OP_ERROR_MEMORY;
    }
    
    // Read file content
    size_t bytes_read = fread(buffer, 1, file_size, file);
    fclose(file);
    
    if (bytes_read != (size_t)file_size) {
        free(buffer);
        return FILE_OP_ERROR_READ;
    }
    
    buffer[file_size] = '\0';
    
    // Update document
    if (!document_set_content(doc, buffer)) {
        free(buffer);
        return FILE_OP_ERROR_MEMORY;
    }
    
    if (!document_set_file_path(doc, path)) {
        free(buffer);
        return FILE_OP_ERROR_MEMORY;
    }
    
    document_mark_saved(doc);
    free(buffer);
    
    return FILE_OP_SUCCESS;
}

FileOperationResult file_operations_write(const char* path, const Document* doc) {
    if (!path || !doc) {
        return FILE_OP_ERROR_INVALID_PATH;
    }
    
    const char* content = document_get_content(doc);
    if (!content) {
        return FILE_OP_ERROR_MEMORY;
    }
    
    FILE* file = fopen(path, "wb");
    if (!file) {
        if (errno == EACCES) {
            return FILE_OP_ERROR_PERMISSION;
        }
        return FILE_OP_ERROR_OPEN;
    }
    
    size_t content_len = strlen(content);
    size_t bytes_written = fwrite(content, 1, content_len, file);
    
    if (bytes_written != content_len) {
        fclose(file);
        return FILE_OP_ERROR_WRITE;
    }
    
    if (fclose(file) != 0) {
        return FILE_OP_ERROR_WRITE;
    }
    
    return FILE_OP_SUCCESS;
}

bool file_operations_exists(const char* path) {
    if (!path) {
        return false;
    }
    
    struct stat st;
    return stat(path, &st) == 0;
}

bool file_operations_is_readable(const char* path) {
    if (!path) {
        return false;
    }
    
    return access(path, R_OK) == 0;
}

bool file_operations_is_writable(const char* path) {
    if (!path) {
        return false;
    }
    
    // Check if file exists
    if (file_operations_exists(path)) {
        return access(path, W_OK) == 0;
    }
    
    // If file doesn't exist, check if directory is writable
    char* path_copy = strdup(path);
    if (!path_copy) {
        return false;
    }
    
    char* last_slash = strrchr(path_copy, '/');
    bool result = false;
    
    if (last_slash) {
        *last_slash = '\0';
        result = access(path_copy, W_OK) == 0;
    } else {
        result = access(".", W_OK) == 0;
    }
    
    free(path_copy);
    return result;
}

const char* file_operations_get_error_message(FileOperationResult result) {
    if (result >= 0 && result < sizeof(error_messages) / sizeof(error_messages[0])) {
        return error_messages[result];
    }
    return "Unknown error";
}
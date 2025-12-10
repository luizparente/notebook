#define _POSIX_C_SOURCE 200809L
#include "clipboard/clipboard_operations.h"
#include <stdlib.h>
#include <string.h>

/**
 * @brief Clipboard operations structure
 * 
 * In a real implementation, this would interface with the system clipboard.
 * For simplicity, we're using an internal buffer that can be extended
 * to use GTK's clipboard API or platform-specific clipboard APIs.
 */
struct ClipboardOperations {
    char* buffer;
    ClipboardCallback callback;
    void* user_data;
};

ClipboardOperations* clipboard_operations_create(void) {
    ClipboardOperations* clipboard = (ClipboardOperations*)malloc(sizeof(ClipboardOperations));
    if (!clipboard) {
        return NULL;
    }
    
    clipboard->buffer = NULL;
    clipboard->callback = NULL;
    clipboard->user_data = NULL;
    
    return clipboard;
}

void clipboard_operations_destroy(ClipboardOperations* clipboard) {
    if (!clipboard) {
        return;
    }
    
    free(clipboard->buffer);
    free(clipboard);
}

bool clipboard_operations_copy(ClipboardOperations* clipboard, const char* text) {
    if (!clipboard || !text) {
        return false;
    }
    
    char* new_buffer = strdup(text);
    if (!new_buffer) {
        return false;
    }
    
    free(clipboard->buffer);
    clipboard->buffer = new_buffer;
    
    if (clipboard->callback) {
        clipboard->callback(clipboard->user_data);
    }
    
    return true;
}

char* clipboard_operations_paste(ClipboardOperations* clipboard) {
    if (!clipboard || !clipboard->buffer) {
        return NULL;
    }
    
    return strdup(clipboard->buffer);
}

bool clipboard_operations_has_text(const ClipboardOperations* clipboard) {
    if (!clipboard) {
        return false;
    }
    
    return clipboard->buffer != NULL && clipboard->buffer[0] != '\0';
}

void clipboard_operations_clear(ClipboardOperations* clipboard) {
    if (!clipboard) {
        return;
    }
    
    free(clipboard->buffer);
    clipboard->buffer = NULL;
}
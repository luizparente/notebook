#ifndef CLIPBOARD_OPERATIONS_H
#define CLIPBOARD_OPERATIONS_H

#include <stdbool.h>

/**
 * @file clipboard_operations.h
 * @brief Clipboard operations interface - handles copy/paste functionality
 * 
 * This module follows the Single Responsibility Principle by handling only
 * clipboard-related operations (copy, cut, paste).
 */

typedef struct ClipboardOperations ClipboardOperations;

/**
 * @brief Callback for clipboard events
 * @param user_data User-provided data
 */
typedef void (*ClipboardCallback)(void* user_data);

/**
 * @brief Creates a new clipboard operations handler
 * @return Pointer to clipboard operations instance, or NULL on failure
 */
ClipboardOperations* clipboard_operations_create(void);

/**
 * @brief Destroys clipboard operations handler and frees resources
 * @param clipboard Clipboard operations instance to destroy
 */
void clipboard_operations_destroy(ClipboardOperations* clipboard);

/**
 * @brief Copies text to the system clipboard
 * @param clipboard Clipboard operations instance
 * @param text Text to copy
 * @return true on success, false on failure
 */
bool clipboard_operations_copy(ClipboardOperations* clipboard, const char* text);

/**
 * @brief Retrieves text from the system clipboard
 * @param clipboard Clipboard operations instance
 * @return Pointer to clipboard text (caller must free), or NULL if empty/error
 */
char* clipboard_operations_paste(ClipboardOperations* clipboard);

/**
 * @brief Checks if clipboard has text content
 * @param clipboard Clipboard operations instance
 * @return true if clipboard has text, false otherwise
 */
bool clipboard_operations_has_text(const ClipboardOperations* clipboard);

/**
 * @brief Clears the clipboard
 * @param clipboard Clipboard operations instance
 */
void clipboard_operations_clear(ClipboardOperations* clipboard);

#endif /* CLIPBOARD_OPERATIONS_H */
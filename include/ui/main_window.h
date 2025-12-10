#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <gtk/gtk.h>
#include "core/application.h"

/**
 * @file main_window.h
 * @brief Main window UI interface - handles the GTK window and widgets
 * 
 * This module follows the Single Responsibility Principle by handling only
 * UI presentation and user interaction. It depends on the Application
 * controller abstraction (Dependency Inversion Principle).
 */

typedef struct MainWindow MainWindow;

/**
 * @brief Creates a new main window
 * @param app Application instance to associate with this window
 * @return Pointer to main window instance, or NULL on failure
 */
MainWindow* main_window_create(Application* app);

/**
 * @brief Destroys main window and frees resources
 * @param window Main window instance to destroy
 */
void main_window_destroy(MainWindow* window);

/**
 * @brief Shows the main window
 * @param window Main window instance
 */
void main_window_show(MainWindow* window);

/**
 * @brief Gets the GTK window widget
 * @param window Main window instance
 * @return Pointer to GTK window widget
 */
GtkWidget* main_window_get_widget(const MainWindow* window);

/**
 * @brief Gets the text view widget
 * @param window Main window instance
 * @return Pointer to GTK text view widget
 */
GtkWidget* main_window_get_text_view(const MainWindow* window);

/**
 * @brief Updates the window title based on document state
 * @param window Main window instance
 * @param file_path Current file path (or NULL for untitled)
 * @param modified Whether document is modified
 */
void main_window_update_title(MainWindow* window, 
                              const char* file_path, 
                              bool modified);

/**
 * @brief Gets the current text content from the editor
 * @param window Main window instance
 * @return Text content (caller must free), or NULL on error
 */
char* main_window_get_text(const MainWindow* window);

/**
 * @brief Sets the text content in the editor
 * @param window Main window instance
 * @param text Text to set
 */
void main_window_set_text(MainWindow* window, const char* text);

/**
 * @brief Applies the current theme to the window
 * @param window Main window instance
 */
void main_window_apply_theme(MainWindow* window);

/**
 * @brief Shows an error dialog
 * @param window Main window instance
 * @param message Error message to display
 */
void main_window_show_error(MainWindow* window, const char* message);

/**
 * @brief Shows a confirmation dialog
 * @param window Main window instance
 * @param message Message to display
 * @return true if user confirmed, false otherwise
 */
bool main_window_confirm(MainWindow* window, const char* message);

/**
 * @brief Shows file chooser dialog for opening
 * @param window Main window instance
 * @return Selected file path (caller must free), or NULL if cancelled
 */
char* main_window_choose_file_open(MainWindow* window);

/**
 * @brief Shows file chooser dialog for saving
 * @param window Main window instance
 * @return Selected file path (caller must free), or NULL if cancelled
 */
char* main_window_choose_file_save(MainWindow* window);

#endif /* MAIN_WINDOW_H */
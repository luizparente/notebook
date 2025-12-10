#ifndef THEME_MANAGER_H
#define THEME_MANAGER_H

#include <stdbool.h>

/**
 * @file theme_manager.h
 * @brief Theme management interface - handles application theming
 * 
 * This module follows the Single Responsibility Principle by handling only
 * theme-related operations (switching themes, getting theme properties).
 * Follows Open/Closed Principle by allowing new themes to be added without
 * modifying existing code.
 */

/**
 * @brief Available theme types
 */
typedef enum {
    THEME_DARK,
    THEME_LIGHT
} ThemeType;

/**
 * @brief Theme color configuration
 */
typedef struct {
    const char* background;
    const char* foreground;
    const char* selection_bg;
    const char* selection_fg;
    const char* cursor;
} ThemeColors;

typedef struct ThemeManager ThemeManager;

/**
 * @brief Callback for theme change events
 * @param theme The new theme type
 * @param user_data User-provided data
 */
typedef void (*ThemeChangeCallback)(ThemeType theme, void* user_data);

/**
 * @brief Creates a new theme manager
 * @param default_theme The default theme to use
 * @return Pointer to theme manager instance, or NULL on failure
 */
ThemeManager* theme_manager_create(ThemeType default_theme);

/**
 * @brief Destroys theme manager and frees resources
 * @param manager Theme manager instance to destroy
 */
void theme_manager_destroy(ThemeManager* manager);

/**
 * @brief Gets the current theme type
 * @param manager Theme manager instance
 * @return Current theme type
 */
ThemeType theme_manager_get_current(const ThemeManager* manager);

/**
 * @brief Sets the current theme
 * @param manager Theme manager instance
 * @param theme Theme to set
 */
void theme_manager_set_theme(ThemeManager* manager, ThemeType theme);

/**
 * @brief Toggles between dark and light themes
 * @param manager Theme manager instance
 */
void theme_manager_toggle(ThemeManager* manager);

/**
 * @brief Gets color configuration for current theme
 * @param manager Theme manager instance
 * @return Pointer to theme colors structure
 */
const ThemeColors* theme_manager_get_colors(const ThemeManager* manager);

/**
 * @brief Registers a callback for theme change events
 * @param manager Theme manager instance
 * @param callback Callback function to register
 * @param user_data User data to pass to callback
 */
void theme_manager_register_callback(ThemeManager* manager, 
                                     ThemeChangeCallback callback,
                                     void* user_data);

/**
 * @brief Gets CSS style for current theme
 * @param manager Theme manager instance
 * @return CSS string for the current theme
 */
const char* theme_manager_get_css(const ThemeManager* manager);

#endif /* THEME_MANAGER_H */
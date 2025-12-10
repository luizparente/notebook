#include "theme/theme_manager.h"
#include <stdlib.h>
#include <string.h>

/**
 * @brief Dark theme colors
 */
static const ThemeColors dark_theme = {
    .background = "#1e1e1e",
    .foreground = "#d4d4d4",
    .selection_bg = "#264f78",
    .selection_fg = "#ffffff",
    .cursor = "#aeafad"
};

/**
 * @brief Light theme colors
 */
static const ThemeColors light_theme = {
    .background = "#ffffff",
    .foreground = "#000000",
    .selection_bg = "#add6ff",
    .selection_fg = "#000000",
    .cursor = "#000000"
};

/**
 * @brief Dark theme CSS
 */
static const char* dark_theme_css = 
    "textview {"
    "  background-color: #1e1e1e;"
    "  color: #d4d4d4;"
    "  font-family: monospace;"
    "  font-size: 11pt;"
    "}"
    "textview text {"
    "  background-color: #1e1e1e;"
    "  color: #d4d4d4;"
    "}"
    "textview selection {"
    "  background-color: #264f78;"
    "  color: #ffffff;"
    "}"
    "textview border {"
    "  background-color: #252526;"
    "  color: #858585;"
    "}"
    "textview:focus {"
    "  background-color: #1e1e1e;"
    "  caret-color: #aeafad;"
    "}"
    "textview text selection:focus {"
    "  background-color: #2b2b2b;"
    "}";

/**
 * @brief Light theme CSS
 */
static const char* light_theme_css = 
    "textview {"
    "  background-color: #ffffff;"
    "  color: #000000;"
    "  font-family: monospace;"
    "  font-size: 11pt;"
    "}"
    "textview text {"
    "  background-color: #ffffff;"
    "  color: #000000;"
    "}"
    "textview selection {"
    "  background-color: #add6ff;"
    "  color: #000000;"
    "}"
    "textview border {"
    "  background-color: #f0f0f0;"
    "  color: #237893;"
    "}"
    "textview:focus {"
    "  background-color: #ffffff;"
    "  caret-color: #000000;"
    "}"
    "textview text selection:focus {"
    "  background-color: #f2f2f2;"
    "}";

/**
 * @brief Theme manager structure
 */
struct ThemeManager {
    ThemeType current_theme;
    ThemeChangeCallback callback;
    void* user_data;
};

ThemeManager* theme_manager_create(ThemeType default_theme) {
    ThemeManager* manager = (ThemeManager*)malloc(sizeof(ThemeManager));
    if (!manager) {
        return NULL;
    }
    
    manager->current_theme = default_theme;
    manager->callback = NULL;
    manager->user_data = NULL;
    
    return manager;
}

void theme_manager_destroy(ThemeManager* manager) {
    if (!manager) {
        return;
    }
    
    free(manager);
}

ThemeType theme_manager_get_current(const ThemeManager* manager) {
    if (!manager) {
        return THEME_DARK;
    }
    
    return manager->current_theme;
}

void theme_manager_set_theme(ThemeManager* manager, ThemeType theme) {
    if (!manager) {
        return;
    }
    
    if (manager->current_theme != theme) {
        manager->current_theme = theme;
        
        if (manager->callback) {
            manager->callback(theme, manager->user_data);
        }
    }
}

void theme_manager_toggle(ThemeManager* manager) {
    if (!manager) {
        return;
    }
    
    ThemeType new_theme = (manager->current_theme == THEME_DARK) 
                          ? THEME_LIGHT 
                          : THEME_DARK;
    
    theme_manager_set_theme(manager, new_theme);
}

const ThemeColors* theme_manager_get_colors(const ThemeManager* manager) {
    if (!manager) {
        return &dark_theme;
    }
    
    return (manager->current_theme == THEME_DARK) 
           ? &dark_theme 
           : &light_theme;
}

void theme_manager_register_callback(ThemeManager* manager, 
                                     ThemeChangeCallback callback,
                                     void* user_data) {
    if (!manager) {
        return;
    }
    
    manager->callback = callback;
    manager->user_data = user_data;
}

const char* theme_manager_get_css(const ThemeManager* manager) {
    if (!manager) {
        return dark_theme_css;
    }
    
    return (manager->current_theme == THEME_DARK) 
           ? dark_theme_css 
           : light_theme_css;
}
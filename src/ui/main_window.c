#include "ui/main_window.h"
#include "theme/theme_manager.h"
#include <gtksourceview/gtksource.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Main window structure - holds all GTK widgets and state
 */
struct MainWindow {
    Application* app;
    GtkWidget* window;
    GtkWidget* text_view;
    GtkTextBuffer* text_buffer;
    GtkCssProvider* css_provider;
    bool ignore_buffer_changes;
};

/* Forward declarations for callbacks */
static void on_new_activated(GtkWidget* widget, gpointer user_data);
static void on_open_activated(GtkWidget* widget, gpointer user_data);
static void on_save_activated(GtkWidget* widget, gpointer user_data);
static void on_save_as_activated(GtkWidget* widget, gpointer user_data);
static void on_quit_activated(GtkWidget* widget, gpointer user_data);
static void on_cut_activated(GtkWidget* widget, gpointer user_data);
static void on_copy_activated(GtkWidget* widget, gpointer user_data);
static void on_paste_activated(GtkWidget* widget, gpointer user_data);
static void on_select_all_activated(GtkWidget* widget, gpointer user_data);
static void on_toggle_theme_activated(GtkWidget* widget, gpointer user_data);
static void on_about_activated(GtkWidget* widget, gpointer user_data);
static void on_buffer_changed(GtkTextBuffer* buffer, gpointer user_data);
static gboolean on_window_delete(GtkWidget* widget, GdkEvent* event, gpointer user_data);
static void on_theme_changed(ThemeType theme, void* user_data);
static void on_document_modified(void* user_data);
static void on_document_saved(void* user_data);
static void on_new_document(void* user_data);
static void on_document_loaded(void* user_data);
static void on_error(const char* message, void* user_data);

/**
 * @brief Creates the menu bar
 */
static GtkWidget* create_menu_bar(MainWindow* window) {
    GtkWidget* menu_bar = gtk_menu_bar_new();
    
    /* File menu */
    GtkWidget* file_menu = gtk_menu_new();
    GtkWidget* file_item = gtk_menu_item_new_with_label("File");
    
    GtkWidget* new_item = gtk_menu_item_new_with_label("New");
    GtkWidget* open_item = gtk_menu_item_new_with_label("Open...");
    GtkWidget* save_item = gtk_menu_item_new_with_label("Save");
    GtkWidget* save_as_item = gtk_menu_item_new_with_label("Save As...");
    GtkWidget* quit_item = gtk_menu_item_new_with_label("Quit");
    
    gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), new_item);
    gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), open_item);
    gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), gtk_separator_menu_item_new());
    gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), save_item);
    gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), save_as_item);
    gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), gtk_separator_menu_item_new());
    gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), quit_item);
    
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(file_item), file_menu);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), file_item);
    
    g_signal_connect(new_item, "activate", G_CALLBACK(on_new_activated), window);
    g_signal_connect(open_item, "activate", G_CALLBACK(on_open_activated), window);
    g_signal_connect(save_item, "activate", G_CALLBACK(on_save_activated), window);
    g_signal_connect(save_as_item, "activate", G_CALLBACK(on_save_as_activated), window);
    g_signal_connect(quit_item, "activate", G_CALLBACK(on_quit_activated), window);
    
    /* Edit menu */
    GtkWidget* edit_menu = gtk_menu_new();
    GtkWidget* edit_item = gtk_menu_item_new_with_label("Edit");
    
    GtkWidget* cut_item = gtk_menu_item_new_with_label("Cut");
    GtkWidget* copy_item = gtk_menu_item_new_with_label("Copy");
    GtkWidget* paste_item = gtk_menu_item_new_with_label("Paste");
    GtkWidget* select_all_item = gtk_menu_item_new_with_label("Select All");
    
    gtk_menu_shell_append(GTK_MENU_SHELL(edit_menu), cut_item);
    gtk_menu_shell_append(GTK_MENU_SHELL(edit_menu), copy_item);
    gtk_menu_shell_append(GTK_MENU_SHELL(edit_menu), paste_item);
    gtk_menu_shell_append(GTK_MENU_SHELL(edit_menu), gtk_separator_menu_item_new());
    gtk_menu_shell_append(GTK_MENU_SHELL(edit_menu), select_all_item);
    
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(edit_item), edit_menu);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), edit_item);
    
    g_signal_connect(cut_item, "activate", G_CALLBACK(on_cut_activated), window);
    g_signal_connect(copy_item, "activate", G_CALLBACK(on_copy_activated), window);
    g_signal_connect(paste_item, "activate", G_CALLBACK(on_paste_activated), window);
    g_signal_connect(select_all_item, "activate", G_CALLBACK(on_select_all_activated), window);
    
    /* View menu */
    GtkWidget* view_menu = gtk_menu_new();
    GtkWidget* view_item = gtk_menu_item_new_with_label("View");
    
    GtkWidget* toggle_theme_item = gtk_menu_item_new_with_label("Toggle Theme");
    
    gtk_menu_shell_append(GTK_MENU_SHELL(view_menu), toggle_theme_item);
    
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(view_item), view_menu);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), view_item);
    
    g_signal_connect(toggle_theme_item, "activate", G_CALLBACK(on_toggle_theme_activated), window);
    
    /* Help menu */
    GtkWidget* help_menu = gtk_menu_new();
    GtkWidget* help_item = gtk_menu_item_new_with_label("Help");
    
    GtkWidget* about_item = gtk_menu_item_new_with_label("About");
    
    gtk_menu_shell_append(GTK_MENU_SHELL(help_menu), about_item);
    
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(help_item), help_menu);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), help_item);
    
    g_signal_connect(about_item, "activate", G_CALLBACK(on_about_activated), window);
    
    return menu_bar;
}

MainWindow* main_window_create(Application* app) {
    if (!app) {
        return NULL;
    }
    
    MainWindow* window = (MainWindow*)malloc(sizeof(MainWindow));
    if (!window) {
        return NULL;
    }
    
    window->app = app;
    window->ignore_buffer_changes = false;
    
    /* Create main window */
    window->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window->window), "Notebook - Untitled");
    gtk_window_set_default_size(GTK_WINDOW(window->window), 800, 600);
    
    /* Create main container */
    GtkWidget* vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(window->window), vbox);
    
    /* Create menu bar */
    GtkWidget* menu_bar = create_menu_bar(window);
    gtk_box_pack_start(GTK_BOX(vbox), menu_bar, FALSE, FALSE, 0);
    
    /* Create scrolled window for text view */
    GtkWidget* scrolled = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled),
                                   GTK_POLICY_AUTOMATIC,
                                   GTK_POLICY_AUTOMATIC);
    gtk_box_pack_start(GTK_BOX(vbox), scrolled, TRUE, TRUE, 0);
    
    /* Create source view with line numbers */
    window->text_view = gtk_source_view_new();
    gtk_source_view_set_show_line_numbers(GTK_SOURCE_VIEW(window->text_view), TRUE);
    gtk_source_view_set_highlight_current_line(GTK_SOURCE_VIEW(window->text_view), TRUE);
    gtk_source_view_set_auto_indent(GTK_SOURCE_VIEW(window->text_view), TRUE);
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(window->text_view), GTK_WRAP_WORD_CHAR);
    gtk_text_view_set_monospace(GTK_TEXT_VIEW(window->text_view), TRUE);
    gtk_container_add(GTK_CONTAINER(scrolled), window->text_view);
    
    /* Get text buffer */
    window->text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(window->text_view));
    
    /* Add custom style scheme directory */
    GtkSourceStyleSchemeManager* scheme_manager = gtk_source_style_scheme_manager_get_default();
    const gchar* const* search_paths = gtk_source_style_scheme_manager_get_search_path(scheme_manager);
    gchar** new_paths = g_new(gchar*, g_strv_length((gchar**)search_paths) + 2);
    gint i;
    for (i = 0; search_paths[i] != NULL; i++) {
        new_paths[i] = g_strdup(search_paths[i]);
    }
    new_paths[i] = g_strdup("styles");
    new_paths[i + 1] = NULL;
    gtk_source_style_scheme_manager_set_search_path(scheme_manager, new_paths);
    g_strfreev(new_paths);
    gtk_source_style_scheme_manager_force_rescan(scheme_manager);
    
    /* Load custom dark theme */
    GtkSourceStyleScheme* scheme = gtk_source_style_scheme_manager_get_scheme(scheme_manager, "notebook-dark");
    if (scheme) {
        gtk_source_buffer_set_style_scheme(GTK_SOURCE_BUFFER(window->text_buffer), scheme);
    }
    
    /* Connect signals */
    g_signal_connect(window->window, "delete-event", G_CALLBACK(on_window_delete), window);
    g_signal_connect(window->text_buffer, "changed", G_CALLBACK(on_buffer_changed), window);
    
    /* Setup CSS provider */
    window->css_provider = gtk_css_provider_new();
    GdkScreen* screen = gdk_screen_get_default();
    gtk_style_context_add_provider_for_screen(screen,
                                              GTK_STYLE_PROVIDER(window->css_provider),
                                              GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    
    /* Register application callbacks */
    ApplicationCallbacks callbacks = {
        .on_document_modified = on_document_modified,
        .on_document_saved = on_document_saved,
        .on_new_document = on_new_document,
        .on_document_loaded = on_document_loaded,
        .on_error = on_error
    };
    application_register_callbacks(app, &callbacks, window);
    
    /* Register theme change callback */
    ThemeManager* theme_manager = application_get_theme_manager(app);
    theme_manager_register_callback(theme_manager, on_theme_changed, window);
    
    /* Apply initial theme */
    main_window_apply_theme(window);
    
    return window;
}

void main_window_destroy(MainWindow* window) {
    if (!window) {
        return;
    }
    
    /* GTK widgets are destroyed with the window */
    free(window);
}

void main_window_show(MainWindow* window) {
    if (!window) {
        return;
    }
    
    gtk_widget_show_all(window->window);
}

GtkWidget* main_window_get_widget(const MainWindow* window) {
    if (!window) {
        return NULL;
    }
    
    return window->window;
}

GtkWidget* main_window_get_text_view(const MainWindow* window) {
    if (!window) {
        return NULL;
    }
    
    return window->text_view;
}

void main_window_update_title(MainWindow* window, const char* file_path, bool modified) {
    if (!window) {
        return;
    }
    
    char title[512];
    const char* filename = "Untitled";
    
    if (file_path) {
        const char* last_slash = strrchr(file_path, '/');
        filename = last_slash ? last_slash + 1 : file_path;
    }
    
    if (modified) {
        snprintf(title, sizeof(title), "Notebook - %s *", filename);
    } else {
        snprintf(title, sizeof(title), "Notebook - %s", filename);
    }
    
    gtk_window_set_title(GTK_WINDOW(window->window), title);
}

char* main_window_get_text(const MainWindow* window) {
    if (!window) {
        return NULL;
    }
    
    GtkTextIter start, end;
    gtk_text_buffer_get_bounds(window->text_buffer, &start, &end);
    return gtk_text_buffer_get_text(window->text_buffer, &start, &end, FALSE);
}

void main_window_set_text(MainWindow* window, const char* text) {
    if (!window || !text) {
        return;
    }
    
    window->ignore_buffer_changes = true;
    gtk_text_buffer_set_text(window->text_buffer, text, -1);
    window->ignore_buffer_changes = false;
}

void main_window_apply_theme(MainWindow* window) {
    if (!window) {
        return;
    }
    
    ThemeManager* theme_manager = application_get_theme_manager(window->app);
    const char* css = theme_manager_get_css(theme_manager);
    ThemeType current_theme = theme_manager_get_current(theme_manager);
    
    gtk_css_provider_load_from_data(window->css_provider, css, -1, NULL);
    
    /* Update style scheme based on theme */
    GtkSourceBuffer* source_buffer = GTK_SOURCE_BUFFER(window->text_buffer);
    GtkSourceStyleSchemeManager* scheme_manager = gtk_source_style_scheme_manager_get_default();
    
    const char* scheme_name = (current_theme == THEME_DARK) ? "notebook-dark" : "notebook-light";
    GtkSourceStyleScheme* scheme = gtk_source_style_scheme_manager_get_scheme(scheme_manager, scheme_name);
    
    if (scheme) {
        gtk_source_buffer_set_style_scheme(source_buffer, scheme);
    }
}

void main_window_show_error(MainWindow* window, const char* message) {
    if (!window || !message) {
        return;
    }
    
    GtkWidget* dialog = gtk_message_dialog_new(GTK_WINDOW(window->window),
                                               GTK_DIALOG_MODAL,
                                               GTK_MESSAGE_ERROR,
                                               GTK_BUTTONS_OK,
                                               "%s", message);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

bool main_window_confirm(MainWindow* window, const char* message) {
    if (!window || !message) {
        return false;
    }
    
    GtkWidget* dialog = gtk_message_dialog_new(GTK_WINDOW(window->window),
                                               GTK_DIALOG_MODAL,
                                               GTK_MESSAGE_QUESTION,
                                               GTK_BUTTONS_YES_NO,
                                               "%s", message);
    gint result = gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    
    return result == GTK_RESPONSE_YES;
}

char* main_window_choose_file_open(MainWindow* window) {
    if (!window) {
        return NULL;
    }
    
    GtkWidget* dialog = gtk_file_chooser_dialog_new("Open File",
                                                     GTK_WINDOW(window->window),
                                                     GTK_FILE_CHOOSER_ACTION_OPEN,
                                                     "_Cancel", GTK_RESPONSE_CANCEL,
                                                     "_Open", GTK_RESPONSE_ACCEPT,
                                                     NULL);
    
    char* filename = NULL;
    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
        filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
    }
    
    gtk_widget_destroy(dialog);
    return filename;
}

char* main_window_choose_file_save(MainWindow* window) {
    if (!window) {
        return NULL;
    }
    
    GtkWidget* dialog = gtk_file_chooser_dialog_new("Save File",
                                                     GTK_WINDOW(window->window),
                                                     GTK_FILE_CHOOSER_ACTION_SAVE,
                                                     "_Cancel", GTK_RESPONSE_CANCEL,
                                                     "_Save", GTK_RESPONSE_ACCEPT,
                                                     NULL);
    
    gtk_file_chooser_set_do_overwrite_confirmation(GTK_FILE_CHOOSER(dialog), TRUE);
    
    char* filename = NULL;
    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
        filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
    }
    
    gtk_widget_destroy(dialog);
    return filename;
}

/* Callback implementations */

static void on_new_activated(GtkWidget* widget, gpointer user_data) {
    (void)widget;
    MainWindow* window = (MainWindow*)user_data;
    
    if (application_has_unsaved_changes(window->app)) {
        if (!main_window_confirm(window, "You have unsaved changes. Continue?")) {
            return;
        }
    }
    
    application_new_document(window->app);
}

static void on_open_activated(GtkWidget* widget, gpointer user_data) {
    (void)widget;
    MainWindow* window = (MainWindow*)user_data;
    
    if (application_has_unsaved_changes(window->app)) {
        if (!main_window_confirm(window, "You have unsaved changes. Continue?")) {
            return;
        }
    }
    
    char* filename = main_window_choose_file_open(window);
    if (filename) {
        application_open_document(window->app, filename);
        g_free(filename);
    }
}

static void on_save_activated(GtkWidget* widget, gpointer user_data) {
    MainWindow* window = (MainWindow*)user_data;
    
    /* Update document content from text buffer */
    char* text = main_window_get_text(window);
    if (text) {
        Document* doc = application_get_document(window->app);
        document_set_content(doc, text);
        g_free(text);
    }
    
    if (!application_save_document(window->app)) {
        /* No file path - trigger Save As */
        on_save_as_activated(widget, user_data);
    }
}

static void on_save_as_activated(GtkWidget* widget, gpointer user_data) {
    (void)widget;
    MainWindow* window = (MainWindow*)user_data;
    
    /* Update document content from text buffer */
    char* text = main_window_get_text(window);
    if (text) {
        Document* doc = application_get_document(window->app);
        document_set_content(doc, text);
        g_free(text);
    }
    
    char* filename = main_window_choose_file_save(window);
    if (filename) {
        application_save_document_as(window->app, filename);
        g_free(filename);
    }
}

static void on_quit_activated(GtkWidget* widget, gpointer user_data) {
    (void)widget;
    MainWindow* window = (MainWindow*)user_data;
    
    if (application_has_unsaved_changes(window->app)) {
        if (!main_window_confirm(window, "You have unsaved changes. Quit anyway?")) {
            return;
        }
    }
    
    gtk_main_quit();
}

static void on_cut_activated(GtkWidget* widget, gpointer user_data) {
    (void)widget;
    MainWindow* window = (MainWindow*)user_data;
    
    GtkTextIter start, end;
    if (gtk_text_buffer_get_selection_bounds(window->text_buffer, &start, &end)) {
        char* text = gtk_text_buffer_get_text(window->text_buffer, &start, &end, FALSE);
        if (text) {
            application_cut(window->app, text);
            
            /* Also use GTK clipboard for system integration */
            GtkClipboard* clipboard = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
            gtk_clipboard_set_text(clipboard, text, -1);
            
            gtk_text_buffer_delete(window->text_buffer, &start, &end);
            g_free(text);
        }
    }
}

static void on_copy_activated(GtkWidget* widget, gpointer user_data) {
    (void)widget;
    MainWindow* window = (MainWindow*)user_data;
    
    GtkTextIter start, end;
    if (gtk_text_buffer_get_selection_bounds(window->text_buffer, &start, &end)) {
        char* text = gtk_text_buffer_get_text(window->text_buffer, &start, &end, FALSE);
        if (text) {
            application_copy(window->app, text);
            
            /* Also use GTK clipboard for system integration */
            GtkClipboard* clipboard = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
            gtk_clipboard_set_text(clipboard, text, -1);
            
            g_free(text);
        }
    }
}

static void on_paste_activated(GtkWidget* widget, gpointer user_data) {
    (void)widget;
    MainWindow* window = (MainWindow*)user_data;
    
    /* Use GTK clipboard for system integration */
    GtkClipboard* clipboard = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
    gchar* text = gtk_clipboard_wait_for_text(clipboard);
    
    if (text) {
        /* Delete selection if any */
        GtkTextIter start, end;
        if (gtk_text_buffer_get_selection_bounds(window->text_buffer, &start, &end)) {
            gtk_text_buffer_delete(window->text_buffer, &start, &end);
        }
        
        /* Insert at cursor */
        gtk_text_buffer_insert_at_cursor(window->text_buffer, text, -1);
        g_free(text);
    }
}

static void on_select_all_activated(GtkWidget* widget, gpointer user_data) {
    (void)widget;
    MainWindow* window = (MainWindow*)user_data;
    
    GtkTextIter start, end;
    gtk_text_buffer_get_bounds(window->text_buffer, &start, &end);
    gtk_text_buffer_select_range(window->text_buffer, &start, &end);
}

static void on_toggle_theme_activated(GtkWidget* widget, gpointer user_data) {
    (void)widget;
    MainWindow* window = (MainWindow*)user_data;
    
    ThemeManager* theme_manager = application_get_theme_manager(window->app);
    theme_manager_toggle(theme_manager);
}

static void on_about_activated(GtkWidget* widget, gpointer user_data) {
    (void)widget;
    MainWindow* window = (MainWindow*)user_data;
    
    GtkWidget* dialog = gtk_message_dialog_new(GTK_WINDOW(window->window),
                                               GTK_DIALOG_MODAL,
                                               GTK_MESSAGE_INFO,
                                               GTK_BUTTONS_OK,
                                               "Notebook\n\nA simple text editor with dark and light themes.\n\nBuilt with GTK3 following SOLID principles.");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

static void on_buffer_changed(GtkTextBuffer* buffer, gpointer user_data) {
    (void)buffer;
    MainWindow* window = (MainWindow*)user_data;
    
    if (window->ignore_buffer_changes) {
        return;
    }
    
    Document* doc = application_get_document(window->app);
    document_mark_modified(doc);
    
    /* Update title */
    const char* file_path = application_get_file_path(window->app);
    main_window_update_title(window, file_path, true);
}

static gboolean on_window_delete(GtkWidget* widget, GdkEvent* event, gpointer user_data) {
    (void)widget;
    (void)event;
    MainWindow* window = (MainWindow*)user_data;
    
    if (application_has_unsaved_changes(window->app)) {
        if (!main_window_confirm(window, "You have unsaved changes. Quit anyway?")) {
            return TRUE; /* Cancel the delete */
        }
    }
    
    gtk_main_quit();  /* Quit the GTK main loop */
    return FALSE; /* Allow the delete */
}

static void on_theme_changed(ThemeType theme, void* user_data) {
    (void)theme;
    MainWindow* window = (MainWindow*)user_data;
    main_window_apply_theme(window);
}

static void on_document_modified(void* user_data) {
    MainWindow* window = (MainWindow*)user_data;
    const char* file_path = application_get_file_path(window->app);
    main_window_update_title(window, file_path, true);
}

static void on_document_saved(void* user_data) {
    MainWindow* window = (MainWindow*)user_data;
    const char* file_path = application_get_file_path(window->app);
    main_window_update_title(window, file_path, false);
}

static void on_new_document(void* user_data) {
    MainWindow* window = (MainWindow*)user_data;
    main_window_set_text(window, "");
    main_window_update_title(window, NULL, false);
}

static void on_document_loaded(void* user_data) {
    MainWindow* window = (MainWindow*)user_data;
    
    Document* doc = application_get_document(window->app);
    const char* content = document_get_content(doc);
    const char* file_path = document_get_file_path(doc);
    
    main_window_set_text(window, content ? content : "");
    main_window_update_title(window, file_path, false);
}

static void on_error(const char* message, void* user_data) {
    MainWindow* window = (MainWindow*)user_data;
    main_window_show_error(window, message);
}
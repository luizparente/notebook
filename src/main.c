#include <gtk/gtk.h>
#include "core/application.h"
#include "ui/main_window.h"

/**
 * @file main.c
 * @brief Application entry point
 * 
 * Initializes GTK, creates the application and main window,
 * and runs the main event loop.
 */

int main(int argc, char* argv[]) {
    /* Initialize GTK */
    gtk_init(&argc, &argv);
    
    /* Create application controller */
    Application* app = application_create();
    if (!app) {
        g_printerr("Failed to create application\n");
        return 1;
    }
    
    /* Create main window */
    MainWindow* window = main_window_create(app);
    if (!window) {
        g_printerr("Failed to create main window\n");
        application_destroy(app);
        return 1;
    }
    
    /* Show window and run main loop */
    main_window_show(window);
    gtk_main();
    
    /* Cleanup */
    main_window_destroy(window);
    application_destroy(app);
    
    return 0;
}
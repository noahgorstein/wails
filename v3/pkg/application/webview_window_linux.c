#include <gtk/gtk.h>
#include <webkit2/webkit2.h>
#include <stdio.h>
#include <limits.h>
#include <stdint.h>
#include "webview_window_common.h"

// TODO: Finish this up.
void* windowNew(unsigned int id, int width, int height, bool fraudulentWebsiteWarningEnabled, bool frameless, bool enableDragAndDrop) {
    GtkWidget* window = NULL;
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size (GTK_WINDOW (window), width, height);
    return window;
}

// setInvisibleTitleBarHeight sets the invisible title bar height
void setInvisibleTitleBarHeight(void* window, unsigned int height) {

}

// Make window transparent
void windowSetTransparent(void* window) {
    // On main thread

}

void windowSetInvisibleTitleBar(void* window, unsigned int height) {
    // On main thread

}


// Set the title of the window
void windowSetTitle(void* window, char* title) {
    // Set window title on main thread
    gtk_window_set_title (GTK_WINDOW (window), title);
}

// Set the size of the window
void windowSetSize(void* window, int width, int height) {
    // Set window size on main thread
    gtk_window_resize (GTK_WINDOW (window), width, height);
}

// Set window always on top
void windowSetAlwaysOnTop(void* window, bool alwaysOnTop) {
    // Set window always on top on main thread

}

// Load URL in window
void navigationLoadURL(void* window, char* url) {
    // Load URL on main thread

}

// Set window resizable
void windowSetResizable(void* window, bool resizable) {
    // Set window resizable on main thread

}

// Set window min size
void windowSetMinSize(void* window, int width, int height) {
    // Set window min size on main thread

}

// Set window max size
void windowSetMaxSize(void* window, int width, int height) {
    // Set window max size on main thread

}

// Enable window devtools
void windowEnableDevTools(void* window) {
    // Enable devtools on main thread

}

// windowZoomReset
void windowZoomReset(void* window) {
    // Reset zoom on main thread

}

// windowZoomSet
void windowZoomSet(void* window, double zoom) {
    // Reset zoom on main thread

}

// windowZoomGet
float windowZoomGet(void* window) {
    // Get zoom
    return 0.0;
}

// windowZoomIn
void windowZoomIn(void* window) {
    // Zoom in on main thread

}

// windowZoomOut
void windowZoomOut(void* window) {
    // Zoom out on main thread
}

// set the window position
void windowSetPosition(void* window, int x, int y) {
    // Set window position on main thread

}

// Execute JS in window
void windowExecJS(void* window, const char* js) {
    // Execute JS on main thread

}

// Make window backdrop translucent
void windowSetTranslucent(void* window) {

}

// Make webview background transparent
void webviewSetTransparent(void* window) {
}

// Set webview background colour
void webviewSetBackgroundColour(void* window, int r, int g, int b, int alpha) {

}

// Set the window background colour
void windowSetBackgroundColour(void* window, int r, int g, int b, int alpha) {

}

bool windowIsMaximised(void* window) {
    return gtk_window_is_maximized( GTK_WINDOW (window) );
}

bool windowIsFullscreen(void* window) {
    return gtk_window_is_maximized( GTK_WINDOW (window) );
}

bool windowIsMinimised(void* window) {
    GdkWindowState state = gdk_window_get_state(GDK_WINDOW(window));
    return state & GDK_WINDOW_STATE_ICONIFIED > 0;
}

// Set Window fullscreen
void windowFullscreen(void* window) {
    if( windowIsFullscreen(window) ) {
        return;
    }
    gtk_window_fullscreen( GTK_WINDOW (window) );
}

void windowUnFullscreen(void* window) {
    if( !windowIsFullscreen(window) ) {
        return;
    }
    gtk_window_unfullscreen( GTK_WINDOW (window) );
}

// restore window to normal size
void windowRestore(void* window) {

}

// disable window fullscreen button
void setFullscreenButtonEnabled(void* window, bool enabled) {

}

// Set the titlebar style
void windowSetTitleBarAppearsTransparent(void* window, bool transparent) {

}

// Set window fullsize content view
void windowSetFullSizeContent(void* window, bool fullSize) {

}

// Set Hide Titlebar
void windowSetHideTitleBar(void* window, bool hideTitlebar) {

}

// Set Hide Title in Titlebar
void windowSetHideTitle(void* window, bool hideTitle) {

}

// Set Window use toolbar
void windowSetUseToolbar(void* window, bool useTool, int toolbarStyle) {

}

// Set window toolbar style
void windowSetToolbarStyle(void* window, int style) {

}

// Set Hide Toolbar Separator
void windowSetHideToolbarSeparator(void* window, bool hideSeparator) {

}

// Set Window appearance type
void windowSetAppearanceTypeByName(void* window, const char *appearanceName) {

}

// Center window on current monitor
void windowCenter(void* window) {

}

// Get the current size of the window
void windowGetSize(void* window, int* width, int* height) {

    // set width and height
    //    *width = frame.size.width;
    //    *height = frame.size.height;
}

// Get window width
int windowGetWidth(void* window) {
  return 0;
}

// Get window height
int windowGetHeight(void* window) {
  return 0;
}

// Get window position
void windowGetPosition(void* window, int* x, int* y) {
    // set x and y
    //  *x = frame.origin.x;
    //  *y = frame.origin.y;
}

// Destroy window
void windowDestroy(void* window) {

}


// windowClose closes the current window
void windowClose(void *window) {

}

// windowZoom
void windowZoom(void *window) {

}

// miniaturize
void windowMiniaturize(void *window) {

}

// webviewRenderHTML renders the given HTML
void windowRenderHTML(void *window, const char *html) {

}

void windowInjectCSS(void *window, const char *css) {

}

void windowMinimise(void *window) {
  gtk_window_iconify( GTK_WINDOW (window) );
}

// zoom maximizes the window to the screen dimensions
void windowMaximise(void *window) {
    gtk_window_maximize( GTK_WINDOW (window) );
}

bool isFullScreen(void *window) {

}

// windowSetFullScreen
void windowSetFullScreen(void *window, bool fullscreen) {

}

// windowUnminimise
void windowUnminimise(void *window) {
    gtk_window_deiconify( GTK_WINDOW (window) );
}

// windowUnmaximise
void windowUnmaximise(void *window) {
    gtk_window_unmaximize( GTK_WINDOW (window) );
}

void windowDisableSizeConstraints(void *window) {

}

void windowShow(void *window) {
    gtk_widget_show( GTK_WINDOW (window) );
}

void windowHide(void *window) {
    gtk_widget_hide( GTK_WINDOW (window) );
}

// windowShowMenu opens an menu at the given coordinates
void windowShowMenu(void *window, void *menu, int x, int y) {

}

// Make the given window frameless
void windowSetFrameless(void *window, bool frameless) {

}

void registerListener(unsigned int event) {

}

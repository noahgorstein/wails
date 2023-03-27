#include <gtk/gtk.h>
#include <webkit2/webkit2.h>
#include <stdio.h>
#include <limits.h>
#include <stdint.h>
#include "webview_window_common.h"


typedef struct WebviewWindow
{
    uint id;
    GtkWidget *window;
    GtkWidget *webview;
    GtkWidget *vbox;
} WebviewWindow;

GtkWidget *GTKWIDGET(WebviewWindow *pointer)
{
    return GTK_WIDGET(pointer->window);
}

GtkWindow *GTKWINDOW(WebviewWindow *pointer)
{
    return GTK_WINDOW(pointer->window);
}

GdkWindow *GDKWINDOW(WebviewWindow *pointer)
{
    return GDK_WINDOW(pointer->window);
}

WebKitWebView* WEB_VIEW(WebviewWindow *pointer)
{
    return WEBKIT_WEB_VIEW(pointer->webview);
}

extern void processURLRequest(unsigned int, void *);
extern void processMessage(unsigned int, void *);

static void processRequest(void *request, gpointer data) {
    WebviewWindow *window = data;
    WebKitURISchemeRequest *req = request;
    char *uri = webkit_uri_scheme_request_get_uri(req);
    printf("processRequest: %d: %s\n", window->id, uri);
    processURLRequest(window->id, request);
}

gboolean close_button_pressed(GtkWidget *widget, GdkEvent *event, gpointer *data)
{
    WebviewWindow *window = data;
    processMessage(window->id, "Q");
    // since we handle the close in processMessage tell GTK to not invoke additional handlers - see:
    // https://docs.gtk.org/gtk3/signal.Widget.delete-event.html
    return TRUE;
}

static void webviewLoadChanged(WebKitWebView *web_view, WebKitLoadEvent load_event, gpointer data)
{
    WebviewWindow *window = data;
    printf("load changed: %d event: %d\n", window->id, load_event);
    if (load_event == 2)
    {
        processMessage(window->id, "DomReady");
    }
}

GtkWidget *SetupWebview(void *contentManager, WebviewWindow *window, int hideWindowOnClose, int gpuPolicy)
{
    GtkWidget *webview = webkit_web_view_new_with_user_content_manager((WebKitUserContentManager *)contentManager);
    //gtk_container_add(GTK_CONTAINER(window), webview);
    WebKitWebContext *context = webkit_web_context_get_default();
    webkit_web_context_register_uri_scheme(context, "wails", (WebKitURISchemeRequestCallback)processRequest, window, NULL);
    g_signal_connect(G_OBJECT(webview), "load-changed", G_CALLBACK(webviewLoadChanged), window);
    if (hideWindowOnClose)
    {
      //        g_signal_connect(GTK_WIDGET(window), "delete-event", G_CALLBACK(gtk_widget_hide_on_delete), NULL);
    }
    else
    {
      //        g_signal_connect(GTK_WIDGET(window), "delete-event", G_CALLBACK(close_button_pressed), NULL);
    }

    WebKitSettings *settings = webkit_web_view_get_settings(WEBKIT_WEB_VIEW(webview));
    webkit_settings_set_user_agent_with_application_details(settings, "wails.io", "");

    switch (gpuPolicy)
    {
    case 0:
        webkit_settings_set_hardware_acceleration_policy(settings, WEBKIT_HARDWARE_ACCELERATION_POLICY_ALWAYS);
        break;
    case 1:
        webkit_settings_set_hardware_acceleration_policy(settings, WEBKIT_HARDWARE_ACCELERATION_POLICY_ON_DEMAND);
        break;
    case 2:
        webkit_settings_set_hardware_acceleration_policy(settings, WEBKIT_HARDWARE_ACCELERATION_POLICY_NEVER);
        break;
    default:
        webkit_settings_set_hardware_acceleration_policy(settings, WEBKIT_HARDWARE_ACCELERATION_POLICY_ON_DEMAND);
    }
    return webview;
}


// TODO: Finish this up.
// Taking from v2/internal/frontend/desktop/linux/window.go | window.c
void* windowNew(unsigned int id, int width, int height, bool fraudulentWebsiteWarningEnabled, bool frameless, bool enableDragAndDrop) {
    WebviewWindow *ww = malloc(sizeof(WebviewWindow));

    ww->id = id;
    ww->window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    g_object_ref_sink(ww->window);
    gtk_window_set_default_size (GTK_WINDOW (ww->window), width, height);

    ww->vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER (ww->window), ww->vbox);

    void *content_manager = webkit_user_content_manager_new();
    webkit_user_content_manager_register_script_message_handler(content_manager, "external");

    ww->webview = SetupWebview(content_manager,
                               ww,
                               0, // hide window on close
                               2); // webview gpu policy never

    gtk_box_pack_start(ww->vbox, ww->webview, 1, 1, 0);
    return ww;
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
    gtk_window_set_title (GTKWINDOW (window), title);
}

// Set the size of the window
void windowSetSize(void* window, int width, int height) {
    // Set window size on main thread
    gtk_window_resize (GTKWINDOW (window), width, height);
}

// Set window always on top
void windowSetAlwaysOnTop(void* window, bool alwaysOnTop) {
    // Set window always on top on main thread

}

// Load URL in window
void navigationLoadURL(void* window, char* url) {
    printf("webkit_web_view_load_uri(WEB_VIEW(window), %s)", url);
    webkit_web_view_load_uri(WEB_VIEW(window), url);
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
  printf("windowExecJS(%s)\n", js);

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
    return gtk_window_is_maximized( GTKWINDOW (window) );
}

bool windowIsFullscreen(void* window) {
    return gtk_window_is_maximized( GTKWINDOW (window) );
}

bool windowIsMinimised(void* window) {
    GdkWindowState state = gdk_window_get_state(GDKWINDOW(window));
    return state & GDK_WINDOW_STATE_ICONIFIED > 0;
}

// Set Window fullscreen
void windowFullscreen(void* data) {
    if( windowIsFullscreen(data) ) {
        return;
    }
    gtk_window_fullscreen( GTKWINDOW (data) );
}

void windowUnFullscreen(void* window) {
    if( !windowIsFullscreen(window) ) {
        return;
    }
    gtk_window_unfullscreen( GTKWINDOW (window) );
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
  printf("windowRenderHTML\n");
}

void windowInjectCSS(void *window, const char *css) {

}

void windowMinimise(void *window) {
    gtk_window_iconify( GTKWINDOW (window) );
}

// zoom maximizes the window to the screen dimensions
void windowMaximise(void *window) {
    gtk_window_maximize( GTKWINDOW (window) );
}

bool isFullScreen(void *window) {

}

// windowSetFullScreen
void windowSetFullScreen(void *window, bool fullscreen) {

}

// windowUnminimise
void windowUnminimise(void *window) {
    gtk_window_deiconify( GTKWINDOW (window) );
}

// windowUnmaximise
void windowUnmaximise(void *window) {
    gtk_window_unmaximize( GTKWINDOW (window) );
}

void windowDisableSizeConstraints(void *window) {

}

void windowShow(void *window) {
    //    gtk_widget_show( GTKWIDGET (window) );
    gtk_widget_show_all( GTKWIDGET (window) );
}

void windowHide(void *window) {
    gtk_widget_hide( GTKWINDOW (window) );
}

// windowShowMenu opens an menu at the given coordinates
void windowShowMenu(void *window, void *menu, int x, int y) {

}

// Make the given window frameless
void windowSetFrameless(void *window, bool frameless) {

}

void registerListener(unsigned int event) {

}

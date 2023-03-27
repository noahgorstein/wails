#ifndef webview_window_common_h
#define webview_window_common_h

#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

extern void registerListener(unsigned int event);

extern void* windowNew(unsigned int id, int width, int height, bool fraudulentWebsiteWarningEnabled, bool frameless, bool enableDragAndDrop);

extern void printWindowStyle(void *window);

// setInvisibleTitleBarHeight sets the invisible title bar height
extern void setInvisibleTitleBarHeight(void* window, unsigned int height);

// Make window transparent
extern void windowSetTransparent(void* window);

// Hide the window's title bar
extern void windowSetInvisibleTitleBar(void* window, unsigned int height);

// Set the title of the window
extern void windowSetTitle(void* window, char* title);

// Set the size of the window
extern void windowSetSize(void* window, int width, int height);

// Set window always on top
extern void windowSetAlwaysOnTop(void* window, bool alwaysOnTop);

// Load URL in window
extern void navigationLoadURL(void* window, char* url);

// Set window resizable
extern void windowSetResizable(void* window, bool resizable);

// Set window min size
extern void windowSetMinSize(void* window, int width, int height);

// Set window max size
extern void windowSetMaxSize(void* window, int width, int height);

// Enable devtools
extern void windowEnableDevTools(void* window);

// windowZoomReset
extern void windowZoomReset(void* window);

// windowZoomSet
extern void windowZoomSet(void* window, double zoom);

// windowZoomGet
extern float windowZoomGet(void* window);

// windowZoomIn
extern void windowZoomIn(void* window);

// windowZoomOut
extern void windowZoomOut(void* window);

// set the window position
extern void windowSetPosition(void* window, int x, int y);

// Execute JS in NSWindow
extern void windowExecJS(void* window, const char* js);

// Make NSWindow backdrop translucent
extern void windowSetTranslucent(void* window);

// Make webview background transparent
extern void webviewSetTransparent(void* window);

// Set webview background colour
extern void webviewSetBackgroundColour(void* window, int r, int g, int b, int alpha);

// Set the window background colour
extern void windowSetBackgroundColour(void* window, int r, int g, int b, int alpha);

extern bool windowIsMaximised(void* window);

extern bool windowIsFullscreen(void* window);

extern bool windowIsMinimised(void* window);

// Set Window fullscreen
extern void windowFullscreen(void* window);

extern void windowUnFullscreen(void* window);

// restore window to normal size
extern void windowRestore(void* window);

// disable window fullscreen button
extern void setFullscreenButtonEnabled(void* window, bool enabled);

// Set the titlebar style
extern void windowSetTitleBarAppearsTransparent(void* window, bool transparent);

// Set window fullsize content view
extern void windowSetFullSizeContent(void* window, bool fullSize);

// Set Hide Titlebar
extern void windowSetHideTitleBar(void* window, bool hideTitlebar);

// Set Hide Title in Titlebar
extern void windowSetHideTitle(void* window, bool hideTitle);

// Set Window use toolbar
extern void windowSetUseToolbar(void* window, bool useToolbar, int toolbarStyle);

// Set window toolbar style
extern void windowSetToolbarStyle(void* window, int style);

// Set Hide Toolbar Separator
extern void windowSetHideToolbarSeparator(void* window, bool hideSeparator);

// Set Window appearance type
extern void windowSetAppearanceTypeByName(void* window, const char *appearanceName);

// Center window on current monitor
extern void windowCenter(void* window);

// Get the current size of the window
extern void windowGetSize(void* window, int* width, int* height);

// Get window width
extern int windowGetWidth(void* window);

// Get window height
extern int windowGetHeight(void* window);

// Get window position
extern void windowGetPosition(void* window, int* x, int* y);

// Destroy window
extern void windowDestroy(void* window);

// windowClose closes the current window
extern void windowClose(void *window);

// windowZoom
extern void windowZoom(void *window);

// miniaturize
extern void windowMiniaturize(void *window);

// webviewRenderHTML renders the given HTML
extern void windowRenderHTML(void *window, const char *html);

extern void windowInjectCSS(void *window, const char *css);

extern void windowMinimise(void *window);

// zoom maximizes the window to the screen dimensions
extern void windowMaximise(void *window);

extern bool isFullScreen(void *window);

// windowSetFullScreen
extern void windowSetFullScreen(void *window, bool fullscreen);

// windowUnminimise
extern void windowUnminimise(void *window);

// windowUnmaximise
extern void windowUnmaximise(void *window);

extern void windowDisableSizeConstraints(void *window);

extern void windowShow(void *window);

extern void windowHide(void *window);

// windowShowMenu opens an NSMenu at the given coordinates
extern void windowShowMenu(void *window, void *menu, int x, int y);

// Make the given window frameless
extern void windowSetFrameless(void *window, bool frameless);

#endif

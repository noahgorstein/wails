//go:build darwin

#include "application.h"
#include "webview_window.h"
#include "webview_window_common.h"
#include <stdlib.h>
#include "Cocoa/Cocoa.h"
#import <WebKit/WebKit.h>
#import <AppKit/AppKit.h>
#import "webview_drag.h"

// Create a new Window
void* windowNew(unsigned int id, int width, int height, bool fraudulentWebsiteWarningEnabled, bool frameless, bool enableDragAndDrop) {

    NSWindowStyleMask styleMask = NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | NSWindowStyleMaskMiniaturizable | NSWindowStyleMaskResizable;
    if (frameless) {
        styleMask = NSWindowStyleMaskBorderless | NSWindowStyleMaskResizable;
    }
    WebviewWindow* window = [[WebviewWindow alloc] initWithContentRect:NSMakeRect(0, 0, width-1, height-1)
        styleMask:styleMask
        backing:NSBackingStoreBuffered
        defer:NO];

    // Create delegate
    WebviewWindowDelegate* delegate = [[WebviewWindowDelegate alloc] init];
    // Set delegate
    [window setDelegate:delegate];
    delegate.windowId = id;
    delegate.window = window;

    // Add NSView to window
    NSView* view = [[NSView alloc] initWithFrame:NSMakeRect(0, 0, width-1, height-1)];
    [view setAutoresizingMask:NSViewWidthSizable | NSViewHeightSizable];
    if( frameless ) {
        [view setWantsLayer:YES];
        view.layer.cornerRadius = 8.0;
    }
    [window setContentView:view];

    // Embed wkwebview in window
    NSRect frame = NSMakeRect(0, 0, width, height);
    WKWebViewConfiguration* config = [[WKWebViewConfiguration alloc] init];
    config.suppressesIncrementalRendering = true;
    config.applicationNameForUserAgent = @"wails.io";
    [config setURLSchemeHandler:delegate forURLScheme:@"wails"];
    if (@available(macOS 10.15, *)) {
         config.preferences.fraudulentWebsiteWarningEnabled = fraudulentWebsiteWarningEnabled;
    }

    // Setup user content controller
    WKUserContentController* userContentController = [WKUserContentController new];
    [userContentController addScriptMessageHandler:delegate name:@"external"];
    config.userContentController = userContentController;

    WKWebView* webView = [[WKWebView alloc] initWithFrame:frame configuration:config];
    [view addSubview:webView];

    // support webview events
    [webView setNavigationDelegate:delegate];

    // Ensure webview resizes with the window
    [webView setAutoresizingMask:NSViewWidthSizable | NSViewHeightSizable];

    delegate.webView = webView;
    delegate.hideOnClose = false;

    if( enableDragAndDrop ) {
        WebviewDrag* dragView = [[WebviewDrag alloc] initWithFrame:NSMakeRect(0, 0, width-1, height-1)];
        [view setAutoresizingMask:NSViewWidthSizable | NSViewHeightSizable];
        [view addSubview:dragView];
        dragView.windowId = id;
    }

    return window;
}


void printWindowStyle(void *window) {
    WebviewWindow* nsWindow = (WebviewWindow*)window;
    NSWindowStyleMask styleMask = [nsWindow styleMask];
    // Get delegate
    WebviewWindowDelegate* windowDelegate = (WebviewWindowDelegate*)[nsWindow delegate];

    printf("Window %d style mask: ", windowDelegate.windowId);

    if (styleMask & NSWindowStyleMaskTitled)
    {
        printf("NSWindowStyleMaskTitled ");
    }

    if (styleMask & NSWindowStyleMaskClosable)
    {
        printf("NSWindowStyleMaskClosable ");
    }

    if (styleMask & NSWindowStyleMaskMiniaturizable)
    {
        printf("NSWindowStyleMaskMiniaturizable ");
    }

    if (styleMask & NSWindowStyleMaskResizable)
    {
        printf("NSWindowStyleMaskResizable ");
    }

    if (styleMask & NSWindowStyleMaskFullSizeContentView)
    {
        printf("NSWindowStyleMaskFullSizeContentView ");
    }

    if (styleMask & NSWindowStyleMaskNonactivatingPanel)
    {
        printf("NSWindowStyleMaskNonactivatingPanel ");
    }

    if (styleMask & NSWindowStyleMaskFullScreen)
    {
        printf("NSWindowStyleMaskFullScreen ");
    }

    if (styleMask & NSWindowStyleMaskBorderless)
    {
        printf("MSWindowStyleMaskBorderless ");
    }

    printf("\n");
}


// setInvisibleTitleBarHeight sets the invisible title bar height
void setInvisibleTitleBarHeight(void* window, unsigned int height) {
    WebviewWindow* nsWindow = (WebviewWindow*)window;
    // Get delegate
    WebviewWindowDelegate* delegate = (WebviewWindowDelegate*)[nsWindow delegate];
    // Set height
    delegate.invisibleTitleBarHeight = height;
}

// Make NSWindow transparent
void windowSetTransparent(void* nsWindow) {
    // On main thread
    dispatch_async(dispatch_get_main_queue(), ^{
    WebviewWindow* window = (WebviewWindow*)nsWindow;
    [window setOpaque:NO];
    [window setBackgroundColor:[NSColor clearColor]];
    });
}

void windowSetInvisibleTitleBar(void* nsWindow, unsigned int height) {
    // On main thread
    dispatch_async(dispatch_get_main_queue(), ^{
        WebviewWindow* window = (WebviewWindow*)nsWindow;
        // Get delegate
        WebviewWindowDelegate* delegate = (WebviewWindowDelegate*)[window delegate];
        // Set height
        delegate.invisibleTitleBarHeight = height;
    });
}


// Set the title of the NSWindow
void windowSetTitle(void* nsWindow, char* title) {
    // Set window title on main thread
    dispatch_async(dispatch_get_main_queue(), ^{
        NSString* nsTitle = [NSString stringWithUTF8String:title];
        [(WebviewWindow*)nsWindow setTitle:nsTitle];
        free(title);
    });
}

// Set the size of the NSWindow
void windowSetSize(void* nsWindow, int width, int height) {
    // Set window size on main thread
    dispatch_async(dispatch_get_main_queue(), ^{
        WebviewWindow* window = (WebviewWindow*)nsWindow;
        NSSize contentSize = [window contentRectForFrameRect:NSMakeRect(0, 0, width, height)].size;
        [window setContentSize:contentSize];
        [window setFrame:NSMakeRect(window.frame.origin.x, window.frame.origin.y, width, height) display:YES animate:YES];
    });
}

// Set NSWindow always on top
void windowSetAlwaysOnTop(void* nsWindow, bool alwaysOnTop) {
    // Set window always on top on main thread
    dispatch_async(dispatch_get_main_queue(), ^{
        [(WebviewWindow*)nsWindow setLevel:alwaysOnTop ? NSStatusWindowLevel : NSNormalWindowLevel];
    });
}

// Load URL in NSWindow
void navigationLoadURL(void* nsWindow, char* url) {
    // Load URL on main thread
    dispatch_async(dispatch_get_main_queue(), ^{
        NSURL* nsURL = [NSURL URLWithString:[NSString stringWithUTF8String:url]];
        NSURLRequest* request = [NSURLRequest requestWithURL:nsURL];
        [[(WebviewWindowDelegate*)[(WebviewWindow*)nsWindow delegate] webView] loadRequest:request];
        free(url);
    });
}

// Set NSWindow resizable
void windowSetResizable(void* nsWindow, bool resizable) {
    // Set window resizable on main thread
    dispatch_async(dispatch_get_main_queue(), ^{
        WebviewWindow* window = (WebviewWindow*)nsWindow;
        if (resizable) {
            NSWindowStyleMask styleMask = [window styleMask] | NSWindowStyleMaskResizable;
            [window setStyleMask:styleMask];
        } else {
            NSWindowStyleMask styleMask = [window styleMask] & ~NSWindowStyleMaskResizable;
            [window setStyleMask:styleMask];
        }
    });
}

// Set NSWindow min size
void windowSetMinSize(void* nsWindow, int width, int height) {
    // Set window min size on main thread
    dispatch_async(dispatch_get_main_queue(), ^{
        WebviewWindow* window = (WebviewWindow*)nsWindow;
        NSSize contentSize = [window contentRectForFrameRect:NSMakeRect(0, 0, width, height)].size;
        [window setContentMinSize:contentSize];
        NSSize size = { width, height };
        [window setMinSize:size];
    });
}

// Set NSWindow max size
void windowSetMaxSize(void* nsWindow, int width, int height) {
    // Set window max size on main thread
    dispatch_async(dispatch_get_main_queue(), ^{
        NSSize size = { FLT_MAX, FLT_MAX };
        size.width = width > 0 ? width : FLT_MAX;
        size.height = height > 0 ? height : FLT_MAX;
        WebviewWindow* window = (WebviewWindow*)nsWindow;
        NSSize contentSize = [window contentRectForFrameRect:NSMakeRect(0, 0, size.width, size.height)].size;
        [window setContentMaxSize:contentSize];
        [window setMaxSize:size];
    });
}

// Enable NSWindow devtools
void windowEnableDevTools(void* nsWindow) {
    // Enable devtools on main thread
    dispatch_async(dispatch_get_main_queue(), ^{
        // Get window delegate
        WebviewWindowDelegate* delegate = (WebviewWindowDelegate*)[(WebviewWindow*)nsWindow delegate];
        // Enable devtools in webview
        [delegate.webView.configuration.preferences setValue:@YES forKey:@"developerExtrasEnabled"];
    });
}

// windowZoomReset
void windowZoomReset(void* nsWindow) {
    // Reset zoom on main thread
    dispatch_async(dispatch_get_main_queue(), ^{
        // Get window delegate
        WebviewWindowDelegate* delegate = (WebviewWindowDelegate*)[(WebviewWindow*)nsWindow delegate];
        // Reset zoom
        [delegate.webView setMagnification:1.0];
    });
}

// windowZoomSet
void windowZoomSet(void* nsWindow, double zoom) {
    // Reset zoom on main thread
    dispatch_async(dispatch_get_main_queue(), ^{
        // Get window delegate
        WebviewWindowDelegate* delegate = (WebviewWindowDelegate*)[(WebviewWindow*)nsWindow delegate];
        // Reset zoom
        [delegate.webView setMagnification:zoom];
    });
}

// windowZoomGet
float windowZoomGet(void* nsWindow) {
    // Get zoom
    WebviewWindowDelegate* delegate = (WebviewWindowDelegate*)[(WebviewWindow*)nsWindow delegate];
    return [delegate.webView magnification];
}

// windowZoomIn
void windowZoomIn(void* nsWindow) {
    // Zoom in on main thread
    dispatch_async(dispatch_get_main_queue(), ^{
        // Get window delegate
        WebviewWindowDelegate* delegate = (WebviewWindowDelegate*)[(WebviewWindow*)nsWindow delegate];
        // Zoom in
        [delegate.webView setMagnification:delegate.webView.magnification + 0.05];
    });
}

// windowZoomOut
void windowZoomOut(void* nsWindow) {
    // Zoom out on main thread
    dispatch_async(dispatch_get_main_queue(), ^{
        // Get window delegate
        WebviewWindowDelegate* delegate = (WebviewWindowDelegate*)[(WebviewWindow*)nsWindow delegate];
        // Zoom out
        if( delegate.webView.magnification > 1.05 ) {
            [delegate.webView setMagnification:delegate.webView.magnification - 0.05];
        } else {
            [delegate.webView setMagnification:1.0];
        }
    });
}

// set the window position
void windowSetPosition(void* nsWindow, int x, int y) {
    // Set window position on main thread
    dispatch_async(dispatch_get_main_queue(), ^{
        [(WebviewWindow*)nsWindow setFrameOrigin:NSMakePoint(x, y)];
    });
}

// Execute JS in NSWindow
void windowExecJS(void* nsWindow, const char* js) {
    // Execute JS on main thread
    dispatch_async(dispatch_get_main_queue(), ^{
        WebviewWindowDelegate* delegate = (WebviewWindowDelegate*)[(WebviewWindow*)nsWindow delegate];
        [delegate.webView evaluateJavaScript:[NSString stringWithUTF8String:js] completionHandler:nil];
        free((void*)js);
    });
}

// Make NSWindow backdrop translucent
void windowSetTranslucent(void* nsWindow) {
    // Set window transparent on main thread
    dispatch_async(dispatch_get_main_queue(), ^{

        // Get window
        WebviewWindow* window = (WebviewWindow*)nsWindow;

        // Get window delegate
        WebviewWindowDelegate* delegate = (WebviewWindowDelegate*)[(WebviewWindow*)nsWindow delegate];

        id contentView = [window contentView];
        NSVisualEffectView *effectView = [NSVisualEffectView alloc];
        NSRect bounds = [contentView bounds];
        [effectView initWithFrame:bounds];
        [effectView setAutoresizingMask:NSViewWidthSizable | NSViewHeightSizable];
        [effectView setBlendingMode:NSVisualEffectBlendingModeBehindWindow];
        [effectView setState:NSVisualEffectStateActive];
        [contentView addSubview:effectView positioned:NSWindowBelow relativeTo:nil];
    });
}

// Make webview background transparent
void webviewSetTransparent(void* nsWindow) {
    // Set webview transparent on main thread
    dispatch_async(dispatch_get_main_queue(), ^{
        // Get window delegate
        WebviewWindowDelegate* delegate = (WebviewWindowDelegate*)[(WebviewWindow*)nsWindow delegate];
        // Set webview background transparent
        [delegate.webView setValue:@NO forKey:@"drawsBackground"];
    });
}

// Set webview background colour
void webviewSetBackgroundColour(void* nsWindow, int r, int g, int b, int alpha) {
    // Set webview background color on main thread
    dispatch_async(dispatch_get_main_queue(), ^{
        // Get window delegate
        WebviewWindowDelegate* delegate = (WebviewWindowDelegate*)[(WebviewWindow*)nsWindow delegate];
        // Set webview background color
        [delegate.webView setValue:[NSColor colorWithRed:r/255.0 green:g/255.0 blue:b/255.0 alpha:alpha/255.0] forKey:@"backgroundColor"];
    });
}

// Set the window background colour
void windowSetBackgroundColour(void* nsWindow, int r, int g, int b, int alpha) {
    // Set window background color on main thread
    dispatch_async(dispatch_get_main_queue(), ^{
        // Get window
        WebviewWindow* window = (WebviewWindow*)nsWindow;
        // Set window background color
        [window setBackgroundColor:[NSColor colorWithRed:r/255.0 green:g/255.0 blue:b/255.0 alpha:alpha/255.0]];
    });
}

bool windowIsMaximised(void* nsWindow) {
    return [(WebviewWindow*)nsWindow isZoomed];
}

bool windowIsFullscreen(void* nsWindow) {
    return [(WebviewWindow*)nsWindow styleMask] & NSWindowStyleMaskFullScreen;
}

bool windowIsMinimised(void* nsWindow) {
    return [(WebviewWindow*)nsWindow isMiniaturized];
}

// Set Window fullscreen
void windowFullscreen(void* nsWindow) {
    if( windowIsFullscreen(nsWindow) ) {
        return;
    }
    dispatch_async(dispatch_get_main_queue(), ^{
        [(WebviewWindow*)nsWindow toggleFullScreen:nil];
    });}

void windowUnFullscreen(void* nsWindow) {
    if( !windowIsFullscreen(nsWindow) ) {
        return;
    }
    dispatch_async(dispatch_get_main_queue(), ^{
        [(WebviewWindow*)nsWindow toggleFullScreen:nil];
    });
}

// restore window to normal size
void windowRestore(void* nsWindow) {
    // Set window normal on main thread
    dispatch_async(dispatch_get_main_queue(), ^{
        // If window is fullscreen
        if([(WebviewWindow*)nsWindow styleMask] & NSWindowStyleMaskFullScreen) {
            [(WebviewWindow*)nsWindow toggleFullScreen:nil];
        }
        // If window is maximised
        if([(WebviewWindow*)nsWindow isZoomed]) {
            [(WebviewWindow*)nsWindow zoom:nil];
        }
        // If window in minimised
        if([(WebviewWindow*)nsWindow isMiniaturized]) {
            [(WebviewWindow*)nsWindow deminiaturize:nil];
        }
    });
}

// disable window fullscreen button
void setFullscreenButtonEnabled(void* nsWindow, bool enabled) {
    // Disable fullscreen button on main thread
    dispatch_async(dispatch_get_main_queue(), ^{
        // Get window
        WebviewWindow* window = (WebviewWindow*)nsWindow;
        NSButton *fullscreenButton = [window standardWindowButton:NSWindowZoomButton];
        fullscreenButton.enabled = enabled;
    });
}

// Set the titlebar style
void windowSetTitleBarAppearsTransparent(void* nsWindow, bool transparent) {
    // Set window titlebar style on main thread
    dispatch_async(dispatch_get_main_queue(), ^{
        if( transparent ) {
            [(WebviewWindow*)nsWindow setTitlebarAppearsTransparent:true];
        } else {
            [(WebviewWindow*)nsWindow setTitlebarAppearsTransparent:false];
        }
    });
}

// Set window fullsize content view
void windowSetFullSizeContent(void* nsWindow, bool fullSize) {
    // Set window fullsize content view on main thread
    dispatch_async(dispatch_get_main_queue(), ^{
        if( fullSize ) {
            [(WebviewWindow*)nsWindow setStyleMask:[(WebviewWindow*)nsWindow styleMask] | NSWindowStyleMaskFullSizeContentView];
        } else {
            [(WebviewWindow*)nsWindow setStyleMask:[(WebviewWindow*)nsWindow styleMask] & ~NSWindowStyleMaskFullSizeContentView];
        }
    });
}

// Set Hide Titlebar
void windowSetHideTitleBar(void* nsWindow, bool hideTitlebar) {
    // Set window titlebar hidden on main thread
    dispatch_async(dispatch_get_main_queue(), ^{
        if( hideTitlebar ) {
            [(WebviewWindow*)nsWindow setStyleMask:[(WebviewWindow*)nsWindow styleMask] & ~NSWindowStyleMaskTitled];
        } else {
            [(WebviewWindow*)nsWindow setStyleMask:[(WebviewWindow*)nsWindow styleMask] | NSWindowStyleMaskTitled];
        }
    });
}

// Set Hide Title in Titlebar
void windowSetHideTitle(void* nsWindow, bool hideTitle) {
    // Set window titlebar hidden on main thread
    dispatch_async(dispatch_get_main_queue(), ^{
        if( hideTitle ) {
            [(WebviewWindow*)nsWindow setTitleVisibility:NSWindowTitleHidden];
        } else {
            [(WebviewWindow*)nsWindow setTitleVisibility:NSWindowTitleVisible];
        }
    });
}

// Set Window use toolbar
void windowSetUseToolbar(void* nsWindow, bool useToolbar, int toolbarStyle) {
    // Set window use toolbar on main thread
    dispatch_async(dispatch_get_main_queue(), ^{
        // get main window
        WebviewWindow* window = (WebviewWindow*)nsWindow;
        if( useToolbar ) {
            NSToolbar *toolbar = [[NSToolbar alloc] initWithIdentifier:@"wails.toolbar"];
            [toolbar autorelease];
            [window setToolbar:toolbar];

            // If macos 11 or higher, set toolbar style
            if (@available(macOS 11.0, *)) {
                [window setToolbarStyle:toolbarStyle];
            }

        } else {
            [window setToolbar:nil];
        }
    });
}

// Set window toolbar style
void windowSetToolbarStyle(void* nsWindow, int style) {
    // use @available to check if the function is available
    // if not, return
    if (@available(macOS 11.0, *)) {
        // Set window toolbar style on main thread
        dispatch_async(dispatch_get_main_queue(), ^{
            // get main window
            WebviewWindow* window = (WebviewWindow*)nsWindow;
            // get toolbar
            NSToolbar* toolbar = [window toolbar];
            // set toolbar style
            [toolbar setShowsBaselineSeparator:style];
        });
    }
}

// Set Hide Toolbar Separator
void windowSetHideToolbarSeparator(void* nsWindow, bool hideSeparator) {
    // Set window hide toolbar separator on main thread
    dispatch_async(dispatch_get_main_queue(), ^{
        // get main window
        WebviewWindow* window = (WebviewWindow*)nsWindow;
        // get toolbar
        NSToolbar* toolbar = [window toolbar];
        // Return if toolbar nil
        if( toolbar == nil ) {
            return;
        }
        if( hideSeparator ) {
            [toolbar setShowsBaselineSeparator:false];
        } else {
            [toolbar setShowsBaselineSeparator:true];
        }
    });
}

// Set Window appearance type
void windowSetAppearanceTypeByName(void* nsWindow, const char *appearanceName) {
    // Set window appearance type on main thread
    dispatch_async(dispatch_get_main_queue(), ^{
        // get main window
        WebviewWindow* window = (WebviewWindow*)nsWindow;
        // set window appearance type by name
        // Convert appearance name to NSString
        NSString* appearanceNameString = [NSString stringWithUTF8String:appearanceName];
        // Set appearance
        [window setAppearance:[NSAppearance appearanceNamed:appearanceNameString]];

        free((void*)appearanceName);
    });
}

// Center window on current monitor
void windowCenter(void* nsWindow) {
    // Center window on main thread
    dispatch_async(dispatch_get_main_queue(), ^{
        // get main window
        WebviewWindow* window = (WebviewWindow*)nsWindow;
        [window center];
    });
}

// Get the current size of the window
void windowGetSize(void* nsWindow, int* width, int* height) {
    // get main window
    WebviewWindow* window = (WebviewWindow*)nsWindow;
    // get window frame
    NSRect frame = [window frame];
    // set width and height
    *width = frame.size.width;
    *height = frame.size.height;
}

// Get window width
int windowGetWidth(void* nsWindow) {
    // get main window
    WebviewWindow* window = (WebviewWindow*)nsWindow;
    // get window frame
    NSRect frame = [window frame];
    // return width
    return frame.size.width;
}

// Get window height
int windowGetHeight(void* nsWindow) {
    // get main window
    WebviewWindow* window = (WebviewWindow*)nsWindow;
    // get window frame
    NSRect frame = [window frame];
    // return height
    return frame.size.height;
}

// Get window position
void windowGetPosition(void* nsWindow, int* x, int* y) {
    // get main window
    WebviewWindow* window = (WebviewWindow*)nsWindow;
    // get window frame
    NSRect frame = [window frame];
    // set x and y
    *x = frame.origin.x;
    *y = frame.origin.y;
}

// Destroy window
void windowDestroy(void* nsWindow) {
    // Destroy window on main thread
    dispatch_async(dispatch_get_main_queue(), ^{
        // get main window
        WebviewWindow* window = (WebviewWindow*)nsWindow;
        // close window
        [window close];
    });
}


// windowClose closes the current window
void windowClose(void *window) {
    dispatch_async(dispatch_get_main_queue(), ^{
        // close window
        [(WebviewWindow*)window close];
    });
}

// windowZoom
void windowZoom(void *window) {
    dispatch_async(dispatch_get_main_queue(), ^{
        // zoom window
        [(WebviewWindow*)window zoom:nil];
    });
}

// miniaturize
void windowMiniaturize(void *window) {
    dispatch_async(dispatch_get_main_queue(), ^{
        // miniaturize window
        [(WebviewWindow*)window miniaturize:nil];
    });
}

// webviewRenderHTML renders the given HTML
void windowRenderHTML(void *window, const char *html) {
    dispatch_async(dispatch_get_main_queue(), ^{
        // get main window
        WebviewWindow* nsWindow = (WebviewWindow*)window;
        // get window delegate
        WebviewWindowDelegate* windowDelegate = (WebviewWindowDelegate*)[nsWindow delegate];
        // render html
        [(WKWebView*)windowDelegate.webView loadHTMLString:[NSString stringWithUTF8String:html] baseURL:nil];
    });
}

void windowInjectCSS(void *window, const char *css) {
    dispatch_async(dispatch_get_main_queue(), ^{
        // get main window
        WebviewWindow* nsWindow = (WebviewWindow*)window;
        // get window delegate
        WebviewWindowDelegate* windowDelegate = (WebviewWindowDelegate*)[nsWindow delegate];
        // inject css
        [(WKWebView*)windowDelegate.webView evaluateJavaScript:[NSString stringWithFormat:@"(function() { var style = document.createElement('style'); style.appendChild(document.createTextNode('%@')); document.head.appendChild(style); })();", [NSString stringWithUTF8String:css]] completionHandler:nil];
        free((void*)css);
    });
}

void windowMinimise(void *window) {
    dispatch_async(dispatch_get_main_queue(), ^{
        // minimize window
        [(WebviewWindow*)window miniaturize:nil];
    });
}

// zoom maximizes the window to the screen dimensions
void windowMaximise(void *window) {
    dispatch_async(dispatch_get_main_queue(), ^{
        // maximize window
        [(WebviewWindow*)window zoom:nil];
    });
}

bool isFullScreen(void *window) {
    // get main window
    WebviewWindow* nsWindow = (WebviewWindow*)window;
    long mask = [nsWindow styleMask];
    return (mask & NSWindowStyleMaskFullScreen) == NSWindowStyleMaskFullScreen;
}

// windowSetFullScreen
void windowSetFullScreen(void *window, bool fullscreen) {
    if (isFullScreen(window)) {
        return;
    }
    dispatch_async(dispatch_get_main_queue(), ^{
        WebviewWindow* nsWindow = (WebviewWindow*)window;
        windowSetMaxSize(nsWindow, 0, 0);
        windowSetMinSize(nsWindow, 0, 0);
        [nsWindow toggleFullScreen:nil];
    });
}

// windowUnminimise
void windowUnminimise(void *window) {
    dispatch_async(dispatch_get_main_queue(), ^{
        // unminimize window
        [(WebviewWindow*)window deminiaturize:nil];
    });
}

// windowUnmaximise
void windowUnmaximise(void *window) {
    dispatch_async(dispatch_get_main_queue(), ^{
        // unmaximize window
        [(WebviewWindow*)window zoom:nil];
    });
}

void windowDisableSizeConstraints(void *window) {
    dispatch_async(dispatch_get_main_queue(), ^{
        // get main window
        WebviewWindow* nsWindow = (WebviewWindow*)window;
        // disable size constraints
        [nsWindow setContentMinSize:CGSizeZero];
        [nsWindow setContentMaxSize:CGSizeZero];
    });
}

void windowShow(void *window) {
    dispatch_async(dispatch_get_main_queue(), ^{
        // show window
        [(WebviewWindow*)window makeKeyAndOrderFront:nil];
    });
}

void windowHide(void *window) {
    dispatch_async(dispatch_get_main_queue(), ^{
        [(WebviewWindow*)window orderOut:nil];
    });
}

// windowShowMenu opens an NSMenu at the given coordinates
void windowShowMenu(void *window, void *menu, int x, int y) {
    dispatch_async(dispatch_get_main_queue(), ^{
        // get main window
        WebviewWindow* nsWindow = (WebviewWindow*)window;
        // get menu
        NSMenu* nsMenu = (NSMenu*)menu;
        // get webview
        WebviewWindowDelegate* windowDelegate = (WebviewWindowDelegate*)[nsWindow delegate];
        // get webview
        WKWebView* webView = (WKWebView*)windowDelegate.webView;
        NSPoint point = NSMakePoint(x, y);
        [nsMenu popUpMenuPositioningItem:nil atLocation:point inView:webView];
    });
}



// Make the given window frameless
void windowSetFrameless(void *window, bool frameless) {
    dispatch_async(dispatch_get_main_queue(), ^{
        // get main window
        WebviewWindow* nsWindow = (WebviewWindow*)window;
        // set the window style to be frameless
        if (frameless) {
            [nsWindow setStyleMask:([nsWindow styleMask] | NSWindowStyleMaskFullSizeContentView)];
        } else {
            [nsWindow setStyleMask:([nsWindow styleMask] & ~NSWindowStyleMaskFullSizeContentView)];
        }
    });
}

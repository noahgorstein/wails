package application

/*
#cgo linux pkg-config: gtk+-3.0 webkit2gtk-4.0

#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <webkit2/webkit2.h>
#include <stdio.h>
#include <limits.h>
#include <stdint.h>

static void init(void) {
   gtk_init(NULL, NULL);
}

static void run(void) {
    gtk_main();
}

// get the current window ID
static unsigned int getCurrentWindowID(void) {
    GList* windows;
    GList* l;
    GtkWindow *window;
    window = NULL;

    windows = gtk_window_list_toplevels ();
    for (l = windows; l; l = l->next) {
        GtkWindow *w = l->data;
        if (gtk_window_has_toplevel_focus (w)) {
            window = w;
            break;
        }
    }
    g_list_free (windows);
    if (!window) {
        printf("Failed to locate the current window\n");
        return 0;
    }
    return gtk_application_window_get_id(GTK_WIDGET(window)) + 1;
};

*/
import "C"
import (
	"os"
	"unsafe"

	"github.com/wailsapp/wails/v2/pkg/assetserver/webview"
	"github.com/wailsapp/wails/v3/pkg/events"
)

func init() {
	// Set GDK_BACKEND=x11 if currently unset and XDG_SESSION_TYPE is unset, unspecified or x11 to prevent warnings
	if os.Getenv("GDK_BACKEND") == "" && (os.Getenv("XDG_SESSION_TYPE") == "" || os.Getenv("XDG_SESSION_TYPE") == "unspecified" || os.Getenv("XDG_SESSION_TYPE") == "x11") {
		_ = os.Setenv("GDK_BACKEND", "x11")
	}

	C.gtk_init(nil, nil)
}

type linuxApp struct {
	applicationMenu unsafe.Pointer
	parent          *App
}

func (m *linuxApp) hide() {
	//	C.hide()
}

func (m *linuxApp) show() {
	//	C.show()
}

func (m *linuxApp) on(eventID uint) {
	//	C.registerListener(C.uint(eventID))
}

func (m *linuxApp) setIcon(icon []byte) {
	//	C.setApplicationIcon(unsafe.Pointer(&icon[0]), C.int(len(icon)))
}

func (m *linuxApp) name() string {
	return "not implemented"
	// appName := C.getAppName()
	// defer C.free(unsafe.Pointer(appName))
	// return C.GoString(appName)
}

func (m *linuxApp) getCurrentWindowID() uint {
	//	return uint(C.getCurrentWindowID())r
	return uint(1) // FIXME
}

func (m *linuxApp) setApplicationMenu(menu *Menu) {
	if menu == nil {
		// Create a default menu for mac
		menu = defaultApplicationMenu()
	}
	menu.Update()

	// Convert impl to macosMenu object
	//	m.applicationMenu = (menu.impl).(*macosMenu).nsMenu
	//	C.setApplicationMenu(m.applicationMenu)
}

func (m *linuxApp) run() error {

	// Add a hook to the ApplicationDidFinishLaunching event
	// FIXME: add Linux specific events - Shouldn't these be translated to Wails events?
	m.parent.On(events.Mac.ApplicationDidFinishLaunching, func() {
		//		C.setApplicationShouldTerminateAfterLastWindowClosed(C.bool(m.parent.options.Mac.ApplicationShouldTerminateAfterLastWindowClosed))
		//		C.setActivationPolicy(C.int(m.parent.options.Mac.ActivationPolicy))
		//		C.activateIgnoringOtherApps()
	})
	// setup event listeners
	for eventID := range m.parent.applicationEventListeners {
		m.on(eventID)
	}
	C.run()
	return nil
}

func (m *linuxApp) destroy() {
	//	C.destroyApp()
}

func newPlatformApp(app *App) *linuxApp {
	C.init()
	return &linuxApp{
		parent: app,
	}
}

//export processApplicationEvent
func processApplicationEvent(eventID C.uint) {
	applicationEvents <- uint(eventID)
}

//export processWindowEvent
func processWindowEvent(windowID C.uint, eventID C.uint) {
	windowEvents <- &WindowEvent{
		WindowID: uint(windowID),
		EventID:  uint(eventID),
	}
}

//export processMessage
func processMessage(windowID C.uint, message *C.char) {
	windowMessageBuffer <- &windowMessage{
		windowId: uint(windowID),
		message:  C.GoString(message),
	}
}

//export processURLRequest
func processURLRequest(windowID C.uint, wkUrlSchemeTask unsafe.Pointer) {
	webviewRequests <- &webViewAssetRequest{
		Request:  webview.NewRequest(wkUrlSchemeTask),
		windowId: uint(windowID),
	}
}

//export processDragItems
func processDragItems(windowID C.uint, arr **C.char, length C.int) {
	var filenames []string
	// Convert the C array to a Go slice
	goSlice := (*[1 << 30]*C.char)(unsafe.Pointer(arr))[:length:length]
	for _, str := range goSlice {
		filenames = append(filenames, C.GoString(str))
	}
	windowDragAndDropBuffer <- &dragAndDropMessage{
		windowId:  uint(windowID),
		filenames: filenames,
	}
}

//export processMenuItemClick
func processMenuItemClick(menuID C.uint) {
	menuItemClicked <- uint(menuID)
}

func setIcon(icon []byte) {
	if icon == nil {
		return
	}
	//C.setApplicationIcon(unsafe.Pointer(&icon[0]), C.int(len(icon)))
}

package application

/*
#include "webview_window_common.h"

*/
import "C"
import (
	"net/url"
	"sync"
	"unsafe"

	"github.com/wailsapp/wails/v3/pkg/events"
)

var showDevTools = func(window unsafe.Pointer) {}

type commonWebviewWindow struct {
	window unsafe.Pointer
	parent *WebviewWindow
}

func (w *commonWebviewWindow) openContextMenu(menu *Menu, data *ContextMenuData) {
	// Create the menu
	thisMenu := newMenuImpl(menu)
	thisMenu.update()
	C.windowShowMenu(w.window, thisMenu.nsMenu, C.int(data.X), C.int(data.Y))
}

func (w *commonWebviewWindow) getZoom() float64 {
	return float64(C.windowZoomGet(w.window))
}

func (w *commonWebviewWindow) setZoom(zoom float64) {
	C.windowZoomSet(w.window, C.double(zoom))
}

func (w *commonWebviewWindow) setFrameless(frameless bool) {
	C.windowSetFrameless(w.window, C.bool(frameless))
	if frameless {
		C.windowSetTitleBarAppearsTransparent(w.window, C.bool(true))
		C.windowSetHideTitle(w.window, C.bool(true))
	} else {
		macOptions := w.parent.options.Mac
		appearsTransparent := macOptions.TitleBar.AppearsTransparent
		hideTitle := macOptions.TitleBar.HideTitle
		C.windowSetTitleBarAppearsTransparent(w.window, C.bool(appearsTransparent))
		C.windowSetHideTitle(w.window, C.bool(hideTitle))
	}
}

func (w *commonWebviewWindow) show() {
	C.windowShow(w.window)
}

func (w *commonWebviewWindow) hide() {
	C.windowHide(w.window)
}

func (w *commonWebviewWindow) setFullscreenButtonEnabled(enabled bool) {
	C.setFullscreenButtonEnabled(w.window, C.bool(enabled))
}

func (w *commonWebviewWindow) disableSizeConstraints() {
	C.windowDisableSizeConstraints(w.window)
}

func (w *commonWebviewWindow) unfullscreen() {
	C.windowUnFullscreen(w.window)
}

func (w *commonWebviewWindow) fullscreen() {
	C.windowFullscreen(w.window)
}

func (w *commonWebviewWindow) unminimise() {
	C.windowUnminimise(w.window)
}

func (w *commonWebviewWindow) unmaximise() {
	C.windowUnmaximise(w.window)
}

func (w *commonWebviewWindow) maximise() {
	C.windowMaximise(w.window)
}

func (w *commonWebviewWindow) minimise() {
	C.windowMinimise(w.window)
}

func (w *commonWebviewWindow) on(eventID uint) {
	C.registerListener(C.uint(eventID))
}

func (w *commonWebviewWindow) zoom() {
	C.windowZoom(w.window)
}

func (w *commonWebviewWindow) minimize() {
	C.windowMiniaturize(w.window)
}

func (w *commonWebviewWindow) windowZoom() {
	C.windowZoom(w.window)
}

func (w *commonWebviewWindow) close() {
	C.windowClose(w.window)
}

func (w *commonWebviewWindow) zoomIn() {
	C.windowZoomIn(w.window)
}

func (w *commonWebviewWindow) zoomOut() {
	C.windowZoomOut(w.window)
}

func (w *commonWebviewWindow) zoomReset() {
	C.windowZoomReset(w.window)
}

func (w *commonWebviewWindow) toggleDevTools() {
	showDevTools(w.window)
}

func (w *commonWebviewWindow) reload() {
	//TODO: Implement
	println("reload called on WebviewWindow", w.parent.id)
}

func (w *commonWebviewWindow) forceReload() {
	//TODO: Implement
	println("forceReload called on WebviewWindow", w.parent.id)
}

func (w *commonWebviewWindow) center() {
	C.windowCenter(w.window)
}

func (w *commonWebviewWindow) isMinimised() bool {
	return w.syncMainThreadReturningBool(func() bool {
		return bool(C.windowIsMinimised(w.window))
	})
}

func (w *commonWebviewWindow) isMaximised() bool {
	return w.syncMainThreadReturningBool(func() bool {
		return bool(C.windowIsMaximised(w.window))
	})
}

func (w *commonWebviewWindow) isFullscreen() bool {
	return w.syncMainThreadReturningBool(func() bool {
		return bool(C.windowIsFullscreen(w.window))
	})
}

func (w *commonWebviewWindow) syncMainThreadReturningBool(fn func() bool) bool {
	var wg sync.WaitGroup
	wg.Add(1)
	var result bool
	globalApplication.dispatchOnMainThread(func() {
		result = fn()
		wg.Done()
	})
	wg.Wait()
	return result
}

func (w *commonWebviewWindow) restore() {
	// restore window to normal size
	C.windowRestore(w.window)
}

func (w *commonWebviewWindow) restoreWindow() {
	C.windowRestore(w.window)
}

func (w *commonWebviewWindow) execJS(js string) {
	C.windowExecJS(w.window, C.CString(js))
}

func (w *commonWebviewWindow) setURL(uri string) {
	if uri != "" {
		url, err := url.Parse(uri)
		if err == nil && url.Scheme == "" && url.Host == "" {
			// TODO handle this in a central location, the scheme and host might be platform dependant.
			url.Scheme = "wails"
			url.Host = "wails"
			uri = url.String()
		}
	}
	C.navigationLoadURL(w.window, C.CString(uri))
}

func (w *commonWebviewWindow) setAlwaysOnTop(alwaysOnTop bool) {
	C.windowSetAlwaysOnTop(w.window, C.bool(alwaysOnTop))
}

func (w *commonWebviewWindow) setTitle(title string) {
	if !w.parent.options.Frameless {
		cTitle := C.CString(title)
		C.windowSetTitle(w.window, cTitle)
	}
}

func (w *commonWebviewWindow) setSize(width, height int) {
	C.windowSetSize(w.window, C.int(width), C.int(height))
}

func (w *commonWebviewWindow) setMinSize(width, height int) {
	C.windowSetMinSize(w.window, C.int(width), C.int(height))
}
func (w *commonWebviewWindow) setMaxSize(width, height int) {
	C.windowSetMaxSize(w.window, C.int(width), C.int(height))
}

func (w *commonWebviewWindow) setResizable(resizable bool) {
	C.windowSetResizable(w.window, C.bool(resizable))
}
func (w *commonWebviewWindow) enableDevTools() {
	C.windowEnableDevTools(w.window)
}

func (w *commonWebviewWindow) size() (int, int) {
	var width, height C.int
	var wg sync.WaitGroup
	wg.Add(1)
	globalApplication.dispatchOnMainThread(func() {
		C.windowGetSize(w.window, &width, &height)
		wg.Done()
	})
	wg.Wait()
	return int(width), int(height)
}

func (w *commonWebviewWindow) setPosition(x, y int) {
	C.windowSetPosition(w.window, C.int(x), C.int(y))
}

func (w *commonWebviewWindow) width() int {
	var width C.int
	var wg sync.WaitGroup
	wg.Add(1)
	globalApplication.dispatchOnMainThread(func() {
		width = C.windowGetWidth(w.window)
		wg.Done()
	})
	wg.Wait()
	return int(width)
}
func (w *commonWebviewWindow) height() int {
	var height C.int
	var wg sync.WaitGroup
	wg.Add(1)
	globalApplication.dispatchOnMainThread(func() {
		height = C.windowGetHeight(w.window)
		wg.Done()
	})
	wg.Wait()
	return int(height)
}

func (w *commonWebviewWindow) run() {
	for eventId := range w.parent.eventListeners {
		w.on(eventId)
	}
	globalApplication.dispatchOnMainThread(func() {
		w.window = C.windowNew(C.uint(w.parent.id),
			C.int(w.parent.options.Width),
			C.int(w.parent.options.Height),
			C.bool(w.parent.options.EnableFraudulentWebsiteWarnings),
			C.bool(w.parent.options.Frameless),
			C.bool(w.parent.options.EnableDragAndDrop),
		)
		w.setTitle(w.parent.options.Title)
		w.setAlwaysOnTop(w.parent.options.AlwaysOnTop)
		w.setResizable(!w.parent.options.DisableResize)
		if w.parent.options.MinWidth != 0 || w.parent.options.MinHeight != 0 {
			w.setMinSize(w.parent.options.MinWidth, w.parent.options.MinHeight)
		}
		if w.parent.options.MaxWidth != 0 || w.parent.options.MaxHeight != 0 {
			w.setMaxSize(w.parent.options.MaxWidth, w.parent.options.MaxHeight)
		}
		//w.setZoom(w.parent.options.Zoom)
		w.enableDevTools()
		w.setBackgroundColour(w.parent.options.BackgroundColour)

		macOptions := w.parent.options.Mac
		switch macOptions.Backdrop {
		case MacBackdropTransparent:
			C.windowSetTransparent(w.window)
			C.webviewSetTransparent(w.window)
		case MacBackdropTranslucent:
			C.windowSetTranslucent(w.window)
			C.webviewSetTransparent(w.window)
		}

		titleBarOptions := macOptions.TitleBar
		if !w.parent.options.Frameless {
			C.windowSetTitleBarAppearsTransparent(w.window, C.bool(titleBarOptions.AppearsTransparent))
			C.windowSetHideTitleBar(w.window, C.bool(titleBarOptions.Hide))
			C.windowSetHideTitle(w.window, C.bool(titleBarOptions.HideTitle))
			C.windowSetFullSizeContent(w.window, C.bool(titleBarOptions.FullSizeContent))
			if titleBarOptions.UseToolbar {
				C.windowSetUseToolbar(w.window, C.bool(titleBarOptions.UseToolbar), C.int(titleBarOptions.ToolbarStyle))
			}
			C.windowSetHideToolbarSeparator(w.window, C.bool(titleBarOptions.HideToolbarSeparator))
		}
		if macOptions.Appearance != "" {
			C.windowSetAppearanceTypeByName(w.window, C.CString(string(macOptions.Appearance)))
		}

		if macOptions.InvisibleTitleBarHeight != 0 {
			C.windowSetInvisibleTitleBar(w.window, C.uint(macOptions.InvisibleTitleBarHeight))
		}

		switch w.parent.options.StartState {
		case WindowStateMaximised:
			w.maximise()
		case WindowStateMinimised:
			w.minimise()
		case WindowStateFullscreen:
			w.fullscreen()

		}
		C.windowCenter(w.window)

		if w.parent.options.URL != "" {
			w.setURL(w.parent.options.URL)
		}
		// We need to wait for the HTML to load before we can execute the javascript
		w.parent.On(events.Mac.WebViewDidFinishNavigation, func(_ *WindowEventContext) {
			if w.parent.options.JS != "" {
				w.execJS(w.parent.options.JS)
			}
			if w.parent.options.CSS != "" {
				C.windowInjectCSS(w.window, C.CString(w.parent.options.CSS))
			}
		})
		if w.parent.options.HTML != "" {
			w.setHTML(w.parent.options.HTML)
		}
		if w.parent.options.Hidden == false {
			C.windowShow(w.window)
		}
	})
}

func (w *commonWebviewWindow) setBackgroundColour(colour *RGBA) {
	if colour == nil {
		return
	}
	C.windowSetBackgroundColour(w.window, C.int(colour.Red), C.int(colour.Green), C.int(colour.Blue), C.int(colour.Alpha))
}

func (w *commonWebviewWindow) position() (int, int) {
	var x, y C.int
	var wg sync.WaitGroup
	wg.Add(1)
	go globalApplication.dispatchOnMainThread(func() {
		C.windowGetPosition(w.window, &x, &y)
		wg.Done()
	})
	wg.Wait()
	return int(x), int(y)
}

func (w *commonWebviewWindow) destroy() {
	C.windowDestroy(w.window)
}

func (w *commonWebviewWindow) setHTML(html string) {
	// Convert HTML to C string
	cHTML := C.CString(html)
	// Render HTML
	C.windowRenderHTML(w.window, cHTML)
}

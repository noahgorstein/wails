//go:build linux

package application

/*
#cgo linux pkg-config: gtk+-3.0 webkit2gtk-4.0

#include "webview_window_common.h"
*/
import "C"

type linuxWebviewWindow struct {
	commonWebviewWindow
}

func newWindowImpl(parent *WebviewWindow) *linuxWebviewWindow {
	result := &linuxWebviewWindow{
		commonWebviewWindow{
			parent: parent,
		},
	}
	return result
}

func (w *linuxWebviewWindow) getScreen() (*Screen, error) {
	return getScreenForWindow(w)
}

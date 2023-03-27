//go:build darwin
package application

/*

#cgo CFLAGS: -mmacosx-version-min=10.13 -x objective-c
#cgo LDFLAGS: -framework Cocoa -framework WebKit

*/

import "C"


type macosWebviewWindow struct {
	commonWebviewWindow
}

func newWindowImpl(parent *WebviewWindow) *macosWebviewWindow {
	result := &macosWebviewWindow{
		commonWebviewWindow{
			parent: parent,
		},
	}
	return result
}

func (w *macosWebviewWindow) getScreen() (*Screen, error) {
	return getScreenForWindow(w)
}

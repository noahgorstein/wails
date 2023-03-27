//go:build linux

package webview

/*
#cgo linux pkg-config: gtk+-3.0 webkit2gtk-4.0 gio-unix-2.0

#include "gtk/gtk.h"
#include "webkit2/webkit2.h"
#include "gio/gunixinputstream.h"

*/
import "C"

import (
	"errors"
	"fmt"
	"io"
	"net/http"
	"os"
	"strconv"
	"syscall"
)

var (
	errRequestStopped   = errors.New("request has been stopped")
	errResponseFinished = errors.New("response has been finished")
)

var _ ResponseWriter = &responseWriter{}

type responseWriter struct {
	r *request

	header      http.Header
	wroteHeader bool

	finished bool

	w    io.WriteCloser
	wErr error
}

func (rw *responseWriter) Header() http.Header {
	if rw.header == nil {
		rw.header = http.Header{}
	}
	return rw.header
}

func (rw *responseWriter) Write(buf []byte) (int, error) {
	if rw.finished {
		return 0, errResponseFinished
	}

	rw.WriteHeader(http.StatusOK)

	if rw.wErr != nil {
		return 0, rw.wErr
	}

	return rw.w.Write(buf)
}

func (rw *responseWriter) WriteHeader(code int) {
	if rw.wroteHeader || rw.finished {
		return
	}
	rw.wroteHeader = true

	contentLength := int64(-1)
	contentType := C.CString(rw.Header().Get("Content-Type"))
	fmt.Println("Content-Type: ", rw.Header().Get("Content-Type"))
	if sLen := rw.Header().Get("Content-Length"); sLen != "" {
		if pLen, _ := strconv.ParseInt(sLen, 10, 64); pLen > 0 {
			contentLength = pLen
		}
	}
	// We can't use os.Pipe here, because that returns files with a finalizer for closing the FD. But the control over the
	// read FD is given to the InputStream and will be closed there.
	// Furthermore we especially don't want to have the FD_CLOEXEC
	rFD, w, err := pipe()
	if err != nil {
		//		rw.finishWithError(http.StatusInternalServerError, fmt.Errorf("unable to open pipe: %s", err))
		//		return
	}
	rw.w = w
	stream := C.g_unix_input_stream_new(C.int(rFD), gtkBool(true))
	defer C.g_object_unref(C.gpointer(stream))

	C.webkit_uri_scheme_request_finish(
		(*C.WebKitURISchemeRequest)(rw.r.task),
		stream,
		C.long(contentLength),
		contentType,
	)
}

func (rw *responseWriter) Finish() error {
	if !rw.wroteHeader {
		rw.WriteHeader(http.StatusNotImplemented)
	}

	if rw.finished {
		return nil
	}
	rw.finished = true

	return nil
}

func pipe() (r int, w *os.File, err error) {
	var p [2]int
	e := syscall.Pipe2(p[0:], 0)
	if e != nil {
		return 0, nil, fmt.Errorf("pipe2: %s", e)
	}

	return p[0], os.NewFile(uintptr(p[1]), "|1"), nil
}

func gtkBool(input bool) C.gboolean {
	if input {
		return C.gboolean(1)
	}
	return C.gboolean(0)
}

//go:build linux

package webview

import (
	"fmt"
	"io"
	"net/http"
	"unsafe"
)

/*
#cgo linux pkg-config: gtk+-3.0 webkit2gtk-4.0

#include <gtk/gtk.h>
#include <webkit2/webkit2.h>
#include "libsoup/soup.h"
#include <stdio.h>
#include <limits.h>
#include <stdint.h>

static const char * URLSchemeTaskRequestMethod(void *wkUrlSchemeTask) {


}

*/
import "C"

// NewRequest creates as new WebViewRequest based on a pointer to an `WebKitURISchemeRequest`
//
// Please make sure to call Release() when finished using the request.
func NewRequest(wkURLSchemeTask unsafe.Pointer) Request {
	//	C.URLSchemeTaskRetain(wkURLSchemeTask)
	return &request{task: wkURLSchemeTask}
}

var _ Request = &request{}

type request struct {
	task unsafe.Pointer

	header http.Header
	body   io.ReadCloser
	rw     *responseWriter
}

func (r *request) AddRef() error {
	//	C.URLSchemeTaskRetain(r.task)
	return nil
}

func (r *request) Release() error {
	//	C.URLSchemeTaskRelease(r.task)
	return nil
}

func (r *request) URL() (string, error) {
	req := (*C.WebKitURISchemeRequest)(r.task)
	uri := C.webkit_uri_scheme_request_get_uri(req)
	return C.GoString(uri), nil
}

func (r *request) Method() (string, error) {
	req := (*C.WebKitURISchemeRequest)(r.task)
	method := C.webkit_uri_scheme_request_get_http_method(req)
	return C.GoString(method), nil
}

func (r *request) Header() (http.Header, error) {
	if r.header != nil {
		return r.header, nil
	}
	req := (*C.WebKitURISchemeRequest)(r.task)
	header := http.Header{}

	hdrs := C.webkit_uri_scheme_request_get_http_headers(req)

	var iter C.SoupMessageHeadersIter
	C.soup_message_headers_iter_init(&iter, hdrs)

	var name *C.char
	var value *C.char

	for C.soup_message_headers_iter_next(&iter, &name, &value) != 0 {
		header.Add(C.GoString(name), C.GoString(value))
	}

	r.header = header
	return header, nil
}

func (r *request) Body() (io.ReadCloser, error) {
	if r.body != nil {
		return r.body, nil
	}
	r.body = &requestBodyStreamReader{task: r.task}
	return r.body, nil
}

func (r *request) Response() ResponseWriter {
	if r.rw != nil {
		return r.rw
	}

	r.rw = &responseWriter{r: r}
	return r.rw
}

var _ io.ReadCloser = &requestBodyStreamReader{}

type requestBodyStreamReader struct {
	task   unsafe.Pointer
	closed bool
}

// Read implements io.Reader
func (r *requestBodyStreamReader) Read(p []byte) (n int, err error) {
	chunkSize := C.ulong(1 * 1024 * 1024)
	chunk := make([]byte, chunkSize)
	pointer := unsafe.Pointer(&chunk[0])
	strm := (*C.GInputStream)(r.task)
	res := C.g_input_stream_read(
		strm,
		pointer,
		chunkSize,
		nil,
		nil,
	)
	fmt.Println("res: ", res)
	// var content unsafe.Pointer
	// var contentLen int
	// if p != nil {
	// 	content = unsafe.Pointer(&p[0])
	// 	contentLen = len(p)
	// }

	// res := C.URLSchemeTaskRequestBodyStreamRead(r.task, content, C.int(contentLen))
	// if res > 0 {
	// 	return int(res), nil
	// }

	// switch res {
	// case 0:
	// 	return 0, io.EOF
	// case -1:
	// 	return 0, fmt.Errorf("body: stream error")
	// case -2:
	// 	return 0, fmt.Errorf("body: no stream defined")
	// case -3:
	// 	return 0, io.ErrClosedPipe
	// default:
	// 	return 0, fmt.Errorf("body: unknown error %d", res)
	// }
	return 0, fmt.Errorf("body: not implemented")
}

func (r *requestBodyStreamReader) Close() error {
	if r.closed {
		return nil
	}
	r.closed = true

	//	C.URLSchemeTaskRequestBodyStreamClose(r.task)
	return nil
}

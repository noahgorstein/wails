//go:build linux

package webview

import (
	"fmt"
	"io"
	"net/http"
	"unsafe"
)

// NewRequest creates as new WebViewRequest based on a pointer to an `id<WKURLSchemeTask>`
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
	return "", nil
	//	return C.GoString(C.URLSchemeTaskRequestURL(r.task)), nil
}

func (r *request) Method() (string, error) {
	//	return C.GoString(C.URLSchemeTaskRequestMethod(r.task)), nil
	return "", nil
}

func (r *request) Header() (http.Header, error) {
	if r.header != nil {
		return r.header, nil
	}

	header := http.Header{}
	// if cHeaders := C.URLSchemeTaskRequestHeadersJSON(r.task); cHeaders != nil {
	// 	if headers := C.GoString(cHeaders); headers != "" {
	// 		var h map[string]string
	// 		if err := json.Unmarshal([]byte(headers), &h); err != nil {
	// 			return nil, fmt.Errorf("unable to unmarshal request headers: %s", err)
	// 		}

	// 		for k, v := range h {
	// 			header.Add(k, v)
	// 		}
	// 	}
	// 	C.free(unsafe.Pointer(cHeaders))
	// }
	// r.header = header
	return header, nil
}

func (r *request) Body() (io.ReadCloser, error) {
	if r.body != nil {
		return r.body, nil
	}

	//	var body unsafe.Pointer
	//	var bodyLen C.int
	// if C.URLSchemeTaskRequestBodyBytes(r.task, &body, &bodyLen) {
	// 	if body != nil && bodyLen > 0 {
	// 		r.body = io.NopCloser(bytes.NewReader(C.GoBytes(body, bodyLen)))
	// 	} else {
	// 		r.body = http.NoBody
	// 	}
	// } else if C.URLSchemeTaskRequestBodyStreamOpen(r.task) {
	// 	r.body = &requestBodyStreamReader{task: r.task}
	// }

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

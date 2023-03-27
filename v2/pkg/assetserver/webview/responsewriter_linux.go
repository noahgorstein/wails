//go:build linux

package webview

/*
 */
import "C"

import (
	"errors"
	"net/http"
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

	//	var content unsafe.Pointer
	var contentLen int
	if buf != nil {
		//		content = unsafe.Pointer(&buf[0])
		contentLen = len(buf)
	}

	// if !C.URLSchemeTaskDidReceiveData(rw.r.task, content, C.int(contentLen)) {
	// 	return 0, errRequestStopped
	// }
	return contentLen, nil
}

func (rw *responseWriter) WriteHeader(code int) {
	if rw.wroteHeader || rw.finished {
		return
	}
	rw.wroteHeader = true

	// header := map[string]string{}
	// for k := range rw.Header() {
	// 	header[k] = rw.Header().Get(k)
	// }
	// headerData, _ := json.Marshal(header)

	// var headers unsafe.Pointer
	// var headersLen int
	// if len(headerData) != 0 {
	// 	headers = unsafe.Pointer(&headerData[0])
	// 	headersLen = len(headerData)
	// }

	//	C.URLSchemeTaskDidReceiveResponse(rw.r.task, C.int(code), headers, C.int(headersLen))
}

func (rw *responseWriter) Finish() error {
	if !rw.wroteHeader {
		rw.WriteHeader(http.StatusNotImplemented)
	}

	if rw.finished {
		return nil
	}
	rw.finished = true

	//	C.URLSchemeTaskDidFinish(rw.r.task)
	return nil
}

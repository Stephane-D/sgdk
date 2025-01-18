#include "mw-api/http.h"


/// Receives data from the HTTP test
static int http_recv(uint32_t len)
{
	int16_t recv_last;
	int err = FALSE;
	uint32_t recvd = 0;
	uint8_t ch = MW_HTTP_CH;

	// For the test, just read and discard the data
	while (recvd < len && !err) {
		recv_last = MW_BUFLEN;
		err = mw_recv_sync(&ch, cmd_buf, &recv_last, TSK_PEND_FOREVER) != MW_ERR_NONE;
		recvd += recv_last;
	}

	return err;
}

void HTTP_start(){
    
    u16 button;
    bool repaint = TRUE;
    
    do{
        HTTP_paint(repaint);
        button = readButton(JOY_1);
        repaint = HTTP_doAction(button, 0);
        print();
    }while(button != BUTTON_A);

}

void HTTP_paint(bool repaint){
    if(repaint){
        clearScreen();        
        VDP_drawText("HTTP Test", 1u, 2u);
        VDP_drawText("Pulse START to launch", 1u, 3u);
        VDP_drawText("Pulse A to return", 1u, 4u);
    }
}

bool HTTP_doAction(u16 button, u8 max_option){    
    switch (button){
    case BUTTON_START:{
        HTTP_test();
    }
    default:
    }    
    return FALSE;
}

/// Sets the certificate for the HTTPS TLS connection
static void http_cert_set(void)
{
	uint32_t hash = mw_http_cert_query();
	if (hash != cert_hash) {
		mw_http_cert_set(cert_hash, cert, cert_len);
	}
}


/// Test an HTTP GET request to https://www.example.com
void HTTP_test() {
	uint32_t len = 0;

	http_cert_set();

	if (mw_http_url_set("https://www.example.com") ||
			mw_http_method_set(MW_HTTP_METHOD_GET) ||
			mw_http_open(0) ||
			mw_http_finish(&len, MS_TO_FRAMES(20000)) < 100) {
		goto err_out;
	}
	if (len) {
		if (http_recv(len)) goto err_out;
	}
	println("HTTP test SUCCESS");

err_out:
	println("HTTP test FAILED");
}
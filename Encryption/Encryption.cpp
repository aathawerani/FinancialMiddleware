#include "Encryption.h"
#include <memory>
#include "openssl/include/bio.h"
#include "openssl/include/evp.h"
#include "openssl/include/buffer.h"

// Automatically link OpenSSL libraries
#pragma comment(lib, "openssl/lib/libssl.lib")
#pragma comment(lib, "openssl/lib/libcrypto.lib")


std::string Encryption::base64(const unsigned char* input, int length) {
    BIO* bmem = nullptr;
    BIO* b64 = nullptr;
    BUF_MEM* bptr = nullptr;

    b64 = BIO_new(BIO_f_base64());
    bmem = BIO_new(BIO_s_mem());
    b64 = BIO_push(b64, bmem);
    
    BIO_write(b64, input, length);
    BIO_flush(b64);
    BIO_get_mem_ptr(b64, &bptr);

    std::string encoded(bptr->data, bptr->length - 1);
    
    BIO_free_all(b64);

    return encoded;
}

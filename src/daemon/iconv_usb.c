#include <iconv.h>
#include "includes.h"

void u16dec_char(char* in, char* out, size_t* srclen, size_t* dstlen){
    iconv_t utf16to8 = iconv_open("UTF-8", "UTF-16LE");
    if((*srclen % 2) || (utf16to8 == (iconv_t) -1)) {
        out[0] = 0; 
        return;
    }    

    size_t srclen2 = 0; 
    for(; srclen2 < *srclen; srclen2 += 2){
        // Since it's UTF16 we need to check both
        if(!(in[srclen2] || in[srclen2 + 1])) 
            break;
    }    

    if(iconv(utf16to8, &in, &srclen2, &out, dstlen) == (size_t) -1)
        out[0] = 0; 

    iconv_close(utf16to8);
}

// UTF-8/UTF-16 conversions (srclen and dstlen in characters - 1 byte UTF8, 2 bytes UTF16)
void u16enc(char* in, ushort* out, size_t* srclen, size_t* dstlen){
    iconv_t utf8to16 = iconv_open("UTF-16LE", "UTF-8");
    memset(out, 0, *dstlen * 2);
    *dstlen = *dstlen * 2 - 2;
    iconv(utf8to16, &in, srclen, (char**)&out, dstlen);
    iconv_close(utf8to16);
}

void u16dec(ushort* in, char* out, size_t* srclen, size_t* dstlen){
    iconv_t utf16to8 = iconv_open("UTF-8", "UTF-16LE");
    size_t srclen2 = 0, srclenmax = *srclen;
    for(; srclen2 < srclenmax; srclen2++){
        if(!in[srclen2])
            break;
    }
    *srclen = srclen2 * 2;
    iconv(utf16to8, (char**)&in, srclen, &out, dstlen);
    iconv_close(utf16to8);
}


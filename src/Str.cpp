
/*
 * String.cpp
 *
 *  Created on: 25-jun.-2013
 *      Author: lieven2
 */

#include "Str.h"
#include <Log.h>

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DOUBLE

Str& Str::format(const char* fmt, ...) {
    va_list ap;
    va_start(ap, fmt); /* bf being the last argument before '...' */
    Str& me = format(fmt, ap);
    va_end(ap);
    return me;
}

Str& Str::format(const char* fmt, va_list va) {
    _limit = vsnprintf((char*)data() + _offset, _capacity - _offset, fmt, va) +
             _offset;
    return *this;
}

/*
Str::Str() :
    Bytes(0)
{
}
*/
Str::Str(int size) : Bytes(size) {}

Str::Str(uint8_t* pstart, int size) : Bytes(pstart, size) {}
#include <string.h>
Str::Str(const char* s) : Bytes((uint8_t*)s, strlen(s)) {}

Str& Str::set(const char* const s) {
    Bytes::clear();
    append(s);
    return *this;
}

Str& Str::clear() {
    Bytes::clear();
    return *this;
}

bool Str::equals(const char* s) {
    uint32_t i;
    uint32_t slen = strlen(s);
    if (slen != length())
        return false;
    for (i = 0; i < slen && i < length(); i++)
        if (s[i] != peek(i))
            return false;
    return true;
}
#include <cstring>
bool Str::find(const char* s) {
    char* pch = strstr((char*)c_str(), s);
    if (pch) {
        return true;
    }
    return false;
}

bool Str::endsWith(const char* s) {
    int sl = strlen(s);
    int offset = length() - sl;
    if (offset < 0)
        return false;
    int i;
    for (i = 0; i < sl; i++)
        if (s[i] != peek(i + offset))
            return false;
    return true;
}

bool Str::startsWith(Str& s) {
    if (s.length() > length())
        return false;
    uint32_t i;
    for (i = 0; i < s.length(); i++)
        if (s.peek(i) != peek(i))
            return false;
    return true;
}

bool Str::startsWith(const char* const s) {
    Str ss(s);
    return startsWith(ss);
}

Str& Str::operator<<(char ch) {
    write(ch);
    return *this;
}

Str& Str::operator<<(int i) {
    append((int32_t)i);
    return *this;
}

Str& Str::operator<<(Str& s) {
    write(s._start, 0, s._limit);
    return *this;
}

Str& Str::operator+=(const char* s) { return append(s); }

Str& Str::operator+=(Str& s) {
    *this << s;
    return *this;
}

const char* hexChar = "0123456789ABCDEF";

char nibbleToHex(uint8_t value) { return hexChar[value & 0xF]; }

Str& Str::appendHex(uint8_t* byt, uint32_t length, char separator) {
    for (uint32_t i = 0; i < length; i++) {
        if (i && separator)
            append(separator);
        appendHex(byt[i]);
    }
    return *this;
}

Str& Str::appendHex(Bytes& bytes) {
    bytes.offset(0);
    while (bytes.hasData()) {
        appendHex(bytes.read());
    };
    return *this;
}

Str& Str::appendHex(uint8_t byt) {
    write(hexChar[byt >> 4]);
    write(hexChar[byt & 0xF]);
    return *this;
}

Str& Str::appendHex(uint16_t word) {
    appendHex((uint8_t)(word >> 8));
    appendHex((uint8_t)(word));
    return *this;
}

Str& Str::appendHex(uint32_t word) {
    appendHex((uint8_t)(word >> 24));
    appendHex((uint8_t)(word >> 16));
    appendHex((uint8_t)(word >> 8));
    appendHex((uint8_t)(word));
    return *this;
}

Str& Str::appendHex(uint64_t word) {
    uint32_t msb = word >> 32;
    appendHex(msb);
    appendHex((uint32_t)(word));
    return *this;
}

#ifdef DOUBLE
Str& Str::operator<<(double d) {
    append(d);
    return *this;
}

Str& Str::operator<<(float d) {
    append(d);
    return *this;
}
#endif

#ifdef DOUBLE
#include <cstdio>
#include <stdlib.h>
Str& Str::append(double d) {
    char buf[80];
    sprintf(buf, "%.5f", (float)d);
    append(buf);
    return *this;
}

Str& Str::append(float f) {
    double d = f;
    append(d);
    return *this;
}

#endif
Str& Str::operator=(Str& s) {
    clear();
    s.offset(0);
    while (s.hasData()) {
        write(s.read());
    };
    return *this;
}
Str& Str::operator=(const char* s) {
    clear();
    return append(s);
}
Str& Str::operator<<(const char* s) { return append(s); }

bool Str::operator==(Str& str) {
    if (str.length() != length())
        return false;
    uint32_t i;
    for (i = 0; i < str.length(); i++)
        if (str.peek(i) != peek(i))
            return false;
    return true;
}

bool Str::operator==(const char* str) {
    if (strlen(str) != length())
        return false;
    uint32_t i = 0;
    while (str[i]) {
        if (str[i] != peek(i))
            return false;
        i++;
    };
    return true;
}

Str& Str::append(const char* s) {
    while (*s != '\0') {
        write((uint8_t)(*s));
        s++;
    }
    return *this;
}

Str& Str::append(Str& s) {
    write(s._start, 0, s._limit);
    return *this;
}

Str& Str::append(char s) {
    write((uint8_t)(s));
    return *this;
}

#define to_digit(c) ((c) - '0')
#define is_digit(c) ((unsigned)to_digit(c) <= 9)
#define to_char(n) ((n) + '0')

bool ishex(uint8_t c) {
    return (c >= '0' || c <= '9') || (c >= 'A' || c <= 'F') ||
           (c >= 'a' || c <= 'f');
}

Str& Str::append(uint64_t val) {
#define MAX_CHAR 22
    char str[MAX_CHAR];
    str[MAX_CHAR - 1] = '\0';
    char* cp = str + MAX_CHAR - 1;
    do {
        *--cp = to_char(val % 10);
        val /= 10;
    } while (val != 0);
    append(cp);
    return *this;
}

Str& Str::append(uint32_t val) {
#define MAX_CHAR_INT32 10
    char str[MAX_CHAR_INT32];
    str[MAX_CHAR_INT32 - 1] = '\0';
    char* cp = str + MAX_CHAR_INT32 - 1;
    do {
        *--cp = to_char(val % 10);
        val /= 10;
    } while (val != 0);
    append(cp);
    return *this;
}

Str& Str::append(int32_t val) {
    uint64_t v = val;
    if (val < 0) {
        write('-');
        v = -val;
    } else
        v = val;
    append(v);
    return *this;
}

Str& Str::append(bool b) {
    if (b)
        append("true");
    else
        append("false");
    return *this;
}

Str& Str::append(void* ptr) {
    union {
        void* ptr;
        uint8_t b[sizeof(int)];
    } u;
    u.ptr = ptr;
    append("0x");
    uint32_t i;
    for (i = 0; i < sizeof(int); i++)
        appendHex(u.b[i]);
    return *this;
}

Str& Str::substr(Str& mstr, uint32_t offset) {
    mstr.offset(offset);
    while (mstr.hasData())
        write(mstr.read());
    return *this;
}

const char* Str::c_str() {
    if (_limit < _capacity)
        *(_start + _limit) = '\0';
    else
        ERROR(" cannot put zero terminator ");
    return (char*)_start;
}

bool Str::isdigit(uint8_t v) { return v >= '0' && v <= '9'; }

Erc Str::parse(Str& val) {
    val = *this;
    return E_OK;
}

Erc Str::parse(int32_t& val) {
    val = atoi(c_str());
    return E_OK;
}

Erc Str::parse(uint64_t& pval) {
    uint64_t val = 0;
    while (hasData()) {
        if (isdigit(peek())) {
            val = val * 10;
            val += read() - '0';
        } else
            return E_INVAL;
    }
    pval = val;
    return E_OK;
}

Erc Str::parse(uint32_t& pval) {
    uint64_t val = 0;
    parse(val);
    pval = val;
    return E_OK;
}

Erc Str::parse(double& d) {
    char buffer[40];
    uint32_t len = length();
    if (len < sizeof(buffer)) {
        memcpy(buffer, data(), len);
        buffer[len] = '\0';
        d = atof(buffer);
        return E_OK;
    }
    return EINVAL;
}

Erc Str::parse(float& f) {
    double d = 0.0;
    Erc erc = parse(d);
    f = d;
    return erc;
}

Erc Str::parse(bool& val) {
    if (*this == "true") {
        val = true;
        return E_OK;
    } else if (*this == "false") {
        val = false;
        return E_OK;
    } else {
        return EINVAL;
    }
}

Erc Str::parseHex(Bytes& bytes) {
    while (true) {
        uint8_t b = 0;
        if (hasData() && ishex(peek())) {
            b = hexToNibble(read());
        } else {
            break;
        };
        if (hasData() && ishex(peek())) {
            b = b << 4;
            b += hexToNibble(read());
        } else {
            return E_NO_DATA;
        }
        bytes.write(b);
    }
    return E_OK;
}

Erc Str::parseHex(uint8_t* pb) {
    uint8_t b = 0;
    int i;
    for (i = 0; i < 2; i++)
        if (hasData())
            if (ishex(peek())) {
                b = b << 4;
                b = hexToNibble(read());
            } else
                return E_INVAL;
        else
            return E_LACK_RESOURCE;
    *pb = b;
    return E_OK;
}

#include <cstdlib>
Erc Str::parse(int64_t& ll) {
    //  ll=atoll(c_str());
    return E_OK;
}

bool Str::isNumber() {
    offset(0);
    enum { BEGIN, WHOLE, FRACT, EXP_BEGIN, EXP_VALUE } state = BEGIN;
    while (hasData()) {
        char ch = read();
        switch (state) {
        case BEGIN: {
            if (ch == '+' || ch == '-' || isdigit(ch))
                state = WHOLE;
            else
                return false;
            break;
        }
        case WHOLE: {
            if (isdigit(ch))
                state = WHOLE;
            else if (ch == '.')
                state = FRACT;
            else if (ch == 'E' || ch == 'e')
                state = EXP_BEGIN;
            else
                return false;
            break;
        }
        case FRACT: {
            if (isdigit(ch))
                state = FRACT;
            else if (ch == 'E' || ch == 'e')
                state = BEGIN;
            else
                return false;
            break;
        }
        case EXP_BEGIN: {
            if (ch == '+' || ch == '-' || isdigit(ch))
                state = EXP_VALUE;
            else
                return false;
            break;
        }
        case EXP_VALUE: {
            if (isdigit(ch))
                state = EXP_VALUE;
            else
                return false;
            break;
        }
        }
    }
    return true;
}

bool Str::ishex(uint8_t c) {
    return (c >= '0' || c <= '9') || (c >= 'A' || c <= 'F') ||
           (c >= 'a' || c <= 'f');
}

uint8_t Str::hexToNibble(uint8_t ch) {
    if (ch >= '0' && ch <= '9')
        return ch - '0';
    if (ch >= 'A' && ch <= 'F')
        return (ch - 'A') + 10;
    if (ch >= 'a' && ch <= 'f')
        return (ch - 'a') + 10;
    return 0;
}
/*
 * String.cpp
 *
 *  Created on: 25-jun.-2013
 *      Author: lieven2
 */
#ifdef FTOA_NEEDED
extern "C" void ftoa(float f, char* str, uint8_t precision) {
    uint8_t i, j, divisor = 1;
    int8_t log_f;
    int32_t int_digits = (int)f; // store the integer digits
    float decimals;
    char s1[12];

    memset(str, 0, strlen(str));
    memset(s1, 0, 10);

    if (f < 0) {
        // if a negative number
        str[0] = '-'; // start the char array with '-'
        str[1] = 0;
        f = -f; // store its positive absolute value
    }
    log_f = ceil(log10(f)); // get number of digits before the decimal
    if (log_f > 0) {
        // log value > 0 indicates a number > 1
        if (log_f == precision) {
            // if number of digits = significant figures
            f += 0.5;        // add 0.5 to round up decimals >= 0.5
            itoa(f, s1, 10); // itoa converts the number to a char array
            strcat(str, s1); // add to the number string
        } else if ((log_f - precision) > 0) {
            // if more integer digits than significant digits
            i = log_f - precision; // count digits to discard
            divisor = 10;
            for (j = 0; j < i; j++)
                divisor *= 10; // divisor isolates our desired integer digits
            f /= divisor;      // divide
            f += 0.5;          // round when converting to int
            int_digits = (int)f;
            int_digits *= divisor; // and multiply back to the adjusted value
            itoa(int_digits, s1, 10);
            strcat(str, s1);
        } else {
            // if more precision specified than integer digits,
            itoa(int_digits, s1, 10); // convert
            strcat(str, s1);          // and append
        }
    }

    else {
        // decimal fractions between 0 and 1: leading 0
        s1[0] = '0';
        strcat(str, s1);
    }

    if (log_f < precision) {
        // if precision exceeds number of integer digits,
        decimals = f - (int)f; // get decimal value as float
        strcat(str, ".");      // append decimal point to char array

        i = precision - log_f; // number of decimals to read
        for (j = 0; j < i; j++) {
            // for each,
            decimals *= 10; // multiply decimals by 10
            if (j == (i - 1))
                decimals += 0.5; // and if it's the last, add 0.5 to round it
            itoa((int)decimals, s1,
                 10);                  // convert as integer to character array
            strcat(str, s1);           // append to string
            decimals -= (int)decimals; // and remove, moving to the next
        }
    }
}

#endif

Erc Str::copyTo(char* buffer, uint32_t maxLength) {
    if (maxLength < (length() - 1))
        return E2BIG;
    memcpy(buffer, data(), length());
    buffer[length()] = '\0';
    return E_OK;
}

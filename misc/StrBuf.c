/*
 *   See file ../COPYRIGHT for copying and redistribution conditions.
 */
#include "config.h"

#include <ctype.h>
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <log.h>
#include "StrBuf.h"

struct strBuf {
    char*       buf;    /* NUL-terminated string */
    size_t      len;    /* length of string (excluding terminating NUL) */
    size_t      max;    /* the size of the allocated buffer */
};

/**
 * Ensures that a string-buffer can hold a given number of characters.
 *
 * Arguments
 *      buf             Pointer to the string-buffer or NULL.
 *      n               The number of characters the string-buffer must hold
 *                      (excluding the terminating NUL).
 * Returns
 *      NULL            Out-of-memory ("log_start()" called) or "buf" is
 *                      NULL.
 *      else            Pointer to the string-buffer.
 */
StrBuf*
sbEnsure(
    StrBuf*             buf,
    const size_t        n)
{
    if (NULL != buf) {
        size_t  newMax = n + 1;

        if (newMax > buf->max) {
            char*   newBuf = realloc(buf->buf, newMax);

            if (NULL == newBuf) {
                log_serror("sbEnsure(): Couldn't reallocate %lu bytes", newMax);
                buf = NULL;
            }
            else {
                buf->buf = newBuf;
                buf->max = newMax;
            }
        }
    }

    return buf;
}

/**
 * Returns a new string-buffer. The buffer's string will be the empty string.
 *
 * Returns
 *      NULL            Out-of-memory. "log_start()" called.
 *      else            Pointer to the string-buffer.
 */      
StrBuf*
sbNew(void)
{
    size_t      nbytes = sizeof(StrBuf);
    StrBuf*     sb = (StrBuf*)malloc(nbytes);

    if (NULL == sb) {
        log_serror("sbNew(): Couldn't allocate %lu bytes for a new instance",
            nbytes);
    }
    else {
        sb->buf = NULL;
        sb->len = 0;
        sb->max = 1;

        if (NULL == sbEnsure(sb, 0)) {
            sbFree(sb);
            sb = NULL;
        }
    }                                   /* "sb" allocated */

    return sb;
}

/**
 * Trims the string of a string-buffer by removing `isspace()` characters from
 * the end.
 *
 * @param[in] buf  The string buffer.
 * @return         The string buffer.
 */
StrBuf*
sbTrim(
        StrBuf* const buf)
{
    size_t i = buf->len;

    for (; i > 0 && isspace(buf->buf[i-1]); --i)
        ;
    buf->buf[i] = 0;
    buf->len = i;

    return buf;
}

/**
 * Truncates the string of a string-buffer to a given length. If the length is
 * larger than then current string, then nothing happens.
 *
 * Arguments
 *      buf             Pointer to the string-buffer to be trimmed or NULL.
 *      n               The number of characters to retain (excluding the
 *                      terminating NUL).
 * Returns
 *      NULL            "buf" is NULL.
 *      else            Pointer to the string-buffer.
 */
StrBuf*
sbTruncate(
    StrBuf* const       buf,
    const size_t        n)
{
    if (NULL != buf && n < buf->len)
        buf->buf[n] = 0;

    return buf;
}

/**
 * Appends a variadic list of strings to the string-buffer.
 *
 * Arguments
 *      buf             Pointer to the string-buffer to append to or NULL.
 *      strings         Pointers to the strings to be appended. The last
 *                      pointer must be NULL.
 * Returns
 *      NULL            Out-of-memory ("log_start()" called) or "buf" is
 *                      NULL.
 *      else            Pointer to the string-buffer.
 */
StrBuf*
sbCatV(
    StrBuf* const       buf,
    va_list             strings)
{
    if (buf != NULL) {
        const char* string;

        for (string = (const char*)va_arg(strings, const char*); NULL != string;
                string = (const char*)va_arg(strings, const char*)) {
            size_t  newLen = buf->len + strlen(string);

            if (NULL == sbEnsure(buf, newLen))
                return NULL;

            (void)strcpy(buf->buf + buf->len, string);

            buf->len = newLen;
        }
    }

    return buf;
}

/**
 * Appends a list of strings to the string-buffer.
 *
 * Arguments
 *      buf             Pointer to the string-buffer to append to or NULL.
 *      ...             Pointers to the strings to be appended. The last
 *                      pointer must be NULL.
 * Returns
 *      NULL            Out-of-memory ("log_start()" called) or "buf" is
 *                      NULL.
 *      else            Pointer to the string-buffer.
 */
StrBuf*
sbCatL(
    StrBuf* const       buf,
    ...)
{
    StrBuf*             sb;
    va_list             ap;

    va_start(ap, buf);

    sb = sbCatV(buf, ap);

    va_end(ap);

    return sb;
}

/**
 * Appends a string to a string-buffer.
 *
 * Arguments
 *      buf             Pointer to the string-buffer to append to or NULL.
 *      string          The string to append.
 * Returns
 *      NULL            Out-of-memory ("log_start()" called) or "buf" is
 *                      NULL.
 *      else            Pointer to the string-buffer.
 */
StrBuf*
sbCat(
    StrBuf* const restrict     buf,
    const char* const restrict string)
{
    return sbCatL(buf, string, NULL);
}

/**
 * Appends at most N characters of a string to the string-buffer.  Characters
 * past the NUL character of the string are not appended.
 *
 * Arguments
 *      buf             Pointer to the string-buffer to append to or NULL.
 *      string          The string to append.
 *      n               The number of characters to append.
 * Returns
 *      NULL            Out-of-memory ("log_start()" called) or "buf" is
 *                      NULL.
 *      else            Pointer to the string-buffer.
 */
StrBuf*
sbCatN(
    StrBuf* const restrict buf,
    const char* restrict   string,
    const size_t           n)
{
    return sbTruncate(sbCatL(buf, string, NULL), n);
}

/**
 * Formats a stdarg argument list.
 *
 * @param[in] buf   The string-buffer into which to format the argument list
 *                  according to the given format.
 * @param[in] fmt   The format to use.
 * @param[in] args  The stdarg argument list.
 * @retval    NULL  Out-of-memory ("log_start()" called) or "buf" is NULL.
 * @return          `buf`.
 */
StrBuf*
sbPrintV(
    StrBuf* restrict           buf,
    const char* const restrict fmt,
    va_list                    args)
{
    if (buf != NULL) {
        va_list ap;
        va_copy(ap, args);

        int nbytes = vsnprintf(buf->buf, buf->max, fmt, args);

        if (nbytes >= buf->max) {
            buf = sbEnsure(buf, nbytes);

            if (buf != NULL)
                (void)vsnprintf(buf->buf, buf->max, fmt, ap);
        }

        va_end(ap);
    }

    return buf;
}

/**
 * Formats arguments.
 *
 * @param[in] buf   The string-buffer into which to format the arguments
 *                  according to the given format.
 * @param[in] fmt   The format to use.
 * @param[in] ...   The arguments to format.
 * @retval    NULL  Out-of-memory ("log_start()" called) or "buf" is NULL.
 * @return          `buf`.
 */
StrBuf*
sbPrint(
    StrBuf* const restrict     buf,
    const char* const restrict fmt,
    ...)
{
    va_list args;

    va_start(args, fmt);
    StrBuf* const sb = sbPrintV(buf, fmt, args);
    va_end(args);

    return sb;
}

/**
 * Clears a string-buffer. The string of a cleared string-buffer is the
 * empty string.
 *
 * Arguments
 *      buf             Pointer to the string-buffer to be cleared or NULL.
 * Returns
 *      NULL            "buf" is NULL.
 *      else            Pointer to the string-buffer.
 */
StrBuf*
sbClear(
    StrBuf* const       buf)
{
    if (NULL != buf) {
        buf->buf[0] = 0;
        buf->len = 0;
    }

    return buf;
}

/**
 * Returns the string of a string-buffer.
 *
 * Arguments
 *      buf             Pointer to the string-buffer or NULL.
 * Returns
 *      NULL            "buf" is NULL.
 *      else            Pointer to the string of the string-buffer.
 */
const char*
sbString(
    const StrBuf* const buf)
{
    return NULL == buf
        ? NULL
        : buf->buf;
}

/**
 * Frees a string-buffer.
 *
 * Arguments
 *      buf             Pointer to the string-buffer or NULL.
 */
void
sbFree(
    StrBuf* const       buf)
{
    if (NULL != buf) {
        free(buf->buf);
        free(buf);
    }
}

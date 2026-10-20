#include "string.h"

void *memset(void *s, int c, size_t n) {
    uint8_t *p = (uint8_t *)s;
    while (n--) *p++ = (uint8_t)c;
    return s;
}

void *memcpy(void *dest, const void *src, size_t n) {
    uint8_t *d = (uint8_t *)dest;
    const uint8_t *s = (const uint8_t *)src;
    while (n--) *d++ = *s++;
    return dest;
}

int memcmp(const void *a, const void *b, size_t n) {
    const uint8_t *pa = a, *pb = b;
    for (size_t i = 0; i < n; i++) {
        if (pa[i] != pb[i]) return pa[i] - pb[i];
    }
    return 0;
}

size_t strlen(const char *s) {
    size_t len = 0;
    while (s[len]) len++;
    return len;
}

int strcmp(const char *a, const char *b) {
    while (*a && *a == *b) { a++; b++; }
    return *(uint8_t *)a - *(uint8_t *)b;
}

int strncmp(const char *a, const char *b, size_t n) {
    for (size_t i = 0; i < n; i++) {
        if (a[i] != b[i] || !a[i]) return (uint8_t)a[i] - (uint8_t)b[i];
    }
    return 0;
}

char *strcpy(char *dest, const char *src) {
    char *d = dest;
    while ((*d++ = *src++));
    return dest;
}

char *strncpy(char *dest, const char *src, size_t n) {
    size_t i;
    for (i = 0; i < n && src[i]; i++) dest[i] = src[i];
    for (; i < n; i++) dest[i] = '\0';
    return dest;
}

char *strcat(char *dest, const char *src) {
    char *d = dest + strlen(dest);
    while ((*d++ = *src++));
    return dest;
}

/* Bounded copy/append (BSD semantics): always NUL-terminate within `size`,
 * never write past it. Return the length the string would have had so callers
 * can detect truncation. Used for path building where the source is attacker-
 * or user-controlled and could otherwise overrun a fixed stack buffer. */
size_t strlcpy(char *dst, const char *src, size_t size) {
    size_t i = 0;
    if (size) {
        for (; i < size - 1 && src[i]; i++) dst[i] = src[i];
        dst[i] = '\0';
    }
    while (src[i]) i++;
    return i;
}

size_t strlcat(char *dst, const char *src, size_t size) {
    size_t dl = 0;
    while (dl < size && dst[dl]) dl++;          /* bounded length of dst */
    if (dl == size) return size + strlen(src);  /* unterminated/full: do not write */
    return dl + strlcpy(dst + dl, src, size - dl);
}

void itoa(int value, char *buf, int base) {
    char tmp[36];
    int i = 0, neg = 0;
    if (value < 0 && base == 10) { neg = 1; value = -value; }
    uint32_t uv = (uint32_t)value;
    do { int d = uv % base; tmp[i++] = d < 10 ? '0' + d : 'a' + d - 10; uv /= base; } while (uv);
    if (neg) tmp[i++] = '-';
    int j = 0;
    while (i > 0) buf[j++] = tmp[--i];
    buf[j] = '\0';
}

void utoa(uint32_t value, char *buf, int base) {
    char tmp[36];
    int i = 0;
    do { int d = value % base; tmp[i++] = d < 10 ? '0' + d : 'a' + d - 10; value /= base; } while (value);
    int j = 0;
    while (i > 0) buf[j++] = tmp[--i];
    buf[j] = '\0';
}

int atoi(const char *s) {
    int n = 0, neg = 0;
    while (*s == ' ') s++;
    if (*s == '-') { neg = 1; s++; }
    while (*s >= '0' && *s <= '9') { n = n * 10 + (*s - '0'); s++; }
    return neg ? -n : n;
}

#include "string.h"
#include "../cpu/type.h"

// TODO: ugly function, find a better way
void int_to_ascii(int n, char str[]) {
    int tmp = n;
    int digit_count = 0;
    int i = 1;
    int sign = 0;

    if (n < 0) {
        n = -n;
        tmp = -tmp;
        i = 0;
        sign = 1;
    }

    do {
        digit_count += 1;
    } while ((tmp /= 10) > 0);

    for (int j = 0; j < digit_count + sign; j++) {
        str[j] = '0';
    }

    if (i == 0) {
        str[0] = '-';
    }

    for (; i <= digit_count - sign; i++) {
        str[(digit_count - i)] = n % 10 + '0';
        n /= 10;
    }
    str[digit_count + 1] = '\0';
}

int strlen(char s[]) {
    int i = 0;
    while (s[i] != '\0') ++i;
    return i;
}

void strcopy(char *dst, char *src) {
    int len = strlen(src);
    for (int i = 0; i < len; i++) {
        dst[i] = src[i];
    }
    dst[len] = '\0';
}

void append(char s[], char n) {
    int len = strlen(s);
    s[len] = n;
    s[len+1] = '\0';
}

void backspace(char s[]) {
    int len = strlen(s);
    s[len-1] = '\0';
}

/* BoP pointers and strings presentation */
int strcmp(char *s1, char *s2) {
    while (*s1 != '\0' && *s1 == *s2) {
        s1++;
        s2++;
    }
    return *s1 - *s2;
}

void hex_to_ascii(int n, char str[]) {
    str[0] = '\0';
    append(str, '0');
    append(str, 'x');
    char zeros = 0;

    s32 tmp;
    int i;
    for (i = 28; i > 0; i -= 4) {
        tmp = (n >> i) & 0xF;
        if (tmp == 0 && zeros == 0) continue;
        zeros = 1;
        if (tmp > 0xA) append(str, tmp - 0xA + 'a');
        else append(str, tmp + '0');
    }

    tmp = n & 0xF;
    if (tmp >= 0xA) append(str, tmp - 0xA + 'a');
    else append(str, tmp + '0');

    append(str, '\0');
}

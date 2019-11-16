void memory_copy(char *source, char *dest, int nbytes) {
    for (int i = 0; i < nbytes; i++) {
        *(dest + i) = *(source + i);
    }
}

// TODO: ugly function code, find a better way
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

// Example of the function found on the internet, but not fully ready
// void int_to_ascii(int n, char str[]) {
//     int i, sign;
//     if ((sign = n) < 0) n = -n;
//     i = 0;
//     do {
//         str[i++] = n % 10 + '0';
//     } while ((n /= 10) > 0);
//     if (sign < 0) str[i++] = '-';
//     str[i] = '\0';
//     /* TODO: implement "reverse" */
// }

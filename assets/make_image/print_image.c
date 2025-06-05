#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

int base64_decode_char(char c) {
    if ('A' <= c && c <= 'Z') return c - 'A';
    if ('a' <= c && c <= 'z') return c - 'a' + 26;
    if ('0' <= c && c <= '9') return c - '0' + 52;
    if (c == '+') return 62;
    if (c == '/') return 63;
    return -1;
}

int decode_base64_block(char *in, uint8_t *out) {
    int val[4];
    for (int i = 0; i < 4; ++i) {
        val[i] = base64_decode_char(in[i]);
        if (val[i] < 0) return -1;
    }

    out[0] = (val[0] << 2) | (val[1] >> 4);
    out[1] = ((val[1] & 0x0F) << 4) | (val[2] >> 2);
    out[2] = ((val[2] & 0x03) << 6) | val[3];
    return 0;
}

int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s WIDTH HEIGHT\n", argv[0]);
        return 1;
    }

    int width = atoi(argv[1]);
    int height = atoi(argv[2]);
    int total_pixels = width * height;
    int pixel_count = 0;

    char in[4];
    uint8_t out[3];
    int c, i = 0;

    while ((c = getchar()) != EOF && pixel_count < total_pixels) {
        if (c == '\n' || c == '\r') continue;

        if (base64_decode_char(c) < 0) continue;

        in[i++] = c;
        if (i == 4) {
            if (decode_base64_block(in, out) != 0) {
                fprintf(stderr, "Invalid base64 block\n");
                return 1;
            }

            printf("\e[48;2;%d;%d;%dm  ", out[0], out[1], out[2]);
            pixel_count++;
            if (pixel_count % width == 0)
                printf("\e[0m\n");

            i = 0;
        }
    }

    printf("\e[0m\n");
    return 0;
}

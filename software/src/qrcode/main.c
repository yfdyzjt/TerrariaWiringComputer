#include <stdint.h>

#include "qrcode.h"
#include "output.h"

static void qrcode_print(QRCode qrcode, const char *text);

int main()
{
    char *text = "HELLO WORLD";

    uint8_t ecc = ECC_LOW;
    uint8_t version = qrcode_getMinimumVersion(text, ecc);

    QRCode qrcode;
    uint8_t qrcodeBytes[qrcode_getBufferSize(version)];

    qrcode_initText(&qrcode, qrcodeBytes, version, ecc, text);
    qrcode_print(qrcode, text);

    return 0;
}

static void qrcode_print(QRCode qrcode, const char *text)
{
    draw_clean();
    for (uint8_t y = 0; y < qrcode.size; y++)
        for (uint8_t x = 0; x < qrcode.size; x++)
            pixel_draw(x, y, qrcode_getModule(&qrcode, x, y) ? 1 : 0);
    string_draw(0, qrcode.size + 2, text);
    draw_refresh();
}

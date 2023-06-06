
#include "io.h"

#include <inttypes.h>
#include <stdbool.h>

#define MAX_COLORS 256

typedef struct bmp BMP;
BMP *bmp_create(Buffer *buf);
void bmp_free(BMP **buf);
void bmp_write(const BMP *bmp, Buffer *buf);
void bmp_reduce_palette(BMP *bmp);

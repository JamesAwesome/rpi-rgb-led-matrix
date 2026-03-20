#include "pillow.h"
#include <stdint.h>

/*
 * Vendored subset of Pillow's ImagingMemoryInstance struct.
 * Layout stable across Pillow 9.x through 11.x.
 * We only need to reach the image32 field.
 *
 * Reference: https://github.com/python-pillow/Pillow/blob/main/src/libImaging/Imaging.h
 */
struct ImagingMemoryInstance {
    char mode[7];           /* IMAGING_MODE_LENGTH */
    int type;
    int depth;
    int bands;
    int xsize;
    int ysize;
    void *palette;          /* ImagingPalettePtr */
    uint8_t **image8;
    int32_t **image32;      /* This is the only field we access */
};

int** get_image32(void* im) {
    struct ImagingMemoryInstance* image = (struct ImagingMemoryInstance*) im;
    return (int**)image->image32;
}

#include <iostream>
#include <time.h>

using namespace std;

int main() {
    FILE *infile, *outfile;
    char fileType[10], *in, *out, *image;
    int width, height, depth, pixels;
    unsigned char r, g, b, red[16], green[16], blue[16], brightness[16], contrast[16], base[16];
    int nR, nG, nB;

    clock_t start, end;
    double cpu_time_used;


    infile = fopen("\\\\psf\\Home\\Desktop\\UFSCar.pnm", "rb");
    outfile = fopen("\\\\psf\\Home\\Desktop\\UFSCar_gain.pnm", "wb");


    fscanf(infile, "%s\n", fileType);
    fprintf(outfile, "%s%c", fileType, 10);

    fscanf(infile, "%d %d %d\n", &width, &height, &depth);
    fprintf(outfile, "%d %d %d%c", width, height, depth, 10);

    pixels = width * height;

    int extra = 16 - (pixels % 16); // the number of pixels that lack to pixels % 16 = 0
    if (extra == 16) { // if the number of pixels is multiple of 16, the above calculation will provide a wrong result (16 - 0 = 16)
        extra = 0; // then, we have to fix it, so that any additional pixels will be allocated.
    }

    in = out = image = (char *) malloc(3 * (pixels + extra));
    fread(image, 3, pixels + extra, infile);

    for (int i = 0; i < 3 * extra; i++) {
        image[(3*pixels) + i] = 0; // fill extra pixels with zeros
    }

    for (int i = 0; i < 16; i++) {
        brightness[i] = 50;
        contrast[i] = 4;
        base[i] = depth;
    }

    start = clock();
    for (int i = 0; i < (pixels + extra) / 16; i++) {
        for (int j = 0; j < 16; j++) {
            red[j] = (unsigned char) *in++;
            green[j] = (unsigned char) *in++;
            blue[j] = (unsigned char) *in++;
        }

        __asm {
            movups xmm0, red
            movups xmm1, green
            movups xmm2, blue
            movups xmm3, brightness
            movups xmm4, base
            paddusb xmm0, xmm3 // red += brightness
            paddusb xmm1, xmm3 // green += brightness
            paddusb xmm2, xmm3 // blue += brightness

            movups red, xmm0
            movups green, xmm1
            movups blue, xmm2
        }

        for (int j = 0; j < 16; j++) {
            *out++ = red[j];
            *out++ = green[j];
            *out++ = blue[j];
        }
    }
    end = clock();


    fwrite(image, 3, pixels, outfile);

    fclose(infile);
    fclose(outfile);

    free(image);

    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("time = %f seconds\n", cpu_time_used);
    system("PAUSE");

    return 0;
}
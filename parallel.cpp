#include <iostream>
#include <time.h>

using namespace std;

int main(int argc, char * argv []) {
    FILE *infile, *outfile;
    char fileType[10], *in, *out, *image;
    int width, height, depth, pixels;
    unsigned char r[16], g[16], b[16], red[16], green[16], blue[16], brightness[16], contrast[16], middle[16], middle_neg[16], zeros[16];
    int brightnessFactor = 50;
    int contrastFactor = 2;

    clock_t start, end;
    double cpu_time_used;

    if (argc < 3) {
        printf("Usage: %s <input.pnm> <output.pnm>\n", argv[0]);
        system("PAUSE");
        exit(1);
    }

    infile = fopen(argv[1], "rb");
    if (!infile) {
        printf("File %s not found!\n", argv[1]);
        system("PAUSE");
        exit(1);
    }

    outfile = fopen(argv[2], "wb");
    if (!outfile) {
        printf("Unable to create file %s!\n", argv[2]);
        system("PAUSE");
        exit(1);
    }

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
        image[(3 * pixels) + i] = 0; // fill extra pixels with zeros
    }

    for (int i = 0; i < 16; i++) {
        brightness[i] = brightnessFactor;
        contrast[i] = contrastFactor;
        middle[i] = 127;
        middle_neg[i] = 128;
        zeros[i] = 0;
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
            paddusb xmm0, xmm3 // red += brightness
            paddusb xmm1, xmm3 // green += brightness
            paddusb xmm2, xmm3 // blue += brightness

            mov eax, contrastFactor
            cmp eax, 2
            jl endd // if contrast < 2, there's no change
            movups xmm3, middle

            movups r, xmm0 // backup
            movups g, xmm1 // backup
            movups b, xmm2 // backup

            psubusb xmm0, xmm3 // red -= threshold
            psubusb xmm1, xmm3 // green -= threshold
            psubusb xmm2, xmm3 // blue -= threshold

            mov eax, contrastFactor
            con :
                paddusb xmm0, xmm0 // red += contrast
                paddusb xmm1, xmm1 // green += contrast
                paddusb xmm2, xmm2 // blue += contrast
            dec eax
            jnz con

            movups xmm3, xmm0 // backup
            movups xmm4, xmm1 // backup
            movups xmm5, xmm2 // backup
            movups xmm6, zeros
            // --
            movups xmm7, r
            movups r, xmm3

            pcmpeqd xmm3, xmm6 // compare channels with zeros
            pand xmm3, xmm7 // and between above comparison and original red channels
            paddusb xmm0, xmm3 // add only where necessary (mask)

            movups xmm3, r
            movups xmm7, middle
            pcmpeqd xmm3, xmm6 // compare channel with zeros
            pandn xmm3, xmm7 // nand between above comparison and threshold

            paddusb xmm0, xmm3 // add only where necessary (mask)
            // --
            movups xmm7, g
            movups g, xmm4

            pcmpeqd xmm4, xmm6 // compare channels with zeros
            pand xmm4, xmm7 // and between above comparison and original red channels
            paddusb xmm1, xmm4 // add only where necessary (mask)

            movups xmm4, g
            movups xmm7, middle
            pcmpeqd xmm4, xmm6 // compare channel with zeros
            pandn xmm4, xmm7 // nand between above comparison and threshold

            paddusb xmm1, xmm4 // add only where necessary (mask)
            // --
            movups xmm7, b
            movups b, xmm5

            pcmpeqd xmm5, xmm6 // compare channels with zeros
            pand xmm5, xmm7 // and between above comparison and original red channels
            paddusb xmm2, xmm5 // add only where necessary (mask)

            movups xmm5, b
            movups xmm7, middle
            pcmpeqd xmm5, xmm6 // compare channel with zeros
            pandn xmm5, xmm7 // nand between above comparison and threshold

            paddusb xmm2, xmm5 // add only where necessary (mask)
        // --

        endd :
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
    printf("Parallel time = %f seconds\n", cpu_time_used);

    return 0;
}
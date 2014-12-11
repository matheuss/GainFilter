#include <iostream>
#include <time.h>

using namespace std;

int main() {
    FILE *infile, *outfile;
    char fileType[10], *in, *out, *image, *inn;
    int width, height, depth, pixels;
    unsigned char r, g, b;
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
    printf("%d", pixels);

    in = inn = out = image = (char *) malloc(3 * pixels);
    fread(image, 3, pixels, infile);


    unsigned char red[16], green[16], blue[16], brightness[16], contrast[16], base[16];

    for (int i = 0; i < 16; i++) {
        brightness[i] = 50;
        contrast[i] = 4;
        red[i] = (unsigned char) *in++;
        green[i] = (unsigned char) *in++;
        blue[i] = (unsigned char) *in++;
        base[i] = depth;
    }

    __asm {
        movups xmm0, red
        movups xmm1, green
        movups xmm2, blue
        movups xmm3, brightness
        movups xmm4, base
        paddb xmm0, xmm3
        paddb xmm1, xmm3
        paddb xmm2, xmm3
        pcmpgtw xmm0, xmm4
        pcmpgtw xmm1, xmm4
        //movups xmm0, a
        //movups xmm1, b1
        //movups xmm3, c
        //paddsw xmm0, xmm1
        //pcmpgtw xmm0, xmm3
        //movups a, xmm0
    }

    start = clock();
    for (int i = 0; i < pixels; i++) {
        r = *in++;
        g = *in++;
        b = *in++;

        __asm {


        }

        //*out++ = nR;
        //*out++ = nG;
        //*out++ = nB;
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
#include <iostream>

using namespace std;

int main() {
    FILE *infile, *outfile;
    char fileType[10], *in, *out, *image;
    int width, height, depth, pixels;
    unsigned char r, g, b;
    int nR, nG, nB;


    infile = fopen("/Users/matheus/Desktop/UFSCar.pnm", "rb");
    outfile = fopen("/Users/matheus/Desktop/UFSCargain.pnm", "wb");


    fscanf(infile, "%s\n", fileType);
    fprintf(outfile, "%s%c", fileType, 10);

    fscanf(infile, "%d %d %d\n", &width, &height, &depth);
    fprintf(outfile, "%d %d %d%c", width, height, depth, 10);

    pixels = width * height;

    in = out = image = (char *) malloc(3 * pixels);

    fread(image, 3, pixels, infile);

    for (int i = 0; i < pixels; i++) {
        r = *in++;
        g = *in++;
        b = *in++;

        nR = (int) r - 50;
        nG = (int) g - 50;
        nB = (int) b - 50;

        if (nR > 255) {
            nR = 255;
        }
        if (nG> 255) {
            nG = 255;
        }
        if (nB > 255) {
            nB = 255;
        }

        if (nR < 0) {
            nR = 0;
        }
        if (nG < 0) {
            nG = 0;
        }
        if (nB < 0) {
            nB = 0;
        }

        *out++ = nR;
        *out++ = nG;
        *out++ = nB;
    }

    fwrite(image, 3, pixels, outfile);

    fclose(infile);
    fclose(outfile);

    free(image);


    return 0;
}
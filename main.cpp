#include <iostream>

using namespace std;

int truncate(int, int);

int main() {
    FILE *infile, *outfile;
    char fileType[10], *in, *out, *image;
    int width, height, depth, pixels;
    unsigned char r, g, b;
    int nR, nG, nB, brightness, contrast;

    brightness = 0;
    contrast = 4;

    infile = fopen("\\\\psf\\Home\\Desktop\\UFSCar.pnm", "rb");
    outfile = fopen("\\\\psf\\Home\\Desktop\\UFSCargain.pnm", "wb");


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

        __asm {
            movzx eax, r
            movzx ebx, g
            movzx ecx, b

            add eax, brightness // r += brightness amount
            add ebx, brightness // g += brightness amount
            add ecx, brightness // b += brightness amount

            mov nR, eax
            mov nG, ebx
            mov nB, ecx
        }

        nR = truncate(depth, nR);
        nG = truncate(depth, nG);
        nB = truncate(depth, nB);

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

int truncate(int threshold, int value) {
    __asm{
        mov eax, value
        cmp eax, 0
        jl truncate_small
        cmp eax, threshold
        jg truncate_big
        jmp endd

        truncate_small:
            mov eax, 0
            jmp endd
        truncate_big:
            mov eax, threshold

        endd:
            mov value, eax
    }
    return value;
}
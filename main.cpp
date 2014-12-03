#include <iostream>

using namespace std;

int main() {
    FILE *infile, *outfile;
    char fileType[10], *in, *out, *image;
    int width, height, depth, pixels;
    unsigned char r, g, b;
    int nR, nG, nB, brightness, contrast;

    brightness = 50;
    contrast = 3;

    infile = fopen("\\\\psf\\Home\\Desktop\\UFSCar.pnm", "rb");
    outfile = fopen("\\\\psf\\Home\\Desktop\\UFSCargain.pnm", "wb");


    fscanf(infile, "%s\n", fileType);
    fprintf(outfile, "%s%c", fileType, 10);

    fscanf(infile, "%d %d %d\n", &width, &height, &depth);
    fprintf(outfile, "%d %d %d%c", width, height, depth, 10);

    pixels = width * height;

    in = out = image = (char *)malloc(3 * pixels);

    fread(image, 3, pixels, infile);

    for (int i = 0; i < pixels; i++) {
        r = *in++;
        g = *in++;
        b = *in++;

        //nR = (int)r - 50;
        //nG = (int)g - 50;
        //nB = (int)b - 50;

        //if (nR > 255) {
        //	nR = 255;
        //}
        //if (nG> 255) {
        //	nG = 255;
        //}
        //if (nB > 255) {
        //	nB = 255;
        //}

        //if (nR < 0) {
        //	nR = 0;
        //}
        //if (nG < 0) {
        //	nG = 0;
        //}
        //if (nB < 0) {
        //	nB = 0;
        //}

        __asm {
            movzx eax, r // r += brightness amount
            add eax, brightness

            movzx ebx, g // g += brightness amount
            add ebx, brightness

            cmp eax, 0 // if red < 0
            jg truncate_small_r

            red_big:
                cmp eax, depth // if reg > depth
                jl truncate_big_r
                jmp green

            truncate_small_r: // if red < 0
                mov eax, 0 // then red = 0
                jmp red_big

            truncate_big_r: // if red > depth
                mov eax, depth // then red = depth

            green:
            cmp ebx, 0 // if green < 0
            jl truncate_small_g

            green_big:
                cmp ebx, depth // if green > depth
                jg truncate_big_g
                jmp blue

            truncate_small_g: // if green < 0
                mov ebx, 0 // then green = 0
                jmp green_big

            truncate_big_g: // if green > depth
                mov ebx, depth // then green = depth

            blue:

            mov nR, eax
            mov nB, ebx
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
#include <iostream>
#include <time.h>

using namespace std;

int truncate(int, int);

int main(int argc, char * argv []) {
    FILE *infile, *outfile;
    char fileType[10], *in, *out, *image;
    int width, height, depth, pixels;
    unsigned char r, g, b;
    int nR, nG, nB, brightness, contrast;

    clock_t start, end;
    double cpu_time_used;

    brightness = 50;
    contrast = 2;

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

    in = out = image = (char *) malloc(3 * pixels);

    fread(image, 3, pixels, infile);

    start = clock();
    for (int i = 0; i < pixels; i++) {
        r = *in++;
        g = *in++;
        b = *in++;

        __asm {
            movzx eax, r
            movzx ebx, g
            movzx ecx, b

            sub eax, 128 // contrast threshold
            sub ebx, 128 // contrast threshold
            sub ecx, 128 // contrast threshold

            mul contrast // contrast factor multiplication - red channel
            mov esi, eax
            mov eax, ebx
            mul contrast // contrast factor multiplication - green channel
            mov ebx, eax
            mov eax, ecx
            mul contrast // contrast factor multiplication - blue channel
            mov ecx, eax
            mov eax, esi

            add eax, 127 // contrast threshold
            add ebx, 127 // contrast threshold
            add ecx, 127 // contrast threshold

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
    end = clock();

    fwrite(image, 3, pixels, outfile);

    fclose(infile);
    fclose(outfile);

    free(image);

    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Serial time   = %f seconds\n", cpu_time_used);

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

        truncate_small :
        mov eax, 0
        jmp endd
        truncate_big :
        mov eax, threshold

        endd :
        mov value, eax
    }
    return value;
}
#ifndef _INC_STDIO
#include <stdio.h>
#endif
#ifndef _INC_STDLIB
#include <stdlib.h>
#endif
#ifndef _INC_STRING
#include <string.h>
#endif
#ifndef _INC_PROCESSOPTS
#include "processopts.h"
#endif


#pragma pack(push, 1) 
typedef struct BitmapFileHeader{
    unsigned short signature;
    unsigned int filesize;
    unsigned short reserved1;
    unsigned short reserved2;
    unsigned int pixelArrOffset;
} BitmapFileHeader; 

typedef struct BitmapInfoHeader{
    unsigned int headerSize;
    unsigned int width;
    unsigned int height;
    unsigned short planes;
    unsigned short bitsPerPixel;
    unsigned int compression;
    unsigned int imageSize;
    unsigned int xPixelsPerMeter;
    unsigned int yPixelsPerMeter;
    unsigned int colorsInColorTable;
    unsigned int importantColorCount;
} BitmapInfoHeader;

typedef struct Rgb{
    unsigned char b;
    unsigned char g;
    unsigned char r;
} Rgb;
#pragma pack(pop)


void print_file_header(BitmapFileHeader header){
    puts("File header:");
    printf("    signature:\t\t%x (%hu)\n", header.signature, header.signature);
    printf("    filesize:\t\t%x (%u)\n", header.filesize, header.filesize);
    printf("    reserved1:\t\t%x (%hu)\n", header.reserved1, header.reserved1);
    printf("    reserved2:\t\t%x (%hu)\n", header.reserved2, header.reserved2);
    printf("    pixelArrOffset:\t%x (%u)\n", header.pixelArrOffset, header.pixelArrOffset);
}

void print_info_header(BitmapInfoHeader header){
    puts("Info header:");
    printf("    headerSize:\t\t%x (%u)\n", header.headerSize, header.headerSize);
    printf("    width:  \t\t%x (%u)\n", header.width, header.width);
    printf("    height: \t\t%x (%u)\n", header.height, header.height);
    printf("    planes: \t\t%x (%hu)\n", header.planes, header.planes);
    printf("    bitsPerPixel:\t%x (%hu)\n", header.bitsPerPixel, header.bitsPerPixel);
    printf("    compression:\t%x (%u)\n", header.compression, header.compression);
    printf("    imageSize:\t\t%x (%u)\n", header.imageSize, header.imageSize);
    printf("    xPixelsPerMeter:\t\t%x (%u)\n", header.xPixelsPerMeter, header.xPixelsPerMeter);
    printf("    yPixelsPerMeter:\t\t%x (%u)\n", header.yPixelsPerMeter, header.yPixelsPerMeter);
    printf("    colorsInColorTable:\t\t%x (%u)\n", header.colorsInColorTable, header.colorsInColorTable);
    printf("    importantColorCount:\t%x (%u)\n", header.importantColorCount, header.importantColorCount);
}

Rgb **read_bmp(char file_name[], BitmapFileHeader* bmfh, BitmapInfoHeader* bmif){
    FILE *f = fopen(file_name, "rb");
    if (!f){
        fprintf(stderr, "File is missing. Error 45\n");
        fclose(f);
        exit(45);
    }
    fread(bmfh, 1, sizeof(BitmapFileHeader), f);
    fread(bmif, 1, sizeof(BitmapInfoHeader), f);
    unsigned int H = bmif->height;
    unsigned int W = bmif->width;
    Rgb **arr = malloc(H * sizeof(Rgb*));
    int padd = 4 - (W * sizeof(Rgb)) % 4;
    for(unsigned int i = 0; i < H; i++){
        arr[i] = malloc(W * sizeof(Rgb) + padd%4);
        fread(arr[i], 1, W * sizeof(Rgb) + padd%4, f); 
    }
    fclose(f);
    return arr;
}

void write_bmp(char *file_name, Rgb **arr, unsigned int H, unsigned int W, BitmapFileHeader bmfh, BitmapInfoHeader bmif){
    FILE *ff = fopen(file_name, "wb");
    fwrite(&bmfh, 1, sizeof(BitmapFileHeader), ff);
    fwrite(&bmif, 1, sizeof(BitmapInfoHeader), ff);
    int padd = 4 - (W * sizeof(Rgb)) % 4;
    for(unsigned int i = 0; i < H; i++){
        fwrite(arr[i], 1, W * sizeof(Rgb) + padd%4, ff);
    }
    fclose(ff);
}

void set_pixel(Rgb ***bmp_file_pixels, unsigned int H, unsigned int W, 
        unsigned char r, unsigned char g, unsigned char b, int x, int y, int thickness){
    y = H-y-1;
    for (int i = -thickness/2; i<(thickness+1)/2; i++){
        for (int j = -thickness/2; j<(thickness+1)/2; j++){
            if (y+i < 0 || x+j < 0 || (unsigned int)(y+i) >= H || (unsigned int)(x+j) >= W){
                continue;
            }
            (*bmp_file_pixels)[y+i][x+j].r = r;
            (*bmp_file_pixels)[y+i][x+j].g = g;
            (*bmp_file_pixels)[y+i][x+j].b = b;
        }
    }
}

void fill(Rgb ***bmp_file_pixels, BitmapInfoHeader bmif, unsigned char r, unsigned char g, unsigned char b,
        int left_x, int up_y, int right_x, int down_y){
    for (int y = up_y; y<down_y; y++){
        for (int x = left_x; x<right_x; x++){
            set_pixel(bmp_file_pixels, bmif.height, bmif.width, r, g, b, x, y, 1);
        }
    }
}

void swap_points(int *x0, int *y0, int *x1, int *y1){
    int c = *x0;
    *x0 = *x1;
    *x1 = c;
    c = *y0;
    *y0 = *y1;
    *y1 = c;
}

void draw_line(Rgb ***bmp_file_pixels, BitmapInfoHeader bmif, unsigned char r, unsigned char g, unsigned char b, 
        int x0, int y0, int x1, int y1, int thickness){
    //y = (x-x0)*(y1-y0)/(x1-x0)+y0
    int deltax = abs(x1 - x0);
    int deltay = abs(y1 - y0);
    float error = 0;
    if (deltax>=deltay){
        if (x0 > x1){
            swap_points(&x0, &y0, &x1, &y1);
            x0 += 1;
            x1 += 1;
            if (y1 - y0 > 0){
                y0 += 1;
                y1 += 1;
            }
            else if (y1 - y0 < 0){
                y0 -= 1;
                y1 -= 1;
            }
        }
        int deltaerr = (deltay + 1);
        int y = y0;
        int diry = y1 - y0;
        if (diry > 0)
            diry = 1;
        if (diry < 0)
            diry = -1;
        for (int x = x0; x < x1; x++){
            set_pixel(bmp_file_pixels, bmif.height, bmif.width, r, g, b, x, y, thickness);
            error = error + deltaerr;
            if (error >= (deltax + 1)){
                y = y + diry;
                error = error - (deltax + 1);
            }
        }
    }
    else{
        if (y0 > y1){
            swap_points(&x0, &y0, &x1, &y1);
            y0 += 1;
            y1 += 1;
            if (x1 - x0 > 0){
                x0 += 1;
                x1 += 1;
            }
            else if (x1 - x0 < 0){
                x0 -= 1;
                x1 -= 1;
            }
        }
        int deltaerr = (deltax + 1);
        int x = x0;
        int dirx = x1 - x0;
        if (dirx > 0)
            dirx = 1;
        if (dirx < 0)
            dirx = -1;
        for (int y = y0; y < y1; y++){
            set_pixel(bmp_file_pixels, bmif.height, bmif.width, r, g, b, x, y, thickness);
            error = error + deltaerr;
            if (error >= (deltay + 1)){
                x = x + dirx;
                error = error - (deltay + 1);
            }
        }
    }
}

void squared_lines(Rgb ***bmp_file_pixels, BitmapInfoHeader bmif, unsigned char r, unsigned char g, unsigned char b, 
        int x0, int y0, int side, int thickness){
    draw_line(bmp_file_pixels, bmif, r, g, b, x0, y0, x0+side, y0+side, thickness);
    draw_line(bmp_file_pixels, bmif, r, g, b, x0, y0, x0+side, y0, thickness);
    draw_line(bmp_file_pixels, bmif, r, g, b, x0, y0, x0, y0+side, thickness);
    draw_line(bmp_file_pixels, bmif, r, g, b, x0, y0+side-1, x0+side, y0-1, thickness);
    draw_line(bmp_file_pixels, bmif, r, g, b, x0+side-1, y0, x0+side-1, y0+side, thickness);
    draw_line(bmp_file_pixels, bmif, r, g, b, x0, y0+side-1, x0+side, y0+side-1, thickness);
}

void rgbfilter(Rgb ***bmp_file_pixels, BitmapInfoHeader bmif, char component_name, int component_value){
    for (unsigned int y = 0; y<bmif.height; y++){
        for (unsigned int x = 0; x<bmif.width; x++){
            switch (component_name)
            {
                case 'r':
                {
                    (*bmp_file_pixels)[y][x].r = component_value;
                    break;
                }
                case 'g':
                {
                    (*bmp_file_pixels)[y][x].g = component_value;
                    break;
                }
                case 'b':
                {
                    (*bmp_file_pixels)[y][x].b = component_value;
                    break;
                }
            }
        }
    }
}

void angle(Rgb ***bmp_file_pixels, Rgb ***original_bf_pixels, BitmapInfoHeader bmif, int left_x, int up_y, 
        int right_x, int down_y, int angle){
    int tmp;
    if (right_x < left_x){
        tmp = right_x;
        right_x = left_x;
        left_x = tmp;
    }
    if (down_y < up_y){
        tmp = down_y;
        down_y = up_y;
        up_y = tmp;
    }

    //Проверка на выход за границы при взятии пикселей с оригинала
    if (up_y < 0){
        up_y = 0;
    }
    if ((unsigned int)(down_y) >= bmif.height){
        down_y = bmif.height-1;
    }
    if (left_x < 0){
        left_x = 0;
    }
    if ((unsigned int)(right_x) >= bmif.width){
        right_x = bmif.width-1;
    }

    int r;
    int g;
    int b;
    int shift;

    switch (angle)
    {
    case 90:
    {
        shift = (down_y - up_y - (right_x - left_x)) / 2;
        for (int x = right_x; x > left_x; x--){
            for (int y = up_y; y < down_y; y++){
                r = (*original_bf_pixels)[bmif.height-y-1][x-1].r;
                g = (*original_bf_pixels)[bmif.height-y-1][x-1].g;
                b = (*original_bf_pixels)[bmif.height-y-1][x-1].b;
                set_pixel(bmp_file_pixels, bmif.height, bmif.width, r, g, b, left_x+y-shift-up_y, up_y+right_x-x+shift, 1);
            }
        }
        break;
    }
    case 180:
    {
        for (int y = down_y; y > up_y; y--){
            for (int x = right_x; x > left_x; x--){
                r = (*original_bf_pixels)[bmif.height-y][x-1].r;
                g = (*original_bf_pixels)[bmif.height-y][x-1].g;
                b = (*original_bf_pixels)[bmif.height-y][x-1].b;
                set_pixel(bmp_file_pixels, bmif.height, bmif.width, r, g, b, left_x+right_x-x, up_y+down_y-y, 1);
            }
        }
        break;
    }
    case 270:
    {
        shift = (down_y - up_y - (right_x - left_x)) / 2;
        for (int x = left_x; x < right_x; x++){
            for (int y = down_y; y > up_y; y--){
                r = (*original_bf_pixels)[bmif.height-y][x].r;
                g = (*original_bf_pixels)[bmif.height-y][x].g;
                b = (*original_bf_pixels)[bmif.height-y][x].b;
                set_pixel(bmp_file_pixels, bmif.height, bmif.width, r, g, b, left_x+down_y-y-shift, up_y+x+shift-left_x, 1);
            }
        }
        break;
    }
    }
}


int main(int argc, char **argv){
    Argf argf;
    if (processopts(argc, argv, &argf)){
        return 0;
    }
    BitmapFileHeader bmfh;
    BitmapInfoHeader bmif;
    Rgb **bmp_file_pixels = read_bmp(argf.input, &bmfh, &bmif);
    if (argf.function == 'b'){
        puts("Course work for option 4.12, created by Nikita Kozmin");
        print_file_header(bmfh);
        print_info_header(bmif);
        return 0;
    }
    else if (argf.function == 'q'){
        if (argf.is_fill){
            fill(&bmp_file_pixels, bmif, argf.fill_color_r, argf.fill_color_g, argf.fill_color_b, 
                argf.left_up_x, argf.left_up_y, argf.left_up_x+argf.side_size, argf.left_up_y+argf.side_size);
        }
        squared_lines(&bmp_file_pixels, bmif, argf.color_r, argf.color_g, argf.color_b,
            argf.left_up_x, argf.left_up_y, argf.side_size, argf.thickness);
    }
    else if (argf.function == 'r'){
        rgbfilter(&bmp_file_pixels, bmif, argf.component_name, argf.component_value);
    }
    else if (argf.function == 't'){
        Rgb **original_bf_pixels = read_bmp(argf.input, &bmfh, &bmif);
        angle(&bmp_file_pixels, &original_bf_pixels, bmif, argf.left_up_x,
                argf.left_up_y, argf.right_down_x, argf.right_down_y, argf.angle);
    }
    write_bmp(argf.output, bmp_file_pixels, bmif.height, bmif.width, bmfh, bmif);
    puts("Course work for option 4.12, created by Nikita Kozmin");
    return 0;
}

#ifndef _INC_PROCESSOPTS
#define _INC_PROCESSOPTS
#endif

typedef struct Argf{
    char *input;
    char *output;
    char function;
    int left_up_x;
    int left_up_y;
    int side_size;
    int thickness;
    int color_r;
    int color_g;
    int color_b;
    char is_fill;
    int fill_color_r;
    int fill_color_g;
    int fill_color_b;
    char component_name;
    int component_value;
    int right_down_x;
    int right_down_y;
    int angle;
} Argf;

char processopts(int argc, char **argv, Argf *argf);

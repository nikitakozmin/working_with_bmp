#include <getopt.h>
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

extern char *optarg;
extern int optind;


void printhelp(){
    puts(
        "Course work for option 4.12, created by Nikita Kozmin\n"
        "--help -h            Display this information\n"
        "--input -i           Input file name (can be omitted; default value is last argument)\n"
        "--output -i          Output file name (can be omitted; default value is \"out.bmp\")\n"
        "--info               Image information\n"
        "--squared_lines      Drawing a square with diagonals\n"
        "   --left_up             Coordinates of the upper left corner (format: \"x.y\")\n"
        "   --side_size           Side size (number greater than zero)\n"
        "   --thicknes            Thickness of lines (number greater than zero)\n"
        "   --color               Line color (format: \"rrr.ggg.bbb\")\n"
        "   --fill                Fill flag (no arguments)\n"
        "   --fill_color          Fill color (format: \"rrr.ggg.bbb\")\n"
        "--rgbfilter          Filter rgb-component\n"
        "   --component_name      Changeable component (possible values: \"red\", \"green\" and \"blue\")\n"
        "   --component_value     New color value (0 to 255)\n"
        "--rotate             Rotate the image (part) by 90/180/270 degrees\n"
        "   --left_up             Coordinates of the upper left corner (format: \"x.y\")\n"
        "   --right_down          Coordinates of the lower right corner (format: \"x.y\")\n"
        "   --angle               Angle of rotation (possible values: \"90\", \"180\", \"270\")"
    );
}

char is_bmp(char *s){
    int len = strlen(s);
    if (len<5){
        return 0;
    }
    if (s[len-4] == '.'){
        return 1;
    }
    return 0;
}

char is_color(int c){
    if (0 <= c && c <= 255){
        return 1;
    }
    return 0;
}

char processopts(int argc, char **argv, Argf *argf){
    if (argc == 1){
        printhelp();
        return 1;
    }

    int c;
    static struct option long_options[] = {
        //name                  has_arg     flag        val
        {"help",                0,          0,          'h'},
        {"input",               1,          0,          'i'},
        {"output",              1,          0,          'o'},
        {"info",                0,          0,          'b'},
        {"squared_lines",       0,          0,          'q'},
        {"left_up",             1,          0,          'u'},
        {"side_size",           1,          0,          's'},
        {"thickness",           1,          0,          'k'},
        {"color",               1,          0,          'c'},
        {"fill",                0,          0,          'f'},
        {"fill_color",          1,          0,          'l'},
        {"rgbfilter",           0,          0,          'r'},
        {"component_name",      1,          0,          'n'},
        {"component_value",     1,          0,          'v'},
        {"rotate",              0,          0,          't'},
        {"right_down",          1,          0,          'd'},
        {"angle",               1,          0,          'a'},
        {0,                     0,          0,           0 }
    };

    char short_options[] = "hi:o:";
    char has_help = 0;
    char has_squared_lines = 0;
    char has_only_squared_lines_opts = 0;
    char has_input = 0;
    char has_output = 0;
    char has_left_up = 0;
    char has_side = 0;
    char has_thickness = 0;
    char has_color = 0;
    char has_fill = 0;
    char has_fill_color = 0;
    char has_rgbfilter = 0;
    char has_only_rgbfilter_opts = 0;
    char has_component_name = 0;
    char has_component_value = 0;
    char has_rotate = 0;
    char has_only_rotate_opts = 0;
    char has_right_down = 0;
    char has_angle = 0;
    char has_info = 0;

    argf->input = NULL;
    argf->output = NULL;
    argf->function = '\0';
    argf->left_up_x = 0;
    argf->left_up_y = 0;
    argf->side_size = 0;
    argf->thickness = 1;
    argf->color_r = 0;
    argf->color_g = 0;
    argf->color_b = 0;
    argf->is_fill = 0;
    argf->fill_color_r = 0;
    argf->fill_color_g = 0;
    argf->fill_color_b = 0;
    argf->component_name = '\0';
    argf->component_value = 0;
    argf->right_down_x = 0;
    argf->right_down_y = 0;
    argf->angle = 0;

    while (1)
    {
        int option_index = 0;
        c = getopt_long(argc, argv, short_options,
                long_options, &option_index);

        if (c == -1)
            break;
        switch (c){
        case 'h':
        {
            // Проверка на дублирование
            if (has_help){
                fprintf(stderr, "Invalid flag (duplicate). Error 46\n");
                exit(46);
            }
            has_help = 1;
            break;
        }
        case 'q':
        {
            // Проверка на дублирование
            if (has_squared_lines){
                fprintf(stderr, "Invalid flag (duplicate). Error 46\n");
                exit(46);
            }
            argf->function = 'q';
            has_squared_lines = 1;
            has_only_squared_lines_opts = 1;
            break;
        }
        case 'i':
        {
            // Проверка на дублирование
            if (has_input){
                fprintf(stderr, "Invalid flag (duplicate). Error 46\n");
                exit(46);
            }
            argf->input = malloc(sizeof(char)*strlen(optarg));
            strcpy(argf->input, optarg);
            if (!is_bmp(argf->input)){
                fprintf(stderr, "Invalid argument (the file isn't bmp). Error 45\n");
                exit(45);
            }
            has_input = 1;
            break;
        }
        case 'o':
        {
            // Проверка на дублирование
            if (has_output){
                fprintf(stderr, "Invalid flag (duplicate). Error 46\n");
                exit(46);
            }
            argf->output = malloc(sizeof(char)*strlen(optarg));
            strcpy(argf->output, optarg);
            if (!is_bmp(argf->output)){
                fprintf(stderr, "Invalid argument (the file isn't bmp). Error 45\n");
                exit(45);
            }
            has_output = 1;
            break;
        }
        case 'u':
        {
            // Проверка на дублирование
            if (has_left_up){
                fprintf(stderr, "Invalid flag (duplicate). Error 46\n");
                exit(46);
            }
            if (sscanf(optarg, "%d.%d", &argf->left_up_x, &argf->left_up_y) != 2){
                fprintf(stderr, "Invalid argument (no coordinates). Error 45\n");
                exit(45);
            }
            has_left_up = 1;
            break;
        }
        case 's':
        {
            // Проверка на дублирование
            if (has_side){
                fprintf(stderr, "Invalid flag (duplicate). Error 46\n");
                exit(46);
            }
            if (sscanf(optarg, "%d", &argf->side_size) != 1 || argf->side_size <= 0){
                fprintf(stderr, "Invalid argument (no side). Error 45\n");
                exit(45);
            }
            has_side = 1;
            has_only_squared_lines_opts = 1;
            break;
        }
        case 'k':
        {
            // Проверка на дублирование
            if (has_thickness){
                fprintf(stderr, "Invalid flag (duplicate). Error 46\n");
                exit(46);
            }
            if (sscanf(optarg, "%d", &argf->thickness) != 1 || argf->thickness <= 0){
                fprintf(stderr, "Invalid argument (no thickness). Error 45\n");
                exit(45);
            }
            has_thickness = 1;
            has_only_squared_lines_opts = 1;
            break;
        }
        case 'c':
        {
            // Проверка на дублирование
            if (has_color){
                fprintf(stderr, "Invalid flag (duplicate). Error 46\n");
                exit(46);
            }
            if (sscanf(optarg, "%d.%d.%d", &argf->color_r, &argf->color_g, &argf->color_b) != 3){
                fprintf(stderr, "Invalid argument (no color). Error 45\n");
                exit(45);
            }
            if (!is_color(argf->color_r) || !is_color(argf->color_g) || !is_color(argf->color_b)){
                fprintf(stderr, "Invalid color. Error 41\n");
                exit(41);
            }
            has_color = 1;
            has_only_squared_lines_opts = 1;
            break;
        }
        case 'f':
        {
            // Проверка на дублирование
            if (has_fill){
                fprintf(stderr, "Invalid flag (duplicate). Error 46\n");
                exit(46);
            }
            argf->is_fill = 1;
            has_fill = 1;
            has_only_squared_lines_opts = 1;
            break;
        }
        case 'l':
        {
            // Проверка на дублирование
            if (has_fill_color){
                fprintf(stderr, "Invalid flag (duplicate). Error 46\n");
                exit(46);
            }
            if (sscanf(optarg, "%d.%d.%d", &argf->fill_color_r, &argf->fill_color_g, &argf->fill_color_b) != 3){
                fprintf(stderr, "Invalid argument (no color). Error 45\n");
                exit(45);
            }
            if (!is_color(argf->fill_color_r) || !is_color(argf->fill_color_g) || !is_color(argf->fill_color_b)){
                fprintf(stderr, "Invalid color. Error 41\n");
                exit(41);
            }
            has_fill_color = 1;
            has_only_squared_lines_opts = 1;
            break;
        }
        case 'r':
        {
            // Проверка на дублирование
            if (has_rgbfilter){
                fprintf(stderr, "Invalid flag (duplicate). Error 46\n");
                exit(46);
            }
            argf->function = 'r';
            has_rgbfilter = 1; 
            has_only_rgbfilter_opts = 1;
            break;
        }
        case 'n':
        {
            // Проверка на дублирование
            if (has_component_name){
                fprintf(stderr, "Invalid flag (duplicate). Error 46\n");
                exit(46);
            }
            if (strcmp("red", optarg) == 0){
                argf->component_name = 'r';
            }
            else if (strcmp("green", optarg) == 0){
                argf->component_name = 'g';
            } 
            else if (strcmp("blue", optarg) == 0){
                argf->component_name = 'b';
            }
            else{
                fprintf(stderr, "Invalid argument (no name color). Error 45\n");
                exit(45);
            }
            has_component_name = 1;
            has_only_rgbfilter_opts = 1;
            break;
        }
        case 'v':
        {
            // Проверка на дублирование
            if (has_component_value){
                fprintf(stderr, "Invalid flag (duplicate). Error 46\n");
                exit(46);
            }
            if (sscanf(optarg, "%d", &argf->component_value) != 1){
                fprintf(stderr, "Invalid argument (no color). Error 45\n");
                exit(45);
            }
            if (!is_color(argf->component_value)){
                fprintf(stderr, "Invalid color. Error 41\n");
                exit(41);
            }
            has_component_value = 1;
            has_only_rgbfilter_opts = 1;
            break;
        }
        case 't':
        {
            // Проверка на дублирование
            if (has_rotate){
                fprintf(stderr, "Invalid flag (duplicate). Error 46\n");
                exit(46);
            }
            argf->function = 't';
            has_rotate = 1; 
            has_only_rotate_opts = 1;
            break;
        }
        case 'd':
        {
            // Проверка на дублирование
            if (has_right_down){
                fprintf(stderr, "Invalid flag (duplicate). Error 46\n");
                exit(46);
            }
            if (sscanf(optarg, "%d.%d", &argf->right_down_x, &argf->right_down_y) != 2){
                fprintf(stderr, "Invalid argument (no coordinates). Error 45\n");
                exit(45);
            }
            has_right_down = 1;
            has_only_rotate_opts = 1;
            break;
        }
        case 'a':
        {
            // Проверка на дублирование
            if (has_angle){
                fprintf(stderr, "Invalid flag (duplicate). Error 46\n");
                exit(46);
            }
            if (sscanf(optarg, "%d", &argf->angle) != 1){
                fprintf(stderr, "Invalid argument (no angle). Error 45\n");
                exit(45);
            }
            if ((argf->angle != 90) && (argf->angle != 180) && (argf->angle != 270)){
                fprintf(stderr, "Invalid argument (no angle). Error 45\n");
                exit(45);
            }
            has_angle = 1;
            has_only_rotate_opts = 1;
            break;
        }
        case 'b':
        {
            // Проверка на дублирование
            if (has_info){
                fprintf(stderr, "Invalid flag (duplicate). Error 46\n");
                exit(46);
            }
            argf->function = 'b';
            has_info = 1;
            break;
        }
        case '?':
        {
            fprintf(stderr, "Invalid or missing flag or argument. Error 46 or 45\n");
            exit(46);
        }
        }
    }

    if (has_help && (has_only_squared_lines_opts || has_only_rgbfilter_opts || has_only_rotate_opts || has_input || has_output || has_info || has_left_up)){
        fprintf(stderr, "Invalid flag. Error 46\n");
        exit(46);
    }

    if (has_info && (has_only_squared_lines_opts || has_only_rgbfilter_opts || has_only_rotate_opts || has_output || has_left_up)){
        fprintf(stderr, "Invalid flag. Error 46\n");
        exit(46);
    }

    if ((has_only_squared_lines_opts || has_left_up) && has_only_rgbfilter_opts){
        fprintf(stderr, "Invalid flag. Error 46\n");
        exit(46);  
    }

    if (has_only_squared_lines_opts && has_only_rotate_opts){
        fprintf(stderr, "Invalid flag. Error 46\n");
        exit(46);  
    }

    if (has_only_rgbfilter_opts && (has_only_rotate_opts || has_left_up)){
        fprintf(stderr, "Invalid flag. Error 46\n");
        exit(46);
    }

    // Проход по аргументам, не являющимися опциями (или их аргументами)
    int has_ignored_argv = 0;
    for (int i = optind; i < argc; i++){
        if (!has_ignored_argv && (has_help || has_input)){
            // Были введены аргументы для флага, который не принимает аргументов или введены лишние аргументы
            fprintf(stderr, "An extra argument encountered. Error 44\n");
            has_ignored_argv = 1;
        }
        
        if (has_ignored_argv){
            fprintf(stderr, "Ignored: %s\n", argv[i]);
        }

        if (!has_input && !has_ignored_argv){
            argf->input = malloc(sizeof(char)*strlen(argv[i]));
            strcpy(argf->input, argv[i]);
            has_input = 1;
        }
    }

    if (has_help && !has_ignored_argv){
        printhelp();
        return 1;
    }
    if (has_ignored_argv){
        exit(44);
    }
    if (!has_input){
        fprintf(stderr, "No input. Error 46\n");
        exit(46);
    }
    if (!is_bmp(argf->input)){
        fprintf(stderr, "Invalid argument (the file isn't bmp). Error 45\n");
        exit(45);
    }
    if (argf->function == 'b'){
        return 0;
    }
    if (!argf->function){
        fprintf(stderr, "Missing flag. Error 46\n");
        exit(46);
    }
    if (!has_output){
        argf->output = malloc(sizeof(char)*8);
        strcpy(argf->output, "out.bmp");
    }
    if (strcmp(argf->input, argf->output) == 0){
        fprintf(stderr, "Identical files. Error 47\n");
        exit(47);
    }
    
    if (argf->is_fill && !has_fill_color){
        fprintf(stderr, "Missing flag. Error 46\n");
        exit(46);
    }
    
    if ((argf->function == 'r') && (!has_component_name || !has_component_value)){
        fprintf(stderr, "Missing flag. Error 46\n");
        exit(46);
    }
    
    if ((argf->function == 't') && !has_angle){
        fprintf(stderr, "Missing flag. Error 46\n");
        exit(46);
    }
    return 0;
}

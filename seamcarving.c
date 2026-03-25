#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "seamcarving.h"
#include "c_img.c"

void calc_energy(struct rgb_img *im, struct rgb_img **grad){
    int h = (*im).height;
    int w = (*im).width;
    create_img(grad, h, w);
    for (int x = 0; x < w; x++){
        for (int y = 0; y < h; y++){
            int x_left = (x + w - 1) % w;
            int x_right = (x + 1) % w;
            int y_top = (y + h - 1) % h;
            int y_bot = (y + 1) % h;
            // get all the rgb values @ x and y
            int rx = get_pixel(im, y, x_right, 0) - get_pixel(im, y, x_left, 0);
            int gx = get_pixel(im, y, x_right, 1) - get_pixel(im, y, x_left, 1);
            int bx = get_pixel(im, y, x_right, 2) - get_pixel(im, y, x_left, 2);
            int ry = get_pixel(im, y_bot, x, 0) - get_pixel(im, y_top, x, 0);
            int gy = get_pixel(im, y_bot, x, 1) - get_pixel(im, y_top, x, 1);
            int by = get_pixel(im, y_bot, x, 2) - get_pixel(im, y_top, x, 2);
            double E_pixel = pow((rx*rx + gx*gx + bx*bx + ry*ry + gy*gy + by*by), 0.5);
            uint8_t E_pixel_stored = (uint8_t)(E_pixel / 10);
            set_pixel(*grad, y, x, E_pixel_stored, E_pixel_stored, E_pixel_stored);
        }
    }
}

void dynamic_seam(struct rgb_img *grad, double **best_arr){
    int h = (*grad).height;
    int w = (*grad).width;
    *best_arr = malloc(h * w * sizeof(double));
    for (int x = 0; x < w; x++){ // initialize every row first
        (*best_arr)[x] = get_pixel(grad, 0, x, 0);
    }
    for (int y = 1; y < h; y++){ // fill in the columns that were made by the rows
        for (int x = 0; x < w; x++){
            double min_cost = (*best_arr)[(y - 1)*w + x];
            if (x > 0 && (*best_arr)[(y - 1)*w + x - 1] < min_cost){
                min_cost = (*best_arr)[(y - 1)*w + x - 1];
            }
            if (x < w - 1 && (*best_arr)[(y - 1)*w + x + 1] < min_cost){
                min_cost = (*best_arr)[(y - 1)*w + x + 1];
            }
            double grad_cost = get_pixel(grad, y, x, 0);
            (*best_arr)[y * w + x] = min_cost + grad_cost;
        }
    }
}

void recover_path(double *best, int height, int width, int **path){
    *path = malloc(height * sizeof(int));
    int min_seam_x = 0;
    double min_val = best[(height - 1) * width];
    for (int x = 1; x < width; x++){
        double val = best[(height - 1) * width + x];
        if (val < min_val){
            min_val = val;
            min_seam_x = x;
        }
    }
    (*path)[height - 1] = min_seam_x;
    for (int y = height - 2; y >= 0; y--){
        int prev_x = (*path)[y + 1];
        int best_x = prev_x;
        double best_val = best[y * width + prev_x];
        if (prev_x > 0){
            double left_val = best[y * width + prev_x - 1];
            if (left_val < best_val){
                best_val = left_val; // save the best left values through minimization
                best_x = prev_x - 1;
            }
        }
        if (prev_x < width - 1){
            double right_val = best[y * width + prev_x + 1];
            if (right_val < best_val){
                best_val = right_val; // save the best right values through minimization
                best_x = prev_x + 1;
            }
        }
        (*path)[y] = best_x; // store the best col for the current row
    }
}

void remove_seam(struct rgb_img *src, struct rgb_img **dest, int *path){
    int h = (*src).height;
    int w = (*src).width;
    create_img(dest, h, w - 1);
    for (int y = 0; y < h; y++){
        int create_x = 0;
        for (int x = 0; x < w; x++){
            if (x == path[y]){
                continue;
            }
            uint8_t r = get_pixel(src, y, x, 0);
            uint8_t g = get_pixel(src, y, x, 1);
            uint8_t b = get_pixel(src, y, x, 2);
            set_pixel(*dest, y, create_x, r, g, b);
            create_x++;
        }
    }
}

int main(){
    struct rgb_img *im;
    struct rgb_img *cur_im;
    struct rgb_img *grad;
    double *best;
    int *path;
    read_in_img(&im, "HJoceanSmall.bin");
    for(int i = 0; i < 5; i++){
        printf("i = %d\n", i);
        calc_energy(im,  &grad);
        dynamic_seam(grad, &best);
        recover_path(best, grad->height, grad->width, &path);
        remove_seam(im, &cur_im, path);
        char filename[200];
        sprintf(filename, "img%d.bin", i);
        write_img(cur_im, filename);
        destroy_image(im);
        destroy_image(grad);
        free(best);
        free(path);
        im = cur_im;
    }
    destroy_image(im);
}
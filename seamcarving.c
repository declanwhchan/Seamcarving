#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "seamcarving.h"

void calc_energy(struct rgb_img *im, struct rgb_img **grad){
    int h = (*im).height;
    int w = (*im).width;
    create_img(grad, h, w);
    for (int x = 0; x < w; x++){
        for (int y = 0; y < h; y++){
            int rx, ry, gx, gy, bx, by;
        }
    }
}

void dynamic_seam(struct rgb_img *grad, double **best_arr){

}

void recover_path(double *best, int height, int width, int **path){

}

void remove_seam(struct rgb_img *src, struct rgb_img **dest, int *path){

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
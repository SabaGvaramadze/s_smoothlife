#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "DFT.h"
#include "smoothlife.h"




int main(){
    smoothlife world = {
        .ri = 7,
        .ra = 21,//usually 3*ri
        .width = 256,
        .height = 64,
        .b1 = 0.278,
        .b2 = 0.365,
        .d1 = 0.267,
        .d2 = 0.445,
        .alpha = 0.028
    };

    world.grid = (double*)malloc(sizeof(double) * world.width * world.height);
    memset(world.grid,0,sizeof(double) * world.width * world.height);

    world.grid_ft = (complex*)malloc(sizeof(complex) * world.width * world.height);
    memset(world.grid_ft,0,sizeof(complex) * world.width * world.height);

    world.grid_fti = (complex*)malloc(sizeof(complex) * world.width * world.height);
    memset(world.grid_fti,0,sizeof(complex) * world.width * world.height);

    world.kernel_ft = (complex*)malloc(sizeof(complex) * world.width * world.height);
    memset(world.kernel_ft,0,sizeof(complex) * world.width * world.height);

    generate_kernel(world);
    srand(time(NULL));
    
    int sqr = 16;

    double center_x = world.width / 2.0;
    double center_y = world.height / 2.0;
    double radius = 20.0; // Size of the initial blob

    for(int i = 0; i < world.height; i++){
        for(int j = 0; j < world.width; j++){
            double dx = j - center_x;
            double dy = i - center_y;
            double r = sqrt(dx * dx + dy * dy);
            
            if(r < radius) {
                // Fill the circle with noisy values centered around 0.5
                world.grid[i * world.width + j] = 0.4 + 0.6 * ((double)rand() / RAND_MAX);
            }
        }
    }
    char* chars = ".,\'\"18{#$@";
    while(1){
        for(int i=0;i<world.height;i++){
            for(int j=0;j<world.width;j++){
                int index = (int)(world.grid[i*world.width+j]*strlen(chars))-1;
                if(index < 0)index = 0;
                printf("%c", chars[index]);
            }
            printf("\n");
        }
        printf("\n");
        step(world);
        struct timespec ts = { .tv_sec = 0, .tv_nsec = 250000000}; // 1 second
        nanosleep(&ts, NULL);
    }
}



#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "DFT.h"
#include "smoothlife.h"




void seed_grid(double *grid, int width, int height);

int main(){
    smoothlife world = {
        .ri = 7,
        .ra = 23,
        .width = 512,
        .height = 256,
        .b1 = 0.14,
        .b2 = 0.44,
        .d1 = 0.13,
        .d2 = 0.44,
        .alpha = 0.13
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

    /*
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

    center_x = world.width / 2.5;
    center_y = world.height / 2.5;
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
    */
    seed_grid(world.grid,world.width,world.height);
    char* chars = " .,\'\"198P{%#$@";

    int iteration = 0;
    while(1){
        for(int i=0;i<world.height;i++){
            for(int j=0;j<world.width;j++){
                int index = (int)(world.grid[i*world.width+j]*strlen(chars))-1;
                if(index < 0)index = 0;
                //printf("%c",chars[index]);
                printf("\033[%d;%d;%dm%c", (int)((double)index/strlen(chars)*7)+41,5,206,chars[index]);
            }
            printf("\n");
        }
        printf("\033[H");

        step(world);
        struct timespec ts = { .tv_sec = 0, .tv_nsec = 50000000}; // 1 second
        nanosleep(&ts, NULL);
    }
}


void seed_grid(double *grid, int width, int height)
{

    for (int i = 0; i < width * height; i++)
        grid[i] = 0.005;

    for (int b = 0; b < 12; b++) {
        double x = 20 + (double)rand() / RAND_MAX * (width - 40);
        double y = 10 + (double)rand() / RAND_MAX * (height - 20);

        double angle = ((double)rand() / RAND_MAX - 0.5) * 0.9;
        double length = 45 + (double)rand() / RAND_MAX * 75;
        double thickness = 2.5 + (double)rand() / RAND_MAX * 1.5;

        double phase = (double)rand() / RAND_MAX * 2.0 * PI;
        double frequency = 0.025 + (double)rand() / RAND_MAX * 0.025;

        for (int t = 0; t < (int)length; t++) {
            angle += sin(t * frequency + phase) * 0.08;

            x += cos(angle);
            y += sin(angle);

            for (int oy = -4; oy <= 4; oy++) {
                for (int ox = -4; ox <= 4; ox++) {
                    int px = (int)x + ox;
                    int py = (int)y + oy;

                    if (px < 0 || px >= width || py < 0 || py >= height)
                        continue;

                    double d = sqrt((double)(ox * ox + oy * oy));

                    if (d <= thickness) {
                        double v = 0.85 * exp(-d * d / (2.0 * thickness * thickness));
                        int i = py * width + px;

                        if (v > grid[i])
                            grid[i] = v;
                    }
                }
            }
        }
    }
}

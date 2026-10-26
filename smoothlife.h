#ifndef SMOOTHLIFE_H
#define SMOOTHLIFE_H

#include <complex.h>
#include <string.h>
#include <math.h>
#include "DFT.h"


typedef struct {
    double* grid;
    complex* grid_ft;// used for intermediate step
    complex* grid_fti;// used for intermediate step
    complex* kernel_ft;
    size_t width;
    size_t height;
    double ri;
    double ra;
    double b1,b2;
    double d1,d2;
    double alpha;
} smoothlife;

static void generate_kernel(smoothlife world);
static void step(smoothlife world);
static double snm(smoothlife world,double n, double m);

static void step(smoothlife world){
    grid_FFT(world.grid,world.grid_ft,world.width,world.height);
    for(int i=0;i<world.height;i++){
        for(int j=0;j<world.width;j++){
            world.grid_ft[i*world.width+j] *= world.kernel_ft[i*world.width+j];
        }
    }
    grid_FFTI(world.grid_ft,world.grid_fti,world.width,world.height);
    for(int i=0;i<world.height;i++){
        for(int j=0;j<world.width;j++){
            world.grid[i*world.width+j] = snm(world,creal(world.grid_fti[i*world.width+j]) / (world.width * world.height),world.grid[i*world.width+j]);
        }
    }
}

static void generate_kernel_root(double* kernel,size_t width,size_t height,double ri, double ra){
    double in_area = ri*ri*PI;
    double out_area = ra*ra*PI-in_area;

    for(int i=0;i<height;i++){
        for(int j = 0; j < width; j++){
            int dx,dy;

            if(j > width/2) dx = width-j;
            else dx = j;

            if(i > height/2) dy = height-i;
            else dy = i;

            double value=0;
            double r = sqrt(dx*dx + dy*dy);
            if(r <= ri){
                value = 1.00/in_area;
            }
            else if(r <= ra){
                value = -1.00/out_area;
            }
            kernel[i*width+j] = value;
        }
    }
}

static void generate_kernel(smoothlife world){
    double* kernel_root = (double*)malloc(sizeof(double)*world.width*world.height);
    generate_kernel_root(kernel_root,world.width,world.height,world.ri,world.ra);
    grid_FFT(kernel_root,world.kernel_ft,world.width,world.height);
    free(kernel_root);
}



static double a1(smoothlife world,double density,double alpha){
    return 1/(1+exp(-(density-alpha)*4/world.alpha));
}

static double a2(smoothlife world,double density,double alpha,double beta){
    return a1(world,density,alpha)*(1-a1(world,density,beta));
}

static double am(smoothlife world,double x,double y,double m){
    return x*(1-a1(world,m,0.5))+y*a1(world,m,0.5);
}

static double snm(smoothlife world,double n, double m){
    return a2(world,n,am(world,world.b1,world.d1,m),am(world,world.b2,world.d2,m));
}

#endif

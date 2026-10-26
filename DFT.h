#ifndef DFT_H
#define DFT_H
#include <math.h>
#include <complex.h>

#define PI 3.14159265358979323846

void DFT(double const* data,complex* out,size_t len){
    for(int i=0;i<len;i++){
        out[i] = 0;
        for(int j=0;j<len;j++){
            out[i] += (float)data[j] * cexp(-I*2*PI/len*(double)i*j);
        }
    }
}

void complex_FFT(complex* data,complex* out,size_t len,size_t stride){
    if(len == 1){
        *out = *data;
        return;
    }
    else{
        complex_FFT(data,out,len/2,stride*2);
        complex_FFT(data+stride,out + len/2,len/2,stride*2);
        for(int i=0;i<len/2;i++){
            complex val = out[i+ len/2]*cexp(-I*2*PI*i/len);
            complex ev = out[i];
            out[i] = ev + val;
            out[i+len/2] = ev - val;
        }
    }
}

void FFT(double* data,complex* out,size_t len,size_t stride){
    if(len == 1){
        *out = *data;
        return;
    }
    else{
        FFT(data,out,len/2,stride*2);
        FFT(data+stride,out + len/2,len/2,stride*2);
        for(int i=0;i<len/2;i++){
            complex val = out[i+ len/2]*cexp(-I*2*PI*i/len);
            complex ev = out[i];
            out[i] = ev + val;
            out[i+len/2] = ev - val;
        }
    }
}

void grid_FFT(double* data,complex* out,size_t width,size_t height){
    
    for(int i=0;i<height;i++){
        FFT(data+i*width,out+i*width,width,1);
    }

    complex* vertical_in = (complex*)malloc(sizeof(complex)*height);
    complex* vertical_out = (complex*)malloc(sizeof(complex)*height);

    for(int i=0;i<width;i++){
        for(int j=0;j<height;j++){
            vertical_in[j] = out[j*width+i];
            vertical_out[j] = 0 + I*0;
        }
        complex_FFT(vertical_in,vertical_out,height,1);
        for(int j=0;j<height;j++){
            out[j*width + i] = vertical_out[j];
        }
    }
    free(vertical_in);
    free(vertical_out);
}

void FFTI(complex* data,complex* out,size_t len,size_t stride){
    if(len == 1){
        *out = *data;
        return;
    }
    else{
        FFTI(data,out,len/2,stride*2);
        FFTI(data+stride,out + len/2,len/2,stride*2);
        for(int i=0;i<len/2;i++){
            complex val = out[i+ len/2]*cexp(I*2*PI*i/len);
            complex ev = out[i];
            out[i] = ev + val;
            out[i+len/2] = ev - val;
        }
    }
}

void grid_FFTI(complex* data,complex* out,size_t width,size_t height){
    
    for(int i=0;i<height;i++){
        FFTI(data+i*width,out+i*width,width,1);
    }

    complex* vertical_in = (complex*)malloc(sizeof(complex)*height);
    complex* vertical_out = (complex*)malloc(sizeof(complex)*height);

    for(int i=0;i<width;i++){
        for(int j=0;j<height;j++){
            vertical_in[j] = out[j*width+i];
            vertical_out[j] = 0 + I*0;
        }
        FFTI(vertical_in,vertical_out,height,1);
        for(int j=0;j<height;j++){
            out[j*width + i] = vertical_out[j];
        }
    }
    free(vertical_in);
    free(vertical_out);
}

#endif

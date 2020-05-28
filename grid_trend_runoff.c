#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "utility.h"


int main(int argc, char *argv[])
{
  FILE *file, *out;
  char filename[256];
  int i, j, k, y, x, c, z, dummy, sea_ID;
  int nx=720, ny=90;
  int num_yr, st_yr = 1979, fn_yr = 2016;
  int size = 0, st_num = 795; 
  int **mask;
  float cov, dec, trd, y_ave, x_ave;
  float *tmp, **tmp_fix, **sum;
  float *ave, *lat, *lon, cum;
  float st_lat, st_lon, *s_lat, *s_lon;
  int n_day[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};


  num_yr = fn_yr - st_yr + 1;
  x_ave = 19.5;

  /*-------------------------------------------------
   	allocate memory
  -------------------------------------------------*/

  mask = imatrix(ny, nx, "mask");
  tmp_fix = fmatrix(num_yr, ny*nx, "tmp_fix");
  tmp = fvector(ny*nx, "tmp");
  ave = fvector(ny*nx, "ave");
  s_lat = fvector(st_num, "s_lat");
  s_lon = fvector(st_num, "s_lon");
  lat = fvector(ny, "lat");
  lon = fvector(nx, "lon");

  /*-------------------------------------------------
   	read location information of river mouth
  -------------------------------------------------*/

  for (y = 0; y < ny; y++) {
    for (x = 0; x < nx; x++) {
      mask[y][x] = 0;
    }
  }

  file = fopen(argv[1], "r");

  while (!feof(file)) {
    fscanf(file, "%f %f %d %d", &st_lat, &st_lon, &sea_ID, &dummy);

    if (st_lon > 180.0)
      st_lon -= 360.0;

      y = (int)((89.75 - st_lat)/0.5);
      x = (int)((st_lon + 179.75)/0.5);

    if (sea_ID > 0) 
      mask[y][x] = 1;
  }
  fclose(file);

  /*-------------------------------------------------
   	read runoff
  -------------------------------------------------*/

  out = fopen(argv[3], "w");
 
  for (i = 0; i < num_yr; i++) {
    if (leap_year(i+st_yr)) 
      n_day[1] = 29;
    else 
      n_day[1] = 28;


    for (c = 0; c < ny*nx; c++)
      tmp_fix[i][c] = 0.0;
  
    for (j = 0, c = 0; j < 12; j++) {
      for (k = 0; k < n_day[j]; k++, c++) {
        sprintf (filename, "%srunoff_%d%02d%02d.bin", argv[2], i+st_yr, j+1, k+1);
        file = fopen (filename, "rb");
        size = fread (tmp, sizeof(float), ny*nx, file);
        fclose(file);
      
	for (y = 0, c = 0; y < ny; y++) {
          for (x = 0; x < nx; x++, c++) {
            if (tmp[c] > -1 && mask[y][x] == 1) 
              tmp_fix[i][c] += tmp[c];
          }
        }
      }
    }
  }

  for (y = 0, c = 0; y < ny; y++) {
    for (x = 0; x < nx; x++, c++) {
      trd = -9999.0;
      y_ave = -9999.0;

      st_lat = 89.75 - y*0.5;
      st_lon = -179.75 + x*0.5;

      if (mask[y][x] == 1) { 
    
        y_ave = 0.0;
        for (i = 0; i < num_yr; i++) 
          y_ave += tmp_fix[i][c];
        y_ave /= (float)num_yr;   
      
        cov  = 0.0;
        dec  = 0.0; 
        for (i = 0; i < num_yr; i++) {
          cov += ((float)(i+1) - x_ave)*(tmp_fix[i][c] - y_ave);
          dec += pow(((float)(i+1) - x_ave), 2);         
        }

        cov /= (float)num_yr;
        dec /= (float)num_yr;
        trd = cov/dec;
      }
   
      printf("result %f %f %f %f\n", st_lat, st_lon, trd, y_ave);
      fprintf(out, "%f\t%f\t%f\t%f\n", st_lat, st_lon, trd, y_ave);
    }
  }
  fclose(out);

}



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "utility.h"


int main(int argc, char *argv[])
{
 FILE *file, *file_o, *out;
 char filename[256];
 int i, j, k, l, c, y, x, dummy, f_day = 366;
 int year, month, day;
 int nx=720, ny=90;
 int num_yr, st_yr = 1979, fn_yr = 2016;
 int size = 0;
 int **mask;
 float *tmp, ***tmp_x, **tmp_fix, **tmp_fix_x;
 float dis, y_ave, x_ave, s_xy, obs, cal, s_x, s_y, r_xy;
 int n_day[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

 num_yr = fn_yr - st_yr + 1;

 /*---------------------------------------------
         allocate memory
 ---------------------------------------------*/

 mask = imatrix(ny, nx, "mask");
 tmp_fix = fmatrix(num_yr, f_day, "tmp_fix");
 tmp_fix_x = fmatrix(num_yr, f_day, "tmp_fix_x");
 tmp = fvector(ny*nx, "tmp");
 tmp_x = ftriple(num_yr, 12, 31, "tmp_x");

 /*---------------------------------------------
    read location information of river mouth
 ---------------------------------------------*/

  y = (int)((89.75 - 70.68)/0.5);
  x = (int)((127.39 + 179.75)/0.5);
  mask[y][x] = 1;
printf("%d %d %d\n", y, x, mask[y][x]);

 /*---------------------------------------------
       calculate correlation coefficient
 ---------------------------------------------*/
 
 out = fopen(argv[3], "w");

 for (i = 0; i < num_yr; i++){
   if (leap_year(i+st_yr)) {
     n_day[1] = 29;
     f_day = 366;
   } else {
     n_day[1] = 28;
     f_day = 365;
   }

   for (l = 0; l < f_day; l++)
     tmp_fix[i][l] = 0.0;

   for (j = 0, l = 0; j < 12; j++) {
     for (k = 0; k < n_day[j]; k++, l++) {
       sprintf (filename, "%s%d%02d%02d.dat", argv[1], i+st_yr, j+1, k+1);
       file = fopen (filename, "rb");
       size = fread (tmp, sizeof(float), ny*nx, file);
       fclose(file);

       for (y = 0, c = 0; y < ny; y++) {
         for (x = 0; x < nx; x++, c++) {
           if (tmp[c] >= 0 && mask[y][x] == 1) 
             tmp_fix[i][l] += tmp[c];
         }
       }
     }
   }
 }

 file_o = fopen(argv[2], "r");
 
 while (!feof(file_o)) {
   fscanf(file_o, "%d %d-%02d-%02d %f", &dummy, &year, &month, &day, &dis);
   if (year >= st_yr && year <= fn_yr)
     tmp_x[year-st_yr][month-1][day-1] = dis;
 }

 for (i = 0; i < num_yr; i++) {

   for(l = 0; l < f_day; l++)
     tmp_fix_x[i][l] = 0.0;

   for (j = 0, l = 0; j < 12; j++) {
     for (k = 0; k < n_day[j]; k++, l++) {
       if (tmp_x[i][j][k] >= 0)
         tmp_fix_x[i][l] += tmp_x[i][j][k];
     }
   }
 }

 for (l = 0; l < f_day; l++) {
 
   y_ave = 0.0;
   x_ave = 0.0;
   s_xy = 0.0;
   obs = 0.0;
   cal = 0.0;

   for (i = 0; i < num_yr; i++) {
     y_ave += tmp_fix[i][l];
     x_ave += tmp_fix_x[i][l];

     y_ave /= (float)num_yr;
     x_ave /= (float)num_yr;

     s_xy += (tmp_fix[i][l] - y_ave)*(tmp_fix_x[i][l] - x_ave);
     obs += powf(tmp_fix_x[i][l] - x_ave, 2.0);
     cal += powf(tmp_fix[i][l] - y_ave, 2.0);

     s_x = sqrt(obs);
     s_y = sqrt(cal);
     
     r_xy = s_xy/(s_x * s_y);
   }
   printf("result %d %f\n", l+1, r_xy);
   fprintf(out, "%d\t%f\n", l+1, r_xy);
 }
 fclose(out);
 fclose(file_o);
}

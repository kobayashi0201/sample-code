#!/bin/bash

rm -fr a.out
icc lena_kusur_daily_r.c -lm
./a.out ../output/discharge/ ../map/dis_obs_data/Lena_1978-2008.dat ../output/discharge_daily_trend/discharge_daily_r_lena_kusur.txt


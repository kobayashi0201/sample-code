#!/bin/bash

rm -fr a.out
icc grid_trend_runoff.c -lm
./a.out ../map/trip_sea_id_new.dat ../forcing_watch/runoff_calc/ ../output/runoff_grid_trend.txt



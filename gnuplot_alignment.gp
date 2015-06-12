#!/usr/bin/gnuplot
reset
set key off
set title "Alignment"


plot "string_v.dat"  using 2:3:1 with labels notitle,\
  "string_w.dat" using 2:3:1 with labels notitle
pause -1

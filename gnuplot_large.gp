#!/usr/local/bin/gnuplot
reset

set style line 1 lc rgb '#0060ad' lt 1 lw 2 pt 7 ps 0.75   # --- blue

plot 'temp_matrix.dat' matrix with image, 'temp_lines.dat' with linespoints ls 1
pause -1

#!/usr/bin/gnuplot
reset

set style line 1 lc rgb '#0060ad' lt 1 lw 2 pt 7 ps 1.5   # --- blue

plot 'temp_matrix.dat' matrix with image,\
	'' matrix using 1:2:(sprintf('%.2f', $3)) with labels font ',10',\
	'temp_lines.dat' with linespoints ls 1
pause -1

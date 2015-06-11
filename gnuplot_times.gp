#!/usr/bin/gnuplot
reset
set key off
set title "Time"
set xlabel "# of Threads"
set ylabel "Time in ms"

set style line 1 lc rgb '#0060ad' lt 1 lw 2 pt 7 ps 0.75   # --- blue
plot 'temp_times.dat' with linespoints ls 1 title "Times"
pause -1

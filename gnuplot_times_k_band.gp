#!/usr/bin/gnuplot
reset
set key off
set title "Time"
set xlabel "# of Threads"
set ylabel "Time in ms"

set style line 1 lc rgb '#0060ad' lt 1 lw 2 pt 7 ps 0.75   # --- blue
set style line 2 lc rgb '#60ad00' lt 1 lw 2 pt 7 ps 0.75   # --- red
plot 'temp_times.dat' with linespoints ls 1 title "Normal algorithm", 'temp_times_k_band.dat' with linespoints ls 2 title "k-band"
pause -1

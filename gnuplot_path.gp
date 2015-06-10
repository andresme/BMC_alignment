#!/usr/bin/gnuplot
reset
set style line 1 lc rgb '#0060ad' lt 1 lw 2 pt 7 ps 0.75   # --- blue
YTICS="`awk 'BEGIN{getline}{printf "%s ",$1}' temp_matrix.dat`"
XTICS="`head -1 temp_matrix.dat`"
set for [i=1:words(XTICS)] xtics ( word(XTICS,i) i-1 )
set for [i=1:words(YTICS)] ytics ( word(YTICS,i) i-1 )
plot "<awk '{$1=\"\"}1' temp_matrix.dat | sed '1 d'"  matrix w image, 'temp_lines.dat' with linespoints ls 1
set datafile separator
pause -1

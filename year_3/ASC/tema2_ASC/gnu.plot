set terminal jpeg medium
set output "performance.jpeg"
set boxwidth 0.75 absolute
set style fill solid 1.00 border -1
set style data histogram
set style histogram cluster gap 1
set grid ytics lc rgb "#aaaaaa" lw 1 lt 0
set xtics 1000 nomirror
set ytics 100 nomirror
set mxtics 2
set mytics 2
set ytics 1
set yrange [0:4]
set ylabel "Time"
set xlabel "35000 elements"

plot 'tdata' using 3 t "Basic", '' using 4 t "With DTRMV", '' using 5:xtic(1) t "Improved"

set terminal jpeg giant size 1200, 900
set output "performance.jpg"
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
set yrange [0:3]
set ylabel "Time"
set xlabel "Test times 1 to 20"

plot 'time_out.txt' using 3 t "CUDA no shared", '' using 4 t "CUDA shared", '' using 5:xtic(1) t "CPU"

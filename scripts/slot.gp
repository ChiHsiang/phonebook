reset
set ylabel 'slots number'
set style fill solid
set title 'HASH FUNCTION (MOD 3571)'
set term png enhanced font 'Vergana,10'
set output 'slot.png'
set format x "%10.0f"
set xtics rotate by 45 right

plot [0:3600][0:150]'slots.txt' using 1:2 with points title '' lc rgb "blue"

set xlabel 'Steps'
set ylabel 'Cumulative Average reward'

set terminal postscript eps color enhanced "Courier" 25
set colorsequence classic
set output "cum.eps"
plot "cum_eps0_1.txt" with linespoints pointtype 4 pointsize 0.1 lc rgb "blue" title 'eps=0.1',\
"cum_eps0_01.txt" with linespoints pointtype 6 pointsize 0.1 lc rgb 'red' title 'eps=0.01',\
"cum_eps0.txt" with linespoints pointtype 8 pointsize 0.1 lc rgb 'green' title 'eps=0'
set output

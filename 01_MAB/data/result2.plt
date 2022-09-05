set xlabel 'Steps'
set ylabel 'Average reward'

set terminal postscript eps color enhanced "Courier" 25
set colorsequence classic
set output "result.eps"
plot "eps0_1.txt" with linespoints pointtype 4 pointsize 0.1 lc rgb "blue" title 'eps=0.1',\
"ucb2.txt" with linespoints pointtype 6 pointsize 0.1 lc rgb 'red' title 'c=2'
set output

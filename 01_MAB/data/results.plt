set xlabel 'Steps'
set ylabel 'Average reward'

set terminal postscript eps color enhanced "Courier" 25
set colorsequence classic
set output "result.eps"
# plot "result.txt" with linespoints pointtype 2 pointsize 0.1 title 'result'
plot "eps0_1.txt" with linespoints pointtype 4 pointsize 0.1 lc rgb "blue" title 'eps=0.1',\
"eps0_01.txt" with linespoints pointtype 6 pointsize 0.1 lc rgb 'red' title 'eps=0.01',\
"eps0.txt" with linespoints pointtype 8 pointsize 0.1 lc rgb 'green' title 'eps=0'
set output

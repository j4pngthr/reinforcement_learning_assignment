set xlabel 'The Unit Time'
set ylabel 'Average rewards'
set key right bottom

set terminal postscript eps color enhanced "Courier" 25
set colorsequence classic
set output "reward.eps"
plot "ucb.txt" with linespoints pointtype 4 pointsize 2,\
  "proposed.txt" with linespoints pointtype 6 pointsize 2
set output

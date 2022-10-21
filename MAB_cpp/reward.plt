set xlabel 'The Unit Time'
set ylabel 'Average rewards'
set key right bottom

set terminal postscript eps color enhanced "Courier" 25
set colorsequence classic
set output "reward.eps"
plot "egreedy.txt" with linespoints pointtype 2 pointsize 2,\
  # "rewards0.txt" with linespoints pointtype 4 pointsize 2
set output

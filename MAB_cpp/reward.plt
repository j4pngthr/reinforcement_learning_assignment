set term postscript enhanced eps
set xlabel 'The Unit Time'
set ylabel 'Average rewards'
set key right bottom

set terminal postscript eps color enhanced "Courier" 25
set colorsequence classic
set output "reward.eps"
plot "ucb.txt" title "ucb(global)" with linespoints pointtype 2 pointsize 2,\
  "ucb2_0.txt" title "UCB(distributed), bias" with linespoints pointtype 4 pointsize 2,\
  "ucb2_1.txt" title "UCB(distributed), bias, excInfo" with linespoints pointtype 6 pointsize 2
  # "excInfo.txt" with linespoints pointtype 8 pointsize 2,\
set output

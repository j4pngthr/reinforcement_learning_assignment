set term postscript enhanced eps
set xlabel 'Unit time'
set ylabel 'Cumulative regrets'
set key left top
set xtics 500
# set yrange [0:120]
# set ytics 20

set terminal postscript eps color enhanced "Courier" 25
set colorsequence classic
set output "regret.eps"
plot "gucb_regret.txt" title "gUCB" with linespoints pointtype 2 pointsize 2,\
  "iucb_regret.txt" title "IUCB" with linespoints pointtype 4 pointsize 2,\
  "gossip_regret.txt" title "gossip" with linespoints pointtype 12 pointsize 2,\
  "proposed_regret.txt" title "proposed" with linespoints pointtype 6 pointsize 2
  # "ucb2_1.txt" title "dUCB" with linespoints pointtype 8 pointsize 2
  # "iucb_global.txt" title "gIUCB" with linespoints pointtype 10 pointsize 2'\
set output

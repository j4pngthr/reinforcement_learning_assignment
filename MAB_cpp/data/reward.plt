set term postscript enhanced eps
set xlabel 'Unit time'
set ylabel 'Average rewards'
set key right bottom
set xtics 500
# set yrange [0:2]
# set ytics 0.5

set terminal postscript eps color enhanced "Courier" 25
set colorsequence classic
set output "reward.eps"
plot "gucb_reward.txt" title "gUCB" with linespoints pointtype 2 pointsize 2,\
  "iucb_reward.txt" title "IUCB" with linespoints pointtype 4 pointsize 2,\
  "gossip_reward.txt" title "gossip" with linespoints pointtype 12 pointsize 2,\
  "proposed_reward.txt" title "proposed" with linespoints pointtype 6 pointsize 2
  # "ucb2_1.txt" title "dUCB" with linespoints pointtype 8 pointsize 2
  # "iucb_global.txt" title "gIUCB" with linespoints pointtype 10 pointsize 2'\
set output

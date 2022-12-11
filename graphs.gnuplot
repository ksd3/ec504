set datafile separator ','
set xlabel "N Points"
set ylabel "Time"
set key right center
plot 'data.txt' using 1:2 with lines lw 2 title "Creation Time (KD Tree)", 'data.txt' using 1:3 with lines lw 2 title "Creation Time (Octree)", 'data.txt' using 1:4 with lines lw 2 title "Find one random point (KD Tree)", 'data.txt' using 1:5 with lines lw 2 title "Find one random point (Octree)", 'data.txt' using 1:6 with lines lw 2 title "10 Nearest Neighbors (KD Tree)", 'data.txt' using 1:7 with lines lw 2 title "10 Nearest Neighbors (Octree)",'data.txt' using 1:8 with lines lw 4 title "Radius Search (KD Tree, 17.3205)",'data.txt' using 1:9 with lines lw 2 dt 5 title "Radius Search (Octree, 17.3205)"

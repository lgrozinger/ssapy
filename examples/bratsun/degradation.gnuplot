set xlabel "Time (seconds)" font ", 16"
set ylabel "Protein Count" font ", 16"
set title "Protein Count over Time for a Degradation Model with no Delay" font ", 24"

plot "degradation" u 1:2 w l notitle
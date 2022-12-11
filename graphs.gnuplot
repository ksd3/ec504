set datafile separator ','
plot "data.csv" using (log($1))/log(5):2 with lines

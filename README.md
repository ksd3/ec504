# This repository holds the final project for EC504

To compile, simply run ``make`` in the terminal and ``./a.out``


The file ``output.txt`` contains the actual output of the program when running for different point cloud sizes. By default the program is set for a point cloud size of 50000 points but this can be changed by modifying ``tree_size`` in ``main.cpp``. The data was compiled painstakingly by hand into ``data.csv`` The unit of the values in ``data.csv`` is seconds. 

# Gnuplot plotting result
Results of plotting from output file ``data.csv``<br>
Plotting with (log($1))/log(5) on x axis 
![Plot](https://github.com/TimZhun/ec504/blob/main/img/Plot%20with%20log%20over%20log.png?raw=true?raw=true)
Plotting with log y axis for more readable results
![Plot](https://github.com/TimZhun/ec504/blob/main/img/log%20y%20axis.png?raw=true?raw=true?raw=true)

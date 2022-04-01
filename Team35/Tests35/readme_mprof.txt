For the run_all_with_mprof, we will run the autotester and the memory profiler on ALL source and query txt files inside the directory. 
Please ensure that firstly, you have python >3.7 and also have matplotlib and memory-profiler installed in python. 
Secondly, ensure that ALL source and query files, would have the SAME NAME, seperated by a _source.txt or _queries.txt. 
Thirdly, ensure that your working directory would be the tests35 folder. (Basically where your source, query and even python files are in)
Note that currently, EACH SOURCE file can only be tagged with ONE query file. 

Example: source1_source.txt and source1_queries.txt 

To run the python file: Sample running code: 

python run_all_with_mprof.py -c ..\.\Code35\out\build\x64-Debug\src\autotester\autotester.exe -t 0.001

For mac, kindly just replace the first argument with the location of your autotester.exe. 

The results would be in the results folder. (xml files). However, due to limitations to the memory-profiler, the graphs for memory consumption will be stored in the tests35 folder. The format of these files would be: <query>_for_<src>_out.xml or <query>_for_<src>_memoryUsageGraph.png. 

For run_with_mprof, it does the same thing, just that we will only run fo a single source and query txt file. They do not need to have the same name. 

Sample running code: 

python run_with_mprof.py -c ..\.\Code35\out\build\x64-Debug\src\autotester\autotester.exe .\Sample_source.txt .\Sample_queries.txt .\out.xml -t 0.001

In this case, you can redefine the autotester, sample source, query and output xml files as you wish. 

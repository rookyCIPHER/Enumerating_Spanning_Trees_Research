Sanjiv Kapoor and H. Ramesh of IIT Delhi provided an algorithm for enumerating all the spanning trees of a graph with improved time complexity. This project contains the simulation of the undirected graphs version of the algorithm along with my attempt on improving the time complexity by making use of parallel processing with the aid of Taskflow library. This project was completed under the guidance of Dr. Rahul CS. 

One can access and clone the Taskflow library from the [here](https://github.com/taskflow/taskflow).

To compile the algo_parallel.cpp file the following command is to be entered: g++ -std=c++2a .\algo_parallel.cpp -I"path-to-the-cloned-taskflow-repository" -O2 -pthread

Note: Due to the time complexity of the code, the code is only tested for smaller number of vertices.


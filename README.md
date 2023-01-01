# CS221-Lab-Project

#### Course : Data Structures & Algorithms (CS221 Lab)
#### University : Ghulam Ishaq Khan Institute
#### Instructor : Sir Usama Arshad
#### Group : Mujtaba Omar (2021495), Syed Basim Mehmood (2021613), Ali Naufil (2021083)
## A Railway Company

We are an emerging railway company and we are planning to build a high-speed train network in Pakistan. We will try to connect many cities all over the country. We have made a list of all the feasible connections between cities, now we just need to program it. Time and distance of each connection is known. We are making a feasibility report and are still considering what will be the best way to go about laying the rail network. The possible options are:

1) We make the network with minimum rail required to connect all the cities. This way we can start off relatively cheap then later on, upgrade our network.

2) We start from a single city, connect it to its neighbours and slowly grow our network. 

3) Keeping in view the convenience of all cities' populations, we should know the shortest paths between any 2 cities. Then, we can do cost-benefit analysis to find out what will be most profitable for us and for the public.

This is just a graphical project to better visualize what we seek to do.

NOTE: Distances and Time used are not real values. They are random values used only for demonstration purposes.

## Program Details
### SFML
SFML is a graphic library for C++. It has been used here to display BFS and MST. In order to compile the .cpp file, the library will be needed.
NOTE: If you don't have SFML, download the .exe file from the Releases in this repository. Run it in the same folder as the source code. 
### Files
#### Cities_Database.txt: 
It contains names, latitudes and longitudes of 157 cities of Pakistan.
#### graph_generator.cpp: 
It reads the city names from the above file and adds only the specified number of cities into __"cities.txt"__. Also, it fills __"Input_Graph.txt"__ with edges with random weights. The # of edges are specified.
#### CS221-Prj-2021495-2021613-2021083.cpp: 
The actual .cpp code that does everything. It takes Pakistan's map as input from __"Pakistan_Map_Province_Wise.jpg"__, font as input from __"solidworks gdt.ttf"__, cities and their locations as input from __"cities.txt"__ and the weighted edges as input from __"Input_Graph.txt"__. 
Implemented algorithms include Floyd Warshall, Kruskal, BFS.

# Individual Programming Project

This project was created as part of the University of Warsaw's Individual Programming Project course.

The repository contains a C implementation of a program to manage road maps. Using standard input it accepts the following commands:
- `country road number;city name;segment length;segment year;city name;segment length;segment year;...;city name` - creates a country road identified by a given number and description. If a city or a road segment doesn't exist, then it's added. If a road segment exists, but was built or repaired earlier than specified in the description, then the segment's year is modified. If a segment exists, but has a different length than specified in the description, then an error is reported.
- `addRoad;city1;city2;length;year` - adds a road between two cities
- `repairRoad;city1;city2;repairYear` - sets or modifies a road's year
- `getRouteDescription;routeId` - returns a country road's description
- `newRoute;routeId;city1;city2` - creates a new country road between two cities. The new road is automatically created by finding a shortest available path between those cities. If there are multiple paths of the same length, the one whose oldest segment is the newest is chosen
- `extendRoute;routeId;city` - extends a country road to the given city
- `removeRoad;city1;city2` - removes a road between two cities. If this road is a part of any country road, the country road is modified by finding an alternative connection between those two cities according the the requirements described in `newRoute` commands
- `removeRoute;routeId` - removes a country road

The solution uses radix and AVL trees implemented in C.

Additionally, the repository contains:
- a bash script whose first parameter is a path to a file containing `getRouteDescription` outputs and a series of country road numbers. For each parameter containing a country road number, the script searches the specified file for information about the country road, calculates its length and prints a line `country road number;length`
- a CMake configuration file
- a doxygen documentation (written in Polish)
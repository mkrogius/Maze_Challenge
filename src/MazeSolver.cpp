#include "MazeGenerator.h"
#include "MazeSolver.h"

#include <cstdio>
#include <ctime>
#include <iostream>
#include <cmath>

struct point {
	int x, y;

	point(int x, int y) : x(x), y(y) {
	}

	int getIndex(int size) {
		return x + y * size;
	}

	point getRelative(int direction) {
		switch (direction) {
			case 0:
				return point(x - 1, y);
			case 1:
				return point(x, y - 1);
			case 2:
				return point(x + 1, y);
			case 3:
				return point(x, y + 1);
			default:
				return point(0, 0);
		}
	}

	int getDirection(point adjacent) {
		int xx = adjacent.x - x;
		switch (xx) {
			case -1:
				return 0;
			case 1:
				return 2;
		}
		int yy = adjacent.y - y;
		switch (yy) {
			case -1:
				return 1;
			case 1:
				return 3;
		}
		return -1;
	}
} ;

struct maze_point {
	point position;
	bool explored[4];

	maze_point(point position) : position(position) {
		explored[0] = explored[1] = explored[2] = explored[3] = false;
	}

	void setWalls(std::vector<int> walls) {
		explored[0] = walls[0];
		explored[1] = walls[1];
		explored[2] = walls[2];
		explored[3] = walls[3];
	}

	void setExplored(int direction) {
		explored[direction] = true;
	}

	int getFirstAvailablePath() {
		if (!explored[0]) {
			return 0;
		} else if (!explored[1]) {
			return 1;
		} else if (!explored[2]) {
			return 2;
		} else if (!explored[3]) {
			return 3;
		} else {
			return -1;
		}
	}
	
	bool hasOneFreePath() {
		return !(explored[0] && explored[1] && explored[2] && explored[3]);
	}
} ;

// Note: empties the passed stack
std::vector<int> toIndexPath(std::stack<maze_point> path, int mazeSize) {
	std::vector<int> indexPath;
	indexPath.reserve(path.size());
	while (!path.empty()) {
		indexPath.push_back(path.top().position.getIndex(mazeSize));
		path.pop();
	}
	return indexPath;
}

std::vector<int> MazeSolver::BreadCrumb(std::vector<std::vector<int> > walls) {
	/*
		A brute force solver. Tries every path until it finds one that works.
		It is biased to attempt paths that are in the direction of the end cell (opposite corner to the start).
		About 130% faster than the example.

		1. Begin with the final cell and an empty maze point stack
		2. Set the position and walls of the current maze cell in a new maze point
		3. Add the last cell (top of path stack) as an explored path, if any
		3. Pick one free path from the current cell
			a. If no free path is available, pop the stack elements until one has a free path
			b. Pop the free path cell and set the current cell as that one
			c. Get the next free path from the current cell
		4. Set the free path as explored in the current cell
		5. Push the current cell to the stack
		6. Set the current cell to the next one, taken from the free path
		7. Repeat until the current cell is the start
		8. Push the last position to the stack to complete the path
		9. Dump path into final path collection, from stack top to bottom
	*/

	int mazeSize = std::sqrt(walls.size());
	point pos(mazeSize - 1, mazeSize - 1);
	std::stack<maze_point> path;

	while (pos.x != 0 && pos.y != 0) {
		maze_point currentCell(pos);
		currentCell.setWalls(walls[currentCell.position.getIndex(mazeSize)]);
		if (!path.empty()) {
			currentCell.setExplored(currentCell.position.getDirection(path.top().position));
		}
		int direction = currentCell.getFirstAvailablePath();
		if (direction == -1) {
			while (!path.top().hasOneFreePath()) {
				path.pop();
			}
			currentCell = path.top();
			path.pop();
			direction = currentCell.getFirstAvailablePath();
		}
		currentCell.setExplored(direction);
		path.push(currentCell);
		pos = currentCell.position.getRelative(direction);
	}
	path.push(maze_point(pos));

	return toIndexPath(path, mazeSize);
}

// A simple example algorithm that solves the maze
std::vector<int> MazeSolver::ExampleSolver(std::vector<std::vector<int> > walls)
{
	std::vector<int> path;
	int currentCell = 0;
	int currentDir = 3; // 0 means west, 1 means north, 2 means east, 3 means south
	int totalNum = walls.size();

	if(totalNum == 0) return path;
	
	// Store the status of visited cells
	std::vector<bool> visited (walls.size(), false);

	int totalNumber = walls.size();						// Total number of cells
	int dimension = (int) sqrt((float)totalNumber);	// Get dimension of the maze
	int currentCell = 0;								// Start from cell 0

	path.push_back(currentCell);

	while (currentCell < totalNumber - 1) {
		visited[currentCell] = true;	// Mark current cell as visited

		std::vector<int> neighbors;

		if (currentCell % dimension != 0 && currentCell > 0) {
			// Left neighbor
			// If it is adjacent to current cell and has not been visited,
			// Add it to valid neighbors list
			if (walls[currentCell][0] == 0 && !visited[currentCell - 1]) {
				neighbors.push_back(currentCell - 1);
			}
		}
		if (currentCell % dimension != dimension - 1 && currentCell < totalNumber - 1) {
			// Right neighbor
			// If it is adjacent to current cell and has not been visited,
			// Add it to valid neighbors list
			if (walls[currentCell][2] == 0 && !visited[currentCell + 1]) {
				neighbors.push_back(currentCell + 1);
			}
		}
		if (currentCell >= dimension) {
			// Upper neighbor
			// If it is adjacent to current cell and has not been visited,
			// Add it to valid neighbors list
			if (walls[currentCell][1] == 0 && !visited[currentCell - dimension]) {
				neighbors.push_back(currentCell - dimension);
			}
		}
		if (currentCell < totalNumber - dimension) {
			// Lower neighbor
			// If it is adjacent to current cell and has not been visited,
			// Add it to valid neighbors list
			if (walls[currentCell][3] == 0 && !visited[currentCell + dimension]) {
				neighbors.push_back(currentCell + dimension);
			}
		}

		if (neighbors.size() > 0) {
			// If there are valid neighbors
			// Take the first one and move to it
			currentCell = neighbors[0];
			path.push_back(currentCell);
		} else {
			// Otherwise go back to previous cell
			path.pop_back();
			currentCell = path.back();
		}
	}
	
	// Return the final path
	return path;
}
std::vector<int> MazeSolver::WallFollower(std::vector<std::vector<int> > walls)
{
	std::vector<int> path;
	int currentCell = 0;
	int currentDir = 3; // 0 means west, 1 means north, 2 means east, 3 means south
	int totalNum = walls.size();

	if(totalNum == 0) return path;

	

	while(currentCell != totalNum - 1)
	{
		//see if I can go right, then straight, then left, then back
		path.push_back(currentCell);
		if (walls[currentCell][(currentDir + 1)%4] == 0)//turn right
			currentDir = (currentDir + 1)%4;
		else if (walls[currentCell][currentDir] == 0)//straight
		{}
		else if (walls[currentCell][(currentDir + 3)%4] == 0)//left
			currentDir = (currentDir + 3)%4;
		else if (walls[currentCell][(currentDir + 2)%4] == 0)//back
		{
			currentDir = (currentDir + 2)%4;
			//path.pop_back();
		}

		

		//Now we know which way we want to go
		//have to find the right cell id
		if (currentDir == 0)
			currentCell--;
		else if (currentDir == 1)
			currentCell -= (int) sqrt(totalNum);
		else if (currentDir == 2)
			currentCell++;
		else if (currentDir == 3)
			currentCell += (int) sqrt(totalNum);
	}

	path.push_back(currentCell);

	return path;
}

// Validate the path for a maze
// Returns true if the path is valid, false otherwise
bool MazeSolver::ValidatePath(int dimension, std::vector<std::vector<int> > walls, std::vector<int> path)
{
	// Get the path length and total number of cells in a maze
	int pathLength = path.size();
	int totalCells = walls.size();

	// First simple check
	// Check the start and end cell
	if (path[0] != 0 || path[pathLength - 1] != totalCells - 1) {
		return false;
	}

	// Check along the path to see if it counters any walls
	for (int i = 0; i < pathLength - 1; i++) {
		// The difference of IDs between next cell and current cell
		// Used to determine the relative position of next cell
		int difference = path[i + 1] - path[i];
		
		if (difference == 1) {
			// The next cell is right to current cell and there is a wall to the right
			if (walls[path[i]][2] == 1) {
				return false;
			}
		} else if (difference == -1) {
			// The next cell is left to current cell and there is a wall to the left
			if (walls[path[i]][0] == 1) {
				return false;
			}
		} else if (difference == dimension) {
			// The next cell is lower to current cell
			if (walls[path[i]][3] == 1) {
				return false;
			}
		} else if (difference == 0 - dimension) {
			// The next cell is upper to current cell
			if (walls[path[i]][1] == 1) {
				return false;
			}
		} else {
			return false;
		}
	}

	// If the path passes validation then it is good
	return true;
}

int main(int argc,char *argv[])
{

	// The dimension of the maze
	int dimension = 100;

	// Generate walls for the maze given the dimension
	std::vector<std::vector<int> > walls = MazeGenerator::GenerateMaze(dimension);

	// Timer
	// Used to compute the time spent by the maze solving algorithm
	// Enable it if you want to measure the time
	std::clock_t startTime;
	startTime = std::clock();

	// Get the path that solves the maze
	std::vector<int> path = MazeSolver::SolveMaze(walls);

	// Timer continued
	double duration = (std::clock() - startTime) / (double) CLOCKS_PER_SEC;
	std::cout<<"Time spent: "<<duration<<"\n";

	// Validate your path
	bool validation = MazeSolver::ValidatePath(dimension, walls, path);
	if (validation)
		std::cout << "Valid Path\n";
	else
		std::cout << "Invalid Path\n";

	return 0;
}

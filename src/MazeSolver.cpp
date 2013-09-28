#include "MazeGenerator.h"
#include "MazeSolver.h"

#include <cstdio>
#include <ctime>
#include <iostream>
#include <cmath>

using namespace std;

/*
 * Team Mazer
 *
 * Members:
 *   Max Krogius
 *   Eduardo Coronado-Montoya
 *   Celina Vivian
 *   David Lavoie-Boutin
 *   Aleksei
 *
 *   Our fastest algorithm is WallFollower and that function should be considered our submission.
 *   We have also included some other algorithms we tried, just to show our work.
 *
 */

// A simple example algorithm that solves the maze
vector<int> MazeSolver::ExampleSolver(vector<vector<int> > walls)
{
	vector<int> path;
	int currentCell = 0;
	int currentDir = 3; // 0 means west, 1 means north, 2 means east, 3 means south
	int totalNumber = walls.size();

	if(totalNumber == 0) return path;
	
	// Store the status of visited cells
	vector<bool> visited (totalNumber, false);

	int dimension = (int) sqrt((float)totalNumber);	// Get dimension of the maze

	path.push_back(currentCell);

	while (currentCell < totalNumber - 1) {
		visited[currentCell] = true;	// Mark current cell as visited

		vector<int> neighbors;

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


//This is our submission!
vector<int> MazeSolver::WallFollower(vector<vector<int> > walls)
{
	//This is a simple yet quick algorithm which follows the righthand wall
	vector<int> path;
	int currentCell = 0;
	int currentDir = 3; // 0 means west, 1 means north, 2 means east, 3 means south
	int totalNum = walls.size();

	if(totalNum == 0) return path;

	

	while(currentCell < totalNum - 1)
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

int sum(vector<int> summands)
{
	int sum = 0;//The sum of an empty vector is zero. Why? Because I say so

	for (unsigned int i = 0; i < summands.size(); i++)
	{
		sum += summands[i];
	}

	return sum;
}

vector<int> MazeSolver::DeadEndFiller(vector<vector<int> > walls)
{
	//This fills in the maze starting at the dead ends
	vector<int> path;
	int totalNum = walls.size();
	int dimension = (int) std::sqrt(totalNum);
	int fullWall_temp[] = {1, 1, 1, 1};
	vector<int> fullWall(fullWall_temp, fullWall_temp + sizeof(fullWall_temp) / sizeof(int));

	if(totalNum == 0) return path;

	for (int i = 1; i < totalNum - 1; i++)
	{
		int j = i;

		while(sum(walls[j]) == 3 && j != 0 && j < totalNum - 1)
		{//We want a dead end and we dont want to fill the end



			if (walls[j][0] == 0)
			{
				walls[j][0] = 1;
				j -= 1;
				walls[j][2] = 1;
			}else if (walls[j][1] == 0)
			{
				walls[j][1] = 1;
				j -= dimension;
				walls[j][3] = 1;
			}else if (walls[j][2] == 0)
			{
				walls[j][2] = 1;
				j += 1;
				walls[j][0] = 1;
			}else
			{
				walls[j][3] = 1;
				j += dimension;
				walls[j][1] = 1;
			}
		}
	}

	path = WallFollower(walls);

	return path;
}

vector<int> MazeSolver::DepthFirstSearch(vector<vector<int> > walls)
{
	vector<int> stack;
	vector<int> path;
	const int totalNum = walls.size();
	const int dimension = (int) sqrt(totalNum);
	bool *visited = new bool[totalNum];
	int cell = 0;
	stack.push_back(cell);
	
	//init visited[]
	for (int i = 0; i < totalNum; i++)
		visited[i] = false;

	while(true)
	{
		cell = stack.back();

		if (cell == totalNum - 1)
			break;

		visited[cell] = true;

		if (walls[cell][3] == 0 && !visited[cell + dimension])//go south first
			stack.push_back(cell + dimension);
		else if (walls[cell][2] == 0 && !visited[cell + 1])//go east second
			stack.push_back(cell + 1);
		else if (walls[cell][0] == 0 && !visited[cell - 1])//go west third
			stack.push_back(cell - 1);
		else if (walls[cell][1] == 0 && !visited[cell - dimension])//go north last
			stack.push_back(cell - dimension);
		else
			stack.pop_back();
	}

	delete [] visited;
	return stack;
}

vector<int> MazeSolver::OptimizedDFS(vector<vector<int> > walls)
{
	vector<int> stack;
	vector<int> path;
	const int totalNum = walls.size();
	const int dimension = (int) sqrt(totalNum);
	short *visited = new short[totalNum];
	bool hasFoundLowerWall = false;
	bool hasFoundSideWall = false;
	int lowerWallTest = totalNum - dimension - 1;//Precompute the bound for lower wall
	int sideWallTest = dimension - 1;
	int cell = 0;
	stack.push_back(cell);
	
	//init visited[]
	for (int i = 0; i < totalNum; i++)
		visited[i] = false;

	while(true)
	{
		cell = stack.back();


		//Check if we are done
		if (cell == totalNum - 1)
				break;

		if (!hasFoundLowerWall && cell > lowerWallTest)
		{
			//If we are on the lower wall
			hasFoundLowerWall = true;



			//Follow our path back to the start
			int prevCell = 0;
			int curCell;
			for (unsigned int i = 1; i < stack.size(); i++)
			{
				prevCell = stack[i - 1];
				curCell = stack[i];

				if (curCell - prevCell == 1)//going east
				{
					if (prevCell + dimension < totalNum)
						visited[prevCell + dimension] = true;
				}
				else if (curCell - prevCell == -1)//going west
				{
					if (prevCell - dimension >= 0)
						visited[prevCell - dimension] = true;
				}
				else if (curCell - prevCell == dimension)//going south
				{
					if (prevCell % dimension != 0)
						visited[prevCell - 1] = true;
				}
				else //going north
				{
					if (prevCell % dimension != dimension - 1)
						visited[prevCell + 1] = true;
				}
			
			}

			if (curCell - prevCell == 1)//going east
			{
				if (curCell + dimension < totalNum)
					visited[curCell + dimension] = true;
			}
			else if (curCell - prevCell == -1)//going west
			{
				if (curCell - dimension >= 0)
					visited[curCell - dimension] = true;
			}
			else if (curCell - prevCell == dimension)//going south
			{
				if (curCell % dimension != 0)
					visited[curCell - 1] = true;
			}
			else //going north
			{
				if (curCell % dimension != dimension - 1)
					visited[curCell + 1] = true;
			}


		}

		if (!hasFoundSideWall && cell % dimension == sideWallTest)
		{
			//If we are on the side wall
			hasFoundSideWall = true;



			//Follow our path back to the start
			int prevCell = 0;
			int curCell;
			for (unsigned int i = 1; i < stack.size(); i++)
			{
				prevCell = stack[i - 1];
				curCell = stack[i];

				if (curCell - prevCell == 1)//going east
				{
					if (prevCell - dimension >= 0)
						visited[prevCell - dimension] = true;
				}
				else if (curCell - prevCell == -1)//going west
				{
					if (prevCell + dimension < totalNum)
						visited[prevCell + dimension] = true;
				}
				else if (curCell - prevCell == dimension)//going south
				{
					if (prevCell % dimension != dimension - 1)
						visited[prevCell + 1] = true;
				}
				else //going north
				{
					if (prevCell % dimension != 0)
						visited[prevCell - 1] = true;
				}
			}


			if (curCell - prevCell == 1)//going east
			{
				if (curCell - dimension >= 0)
					visited[curCell - dimension] = true;
			}
			else if (curCell - prevCell == -1)//going west
			{
				if (curCell + dimension < totalNum)
					visited[curCell + dimension] = true;
			}
			else if (curCell - prevCell == dimension)//going south
			{
				if (curCell % dimension != dimension - 1)
					visited[curCell + 1] = true;
			}
			else //going north
			{
				if (curCell % dimension != 0)
					visited[curCell - 1] = true;
			}
		}

		visited[cell] = 1;

		if (walls[cell][3] == 0 && !visited[cell + dimension])//go south first
			stack.push_back(cell + dimension);
		else if (walls[cell][2] == 0 && !visited[cell + 1])//go east second
			stack.push_back(cell + 1);
		else if (walls[cell][0] == 0 && !visited[cell - 1])//go west third
			stack.push_back(cell - 1);
		else if (walls[cell][1] == 0 && !visited[cell - dimension])//go north last
			stack.push_back(cell - dimension);
		else
			stack.pop_back();
	}

	delete [] visited;
	return stack;
}


// Validate the path for a maze
// Returns true if the path is valid, false otherwise
bool MazeSolver::ValidatePath(int dimension, vector<vector<int> > walls, vector<int> path)
{
	// Get the path length and total number of cells in a maze
	int pathLength = path.size();
	int totalCells = walls.size();

	// First simple check
	// Check the start and end cell
	if (path[0] != 0 || path[pathLength - 1] != totalCells - 1) {
		return false;
	}

	// Check along the path to see if it encounters any walls
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

double testFunction(vector<int> (*func)(vector<vector<int> >), int dimension)
{
	int numRuns = 3;
	vector<vector<int> > maze;
	std::vector<int> path;
	std::clock_t startTime;
	double duration = 0;


	for(int i = 0; i < numRuns; i++)
	{
		vector<vector<int> > maze;
		std::vector<int> path;

		maze = MazeGenerator::GenerateMaze(dimension);

		startTime = std::clock();
		path = func(maze);
		duration += (std::clock() - startTime) / (double) CLOCKS_PER_SEC;
		if (! MazeSolver::ValidatePath(dimension, maze, path))
		{
			printf("Error: Function Gave an invalid solution\n");
			return -1.0f;
		}
	}
	return duration / ((double) numRuns);
}

int main(int argc,char *argv[])
{
	//My laptop can't generate a maze of size 10000
	/*printf("Testing ExampleSolver\n");
	printf("n\t\t\truntime\n");
	printf("%i\t\t\t%f\n",10,testFunction(MazeSolver::ExampleSolver, 10));     //0.001
	printf("%i\t\t\t%f\n",100,testFunction(MazeSolver::ExampleSolver, 100));   //0.108
	printf("%i\t\t\t%f\n",1000,testFunction(MazeSolver::ExampleSolver, 1000));//10.803
	

	
	printf("Testing DeadEndFiller\n");
	printf("n\t\t\truntime\n");
	printf("%i\t\t\t%f\n",10,testFunction(MazeSolver::DeadEndFiller, 10));     //0.001
	printf("%i\t\t\t%f\n",100,testFunction(MazeSolver::DeadEndFiller, 100));   //0.151
	printf("%i\t\t\t%f\n",1000,testFunction(MazeSolver::DeadEndFiller, 1000));//15.596

	printf("Testing DepthFirstSearch\n");
	printf("n\t\t\truntime\n");
	printf("%i\t\t\t%f\n",10,testFunction(MazeSolver::DepthFirstSearch, 10));     //0.001
	printf("%i\t\t\t%f\n",100,testFunction(MazeSolver::DepthFirstSearch, 100));   //0.130
	printf("%i\t\t\t%f\n",1000,testFunction(MazeSolver::DepthFirstSearch, 1000));//12.074*/


	printf("Testing WallFollower\n");
	printf("n\t\t\truntime\n");
	printf("%i\t\t\t%f\n",10,testFunction(MazeSolver::WallFollower, 10));    //0.000
	printf("%i\t\t\t%f\n",100,testFunction(MazeSolver::WallFollower, 100));  //0.043
	printf("%i\t\t\t%f\n",500,testFunction(MazeSolver::WallFollower, 1000));//4.171

	printf("Testing OptimizedDFS\n");
	printf("n\t\t\truntime\n");
	printf("%i\t\t\t%f\n",10,testFunction(MazeSolver::OptimizedDFS, 10));     //0.001
	printf("%i\t\t\t%f\n",100,testFunction(MazeSolver::OptimizedDFS, 100));   //0.130
	printf("%i\t\t\t%f\n",500,testFunction(MazeSolver::OptimizedDFS, 1000));//12.074
}

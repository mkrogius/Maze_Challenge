#ifndef MAZESOLVER_H
#define MAZESOLVER_H

#include <vector>

class MazeSolver
{
	public:
		static std::vector<int> WallFollower(std::vector<std::vector<int> > walls);
		static std::vector<int> BreadCrumb(std::vector<std::vector<int> > walls);
		static std::vector<int> ExampleSolver(std::vector<std::vector<int> > walls);
		static std::vector<int> MazeSolver::DeadEndFiller(std::vector<std::vector<int> > walls);
		static double testFunction(std::vector<int> (*func)(std::vector<std::vector<int> >), int dimension);
		static bool ValidatePath(int dimension, std::vector<std::vector<int> > walls, std::vector<int> path);
};

#endif

#pragma once

#include "../glm/include/common.hpp"

#include <vector>

class Grid2D; // FW Declaration

class Cell2D {
public:
	friend Grid2D;
	Cell2D(glm::vec2 cellPosition, glm::vec2 cellDimension);
	~Cell2D();
	
	glm::vec2 getPosition();
	glm::vec2 getDimension();
	
protected:
	bool		walkable;
	bool		wall;
	glm::vec2	position;
	glm::vec2	dimension;
};

class Grid2D {
public:
	Grid2D(glm::vec2 startPosOfGrid, glm::vec2 dimensionOfCell, int amountOfRows, int amountOfCellsInRow, bool invertX, bool invertY);
	~Grid2D();
	
	void create2DGrid(glm::vec2 startPosOfGrid, glm::vec2 dimensionOfCell, int amountOfRows, int amountOfCellsInRow, bool invertX, bool invertY);
	Cell2D* getCell2D(int x, int y);
	std::vector< std::vector<Cell2D*>> getGrid();
	
	
private:
	// Row Vector with vector of Cells
	std::vector< std::vector<Cell2D*> > m_gridOfCells;
};
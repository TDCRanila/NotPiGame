#include "../Headers/Grid.h"

Cell2D::Cell2D(glm::vec2 cellPosition, glm::vec2 cellDimension)	: position(cellPosition), dimension(cellDimension) {
	
}

Cell2D::~Cell2D() {
	
}

glm::vec2 Cell2D::getPosition() {
	return position;
}
glm::vec2 Cell2D::getDimension() {
	return dimension;
}

Grid2D::Grid2D(glm::vec2 startPos, glm::vec2 dimension, int amountOfRows, int amountOfCellsInRow, bool invertX, bool invertY) {
	this->create2DGrid(startPos, dimension, amountOfRows, amountOfCellsInRow, invertX, invertY);
}

Grid2D::~Grid2D() {
	// Delete all the cells
	for (int i = 0; i < m_gridOfCells.size(); i++) {
		for (int j = 0; j < m_gridOfCells[i].size(); j++) {
			delete m_gridOfCells[i][j];
			m_gridOfCells[i][j] = nullptr;
	} }
}

void Grid2D::create2DGrid(glm::vec2 startPosOfGrid, glm::vec2 dimensionOfCell, int amountOfRows, int amountOfCellsInRow, bool invertX, bool invertY) {
	glm::vec2 dimCell = dimensionOfCell;
	if (invertX) { dimCell.x = -dimensionOfCell.x; }
	if (invertY) { dimCell.y = -dimensionOfCell.y; }
	glm::vec2 cellStartPos = glm::vec2(startPosOfGrid.x + dimCell.x / 2, startPosOfGrid.y + dimCell.y / 2);
	glm::vec2 nextPosInRow;
	glm::vec2 nextPosPerRow;
	
	// Create the cells at their correct locations
	for (int i = 0; i < amountOfRows; i++) {
		std::vector<Cell2D*> cellsInRow;
		
		// Calculate the new offset per row
		nextPosPerRow = glm::vec2(0.0f, dimCell.y * i) + cellStartPos;
		
		for (int j = 0; j < amountOfCellsInRow; j++) {
			// Calculate the new offset for the row
			nextPosInRow = glm::vec2(dimCell.x * j, 0) + cellStartPos;
			
			// Calculate the new position for the cell
			glm::vec2 newPos = glm::vec2(nextPosInRow.x, nextPosPerRow.y);
			
			// Create a new cell and push it in the vector 
			Cell2D* newCell = new Cell2D(newPos, dimensionOfCell);
			cellsInRow.push_back(newCell);
		}
		
		// Now when finnished with the other vector, push that vector in the list of vectors
		this->m_gridOfCells.push_back(cellsInRow);
	}
	
}

Cell2D* Grid2D::getCell2D(int x, int y) {
	if (x > m_gridOfCells.size())		{ printf("That cell doesn't exist\n"); return nullptr; }
	if (y > m_gridOfCells[x].size())	{ printf("That cell doesn't exist\n"); return nullptr; }
	return m_gridOfCells[x][y];
}

std::vector< std::vector<Cell2D*>> Grid2D::getGrid() {
	return m_gridOfCells;
}
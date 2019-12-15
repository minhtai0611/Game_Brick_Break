#pragma once
#include"Brick.h"
class Grid
{
private:
	unsigned int rows;
	unsigned int columns;

	vector<Brick>* bricks;

public:
	long width, height;
	Grid()
	{
		rows = 0;
		columns = 0;
		bricks = NULL;
	}

	~Grid()
	{
		if (bricks != NULL)
			delete bricks;
	}

	void setDimensions(unsigned int col, unsigned int row)
	{
		rows = row;
		columns = col;
		if (bricks != NULL)
			delete bricks;

		bricks = new std::vector<Brick>(rows * columns);
		init();
	}

	Vector2f cellSize()
	{
		return Vector2f(width / columns, height / rows);
	}

	void init()
	{
		for (int i = 0; i < rows; i++)
			for (int j = 0; j < columns; j++)
			{
				bricks->at(i + j * rows).setSize(cellSize());
				bricks->at(i + j * rows).setFillColor(Color::Blue);
				bricks->at(i + j * rows).setOutlineColor(Color::Black);
				bricks->at(i + j * rows).setOutlineThickness(3);
				bricks->at(i + j * rows).setPosition(left + j * cellSize().x, top + i * cellSize().y);
			}
	}

	void display(RenderWindow& window)
	{
		for (int i = 0; i < rows; i++)
			for (int j = 0; j < columns; j++)
			{
				if (bricks->at(i + j * rows).armor <= 0)
					continue;
				window.draw(bricks->at(i + j * rows));
			}
	}

	bool collide(RectangleShape& ball, Vector2f& ballSpeed, unsigned int& score)
	{
		for (int i = 0; i < rows; i++)
			for (int j = 0; j < columns; j++)
			{
				if (bricks->at(i + j * rows).armor <= 0)
					continue;
				if (bricks->at(i + j * rows).collide(ball, ballSpeed))
				{
					bricks->at(i + j * rows).armor--;
					bricks->at(i + j * rows).setFillColor(Color::Green);
					if (bricks->at(i + j * rows).armor == 0)
						score += 700;

					return true;
				}
			}
		return false;
	}

	bool isGameWon()
	{
		for (int i = 0; i < rows; i++)
			for (int j = 0; j < columns; j++)
				if (bricks->at(i + j * rows).armor > 0)
					return false;
		return true;
	}

};



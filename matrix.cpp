#include "matrix.h"
#include <iostream>
#include <cmath>
#include <sstream>
#include <string>

#define PRINT_SIZE 3

using namespace std;


void moveDownZeros(double ** mat, int maxRows, int colNum, int maxCols)
{
	int dRow = 0;
	while ((dRow < maxRows) && (*(*(mat + dRow) + colNum) == 0))
	{
		dRow++;
	}
	if (dRow == maxRows)
	{
		//If colNum < maxCols - 1, recursion for next col
		if (colNum < maxCols - 1)
			moveDownZeros(mat, maxRows, colNum + 1, maxCols);
	}
	else
	{
		//swap dRow with row zero
		double * tempRow = *mat;
		*mat = *(mat + dRow);
		*(mat + dRow) = tempRow;
		//if maxRows > 2 and colNum < maxCols - 1, recursion for next col + row
		if ((maxRows > 2) && (colNum < maxCols - 1))
			moveDownZeros(mat + 1, maxRows - 1, colNum + 1, maxCols);
	}
}

//row r1 -> r1 - a * r2 
void subtMultRow(double ** mat, int r1, int r2, double a, int cols)
{
	for (int i = 0; i < cols; i++)
	{
		*(*(mat + r1) + i) -= a * *(*(mat + r2) + i);
	}
}

void reduceDir(double ** mat, int dRow, int times, int col, int cols)
{
	if (*(*mat + col) != 0)
	{
		for (int i = 1; i <= times; i++)
		{
			subtMultRow(mat, i * dRow, 0, *(*(mat + i * dRow) + col) /
				*(*mat + col), cols);
		}
	}
}

//r1 -> r1 * a
void multRow (double ** mat, double a, int cols)
{
	for (int i = 0; i < cols; i++)
	{
		*(*mat + i) *= a;
	}
}

Matrix Matrix::copy()
{
	Matrix cop();
	cop.rows = rows;
	cop.cols = cols;
	cop.mat = new double * [rows];
	for (int row = 0; row < rows; row++)
	{
		cop.mat[row] = new double[cols];
		for (int col = 0; col < cols; col++)
		{
			cop.mat[row][col] = mat[row][col];
		}
	}
}

Matrix::Matrix(int rs, int cls)
{
	rows = rs;
	cols = cls;
	//double * tmp[rows];
	mat = new double * [rows];
	for (int row = 0; row < rows; row++)
	{
		mat[row] = new double[cols];
		for (int col = 0; col < cols; col++)
		{
			mat[row][col] = 0;
		}
	}
}

void Matrix::set(int r, int c, double val)
{
	mat[r][c] = val;
}

double Matrix::get(int r, int c)
{
	return mat[r][c];
}

void Matrix::print()
{
	cout << "------" << endl;
	for (int r = 0; r < rows; r++)
	{
		for (int c = 0; c < cols; c++)
		{
			stringstream strStr;
			strStr << mat[r][c];
			string str = strStr.str();
			if (str.size() > PRINT_SIZE) {
				str = str.substr(0, PRINT_SIZE);
				if (str.at(PRINT_SIZE - 1) == '.')
					str = str.substr(0, PRINT_SIZE - 1);
			}
			cout << str;
			for (int i = 0; i < PRINT_SIZE + 1 - str.size(); i++)
			{
				cout << " ";
			}
		}	
		cout << endl;
	}
	cout << "------" << endl;
}

void Matrix::getCharPol(int rowNum, vector<int> cols, double * pol)
{
	if (rowNum == rows - 1)
	{
		pol[0] += mat[rowNum][cols.at(0)];
		pol[1] += -1;
	}
	else
	{
		for 
	}
}

void Matrix::rowReduce()
{
	int minDim = min(rows, cols);
	int nextCol = 0;
	int curRow = 0;
	//get into eschelon form
	while (curRow < minDim - 1)
	{
		//make sure all zeroes go down
		moveDownZeros(mat, rows, 0, cols);
		while ((nextCol < cols) && (*(*(mat + curRow) + nextCol) == 0))
			nextCol++;
		if (nextCol == cols)
			break;
		reduceDir(mat + curRow, 1, rows - curRow - 1, nextCol, cols);
		curRow++;
	}
	curRow--;
	
	//finish row-reduced eschelon
	while (curRow >= 0)
	{
		while ((nextCol > 0) && (*(*mat + nextCol) == 0))
			nextCol--;
		multRow(mat + curRow, 1 / *(*(mat + curRow) + nextCol), cols);
		reduceDir(mat + curRow, -1, curRow, nextCol, cols);
		curRow--;
	}
}

//finds null vector of row-reduced eschelon-form matrix=
void findNullVector(double ** mat, int rows, int cols, double * vec)
{
	//go through each column
	//if only one value, put - summ of other rows, else put 1
	int row = 0;
	for (int col = 0; col < cols; col++)
	{
		if (abs(*(*(mat + row) + col)) > 0.0000000001)
		{
			*vec = 0;
			//loop through all columns in pivot row, get negative 
			for (int chkCol = col + 1; chkCol < cols; chkCol++)
			{
				*vec -= *(*(mat + row) + chkCol);
			}
			row++;
			if (row >= rows)
			{
				for (int i = col + 1; i < cols; i++)
					*(vec++) = 1;
				break;
			}
			vec++;
		}
		else
		{
			*(vec++) = 1;
		}
	}
}

void copyMatrix(double m1[2][2], double m2[2][2])
{
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			m2[i][j] = m1[i][j];
		}
	}
}

void getEigenvector(double mat[2][2], double val, double * iVec)
{
	double matCop[2][2];
	copyMatrix(mat, matCop);
	for (int i = 0; i < 2; i++)
	{
		matCop[i][i] -= val;
	}
	double * sMat[2] = { matCop[0], matCop[1] };
	rowReduce(sMat, 2, 2);
	//cout << endl << "-----" << endl;
	//printM(sMat,2,2);
	//cout << "-----" << endl;
	findNullVector(sMat, 2, 2, iVec);
}

vector<Eigenpair> Matrix::getEigenpairs()
{
	//assume rows = cols
}

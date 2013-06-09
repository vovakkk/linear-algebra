#include "data.h"
#include <iostream>
#include <cmath>

using namespace std;

Data::Data(int d)
{
	dim = d;
}

vector<Eigenpair> Data::getPairs()
{
	return egPrs;
}

void Data::load(double * points, int numPts)
{
	egPrs.empty();
	//get avg for each coord
	avgVec = new double[dim];
	for (int i = 0; i < dim; i++)
		avgVec[i] = 0;
	for (int ptInd = 0; ptInd < numPts; ptInd++)
	{
		for (int dmInd = 0; dmInd < dim; dmInd++)
		{
			avgVec[dmInd] += *(points + ptInd * dim + dmInd) / numPts;
		}
	}
		
	//compute covariance matrix
	Matrix covM(dim, dim);
	for (int ptInd = 0; ptInd < numPts; ptInd++)
	{
		for (int i = 0; i < dim; i++)
		{
			//cout << points[ptInd * dim + i] << endl;
			for (int j = 0; j < dim; j++)
			{
				//cout << (*(points + ptInd * dim + i) - avgVec[i]) *
				//	(*(points + ptInd * dim + j) - avgVec[j]) << " ";
				covM.add(i, j, (*(points + ptInd * dim + i) - avgVec[i]) *
					(*(points + ptInd * dim + j) - avgVec[j]));
			}
		}
	}
	
	//covM.set(0, 0, 0.0575962);        covM.set(0, 1,  0.0102312  );       
	//covM.set(1, 0, 0.0102312   );  covM.set(1, 1,    0.00181744);
	
	//covM.print();
	
	//return eigen pairs for covariance matrix
	egPrs = covM.getEigenpairs();
}

vector<double *> Data::getOrthoBasis()
{
	return basis;
}

double dot(double * v1, double * v2, int dim)
{
	double ret = 0;
	for (int i = 0; i < dim; i++)
	{
		ret += *(v1++) * *(v2++);
	}
	return ret;
}

void Data::compOrthoBasis(double minIVal)
{
	basis.empty();
	for (int i = 0; i < egPrs.size(); i++)
	{
		if (egPrs.at(i).value >= minIVal)
		{
			double len = sqrt(dot(egPrs.at(i).vector, egPrs.at(i).vector, dim));
			double * bs = new double[dim];
			for (int j = 0; j < dim; j++)
			{
				bs[j] = egPrs.at(i).vector[j] / len;
			}
			basis.push_back(bs);
		}
	}
}

double getDist(double * v1, double * v2, int dim)
{
	double diff[dim];
	for (int i = 0; i < dim; i++)
	{
		diff[i] = v1[i] - v2[i];
	}
	return sqrt(dot(diff, diff, dim));
}

int Data::match(vector<double *>vecs, double * v)
{
	if (vecs.size() == 0)
	{
		return -1;
	}
	else
	{
		double vProj[basis.size()];
		getProjection(v, vProj);
		double tProj[basis.size()];
		getProjection(vecs.at(0), tProj);
		double minDist = getDist(vProj, tProj, basis.size());
		int minInd = 0;
		for (int i = 1; i < vecs.size(); i++)
		{
			getProjection(vecs.at(i), tProj);
			double curDist = getDist(vProj, tProj, basis.size());
			if (curDist < minDist)
			{
				minDist = curDist;
				minInd = i;
			}
		}
		return minInd;
	}
}

void Data::getProjection(double * vec, double * proj)
{
	double vecMAvg[dim];
	for (int i = 0; i < dim; i++)
	{
		vecMAvg[i] = vec[i] - avgVec[i];
	}
	for (int bInd = 0; bInd < basis.size(); bInd++)
	{
		proj[bInd] = dot(basis[bInd], vecMAvg, dim);
	}
}

#define _USE_MATH_DEFINES
#include "Crystal.h"
#include <chrono>
#include <fstream>
struct CrystalInitData
{
	int n = 5;
    double mass = 40;
    double epsilon = 1;
    double r = 0.38;
    double f = 10000;
    double l = 2.3;
    double a = 0.38;
    double T = 1000;
};
struct CrystalUpdateData
{
    double tau = 0.01;
	int s_o = 100;
	int s_d = 50000;
	int s_out = 100;
	int s_xyz = 100;
};


void loadData(CrystalInitData& initData, CrystalUpdateData& updateData)
{
	std::ifstream dataFile("params.txt");
	dataFile >> initData.n;
	dataFile >> initData.mass;
	dataFile >> initData.epsilon;
	dataFile >> initData.r;
	dataFile >> initData.f;
	dataFile >> initData.l;
	dataFile >> initData.a;
	dataFile >> initData.T;

	dataFile >> updateData.tau;
	dataFile >> updateData.s_o;
	dataFile >> updateData.s_d;
	dataFile >> updateData.s_out;
	dataFile >> updateData.s_xyz;
}

int main(int argc, char *argv[])
{
    const auto t1 = std::chrono::high_resolution_clock::now();
	{
		CrystalInitData initData;
		CrystalUpdateData updateData;
		loadData(initData, updateData);
        const int N = initData.n * initData.n * initData.n;
		Crystal crystal(N, initData.mass, initData.epsilon, initData.r, initData.f, initData.l);
		crystal.Initialize(initData.n, initData.a, initData.T);
		crystal.Update(updateData.tau, updateData.s_o, updateData.s_d, updateData.s_out, updateData.s_xyz);
	}
    const auto t2 = std::chrono::high_resolution_clock::now();
    printf( "took %lld  ms\n",std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count());

    return 0;
}













































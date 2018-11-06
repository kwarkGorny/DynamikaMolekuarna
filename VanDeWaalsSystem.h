#ifndef VANDEWAALSSYSTEM_H
#define VANDEWAALSSYSTEM_H

#include<vector>
#include <algorithm>
#include "Point3D.h"


class VanDeWaalsSystem
{
public:
    VanDeWaalsSystem(int n, double r)noexcept
		: m_Force(n)
		, m_Potential(n)
        , m_RPowerSix(r*r*r*r*r*r)
    {}
	
    inline void Update(std::vector<PointF>const& postions, int n, double e) noexcept
    {
		Reset();
        const double influence = e * m_RPowerSix;
        for(int ii = 1; ii < n; ++ii)
        {
            for(int jj =0; jj < ii; ++jj)
            {
				const PointF diff = postions[ii] - postions[jj];
                const double distanceSqr = diff.GetMagitudeSqr();
                const double distancePowerOfSix = distanceSqr * distanceSqr * distanceSqr;
                const double distancePowerOfTwelve = distancePowerOfSix * distancePowerOfSix;

                const double tempA = influence / distancePowerOfTwelve;
                const double tempB = m_RPowerSix - distancePowerOfSix;

                m_Potential[ii] += tempA * (tempB - distancePowerOfSix);
                const PointF constF = (12 * tempA * tempB / distanceSqr) * diff;
				m_Force[ii] += constF;
				m_Force[jj] -= constF;
            }
        }
    }

	inline void Reset()noexcept
	{
		for (auto&& f : m_Force)
		{
			f = {};
		}
		for (auto&& v : m_Potential)
		{
			v = 0;
		}
	}
    inline double CalcPotential()const noexcept
	{
		return std::accumulate(m_Potential.begin(), m_Potential.end(), 0);
	}
	inline std::vector<PointF>const& GetForce()const noexcept
	{
		return m_Force;
	}

    inline std::vector<double>const& GetPotential()const noexcept
	{
		return m_Potential;
	}
protected:
	std::vector<PointF> m_Force;
    std::vector<double> m_Potential;
    double m_RPowerSix;
};

#endif // VANDEWAALSSYSTEM_H

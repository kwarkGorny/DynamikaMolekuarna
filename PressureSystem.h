#ifndef PRESSURESYSTEM_H
#define PRESSURESYSTEM_H
#include<vector>
#include<algorithm>

#include "Point3D.h"

class PressureSystem
{
public:
    PressureSystem(int n)noexcept
		: m_Force(n)
		, m_Potential(n)
    {}

    inline void Update(std::vector<PointF>const& postions, int n, double l, double f) noexcept
    {
        const double lSqr = l * l;
        for(int ii = 0; ii < n; ++ii )
        {
            const double rSqr = postions[ii].GetMagitudeSqr();
            if(rSqr > lSqr)
            {
                const double r = std::sqrt(rSqr);
                const double diff = (l - r) ;
				m_Potential[ii] = f/2 * diff * diff;

                const double constF = (f * diff / r);
				m_Force[ii] = postions[ii] * constF;
            }
			else
			{
				m_Potential[ii] = 0;
				m_Force[ii] = PointF();
			}
        }
    }
    inline double CalcPressure(double l)const noexcept
	{
        double pressure = 0;
		for (int ii = 0; ii < m_Potential.size(); ++ii)
		{
			pressure += m_Potential[ii];
		}
        return pressure / (2 * l * l * static_cast<double>(M_PI));
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
};


#endif // PRESSURESYSTEM_H

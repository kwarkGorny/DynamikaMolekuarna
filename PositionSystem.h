#ifndef POSITIONSYSTEM_H
#define POSITIONSYSTEM_H

#include<vector>

#include"Point3D.h"

class PositionSystem
{
public:
    PositionSystem(int n)noexcept
		: m_Positions(n)
    {}


    inline void Init(const int width, const double a)noexcept
    {
        const double constA = static_cast<double>(std::sqrt(3)/6);
        const double constB = static_cast<double>(std::sqrt(2/3.f));
        const double halfOfN = (width - 1)/2.0f;

        for(int ii =0; ii < width; ++ii)
        {
            for(int jj =0; jj < width; ++jj)
            {
                for(int kk =0; kk < width; ++kk)
                {
                    const int i = ii + jj*width + kk*width*width;
					m_Positions[i].x = a/2.f * (2*ii + jj + kk - 4*halfOfN);
					m_Positions[i].y = constA * a * (3 * jj + kk - 4*halfOfN);
					m_Positions[i].z = constB * a * (kk - halfOfN);
                }
            }
        }
    }

    inline void Update(std::vector<PointF>const& momentums, double tau, double mass)noexcept
    {
        const double updateConst = tau/ mass;
        for(int ii=0; ii< m_Positions.size(); ++ii)
        {
			m_Positions[ii] += momentums[ii] * updateConst;
        }
    }
	inline std::vector<PointF>const& GetPositions()const noexcept 
	{
		return m_Positions;
	}

protected:
	std::vector<PointF> m_Positions;
};

#endif // POSITIONSYSTEM_H

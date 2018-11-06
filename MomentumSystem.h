#ifndef MOMENTUMSYSTEM_H
#define MOMENTUMSYSTEM_H

#include <vector>
#include <random>
#include <algorithm>
#include "Point3D.h"


class MomentumSystem
{
public:
    MomentumSystem(int n)noexcept
		: m_Momentums(n)
    {}

    inline void Init( const double T, const double mass) noexcept
    {
        constexpr double k = 8.31/1000.0;
        std::mt19937 g_RandomEngine(std::random_device{}());
        std::uniform_int_distribution<int> randomBool(0,1);
        std::uniform_real_distribution<double> randomLambda(0,1);
        const double constE = - k * mass * T;
        std::generate(m_Momentums.begin(), m_Momentums.end(),
                      [&](){ return PointF{1 * (double) std::sqrt(constE * std::log(0.5)),
                                           1 * (double) std::sqrt(constE * std::log(0.5)),
                                           1 * (double) std::sqrt(constE * std::log(0.5))};});

        const PointF sumP = std::accumulate(m_Momentums.begin(), m_Momentums.end(), PointF()) / m_Momentums.size();
        std::for_each(m_Momentums.begin(), m_Momentums.end(),[&](PointF& p){ p -= sumP;});
    }

    inline void Update(std::vector<PointF>const& forcesS, std::vector<PointF>const& forcesP,const double tau) noexcept
    {
        const double halfTau = tau/2;
        for(int ii=0; ii < m_Momentums.size(); ++ii)
        {
            m_Momentums[ii] += forcesS[ii] * halfTau;
        }
        for(int ii=0; ii < m_Momentums.size(); ++ii)
        {
            m_Momentums[ii] += forcesP[ii] * halfTau;
        }
    }
    inline double CalcKineticEnergy(const double mass)const noexcept
	{
        double sum = 0;
		for (auto const& p : m_Momentums)
		{
			sum += p.GetMagitudeSqr();
		}
		return sum / (2 * mass);
	}
	inline std::vector<PointF>const& GetMomentums()const noexcept
	{
		return m_Momentums;
	}
protected:
	std::vector<PointF> m_Momentums;
};


#endif // MOMENTUMSYSTEM_H

#ifndef CRYSTAL_H
#define CRYSTAL_H
#include <numeric>
#include <vector>
#include "Point3D.h"
#include "PositionSystem.h"
#include "MomentumSystem.h"
#include "PressureSystem.h"
#include "VanDeWaalsSystem.h"

#include <fstream>

class Crystal
{
public:
    Crystal(int N = 125, double mass = 40, double epsilon = 1, double r = 0.38, double f =10000, double l =2.3) noexcept
        : m_PositionSystem(N)
        , m_MomentumSystem(N)
        , m_PressureSystem(N)
        , m_VanDeWaalsSystem(N, r)
        , m_N(N)
        , m_Mass(mass)
		, m_F(f)
		, m_L(l)
		, m_Epsilon(epsilon)
    {
    }
    inline void Initialize(int n = 5, double a=0.38, double T=1000)noexcept
    {
        m_PositionSystem.Init(n, a);
        m_MomentumSystem.Init(T, m_Mass);
        m_PressureSystem.Update(m_PositionSystem.GetPositions(), m_N, m_L, m_F);
        m_VanDeWaalsSystem.Update(m_PositionSystem.GetPositions(), m_N, m_Epsilon);
    }

    inline void Update(double tau = 0.01, int s_o =100, int s_d = 50000, int s_out=100, int s_xyz=100)noexcept
    {
		std::ofstream positionFile("positions.xyz");
		std::ofstream characteristicsFile("characteristics.txt");
		characteristicsFile << "t             H             V             T             P\n";
        double sumT=0, sumH =0, sumP =0 ;
        const int iterCount = s_o + s_d;
        for(int ii =0; ii<iterCount; ++ii)
        {
            m_MomentumSystem.Update(m_PressureSystem.GetForce(), m_VanDeWaalsSystem.GetForce(), tau);
            m_PositionSystem.Update(m_MomentumSystem.GetMomentums(), tau, m_Mass);

			m_PressureSystem.Update(m_PositionSystem.GetPositions(), m_N, m_L, m_F);
			m_VanDeWaalsSystem.Update(m_PositionSystem.GetPositions(), m_N, m_Epsilon);

            m_MomentumSystem.Update(m_PressureSystem.GetForce(), m_VanDeWaalsSystem.GetForce(), tau);

			
            if(ii % s_out == 0 || ii > s_o)
            {
                const double pressure = m_PressureSystem.CalcPressure(m_L);
                const double kineticEnergy = m_MomentumSystem.CalcKineticEnergy(m_Mass);
                const double temperature = CalcTemperature(kineticEnergy);
                const double potential = CalcPotential();
                const double H = kineticEnergy + potential;
                if (ii % s_out == 0)
				{
					characteristicsFile << (tau * ii) << "\t" << H << "\t" << kineticEnergy << "\t" << potential << "\t" << temperature << "\t" << pressure << "\n";
                    printf("H: %f   V: %2.6f   Ek: %2.6f   T: %2.6f   P: %2.6f \n", H, potential, kineticEnergy, temperature, pressure);
                }
                else
				{
					sumT += temperature;
					sumH += H;
					sumP += pressure;
				}
            }
            if( ii % s_xyz == 0)
            {
				positionFile << m_N << "\n\n";
				for (auto const& pos : m_PositionSystem.GetPositions())
				{
					positionFile <<"Ar\t"<<pos.x<<"\t"<<pos.y<<"\t"<<pos.z<<"\n";
				}
            }
            
        }
		positionFile.close();
		characteristicsFile.close();

        std::ofstream averagesFile("averages.txt");
        averagesFile <<"Average H: "<<(sumH/s_d)<<"\nAverage T: "<<(sumT/s_d)<<"\nAverage H:"<<(sumP/s_d)<<"\n";

    }

   
    inline double CalcTemperature(const double kineticEnergy)const noexcept
    {
        constexpr double k = 8.31/1000.0;
        return 2 * kineticEnergy / (3 * m_N * k);
    }
    inline double CalcPotential()const noexcept
	{
		return m_PressureSystem.CalcPotential() + m_VanDeWaalsSystem.CalcPotential();
	}
	
protected:

    PositionSystem m_PositionSystem;
    MomentumSystem m_MomentumSystem;
    PressureSystem m_PressureSystem;
    VanDeWaalsSystem m_VanDeWaalsSystem;
	

    int m_N;
    double m_Mass;
    double m_F;
    double m_L;
    double m_Epsilon;

};

#endif // CRYSTAL_H

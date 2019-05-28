#pragma once

#include "BaseSystem.h"

/* Update
	| vx    | = | 1-Ts/Td 0  0   0 0 0 0 | | vx   | + | a*Ts/Td  a*Ts/Td a*Ts/Td  a*Ts/Td | w + | 0                            |
	| vy    |   | 0 1-Ts/Td  0   0 0 0 0 | | vy   |   | -a*Ts/Td a*Ts/Td a*Ts/Td -a*Ts/Td |     | 0                            |
	| om    |   | 0  0  1-Ts/Td  0 0 0 0 | | om   |   | -b*Ts/Td b*Ts/Td -b*Ts/Td b*Ts/Td |     | 0                            |
	| x     |   | 0    0     0   1 0 0 0 | | x    |   | 0         0        0         0    |     | Ts*(vx*cos(phi)-vy*sin(phi)) |
	| y     |   | 0    0     0   0 1 0 0 | | y    |   | 0         0        0         0    |     | Ts*(vx*sin(phi)+vy*cos(phi)) |
	| phi   |   | 0    0     Ts  0 0 1 0 | | phi  |   | 0         0        0         0    |	    | 0                            |
	| null  |   | 0    0     0   0 0 0 0 | | null |   | -b        -b       b         b    |	    | 0                            |
*/

/* Output
	| null  | = | 0 0 0 0 0 0 1 | x + [] v + []
*/
// FL,FR,BL,BR

class youBotSystem : public BaseSystem {
private:
	const double dTdyn = 0.02;
	double dGeomL;
	double dGeomW;
	double dGeomR;

public:
	youBotSystem(double Tdyn, double L, double W, double R);

	unsigned int getNumOfDisturbances() const override;

	unsigned int getNumOfOutputs() const override;

	unsigned int getNumOfNoises() const override;

	unsigned int getNumOfStates() const override;

	Eigen::MatrixXd getA(double Ts) const override;

	Eigen::MatrixXd getB(double Ts) const override;

	Eigen::MatrixXd getC(double Ts) const;

	Eigen::MatrixXd getD(double Ts) const;

	Eigen::VectorXi getUpdateNonlinearXDependencies() const;

	Eigen::VectorXd UpdateNonlinearPart(double Ts, const Eigen::VectorXd& state, const Eigen::VectorXd& disturbance) const override;

	std::vector<std::string> getStateNames() const override;

	std::vector<std::string> getNoiseNames() const override;

	std::vector<std::string> getDisturbanceNames() const override;

	std::vector<std::string> getOutputNames() const override;

	std::string getName() const override;
};

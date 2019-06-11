
#include "PartialCholevski.h"

typedef Eigen::LLT<Eigen::MatrixXd>::RealScalar Real;

Eigen::MatrixXd PartialChol(Eigen::MatrixXd a, Eigen::VectorXi v) {
	const Eigen::Index n = a.rows();
	const Eigen::Index nOut = v.sum();
	Eigen::MatrixXd out = Eigen::MatrixXd(n, nOut);
	unsigned int i = 0;
	for (unsigned int j = 0; j < n; j++)
		if (v[j] == 1) {
			Real temp = a(j, j);
			for (unsigned int k = 0; k < i; k++)
				temp -= out(j, k) * out(j, k);
			if (temp <= Real(0)) {
				std::cout << std::endl << a << std::endl << std::endl;
				std::cout << std::endl << v.transpose() << std::endl << std::endl;
				throw std::runtime_error(std::string("The matrix is not positive definite or numerical error."));
			}
			out(j, i) = sqrtl(temp);
			for (unsigned int k = 0; k < n; k++) {
				if (v[k] == 1 && k < j)
					out(k, i) = 0;
				else if (k != j) {
					temp = a(k, j);
					for (unsigned int l = 0; l < i; l++)
						temp -= out(k, l)*out(j, l);
					temp /= out(j, i);
					out(k, i) = temp;
				}
			}
			i++;
		}
	return out;
}

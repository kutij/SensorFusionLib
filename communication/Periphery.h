#pragma once
#include "Application.h"

namespace SF {

	/*! \brief Class to send datamsgs from sensors, basesystem, etc
	*
	* The class is NOT thread safe! The same thread must initialize the class and call its functions.
	*/
	class Periphery {
		Sender::SenderPtr ptr;
	public:
		Periphery(const std::string& address, int hwm = 10); /*!< Constructor, address e.g..: "tcp://*:5678" */

		void SendValue(unsigned char sensorID, const Eigen::VectorXd& value,
			DataType type, Time t = Now(), OperationType source = OperationType::SENSOR); /*!< Publish a DataMsg with a given values */

		void SendVariance(unsigned char sensorID, const Eigen::MatrixXd& variance,
			DataType type, Time t = Now(), OperationType source = OperationType::SENSOR); /*!< Publish a DataMsg with a given values */

		void SendValueAndVariance(unsigned char sensorID, const Eigen::VectorXd& value,
			const Eigen::MatrixXd& variance, DataType type,
			Time t = Now(), OperationType source = OperationType::SENSOR); /*!< Publish a DataMsg with a given values */
	};

}
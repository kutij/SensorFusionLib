#pragma once
#include <zmq.hpp>
#include "Application.h"

namespace SF {

	/*! \brief Reciever implementation for ZMQ based communication
	*
	*/
	class RecieverViaZMQ : public Reciever {

		zmq::context_t context;
		zmq::socket_t socket;
		std::mutex socketguard;

		void Run(DTime Ts);

	public:
		RecieverViaZMQ();  /*!< Constructor */

		~RecieverViaZMQ();  /*!< Destructor */

		void ConnectToAddress(const std::string& address) override;
	};

}

#pragma once
#include "SystemManager.h"

class KalmanFilter : public SystemManager {
public:
	KalmanFilter(BaseSystemData data, StatisticValue state_);
	~KalmanFilter();

	void Step(double dT) override;

	typedef std::shared_ptr<KalmanFilter> KalmanFilterPtr;
};

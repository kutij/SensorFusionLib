#include "msg2buf.h"
#include "flatbuffers/flatbuffers.h"
#include "msg_generated.h"
#include <iostream>

#include "TimeUS.h"

unsigned long getTimeInMicroseconds() {
	return TimeUS().TimeInUS();
}


Buffer::Buffer(const unsigned char * buf_, size_t size_) :
	size(size_) {
	_allocandcopy(buf_);
}

Buffer::Buffer(const Buffer & buf_) :
	size(buf_.size) {
	_allocandcopy(buf_.Buf());
}

Buffer::Buffer(Buffer && o) : size(std::move(o.size)) { _allocandcopy(o.Buf()); }

Buffer::Buffer() : size(0), buf(NULL) {}

bool Buffer::isNull() const { return size == 0; }

void Buffer::print() const {
	std::cout << "Size: " << size << std::endl;
	for (size_t i = 0; i < size; i++)
		printf(" %i\n", buf[i]);
}

size_t Buffer::Size() const { return size; }

const unsigned char* Buffer::Buf() const { return buf; }

void Buffer::_allocandcopy(const unsigned char * buf_) {
	buf = new unsigned char[size];
	for (size_t i = 0; i < size; i++)
		buf[i] = buf_[i];
}

Buffer::~Buffer() { delete buf; }

Buffer& Buffer::operator=(const Buffer& buf0) {
	if (this != &buf0) {
		if (!isNull())
			delete buf;
		size = buf0.size;
		_allocandcopy(buf0.Buf());
	}
	return *this;
}

void SystemDataMsg::print() const {
	unsigned long t = getTimeInMicroseconds();
	if (contentType == EMPTY) {
		printf("EMTPY DataMsg.\n\n");
		return;
	}
	printf("SourceID: %d Content: ", sourceID);
	switch (contentType)
	{
	case TOFILTER_MEASUREMENT:
		printf("TOFILTER_MEASUREMENT");
		break;
	case TOFILTER_DISTURBANCE:
		printf("TOFILTER_DISTURBANCE");
		break;
	case FROMFILTER_FILTEREDSTATE:
		printf("FROMFILTER_FILTEREDSTATE");
		break;
	case FROMFILTER_MEASUREDOUTPUT:
		printf("FROMFILTER_MEASUREDOUTPUT");
		break;
	case FROMFILTER_PREDICTEDOUTPUT:
		printf("FROMFILTFROMFILTER_PREDICTEDOUTPUTER_FILTEREDSTATE");
		break;
	case FROMFILTER_PREDICTEDSTATE:
		printf("FROMFILTER_PREDICTEDSTATE");
		break;
	case FROMFILTER_USEDDISTURBANCE:
		printf("FROMFILTER_USEDDISTURBANCE");
		break;
	case FROMFILTER_USEDNOISE:
		printf("FROMFILTER_USEDNOISE");
		break;
	}
	printf(" Age: %f [ms]\n", (float)(t - timestamp_in_us)*1.e-3);
	if (hasValue)
		std::cout << "Value:\n" << value << std::endl;
	if (hasVariance)
		std::cout << "Variance:\n" << variance << std::endl;
	printf("\n");
}

void SystemDataMsg::SetVarianceMatrix(const Eigen::MatrixXd& m) {
	variance = m;
	hasVariance = true;
}

void SystemDataMsg::SetValueVector(const Eigen::VectorXd& v) {
	value = v;
	hasValue = true;
}

SystemDataMsg::SystemDataMsg(unsigned char ID, ContentTypes type, unsigned long timestamp_in_us_) :
	sourceID(ID), contentType(type), hasValue(false), hasVariance(false), timestamp_in_us(timestamp_in_us_) {
	if (type==SystemDataMsg::EMPTY)
		throw std::runtime_error(std::string("DataMsg::DataMsg wrong argument"));
}

SystemDataMsg::SystemDataMsg() : contentType(EMPTY), hasValue(false), hasVariance(false) {}

SystemDataMsg::SystemDataMsg(const Buffer & buf) {
	auto msg = SensorDataMsg::GetMsg(buf.Buf());

	hasValue = flatbuffers::IsFieldPresent(msg, SensorDataMsg::Msg::VT_VALUE_VECTOR);
	if (hasValue) {
		auto v = msg->value_vector();
		size_t N = v->size();
		value = Eigen::VectorXd(N);
		for (unsigned int n = 0; n < N; n++)
			value[n] = v->Get(n);
	}

	hasVariance = flatbuffers::IsFieldPresent(msg, SensorDataMsg::Msg::VT_VARIANCE_MATRIX);
	if (hasVariance) {
		auto v = msg->variance_matrix();
		size_t K = v->size();
		size_t N = static_cast<size_t>((sqrt(8 * K + 1) - 1) / 2);
		variance = Eigen::MatrixXd(N, N);
		int k = 0;
		for (unsigned int i = 0; i < N; i++)
			for (unsigned int j = i; j < N; j++) {
				variance(i, j) = v->Get(k);
				variance(j, i) = variance(i, j);
				k++;
			}
	}

	switch (msg->type()) {
	case SensorDataMsg::ValueType::ValueType_TOFILTER_MEASUREMENT:
		contentType = TOFILTER_MEASUREMENT;
		break;
	case SensorDataMsg::ValueType::ValueType_TOFILTER_DISTURBANCE:
		contentType = TOFILTER_DISTURBANCE;
		break;
	case SensorDataMsg::ValueType::ValueType_FROMFILTER_FILTEREDSTATE:
		contentType = FROMFILTER_FILTEREDSTATE;
		break;
	case SensorDataMsg::ValueType::ValueType_FROMFILTER_MEASUREDOUTPUT:
		contentType = FROMFILTER_MEASUREDOUTPUT;
		break;
	case SensorDataMsg::ValueType::ValueType_FROMFILTER_PREDICTEDOUTPUT:
		contentType = FROMFILTER_PREDICTEDOUTPUT;
		break;
	case SensorDataMsg::ValueType::ValueType_FROMFILTER_PREDICTEDSTATE:
		contentType = FROMFILTER_PREDICTEDSTATE;
		break;
	case SensorDataMsg::ValueType::ValueType_FROMFILTER_USEDDISTURBANCE:
		contentType = FROMFILTER_USEDDISTURBANCE;
		break;
	case SensorDataMsg::ValueType::ValueType_FROMFILTER_USEDNOISE:
		contentType = FROMFILTER_USEDNOISE;
		break;
	}

	timestamp_in_us = msg->timestamp_in_us();

	sourceID = msg->sensorID();
}

bool SystemDataMsg::IsEmpty() const { return contentType == EMPTY; }

bool SystemDataMsg::IsToFilter() const { return contentType == TOFILTER_DISTURBANCE || contentType == TOFILTER_MEASUREMENT; }

bool SystemDataMsg::IsFromFilter() const {
	return contentType == FROMFILTER_FILTEREDSTATE ||
		contentType == FROMFILTER_MEASUREDOUTPUT ||
		contentType == FROMFILTER_PREDICTEDOUTPUT ||
		contentType == FROMFILTER_PREDICTEDSTATE ||
		contentType == FROMFILTER_USEDDISTURBANCE ||
		contentType == FROMFILTER_USEDNOISE;
}

bool SystemDataMsg::HasValue() const { return hasValue; }

bool SystemDataMsg::HasVariance() const { return hasVariance; }

Eigen::VectorXd SystemDataMsg::Value() const { return value; }

Eigen::MatrixXd SystemDataMsg::Variance() const { return variance; }

unsigned char SystemDataMsg::SourceID() const { return sourceID; }

SystemDataMsg::ContentTypes SystemDataMsg::ContentType() const { return contentType; }

Buffer SystemDataMsg::GetMsgBuffer() const {
	flatbuffers::FlatBufferBuilder fbb(1024);

	flatbuffers::Offset<flatbuffers::Vector<float>> fbb_value;
	if (hasValue) {
		size_t N = value.size();
		float* v = new float[N];
		for (unsigned int i = 0; i < N; i++)
			v[i] = (float)value[i];
		fbb_value = fbb.CreateVector<float>(v, 4);
	}

	flatbuffers::Offset<flatbuffers::Vector<float>> fbb_variance;
	if (hasVariance) {
		unsigned int N = static_cast<unsigned int>(variance.rows());
		unsigned int K = static_cast<unsigned int>((N*(N + 1)) / 2);
		float* v = new float[K];
		int k = 0;
		for (unsigned int i = 0; i < N; i++)
			for (unsigned int j = i; j < N; j++) {
				v[k] = (float)variance(i, j);
				k++;
			}
		fbb_variance = fbb.CreateVector<float>(v, K);
	}

	SensorDataMsg::MsgBuilder msgBuilder(fbb);
	switch (contentType)
	{
	case TOFILTER_MEASUREMENT:
		msgBuilder.add_type(SensorDataMsg::ValueType_TOFILTER_MEASUREMENT);
		break;
	case TOFILTER_DISTURBANCE:
		msgBuilder.add_type(SensorDataMsg::ValueType_TOFILTER_DISTURBANCE);
		break;
	case FROMFILTER_FILTEREDSTATE:
		msgBuilder.add_type(SensorDataMsg::ValueType_FROMFILTER_FILTEREDSTATE);
		break;
	case FROMFILTER_MEASUREDOUTPUT:
		msgBuilder.add_type(SensorDataMsg::ValueType_FROMFILTER_MEASUREDOUTPUT);
		break;
	case FROMFILTER_PREDICTEDOUTPUT:
		msgBuilder.add_type(SensorDataMsg::ValueType_FROMFILTER_PREDICTEDOUTPUT);
		break;
	case FROMFILTER_PREDICTEDSTATE:
		msgBuilder.add_type(SensorDataMsg::ValueType_FROMFILTER_PREDICTEDSTATE);
		break;
	case FROMFILTER_USEDDISTURBANCE:
		msgBuilder.add_type(SensorDataMsg::ValueType_FROMFILTER_USEDDISTURBANCE);
		break;
	case FROMFILTER_USEDNOISE:
		msgBuilder.add_type(SensorDataMsg::ValueType_FROMFILTER_USEDNOISE);
		break;
	}

	msgBuilder.add_sensorID(sourceID);

	if (hasValue)
		msgBuilder.add_value_vector(fbb_value);

	if (hasVariance)
		msgBuilder.add_variance_matrix(fbb_variance);

	msgBuilder.add_timestamp_in_us(timestamp_in_us);

	auto msg = msgBuilder.Finish();
	fbb.Finish(msg);

	return Buffer(fbb.GetBufferPointer(), fbb.GetSize());
}

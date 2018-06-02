#include "stdafx.h"
#include "DeviceIdleOptions.h"

const DeviceIdleOptions DeviceIdleOptions::Default(std::chrono::seconds(5), true, TimeUnit::minutes);

DeviceIdleOptions::DeviceIdleOptions(clock::duration timeout, bool disconnect, TimeUnit unit)
{
	this->Timeout    = timeout;
	this->Disconnect = disconnect;
	this->Unit       = unit;
}

DeviceIdleOptions::DeviceIdleOptions(const DeviceIdleOptions& other)
{
	Timeout    = other.Timeout;
	Disconnect = other.Disconnect;
	Unit       = other.Unit;
}

bool DeviceIdleOptions::operator==(const DeviceIdleOptions& other) const
{
	return Disconnect == other.Disconnect && Timeout == other.Timeout;
}

void DeviceIdleOptions::readJson(const QJsonObject& json)
{
	this->Timeout    = std::chrono::milliseconds(json["timeout"].toInt());
	this->Disconnect = json["disconnect"].toBool();
	this->Unit       = TimeUnit::_from_string(json["unit"].toString().toStdString().c_str()); // this is disgusting
}

void DeviceIdleOptions::writeJson(QJsonObject& json) const
{
	json["timeout"]    = this->Timeout.count();
	json["disconnect"] = this->Disconnect;
	json["unit"]       = this->Unit._to_string();
}
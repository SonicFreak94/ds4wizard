#pragma once

#include <array>
#include <map>
#include <mutex>
#include <string>

#include <hid_instance.h>
#include "Ds4Device.h"
#include "EventHandler.h"

class DeviceOpenedEventArgs
{
public:
	/**
	 * \brief 
	 * The device that triggered the event.
	 */
	const std::shared_ptr<Ds4Device> device;

	/**
	 * \brief 
	 * \c true if this is the first connection for this
	 * device, \c false if an additional connection has
	 * been added to this device.
	 */
	const bool unique;

	DeviceOpenedEventArgs(std::shared_ptr<Ds4Device> device, bool unique)
		: device(std::move(device)),
		  unique(unique)
	{
	}
};

class DeviceClosedEventArgs
{
public:
	/**
	 * \brief 
	 * The device that triggered the event.
	 */
	const std::shared_ptr<Ds4Device> device;

	explicit DeviceClosedEventArgs(std::shared_ptr<Ds4Device> device)
		: device(std::move(device))
	{
	}
};

class Ds4DeviceManager
{
	std::recursive_mutex sync_lock, devices_lock;

public:
	std::map<std::wstring, std::shared_ptr<Ds4Device>> devices;

	/**
	 * \brief 
	 * Defines the DualShock 4 Vendor ID.
	 */
	static const short vendorId = 0x54c;

	/**
	 * \brief 
	 * Defines validated DualShock 4 Product IDs.
	 */
	inline static const std::array<short, 3> productIds = { 0xba0, 0x5c4, 0x9cc };

	/**
	 * \brief 
	 * Fired when a device is opened.
	 * \sa DeviceOpenedEventArgs
	 */
	EventHandler<Ds4DeviceManager, DeviceOpenedEventArgs> deviceOpened;

	/**
	 * \brief 
	 * Fired when a device is closed.
	 * \sa DeviceClosedEventArgs
	 */
	EventHandler<Ds4DeviceManager, DeviceClosedEventArgs> deviceClosed;

	~Ds4DeviceManager();

	/**
	 * \brief 
	 * Checks if a device is a DualShock 4.
	 * \param hid The HID instance to be checked.
	 * \return \c true if this device is a DualShock 4.
	 */
	static bool isDs4(const hid::HidInstance& hid);

	/**
	 * \brief 
	 * Checks if a device is a DualShock 4.
	 * \param devicePath The path to the device to be checked.
	 * \return \c true if this device is a DualShock 4.
	 */
	static bool isDs4(const std::wstring& devicePath);

	/**
	 * \brief 
	 * Discovers and connects to all DualShock 4 devices.
	 */
	void findControllers();

	void findController(const std::wstring& devicePath);

	size_t deviceCount();

	std::unique_lock<std::recursive_mutex> lockDevices();

private:
	bool handleDevice(hid::HidInstance& hid);
	void onDs4DeviceClosed(Ds4Device* sender, std::shared_ptr<EventArgs> eventArgs);

public:
	/**
	 * \brief 
	 * Closes and removes all managed devices.
	 */
	void close();

	/**
	 * \brief Automatically prompts for elevation and toggles a device.
	 * \param instanceId The instance ID of the device to toggle.
	 */
	static void toggleDevice(const std::wstring& instanceId);
};

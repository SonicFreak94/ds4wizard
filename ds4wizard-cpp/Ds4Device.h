#pragma once

#include <chrono>
#include <cstdint>
#include <memory>
#include <thread>

#include "DeviceSettings.h"
#include "DeviceProfile.h"
#include "hid_instance.h"
#include "Stopwatch.h"
#include "Ds4Input.h"
#include "Ds4Output.h"
#include "Event.h"

#include "Latency.h"
#include "InputSimulator.h"

#include <windows.h>
#include <mmdeviceapi.h>
#include <endpointvolume.h>

class Ds4Device
{
	bool peak_max = false;
	size_t peak_i = 0;
	std::array<float, 5> peak_f {};
	float peak_last = 0.0f;
	Stopwatch peak_sw;
	
	bool peakedLatencyThreshold = false;
	std::string macAddress_;
	std::string safeMacAddress_;

	// HACK: work around for what may or may not be a bug with recursive_mutex;
	// the main thread fails to acquire a lock when the mutex is UNLOCKED,
	// and I suspect it might be due to async file IO that is used with hid.
	bool running = false;
	std::recursive_mutex sync_lock;

	// Read-Only
	Stopwatch idleTime {};

	inline static const Ds4Color fadeColor {};

	ptrdiff_t colorIndex = -1;

	Latency readLatency;
	Latency writeLatency;

	bool dataReceived = false;

	std::unique_ptr<std::thread> deviceThread = nullptr;

	std::unique_ptr<hid::HidInstance> usbDevice;
	std::unique_ptr<hid::HidInstance> bluetoothDevice;

	Ds4LightOptions activeLight;

	bool disconnectOnIdle() const;
	std::chrono::microseconds idleTimeout() const;
	bool isIdle() const;

	InputSimulator simulator;

	// Delta time (for things like mouse movement).
	// Assumes 1000 Hz virtual polling rate.
	Stopwatch deltaStopwatch {};

	IMMDeviceEnumerator* pEnumerator = nullptr;
	IMMDevice* pDevice = nullptr;
	IAudioMeterInformation* pMeterInfo = nullptr;

public:
	enum class BluetoothDisconnectReason
	{
		none,
		idle
	};

	// Delta time (for things like mouse movement).
	// Assumes 1000 Hz virtual polling rate.
	float deltaTime = 1.0f;

	Event<Ds4Device> onDeviceClosed;
	Event<Ds4Device> onBatteryLevelChanged;
	Event<Ds4Device> onBluetoothExclusiveFailure;
	Event<Ds4Device> onBluetoothConnected;
	Event<Ds4Device> onBluetoothIdleDisconnect;
	Event<Ds4Device> onBluetoothDisconnected;
	Event<Ds4Device> onUsbExclusiveFailure;
	Event<Ds4Device> onUsbConnected;

	// value, threshold
	Event<Ds4Device, std::chrono::milliseconds, std::chrono::milliseconds> onLatencyThresholdExceeded;

	DeviceSettings settings;
	DeviceProfile profile;

	Ds4Input input {};
	Ds4Output output {};

	bool bluetoothConnected();
	bool usbConnected();
	bool connected();

	const std::string& macAddress() const;
	const std::string& safeMacAddress() const;

	/**
	 * \brief Gets the battery charge level in the range 1 to 10.
	 */
	uint8_t battery() const;

	/**
	 * \brief Indicates if the controller is charging.
	 */
	bool charging() const;

	const std::string& name() const;

	Ds4Device();
	explicit Ds4Device(hid::HidInstance& device);
	~Ds4Device();

	void open(hid::HidInstance& device);

	std::unique_lock<std::recursive_mutex> lock();

	Latency getReadLatency();
	Latency getWriteLatency();

	void resetReadLatencyPeak();
	void resetWriteLatencyPeak();

private:
	void closeImpl();

public:
	void saveSettings();
	void applySettings(const DeviceSettings& newSettings);

	/**
	 * \brief Applies changes made to the device profile and opens device handles.
	 */
	void applyProfile();

private:
	void releaseAutoColor();

public:
	void onProfileChanged(const std::string& newName);
	void close();

private:
	void closeBluetoothDevice();

public:
	void disconnectBluetooth(BluetoothDisconnectReason reason);

private:
	void closeUsbDevice();
	static bool openDevice(hid::HidInstance& device, bool exclusive);

public:
	void openBluetoothDevice(hid::HidInstance& device);
	void openUsbDevice(hid::HidInstance& device);

private:
	void setupBluetoothOutputBuffer() const;
	void setupUsbOutputBuffer() const;
	void writeUsbAsync();
	void writeBluetooth();
	void run();
	void controllerThread();

public:
	void start();
};

#pragma once

#include <string>
#include <list>
#include <unordered_map>
#include <mutex>
#include <optional>

#include "DeviceSettings.h"
#include "DeviceProfile.h"
#include "program.h"
#include "Ds4DeviceManager.h"

class DeviceProfileCache
{
	std::shared_ptr<Ds4DeviceManager> deviceManager;
	std::unordered_map<std::string, DeviceSettings> deviceSettings;

public:
	std::recursive_mutex deviceManager_lock;
	std::recursive_mutex profiles_lock;
	std::recursive_mutex deviceSettings_lock;
	std::recursive_mutex devices_lock;
	std::list<DeviceProfile> profiles;

	// TODO: event EventHandler Loaded;
	// TODO: event EventHandler ProfileChanged;

	DeviceProfileCache() = default;

	void setDevices(const std::shared_ptr<Ds4DeviceManager>& deviceManager);

	void load();

	/// <summary>
	/// Get a profile copy by name.
	/// </summary>
	/// <param name="profileName">The name of the profile to get.</param>
	/// <returns>A copy of the profile if found, else nullptr.</returns>
	std::optional<DeviceProfile> getProfile(const std::string& profileName);

	/// <summary>
	/// Returns a copy of the cached settings for the specified MAC address.
	/// </summary>
	/// <param name="id">The MAC address of the device whose settings are to be copied.</param>
	/// <returns>The settings associated with the MAC address, or nullptr if none.</returns>
	std::optional<DeviceSettings> getSettings(const std::string& id);

	/// <summary>
	/// Adds (or replaces) settings for the specified MAC address, then saves changes to disk.
	/// </summary>
	/// <param name="id">The MAC address of the device whose settings are being stored.</param>
	/// <param name="settings">The settings to be stored.</param>
	void saveSettings(const std::string& id, const DeviceSettings& settings);

	/// <summary>
	/// Removes a profile from the profile cache.
	/// </summary>
	/// <param name="profile">The profile to be removed.</param>
	void removeProfile(const DeviceProfile& profile);

	/// <summary>
	/// Updates a profile and notifies all devices of the change.
	/// </summary>
	/// <param name="last">The profile to be replaced.</param>
	/// <param name="current">The new profile.</param>
	void updateProfile(const DeviceProfile& last, const DeviceProfile& current);

private:
	std::optional<DeviceProfile> findProfile(const std::string& profileName);
	void loadImpl();
	void onProfileChanged(const std::string& oldName, const std::string& newName);
};

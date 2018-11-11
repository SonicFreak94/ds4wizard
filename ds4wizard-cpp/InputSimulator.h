#pragma once

#include <deque>
#include <functional>
#include <mutex>

#include "KeyboardSimulator.h"
#include "MouseSimulator.h"
#include "enums.h"
#include "Pressable.h"
#include "InputMap.h"
#include "ScpDevice.h"

class Ds4Device;

class InputSimulator
{
	static constexpr Ds4Buttons_t touchMask = Ds4Buttons::touch1 | Ds4Buttons::touch2;
	static std::recursive_mutex s_scpLock;
	static std::shared_ptr<ScpDevice> s_scpDevice;

	Ds4Device* parent = nullptr;
	DeviceProfile* profile = nullptr;

	KeyboardSimulator keyboard;
	MouseSimulator mouse;

	std::deque<InputMap*> map_cache; // TODO: OPTIMIZE !!!
	std::deque<Ds4TouchRegion*> touchRegions;

	int realXInputIndex = 0;
	XInputGamepad xpad {};
	XInputGamepad last_xpad {};
	std::shared_ptr<ScpDevice> scpDevice;
	XInputAxis_t simulatedXInputAxis = 0;

public:
	Event<Ds4Device> onScpXInputHandleFailure;

	InputSimulator() = delete;
	InputSimulator(const InputSimulator&) = delete;
	InputSimulator(InputSimulator&&) = delete;

	InputSimulator& operator=(const InputSimulator&) = delete;
	InputSimulator& operator=(InputSimulator&&) = delete;

	explicit InputSimulator(Ds4Device* parent);
	~InputSimulator();

	void simulateXInputButton(XInputButtons_t buttons, PressedState state);
	void simulateXInputAxis(XInputAxes& axes, float m);
	void getActive(InputMapBase& map);
	bool isOverriddenByModifierSet(InputMapBase& map);
	void runMap(InputMap& m, InputModifier* modifier);
	void applyProfile(DeviceProfile* profile);
	void readXInput() const;
	static PressedState handleTouchToggle(InputMap& m, InputModifier* modifier, const Pressable& pressable);
	void applyMap(InputMap& m, InputModifier* modifier, PressedState state, float analog);
	void simulateMouse(const InputMap& m, PressedState state, float analog);
	void simulateKeyboard(const InputMap& m, PressedState state);
	void runAction(ActionType action) const;
	void runMaps();
	void runPersistent();
	void updateTouchRegions();
	void updateTouchRegion(Ds4TouchRegion& region, InputModifier* modifier, Ds4Buttons_t sender, Ds4Vector2& point, Ds4Buttons_t& disallow);
	void updatePressedStateImpl(InputMapBase& instance, const std::function<void()>& press, const std::function<void()>& release);
	void updatePressedState(InputModifier& modifier);
	void updatePressedState(InputMap& map, InputModifier* modifier);
	void updateBindingState(InputMap& m, InputModifier* modifier);

private:
	bool scpConnect();
	void scpDisconnect() const;

	static bool scpDeviceOpen();
	static void scpDeviceClose();
};

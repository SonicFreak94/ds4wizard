#include "pch.h"

#include <ViGEm/Client.h>

#include "ViGEmTarget.h"
#include "ViGEmDriver.h"

namespace vigem
{
	std::recursive_mutex XInputTarget::static_mutex;
	std::unordered_map<PVIGEM_TARGET, XInputTarget*> XInputTarget::targets;

	XInputTarget::XInputTarget(Driver* parent)
		: parent(parent)
	{
		target = vigem_target_x360_alloc();

		if (target == nullptr)
		{
			throw std::runtime_error("Unable to allocate XInput ViGEm target.");
		}
	}

	XInputTarget::XInputTarget(XInputTarget&& rhs) noexcept
		: parent(rhs.parent),
		  target(rhs.target),
		  connected_(rhs.connected_)
	{
		rhs.target = nullptr;
	}

	XInputTarget::~XInputTarget()
	{
		close();
	}

	VIGEM_ERROR XInputTarget::connect()
	{
		if (connected())
		{
			return VIGEM_ERROR_NONE;
		}

		VIGEM_ERROR result = vigem_target_add(parent->client, target);

		if (!VIGEM_SUCCESS(result))
		{
			return result;
		}

		connected_ = true;

		result = vigem_target_x360_register_notification(parent->client, target, &XInputTarget::raiseEvent);

		if (!VIGEM_SUCCESS(result))
		{
			disconnect();
		}
		else
		{
			static_mutex.lock();
			targets[target] = this;
			static_mutex.unlock();
		}

		return result;
	}

	bool XInputTarget::connected() const
	{
		return connected_;
	}

	VIGEM_ERROR XInputTarget::disconnect()
	{
		if (!connected())
		{
			return VIGEM_ERROR_NONE;
		}

		const VIGEM_ERROR result = vigem_target_remove(parent->client, target);

		if (VIGEM_SUCCESS(result))
		{
			connected_ = false;

			vigem_target_x360_unregister_notification(target);

			static_mutex.lock();
			targets.erase(target);
			static_mutex.unlock();
		}

		return result;
	}

	void XInputTarget::close()
	{
		disconnect();

		if (target)
		{
			vigem_target_free(target);
			target = nullptr;
		}
	}

	void XInputTarget::raiseEvent(PVIGEM_CLIENT /*client*/, PVIGEM_TARGET target, uint8_t largeMotor, uint8_t smallMotor, uint8_t ledNumber)
	{
		XInputTarget* xtarget = nullptr;

		static_mutex.lock();
		xtarget = targets[target];
		static_mutex.unlock();

		xtarget->raiseEvent(largeMotor, smallMotor, ledNumber);
	}

	void XInputTarget::raiseEvent(uint8_t largeMotor, uint8_t smallMotor, uint8_t ledNumber)
	{
		notification.invoke(this, largeMotor, smallMotor, ledNumber);
	}
}

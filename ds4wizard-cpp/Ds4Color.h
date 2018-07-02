#pragma once

#include <cstdint>
#include "JsonData.h"

struct Ds4Color : JsonData
{
	uint8_t red   = 0;
	uint8_t green = 0;
	uint8_t blue  = 0;

	Ds4Color() = default;
	Ds4Color(uint8_t r, uint8_t g, uint8_t b);
	Ds4Color(const Ds4Color& color);

	bool operator==(const Ds4Color& other) const;
	bool operator!=(const Ds4Color& other) const;

	static Ds4Color lerp(const Ds4Color& a, const Ds4Color& b, float f);
	void readJson(const nlohmann::json& json) override;
	void writeJson(nlohmann::json& json) const override;
};

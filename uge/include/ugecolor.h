#pragma once


struct ugeColor32
{
	uint32_t argb{};

	constexpr ugeColor32() : argb(0) {}

	constexpr ugeColor32(uint32_t argb_) : argb(argb_) {} // NOLINT(google-explicit-constructor)

	constexpr ugeColor32(uint8_t a, uint8_t r, uint8_t g, uint8_t b) : argb(
		(uint32_t(a) << 24) + (uint32_t(r) << 16) + (uint32_t(g) << 8) + uint32_t(b)
	) {}

	static constexpr ugeColor32 WHITE() { return { 0xFFFFFFFF }; }

	static constexpr ugeColor32 GREY() { return { 0xCCCCCCCC }; }

	static constexpr ugeColor32 BLACK() { return { 0x00000000 }; }
};
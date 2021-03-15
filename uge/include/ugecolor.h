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

//
// HGE Blending constants
//
enum ugeBlendMode : uint32_t {
    BLEND_COLORADD = 1 << 0, BLEND_COLORMUL = 0,
    BLEND_ALPHABLEND = 1 << 1, BLEND_ALPHAADD = 0,
    BLEND_ZWRITE = 1 << 2, BLEND_NOZWRITE = 0,

    // Darken does real color multiplication, white source pixels don't change destination, while
    // black source pixels make destination completely black
    // Use example: http://relishgames.com/forum/index.php?p=/discussion/5799/darken-screen-plus-uneffected-hole/p1
    BLEND_DARKEN = 1 << 3,
    BLEND_BLACKEN = BLEND_DARKEN, /* synonym for darken */
    BLEND_DEFAULT = (BLEND_COLORMUL | BLEND_ALPHABLEND | BLEND_NOZWRITE),
    BLEND_DEFAULT_Z = (BLEND_COLORMUL | BLEND_ALPHABLEND | BLEND_ZWRITE),
};
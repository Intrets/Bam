#include "common.h"

#include "UIOConstructButtons.h"

#include "UIOButton.h"
#include "UIOSimpleTextDisplay.h"
#include "UIOPad.h"
#include "UIOConstrainSize.h"
#include "UIOSizeType.h"

std::pair<UniqueReference<UIOBase, UIOBase>, UIOButton*> constructButtonWithText(std::string text, int32_t padding) {
	auto& refMan = Locator<ReferenceManager<UIOBase>>::ref();

	auto centeredText = refMan.makeUniqueRef<UIOConstrainSize>(refMan.makeUniqueRef<UIOSimpleTextDisplay>(text));
	centeredText.get()->alignment = CONSTRAIN_ALIGNMENT::CENTER;

	auto button = refMan.makeUniqueRef<UIOButton>(std::move(centeredText));
	auto buttonPtr = button.get();

	auto paddedButton = refMan.makeUniqueRef<UIOPad>(std::move(button), UIOSizeType(UIOSizeType::PX, padding));

	return { std::move(paddedButton), buttonPtr };
}

std::pair<UniqueReference<UIOBase, UIOBase>, UIOButton*> constructButtonWithText(std::string text, glm::ivec2 size, int32_t padding) {
	auto& refMan = Locator<ReferenceManager<UIOBase>>::ref();

	auto centeredText = refMan.makeUniqueRef<UIOConstrainSize>(refMan.makeUniqueRef<UIOSimpleTextDisplay>(text));
	centeredText.get()->alignment = CONSTRAIN_ALIGNMENT::CENTER;

	auto button = refMan.makeUniqueRef<UIOButton>(std::move(centeredText));
	auto buttonPtr = button.get();

	auto paddedButton = refMan.makeUniqueRef<UIOPad>(std::move(button), UIOSizeType(UIOSizeType::PX, padding));

	auto sizedButton = refMan.makeUniqueRef<UIOConstrainSize>(std::move(paddedButton));
	sizedButton.get()->maybeWidth = UIOSizeType(UIOSizeType::PX, size.x);
	sizedButton.get()->maybeHeight = UIOSizeType(UIOSizeType::PX, size.y);

	return { std::move(sizedButton), buttonPtr };
}

std::pair<UniqueReference<UIOBase, UIOConstrainSize>, UIOButton*> constructButton(glm::ivec2 size, int32_t padding) {
	auto& refMan = Locator<ReferenceManager<UIOBase>>::ref();
	if (padding == 0) {
		auto button = refMan.makeUniqueRef<UIOButton>();
		auto sizedButton = refMan.makeUniqueRef<UIOConstrainSize>(std::move(button));
		sizedButton.get()->maybeWidth = UIOSizeType(UIOSizeType::PX, size.x);
		sizedButton.get()->maybeHeight = UIOSizeType(UIOSizeType::PX, size.y);
		return { std::move(sizedButton), button.get() };
	}
	else {
		auto button = refMan.makeUniqueRef<UIOButton>();
		auto paddedButton = refMan.makeUniqueRef<UIOPad>(std::move(button), UIOSizeType(UIOSizeType::PX, padding));
		auto sizedButton = refMan.makeUniqueRef<UIOConstrainSize>(std::move(paddedButton));
		sizedButton.get()->maybeWidth = UIOSizeType(UIOSizeType::PX, size.x);
		sizedButton.get()->maybeHeight = UIOSizeType(UIOSizeType::PX, size.y);
		return { std::move(sizedButton), button.get() };
	}
}

std::pair<UniqueReference<UIOBase, UIOBase>, UIOButton*> constructButton(int32_t padding) {
	auto& refMan = Locator<ReferenceManager<UIOBase>>::ref();
	if (padding == 0) {
		auto button = refMan.makeUniqueRef<UIOButton>();
		return { std::move(button), button.get() };
	}
	else {
		auto button = refMan.makeUniqueRef<UIOButton>();
		auto paddedButton = refMan.makeUniqueRef<UIOPad>(std::move(button), UIOSizeType(UIOSizeType::PX, padding));
		return { std::move(paddedButton), button.get() };
	}
}

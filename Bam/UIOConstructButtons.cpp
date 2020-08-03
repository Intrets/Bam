#include "common.h"

#include "UIOConstructButtons.h"

#include "UIOButton.h"
#include "UIOSimpleTextDisplay.h"
#include "UIOPad.h"
#include "UIOConstrainSize.h"

std::pair<UniqueReference<UIOBase, UIOBase>, UIOButton*> constructButtonWithText(std::string text) {
	auto& refMan = Locator<ReferenceManager<UIOBase>>::ref();

	auto centeredText = refMan.makeUniqueRef<UIOConstrainSize>(refMan.makeUniqueRef<UIOSimpleTextDisplay>(text));
	centeredText.get()->alignment = CONSTRAIN_ALIGNMENT::CENTER;

	auto button = refMan.makeUniqueRef<UIOButton>(std::move(centeredText));
	auto buttonPtr = button.get();

	auto paddedButton = refMan.makeUniqueRef<UIOPad>(std::move(button), UIOSizeType(UIOSizeType::PX, 5));

	return { std::move(paddedButton), buttonPtr };
}

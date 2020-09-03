#pragma once

#include "UIOBase.h"
#include "UIOTextDisplay.h"

template<class>
class UIOConstructer;

namespace TextConstructer
{
	UIOConstructer<UIOTextDisplay> constructTextEdit(std::string const text);
	UIOConstructer<UIOTextDisplay> constructTextEdit(std::vector<std::string> text);

	UIOConstructer<UIOTextDisplay> constructSingleLineTextEdit(std::string text);

	UIOConstructer<UIOTextDisplay> constructDisplayText(std::string text);

	UIOConstructer<UIOTextDisplay> constructSingleLineDisplayText(std::string text, bool shrink = true);
}

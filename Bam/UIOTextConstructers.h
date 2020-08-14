#pragma once

#include "UIOBase.h"
#include "UIOTextDisplay.h"

template<class>
class UIOConstructer;

UIOConstructer<UIOTextDisplay> constructTextEdit(std::string text);

UIOConstructer<UIOTextDisplay> constructSingleLineTextEdit(std::string text);

UIOConstructer<UIOTextDisplay> constructDisplayText(std::string text);

UIOConstructer<UIOTextDisplay> constructSingleLineDisplayText(std::string text);



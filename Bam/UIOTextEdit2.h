#pragma once

#include "UIOBase.h"
#include "UIOTextDisplay.h"
#include "UIOConstructer.h"


UniqueReference<UIOBase, UIOTextDisplay> constructTextEdit(std::string text = "");

UIOConstructer<UIOTextDisplay> constructTextEdit2(std::string text);


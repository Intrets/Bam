#pragma once

#include "UIOBase.h"
#include "UIOTextDisplay.h"


UniqueReference<UIOBase, UIOTextDisplay> constructTextEdit(std::string text = "");


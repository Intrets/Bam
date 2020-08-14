#pragma once

#include "ReferenceManager.h"

class UIOBase;
class UIOButton;
class UIOConstrainSize;
class UIOPad;

template<class>
class UIOConstructer;

std::pair<UniqueReference<UIOBase, UIOBase>, UIOButton*> constructButtonWithText(std::string text, int32_t padding = 0);

std::pair<UniqueReference<UIOBase, UIOBase>, UIOButton*> constructButtonWithText(std::string text, glm::ivec2 size, int32_t padding = 0);

std::pair<UniqueReference<UIOBase, UIOConstrainSize>, UIOButton*> constructButton(glm::ivec2 size, int32_t padding = 0);
std::pair<UniqueReference<UIOBase, UIOBase>, UIOButton*> constructButton(int32_t padding = 0);

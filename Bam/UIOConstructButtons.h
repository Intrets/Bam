#pragma once

#include "ReferenceManager.h"

class UIOBase;
class UIOButton;

std::pair<UniqueReference<UIOBase, UIOBase>, UIOButton*> constructButtonWithText(std::string text);

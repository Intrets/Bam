#pragma once
#include "ReferenceManager.h"

class Saver;
class Activity;

bool save(Saver& saver, ReferenceManager<Activity>& manager);

#pragma once
//#include "Loader.h"
#include "ReferenceManager.h"

class Activity;
class Loader;

bool load(Loader& loader, ReferenceManager<Activity>& manager);

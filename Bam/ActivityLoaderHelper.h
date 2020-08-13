#pragma once

class Activity;
class Loader;

template<class>
class ReferenceManager;

bool load(Loader& loader, ReferenceManager<Activity>& manager);

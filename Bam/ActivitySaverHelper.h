#pragma once

class Saver;
class Activity;

template<class>
class ReferenceManager;

void save(Saver& saver, ReferenceManager<Activity>& manager);

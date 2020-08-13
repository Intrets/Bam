#pragma once

class Saver;
class Activity;

template<class>
class ReferenceManager;

bool save(Saver& saver, ReferenceManager<Activity>& manager);

#include <algorithm>
#include "MonitorObject.h"

MonitorObject::MonitorObject()
{
    objectId = 0; // Initialize the objectId or set it to a desired value
    objectType = noObject;
}

MonitorObject::~MonitorObject()
{
	std::for_each(threads.begin(), threads.end(), [](std::thread &t) {
		t.join();
	});
}
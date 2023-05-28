#ifndef MONITOROBJECT_H
#define MONITOROBJECT_H


#include <vector>
#include <thread>
#include <mutex>

enum ObjectType
{
	noObject,
	objectCamera,
	objectSensor,
};

class MonitorObject
{
public:
	MonitorObject();
	~MonitorObject();

	MonitorObject(const MonitorObject&) = delete;
	MonitorObject& operator=(const MonitorObject&) = delete;
	MonitorObject(MonitorObject&&) = delete;
	MonitorObject& operator=(MonitorObject&&) = delete;

	int getID() {return objectId;}

	virtual void simulate(){};

protected:
	int objectId;
	ObjectType objectType;
	std::vector<std::thread> threads;

private:


};

#endif
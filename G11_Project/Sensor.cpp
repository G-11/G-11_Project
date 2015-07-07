#include "Sensor.h"

Sensor* Sensor::Create(const vector2& pos,float r)
{
	Sensor* sensor = new Sensor;
	if (sensor == nullptr){ return nullptr; }

	sensor->_Pos = Vector3(pos,0);
	sensor->_Size = vector3(100.0f,100.0f,1.0f);

#ifdef _DEBUG
	sensor->_Color = WHITE(1.0f);
#else
	sensor->_Color = WHITE(0);
#endif
	
	return sensor;
}

bool Sensor::HitAffect(void)
{
	return true;
}
#include "ObjectBase.h"

bool ObjectBase::CheckCollision(ObjectBase* obj)
{
	bool result = false;

	VECTOR n = VSub(m_status.pos, obj->GetPos());
	float tR = obj->m_status.radius + m_status.radius;
	if (VSize(n) < tR)
	{
		result = true;
	}

#ifdef _DEBUG
	//DrawSphere3D(m_status.pos, m_status.radius, 16.0f, 0xffffff, 0xffffff, false);
#endif // _DEBUG

	return result;
}

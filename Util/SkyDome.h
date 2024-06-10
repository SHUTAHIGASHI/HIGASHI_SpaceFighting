#pragma once
#include<memory>
#include "Game.h"

class Model;

class SkyDome
{
public:
	SkyDome();
	~SkyDome();

	void Init(VECTOR playerPos);
	void Update(VECTOR playerPos);
	void Draw();

	void SetScale(float scale);
private:
	std::shared_ptr<Model> m_pModel;

	float m_scale;
	float m_angle = 0.0f;

	VECTOR m_dir;
};


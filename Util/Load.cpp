#include "Load.h"
#include <DxLib.h>

Load::~Load()
{
}

void Load::AllLoadData()
{
	// データをロードする
	m_data["skydome"] = MV1LoadModel("Data/ModelData/SKYDOME.mv1");
	m_data["ship"] = MV1LoadModel("Data/ModelData/SHIP.mv1");
	m_data["ufo"] = MV1LoadModel("Data/ModelData/UFO.mv1");
	m_data["rock"] = MV1LoadModel("Data/ModelData/ASTROIDS.mv1");
	m_data["item"] = MV1LoadModel("Data/ModelData/CRYSTAL.mv1");
	m_data["satellite"] = MV1LoadModel("Data/ModelData/SATELLITE.mv1");
}

void Load::DeleteAllData()
{
	// データを削除する
	for (auto& data : m_data)
	{
		MV1DeleteModel(data.second);
	}
	m_data.clear();
}

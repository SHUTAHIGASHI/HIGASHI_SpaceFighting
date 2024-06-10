#pragma once
#include "StageObjectBase.h"

class ObjectItem :
    public StageObjectBase
{
public:
    ObjectItem();
    virtual ~ObjectItem();

    void OnCollision();

private:
};


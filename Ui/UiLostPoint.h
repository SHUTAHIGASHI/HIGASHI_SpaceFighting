#pragma once
#include "UiPointBase.h"

class UiLostPoint :
    public UiPointBase
{
public:
    UiLostPoint();
    UiLostPoint(int point);
    virtual ~UiLostPoint();

    void Update();

private:
};


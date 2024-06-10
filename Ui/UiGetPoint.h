#pragma once
#include "UiPointBase.h"

class UiGetPoint :
    public UiPointBase
{
public:
    UiGetPoint();
    UiGetPoint(int point);
    virtual ~UiGetPoint();

    void Update();

private:
};


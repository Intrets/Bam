#include "common.h"

#include "UIOActivityLuaTest.h"

UIOActivityLuaTest::UIOActivityLuaTest(Handle self) {
    this->selfHandle = self;
}

ScreenRectangle UIOActivityLuaTest::updateSize(ScreenRectangle newScreenRectangle) {
    newScreenRectangle.setHeight(0.0f);
    newScreenRectangle.setWidth(0.0f);

    this->screenRectangle = newScreenRectangle;

    return this->screenRectangle;
}

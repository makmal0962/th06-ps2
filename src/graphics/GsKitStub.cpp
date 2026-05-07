#ifdef __PS2__
#include "GsKitStub.hpp"

static GsKitStub s_instance;

GfxInterface *GsKitStub::Init() {
    return &s_instance;
}
#endif
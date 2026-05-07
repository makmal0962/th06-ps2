#pragma once
#ifdef __PS2__
#include "GfxInterface.hpp"

struct GsKitStub : public GfxInterface {
    static GfxInterface *Init();
    void SetFogRange(f32, f32) override {}
    void SetFogColor(ZunColor) override {}
    void ToggleVertexAttribute(u8, bool) override {}
    void SetAttributePointer(VertexAttributeArrays, std::size_t, void *) override {}
    void SetColorOp(TextureOpComponent, ColorOp) override {}
    void SetTextureFactor(ZunColor) override {}
    void SetTransformMatrix(TransformMatrix, const ZunMatrix &) override {}
    void Draw() override {}
};
#endif
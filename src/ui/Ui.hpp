#pragma once

#include <functional>
#include "../meshrenderer/GLInfo.hpp"

class UI {
public:
    bool Init(const GLInfo &glInfo, struct GLFWwindow *window);
    void Update(uint8_t &selectedMeshIdx, bool &colorSpecified, float *meshColor);
    static void Draw();
    static void Destroy();

    UI &WithOnSelectedMeshIdxChangedCallback(std::function<void()> &&callback);
    UI &WithOnIsColorSpecifiedChangedCallback(std::function<void()> &&callback);
    UI &WithOnSpecifiedColorChangedCallback(std::function<void()> &&callback);

    float m_DeltaTime;
private:
    void MeshSection(uint8_t &selectedMeshIdx) const;
    void ColorSection(bool &specified, float *meshColor) const;
    void InfoAndMetricsSection() const;

    std::function<void()> m_pOnSelectedMeshIdxChanged, m_pOnIsColorSpecifiedChanged, m_pOnSpecifiedColorChanged;
    GLInfo m_pInfo;
};

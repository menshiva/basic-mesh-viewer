#pragma once

#include <functional>
#include "../meshrenderer/GLInfo.hpp"

class UI {
public:
    bool Init(const GLInfo &glInfo, struct GLFWwindow *window);
    void Update(bool &colorSpecified, float *meshColor);
    static void Draw();
    static void Destroy();

    UI &WithOnIsColorSpecifiedChangedCallback(std::function<void()> &&callback);
    UI &WithOnSpecifiedColorChangedCallback(std::function<void()> &&callback);

    float m_DeltaTime;
private:
    void ColorSection(bool &specified, float *meshColor) const;
    void InfoAndMetricsSection() const;

    std::function<void()> m_pOnIsColorSpecifiedChanged, m_pOnSpecifiedColorChanged;
    GLInfo m_pInfo;
};

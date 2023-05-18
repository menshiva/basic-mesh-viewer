#pragma once

#include <functional>
#include "../meshrenderer/GLInfo.hpp"

class UI {
public:
    UI();

    bool Init(const GLInfo &glInfo, struct GLFWwindow *window);
    void Update(bool &isColorSpecified, float *color);
    static void Draw();
    static void Destroy();

    bool IsColorSpecifiedChanged();
    bool IsColorChanged();

    float m_DeltaTime;
private:
    void ColorSection(bool &isSpecified, float *color);
    void InfoAndMetricsSection() const;

    GLInfo m_pInfo;
    bool m_pIsColorSpecifiedChanged, m_pColorChanged;
};

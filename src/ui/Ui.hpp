#pragma once

#include <functional>
#include "../meshrenderer/GLInfo.hpp"

class UI {
public:
    UI();

    bool Init(const GLInfo &glInfo, struct GLFWwindow *window);
    void Update(int &selectedMeshIdx, const std::vector<const char*> &meshNames, bool &isColorSpecified, float *color);
    static void Draw();
    static void Destroy();

    bool IsSelectedMeshIdxChanged() { return GetPropertyAndReset(m_pIsSelectedMeshIdxChanged); }
    bool IsColorSpecifiedChanged() { return GetPropertyAndReset(m_pIsColorSpecifiedChanged); }
    bool IsColorChanged() { return GetPropertyAndReset(m_pColorChanged); }

    float m_DeltaTime;
private:
    void TypeSection(int &selectedMeshIdx, const std::vector<const char*> &meshNames);
    void SettingsSection();
    void ColorSection(bool &isSpecified, float *color);
    void InfoAndMetricsSection() const;

    static bool GetPropertyAndReset(bool &property);

    GLInfo m_pInfo;
    bool m_pIsSelectedMeshIdxChanged, m_pIsColorSpecifiedChanged, m_pColorChanged;
};

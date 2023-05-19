#pragma once

#include <functional>
#include "../meshrenderer/GLInfo.hpp"
#include "../meshrenderer/mesh/MeshSettings.hpp"

class UI {
public:
    UI();

    bool Init(const GLInfo &glInfo, struct GLFWwindow *window);
    void Update(
        int &selectedMeshIdx, const std::vector<const char*> &meshNames,
        MeshSettings *meshSettings,
        bool &isColorSpecified, float *color
    );
    static void Draw();
    static void Destroy();

    bool IsSelectedMeshIdxChanged() { return GetPropertyAndReset(m_pIsSelectedMeshIdxChanged); }
    bool AreSelectedMeshSettingsChanged() { return GetPropertyAndReset(m_pAreSelectedMeshSettingsChanged); }
    bool IsColorSpecifiedChanged() { return GetPropertyAndReset(m_pIsColorSpecifiedChanged); }
    bool IsColorChanged() { return GetPropertyAndReset(m_pColorChanged); }

    float m_DeltaTime;
private:
    void TypeSection(int &selectedMeshIdx, const std::vector<const char*> &meshNames);
    void SettingsSection(MeshSettings *meshSettings);
    void ColorSection(bool &isSpecified, float *color);
    void InfoAndMetricsSection() const;

    static bool GetPropertyAndReset(bool &property);

    GLInfo m_pInfo;
    bool m_pIsSelectedMeshIdxChanged, m_pAreSelectedMeshSettingsChanged, m_pIsColorSpecifiedChanged, m_pColorChanged;
};

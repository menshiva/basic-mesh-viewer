#pragma once

#include <functional>
#include "../helper.hpp"

class UI {
public:
    bool Init(const HelperStructs::GLInfo &glInfo, struct GLFWwindow *window);
    void Update(uint8_t &selectedMeshIdx, float *meshColor) const;
    static void Draw();
    static void Destroy();

    UI &WithOnSelectedMeshIdxChangedCallback(std::function<void()> &&callback);
    UI &WithOnColorChangedCallback(std::function<void()> &&callback);
private:
    void MeshSection(uint8_t &selectedMeshIdx) const;
    void ColorSection(float availableParentWidth, float *meshColor) const;
    static void InfoAndMetricsSection(const HelperStructs::GLInfo &glInfo, float framerate);

    std::function<void()> m_pOnSelectedMeshIdxChanged, m_pOnColorChanged;
    HelperStructs::GLInfo m_pInfo;
};

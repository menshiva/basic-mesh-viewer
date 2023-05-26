#pragma once

#include <vector>
#include "../meshrenderer/MeshRendererUiData.hpp"

class UI {
public:
    bool Init(const GLInfo &glInfo, struct GLFWwindow *window);
    void Update(
        const std::vector<std::string_view> &meshesNames,
        UpdateParams &updateParams,
        struct MeshSettings *meshSettings,
        DrawInfo drawInfo
    );
    static void Draw();
    static void Destroy();
private:
    static float TypeSection(const std::vector<std::string_view> &meshesNames, int &selectedMeshIdx);
    static float ColorSection(UpdateParams::ColorType &colorType, glm::vec3 &specifiedColor, glm::vec3 &overrideColor);
    static void ScaleSection(float &scale);
    static void VerticesNumSection(struct Polygon2DSettings *poly2DSettings);
    void InfoAndMetricsSection(DrawInfo drawInfo) const;

    GLInfo m_pInfo;
};

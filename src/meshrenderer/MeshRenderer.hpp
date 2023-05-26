#pragma once

#include <memory>
#include "mesh/Mesh.hpp"
#include "mesh/MeshSettings.hpp"
#include "MeshRendererUiData.hpp"
#include "../config.hpp"

class MeshRenderer {
public:
    bool Init(GLInfo &infoOut);
    void Resize(int w, int h) const;
    void Update(const UpdateParams &NewParams, MeshSettings *NewMeshSettings);
    void Draw() const;
    void Destroy();

    const std::vector<std::string_view> &GetMeshesNames() const { return m_pMeshesNames; }
    UpdateParams GetParamsCopy() const { return m_pParams; }
    MeshSettings *GetMeshSettingsCopy() const { return GetSelectedMesh()->GetSettingsCopy(); }
    DrawInfo GetDrawInfo() const { return m_pDrawInfo; }
private:
    Mesh *GetSelectedMesh() const { return m_pMeshes[m_pParams.m_SelectedMeshIdx].get(); }

    void CreateMeshes();
    static void EnableVertexAttribPointers();

    void UpdateUniformsIfNeeded(const UpdateParams &NewParams, bool force = false);
    void UpdateBuffers();

    static bool FillGLInfo(GLInfo &infoOut);
    static bool CreateProgram(std::string_view path, uint32_t &programIdOut);
    static bool CompileShader(uint32_t shaderType, std::string_view path, uint32_t &shaderIdOut);

    std::vector<std::string_view> m_pMeshesNames;
    std::vector<std::unique_ptr<Mesh>> m_pMeshes;

    UpdateParams m_pParams{
        Config::SELECTED_MESH_IDX_DEFAULT,
        Config::MESH_SETTINGS_SCALE_DEFAULT,
        (UpdateParams::ColorType) Config::MESH_SETTINGS_COLOR_TYPE_DEFAULT
    };
    DrawInfo m_pDrawInfo;

    uint32_t m_pProgramId;
    int32_t m_pAspectRatioUniformLocation, m_pCenterOffsetUniformLocation;
    int32_t m_pUseOverrideColorLocation, m_pOverrideColorLocation, m_pScaleUniformLocation;
    uint32_t m_pVao, m_pVbo, m_pIbo;
};

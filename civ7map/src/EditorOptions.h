#pragma once

class EditorOptions
{
public:
    static EditorOptions & Get();

    bool Load();
    bool Save();

    float m_editorGlobalScale = 1.0f;
    int m_elevationStep = 128;
    bool m_useModdedCivsForTSL = false;
};
#pragma once

class EditorOptions
{
public:
    static EditorOptions & Get();

    bool Load();
    bool Save();

    float m_editorGlobalScale = 1.0f;
    int m_elevationStep = 128;
    int m_snowStep = 10;
    bool m_useModdedCivsForTSL = false;
};
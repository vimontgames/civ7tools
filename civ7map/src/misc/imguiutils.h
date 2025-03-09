#pragma once


void DrawColoredSquare(const float4 & _color);
bool DrawColoredCheckbox(const float4 & _color, bool * _checked);

void PushDisabled(bool _disabled);
void PopDisabled();
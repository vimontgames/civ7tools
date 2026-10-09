#pragma once

enum class ContinentType : i8;
using LandmassType = u8;
class Map;

string GetContinentShortName(const Map * map, ContinentType continent);
bool EditContinent(const Map * map, ContinentType & continent, bool * pBool = nullptr);
bool EditLandmass(const Map * map, LandmassType & landmass, bool * pBool = nullptr);

void DrawColoredSquare(const float4 & _color);
void DrawSmallColoredSquare(const float4 & _color);
bool DrawColoredCheckbox(const float4 & _color, bool * _checked);

void PushDisabled(bool _disabled);
void PopDisabled();

void ApplyDisabledStyle(bool _disabled);

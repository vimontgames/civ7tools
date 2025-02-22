#pragma once

using ShaderID = u32;
static const ShaderID invalidShaderID = (ShaderID)-1;

struct ShaderInfo
{
	char * vs;
	char * ps;
	sf::Shader * shader;
    bool duplicate = false;
};

class ShaderManager
{
public:
	static void init();
	static void deinit();
	static u32 update();

	static ShaderID add(const char * _vs, const char * _ps, bool _default = false);
	static sf::Shader * get(ShaderID _shaderID) { return invalidShaderID != _shaderID ? s_shaders[_shaderID].shader : nullptr; }

private:
	static sf::Shader * compile(const char * _vs, const char * _ps, bool _default = false);
	static ShaderID find(const char * _vs, const char * _ps);
	static bool fileToString(sf::String & _string, const char * _filepath);
	static void dump(const char * _type, const char * _path, const char * _src);

private:
	static vector<ShaderInfo> s_shaders;
	static ShaderID s_defaultShaderID;
};
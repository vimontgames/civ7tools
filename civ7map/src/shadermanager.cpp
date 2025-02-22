#include "shadermanager.h"

vector<ShaderInfo> ShaderManager::s_shaders;
ShaderID ShaderManager::s_defaultShaderID = invalidShaderID;

//--------------------------------------------------------------------------
void ShaderManager::init()
{
	s_defaultShaderID = add("data/shader/default_vs.fx", "data/shader/default_ps.fx", true);
}

//--------------------------------------------------------------------------
void ShaderManager::deinit()
{
	for (auto & info : s_shaders)
	{
        if (!info.duplicate)
		    SAFE_DELETE(info.shader);

		SAFE_FREE(info.vs);
		SAFE_FREE(info.ps);
	}
	s_shaders.clear();
}

//--------------------------------------------------------------------------
u32 ShaderManager::update()
{
	assert(invalidShaderID != s_defaultShaderID);
	auto * defaultShader = get(s_defaultShaderID);

	for (u32 i = 0; i < s_shaders.size(); ++i)
	{
		auto & info = s_shaders[i];
		const bool isDefaultShader = (i == s_defaultShaderID);
		if (isDefaultShader || info.shader != defaultShader)
		{
			SAFE_DELETE(info.shader);
		}
		info.shader = compile(info.vs, info.ps, isDefaultShader);
	}

	return 0;
}

//--------------------------------------------------------------------------
ShaderID ShaderManager::find(const char * _vs, const char * _ps)
{
	for (u32 i = 0; i < s_shaders.size(); ++i)
	{
		auto & info = s_shaders[i];
		if (!_strcmpi(info.vs, _vs) && !_strcmpi(info.ps, _ps))
		{
			return i;
		}
	}

	return invalidShaderID;
}

//--------------------------------------------------------------------------
sf::Shader * ShaderManager::compile(const char * _vs, const char * _ps, bool _default)
{
	sf::String header, vs, ps;

	fileToString(header, "data/shader/hlsl2glsl.h");
	fileToString(vs, _vs);
	fileToString(ps, _ps);

	if (_default)
	{
		vs = header + vs;
		ps = header + ps;
	}
	else
	{
        // shared helpers
		sf::String fx;
		fileToString(fx, "data/shader/fx.h");

        // shared uniforms
        sf::String common;
        fileToString(fx, "data/shader/common.h");

        vs = header + fx + common + vs;
        ps = header + fx + common + ps;
	}

	sf::Shader * shader = new sf::Shader();
	
	if (shader->loadFromMemory(vs, ps))
	{
		return shader;
	}
	else
	{
		SAFE_DELETE(shader);
		dump("VS", _vs, vs.toAnsiString().c_str());
		dump("PS", _ps, ps.toAnsiString().c_str());

		assert(s_defaultShaderID != invalidShaderID);
		return s_shaders[s_defaultShaderID].shader;
	}
}

//--------------------------------------------------------------------------
ShaderID ShaderManager::add(const char * _vs, const char * _ps, bool _default)
{
	ShaderID id = find(_vs, _ps);
	if (invalidShaderID != id)
	{
		return id;
	}

	ShaderInfo info;
			   info.vs = _strdup(_vs);
			   info.ps = _strdup(_ps);
			   info.shader = compile(_vs, _ps, _default);

    bool found = false;
    for (u32 i = 0; i < s_shaders.size(); ++i)
    {
        if (s_shaders[i].shader == info.shader)
        {
            found = true;
            info.duplicate = true;
            break;
        }
    }

    if (!found)
        info.duplicate = false;

	s_shaders.push_back(info);

	return (ShaderID)(s_shaders.size()-1);
}

//--------------------------------------------------------------------------
bool ShaderManager::fileToString(sf::String & _string, const char * _filepath)
{
	FILE * fp = nullptr;

	errno_t err = fopen_s(&fp, _filepath, "r");
	if (0 != err)
	{
		debugPrint("Failed to open file \"%s\"\n", _filepath);
		_string = "";
		return false;
	}
	else
	{
		fseek(fp, 0, SEEK_END);
		u32 size = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		static char temp[16384];
		size_t read = fread_s(&temp[0], sizeof(temp), 1, size, fp);
		temp[read] = '\0';
		fclose(fp);

		_string = sf::String((char*)&temp[0]);
		return true;
	}
}

//--------------------------------------------------------------------------
void ShaderManager::dump(const char * _type, const char * _path, const char * _src)
{
	debugPrint("\n%s = \"%s\"\n\n", _type, _path);

	const char * c = _src;
	char temp[1024];
	u32 line = 0;

	sprintf_s(temp, "%s(%u): ", _path, line++);
	size_t index = strlen(temp);

	while (*c != '\0')
	{
		temp[index++] = *c;

		if (*c == '\n')
		{
			temp[index++] = '\0';
			debugPrint(temp);

			sprintf_s(temp, "%s(%u): ", _path, line++);
			index = strlen(temp);
		}

		++c;
	}
	temp[index++] = '\n';
	temp[index++] = '\0';
	debugPrint(temp);
}
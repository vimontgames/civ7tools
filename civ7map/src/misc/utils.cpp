
//--------------------------------------------------------------------------------------
bool CreateFolder(const string & path)
{
    return CreateDirectoryA(path.c_str(), nullptr);
}

//--------------------------------------------------------------------------------------
//string GetParentFolder(const string & path)
//{
//    return std::filesystem::path(path).parent_path().string();
//}

//--------------------------------------------------------------------------------------
string GetFolder(const string & _fullpath)
{
    auto path = _fullpath.find_last_of("/\\");
    if (path != string::npos)
        return _fullpath.substr(0, path);

    return ""; // No folder found, return empty string
}

//--------------------------------------------------------------------------------------
string GetFilename(const string & _fullpath)
{
    string shortName = _fullpath;

    auto path = shortName.find_last_of("/\\");
    if (path != string::npos)
        shortName.erase(0, path + 1);

    return shortName;
}

//--------------------------------------------------------------------------------------
string GetFilenameWithoutExtension(const string & _fullpath)
{
    string shortName = _fullpath;

    auto path = shortName.find_last_of("/\\");
    if (path != string::npos)
        shortName.erase(0, path + 1);

    const size_t ext = shortName.rfind('.');
    if (ext != string::npos)
        shortName.erase(ext);

    return shortName;
}

//--------------------------------------------------------------------------------------
bool FileExists(const string & _fullpath)
{
    FILE * file = fopen(_fullpath.c_str(), "rb");
    if (file) 
    {
        fclose(file);
        return true; 
    }
    return false; 
}

//--------------------------------------------------------------------------------------
bool ReadFile(const string & _fullpath, string & _data)
{
    FILE * fp = fopen(_fullpath.c_str(), "rb");
    if (fp)
    {
        fseek(fp, 0, SEEK_END);
        size_t filesize = ftell(fp);
        rewind(fp);
        char * temp = (char *)malloc(filesize + 1);
        size_t read = fread(temp, 1, filesize, fp);
        temp[filesize] = '\0';
        fclose(fp);
        _data = (string)temp;
        free(temp);
        return true;
    }

    LOG_WARNING("Could not read file \"%s\"", _fullpath.c_str());
    _data = {};
    return false;
}

//--------------------------------------------------------------------------------------
string CapitalizeWords(const string & input)
{
    std::stringstream ss(input);
    std::string word, result;

    std::istringstream iss(input);
    while (std::getline(iss, word, '_'))
    {
        if (!word.empty())
        {
            word[0] = std::toupper(word[0]);
            for (size_t i = 1; i < word.size(); ++i)
                word[i] = std::tolower(word[i]);
            if (!result.empty()) result += " ";
            result += word;
        }
    }

    return result;
}

//--------------------------------------------------------------------------------------
bool isDigits(const string & str)
{
    return !str.empty() && all_of(str.begin(), str.end(), ::isdigit);
}

//--------------------------------------------------------------------------------------
bool BeginsWith(const string & _string, const string & _prefix)
{
    return _string.size() >= _prefix.size() && _string.compare(0, _prefix.size(), _prefix) == 0;
}

//--------------------------------------------------------------------------------------
bool EndsWith(const string & _string, const string & _suffix)
{
    return _string.size() >= _suffix.size() && _string.compare(_string.size() - _suffix.size(), _suffix.size(), _suffix) == 0;
}

//--------------------------------------------------------------------------------------
string ToUpperLabel(const string & str)
{
    string result = str;
    transform(result.begin(), result.end(), result.begin(), [](unsigned char c) 
    {
        return (std::isalpha(c)) ? std::toupper(c) : (std::isdigit(c) ? c : '_');
    });
    return result;
}

//--------------------------------------------------------------------------------------
string GetExtension(const string & _fullpath)
{
    string filename = GetFilename(_fullpath);
    size_t pos = filename.find_last_of('.');
    if (pos != string::npos && pos > 0) {
        return filename.substr(pos);
    }
    return "";
}

//--------------------------------------------------------------------------------------
bool DeleteFile(const string & path)
{
    if (remove(path.c_str()) == 0)
    {
        return true;
    }
    
    LOG_WARNING("Could not delete file \"%s\"", path.c_str());
    return false;
}

//--------------------------------------------------------------------------------------
bool CopyFile(const string & path, const string & newPath)
{
    FILE * src = fopen(path.c_str(), "rb");
    if (!src)
    {
        LOG_ERROR("Could not open file \"%s\" for copying", path.c_str());
        return false;
    }

    FILE * dst = fopen(newPath.c_str(), "wb");
    if (!dst)
    {
        fclose(src);
        LOG_ERROR("Could not create file \"%s\"", newPath.c_str());
        return false;
    }

    char buffer[64 * 1024];
    size_t bytesRead;

    while ((bytesRead = fread(buffer, 1, sizeof(buffer), src)) > 0)
    {
        if (fwrite(buffer, 1, bytesRead, dst) != bytesRead)
        {
            fclose(src);
            fclose(dst);

            LOG_ERROR("Could not write file \"%s\"", newPath.c_str());
            return false;
        }
    }

    const bool success = !ferror(src);

    fclose(src);
    fclose(dst);

    if (!success)
    {
        LOG_ERROR("Could not read file \"%s\"", path.c_str());
        return false;
    }

    LOG_INFO("File \"%s\" created from template \"%s\"", newPath.c_str(), path.c_str());
    return true;
}

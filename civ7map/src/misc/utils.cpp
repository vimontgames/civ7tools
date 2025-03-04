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
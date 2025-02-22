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
bool ReadFile(const string & _fullpath, string & _data)
{
    FILE * fp = fopen(_fullpath.c_str(), "r");
    if (fp)
    {
        fseek(fp, 0, SEEK_END);
        size_t filesize = ftell(fp);
        rewind(fp);
        char * temp = (char *)malloc(filesize + 1);
        fread(temp, 1, filesize, fp);
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
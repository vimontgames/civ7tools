#pragma once

string GetFolder(const string & _fullpath);
string GetFilename(const string & _fullpath);
string GetFilenameWithoutExtension(const string & _fullpath);

bool ReadFile(const string & path, string & _data);

#pragma once

bool CreateFolder(const string & path);
string GetParentFolder(const string & path);
string GetFolder(const string & _fullpath);
string GetFilename(const string & _fullpath);
string GetFilenameWithoutExtension(const string & _fullpath);

bool FileExists(const string & path);
bool ReadFile(const string & path, string & _data);

string CapitalizeWords(const string & input);
bool isDigits(const string & str);

bool EndsWith(const string & _string, const string & _suffix);

string ToUpperLabel(const string & str);

template <typename E> inline constexpr string asString(E value)
{
    return (string)magic_enum::enum_name(value);
}

template <typename E> inline constexpr uint enumCount()
{
    return (uint)magic_enum::enum_count<E>();
}

template <typename E> inline constexpr auto enumValues()
{
    return magic_enum::enum_entries<E>();
}























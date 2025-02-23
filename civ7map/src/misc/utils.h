#pragma once

string GetFolder(const string & _fullpath);
string GetFilename(const string & _fullpath);
string GetFilenameWithoutExtension(const string & _fullpath);

bool ReadFile(const string & path, string & _data);

string CapitalizeWords(const string & input);

template <typename E> string asString(E value)
{
    return (string)magic_enum::enum_name(value);
}

template <typename E> uint enumCount()
{
    return (uint)magic_enum::enum_count<E>();
}























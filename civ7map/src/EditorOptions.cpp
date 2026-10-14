#pragma once
#include "EditorOptions.h"
#include "winsqlite/winsqlite3.h"

//--------------------------------------------------------------------------------------
EditorOptions & EditorOptions::Get()
{
    static EditorOptions g_EditorOptions;
    return g_EditorOptions;
}

string filename = "options.sql";

//--------------------------------------------------------------------------------------
bool EditorOptions::Load()
{
    sqlite3 * db = nullptr;

    if (sqlite3_open(filename.c_str(), &db) != SQLITE_OK)
    {
        if (db)
            sqlite3_close(db);

        return false;
    }

    const char * createTableSQL =
        "CREATE TABLE IF NOT EXISTS EditorOptions ("
        "EditorGlobalScale REAL NOT NULL,"
        "ElevationStep INTEGER NOT NULL,"
        "UseModdedCivsForTSL INTEGER NOT NULL"
        ");";

    if (sqlite3_exec(db, createTableSQL, nullptr, nullptr, nullptr) != SQLITE_OK)
    {
        sqlite3_close(db);
        return false;
    }

    const char * selectSQL =
        "SELECT EditorGlobalScale, ElevationStep, UseModdedCivsForTSL "
        "FROM EditorOptions LIMIT 1;";

    sqlite3_stmt * statement = nullptr;

    if (sqlite3_prepare_v2(db, selectSQL, -1, &statement, nullptr) != SQLITE_OK)
    {
        sqlite3_close(db);
        return false;
    }

    const int result = sqlite3_step(statement);

    if (result == SQLITE_ROW)
    {
        m_editorGlobalScale = static_cast<float>(sqlite3_column_double(statement, 0));
        m_elevationStep = sqlite3_column_int(statement, 1);
        m_useModdedCivsForTSL = sqlite3_column_int(statement, 2) != 0;

        sqlite3_finalize(statement);
        sqlite3_close(db);
        return true;
    }

    sqlite3_finalize(statement);
    sqlite3_close(db);

    // No saved options: keep the default values.
    return result == SQLITE_DONE;
}

//--------------------------------------------------------------------------------------
bool EditorOptions::Save()
{
    sqlite3 * db = nullptr;

    if (sqlite3_open(filename.c_str(), &db) != SQLITE_OK)
    {
        if (db)
            sqlite3_close(db);

        return false;
    }

    const char * createTableSQL =
        "CREATE TABLE IF NOT EXISTS EditorOptions ("
        "EditorGlobalScale REAL NOT NULL,"
        "ElevationStep INTEGER NOT NULL,"
        "UseModdedCivsForTSL INTEGER NOT NULL"
        ");";

    char * errorMessage = nullptr;

    if (sqlite3_exec(db, createTableSQL, nullptr, nullptr, &errorMessage) != SQLITE_OK)
    {
        sqlite3_free(errorMessage);
        sqlite3_close(db);
        return false;
    }

    const char * deleteSQL =
        "DELETE FROM EditorOptions;";

    if (sqlite3_exec(db, deleteSQL, nullptr, nullptr, &errorMessage) != SQLITE_OK)
    {
        sqlite3_free(errorMessage);
        sqlite3_close(db);
        return false;
    }

    const char * insertSQL =
        "INSERT INTO EditorOptions "
        "(EditorGlobalScale, ElevationStep, UseModdedCivsForTSL) "
        "VALUES (?, ?, ?);";

    sqlite3_stmt * statement = nullptr;

    if (sqlite3_prepare_v2(db, insertSQL, -1, &statement, nullptr) != SQLITE_OK)
    {
        sqlite3_close(db);
        return false;
    }

    sqlite3_bind_double(statement, 1, m_editorGlobalScale);
    sqlite3_bind_int(statement, 2, m_elevationStep);
    sqlite3_bind_int(statement, 3, m_useModdedCivsForTSL ? 1 : 0);

    const bool success = sqlite3_step(statement) == SQLITE_DONE;

    sqlite3_finalize(statement);
    sqlite3_close(db);

    return success;
}

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
        {
            LOG_ERROR(fmt::sprintf("Failed to open '%s': %s", filename.c_str(), sqlite3_errmsg(db)).c_str());
            sqlite3_close(db);
        }
        else
        {
            LOG_ERROR(fmt::sprintf("Failed to open '%s'.", filename.c_str()).c_str());
        }

        return false;
    }

    const char * createTableSQL =
        "CREATE TABLE IF NOT EXISTS EditorOptions ("
        "Key TEXT PRIMARY KEY,"
        "Value TEXT NOT NULL"
        ");";

    char * errorMessage = nullptr;

    if (sqlite3_exec(db, createTableSQL, nullptr, nullptr, &errorMessage) != SQLITE_OK)
    {
        LOG_ERROR(fmt::sprintf("Failed to create EditorOptions table: %s", errorMessage ? errorMessage : sqlite3_errmsg(db)).c_str());

        sqlite3_free(errorMessage);
        sqlite3_close(db);
        return false;
    }

    const char * selectSQL =
        "SELECT Key, Value FROM EditorOptions;";

    sqlite3_stmt * statement = nullptr;

    if (sqlite3_prepare_v2(db, selectSQL, -1, &statement, nullptr) != SQLITE_OK)
    {
        LOG_ERROR(fmt::sprintf("Failed to prepare EditorOptions SELECT: %s", sqlite3_errmsg(db)).c_str());

        sqlite3_close(db);
        return false;
    }

    while (true)
    {
        const int result = sqlite3_step(statement);

        if (result == SQLITE_DONE)
            break;

        if (result != SQLITE_ROW)
        {
            LOG_ERROR(fmt::sprintf("Failed to read EditorOptions: %s", sqlite3_errmsg(db)).c_str());

            sqlite3_finalize(statement);
            sqlite3_close(db);
            return false;
        }

        const char * key = reinterpret_cast<const char *>(sqlite3_column_text(statement, 0));
        const char * value = reinterpret_cast<const char *>(sqlite3_column_text(statement, 1));

        if (!key || !value)
        {
            LOG_WARNING("Ignoring EditorOptions entry with missing key or value.");
            continue;
        }

        if (strcmp(key, "EditorGlobalScale") == 0)
            m_editorGlobalScale = static_cast<float>(atof(value));
        else if (strcmp(key, "ElevationStep") == 0)
            m_elevationStep = atoi(value);
        else if (strcmp(key, "SnowStep") == 0)
            m_snowStep = atoi(value);
        else if (strcmp(key, "UseModdedCivsForTSL") == 0)
            m_useModdedCivsForTSL = atoi(value) != 0;
        else
            LOG_WARNING(fmt::sprintf("Unknown EditorOption '%s'.", key).c_str());
    }

    sqlite3_finalize(statement);
    sqlite3_close(db);

    return true;
}

//--------------------------------------------------------------------------------------
bool EditorOptions::Save()
{
    sqlite3 * db = nullptr;

    if (sqlite3_open(filename.c_str(), &db) != SQLITE_OK)
    {
        if (db)
        {
            LOG_ERROR(fmt::sprintf("Failed to open '%s': %s", filename.c_str(), sqlite3_errmsg(db)).c_str());
            sqlite3_close(db);
        }
        else
        {
            LOG_ERROR(fmt::sprintf("Failed to open '%s'.", filename.c_str()).c_str());
        }

        return false;
    }

    // If an old version of options.sql exists, its EditorOptions table has the old
    // column-based format. Drop it so it can be recreated using the new Key/Value format.
    const char * createTableSQL =
        "DROP TABLE IF EXISTS EditorOptions;"
        "CREATE TABLE EditorOptions ("
        "Key TEXT PRIMARY KEY,"
        "Value TEXT NOT NULL"
        ");";

    char * errorMessage = nullptr;

    if (sqlite3_exec(db, createTableSQL, nullptr, nullptr, &errorMessage) != SQLITE_OK)
    {
        LOG_ERROR(fmt::sprintf("Failed to create EditorOptions table: %s", errorMessage ? errorMessage : sqlite3_errmsg(db)).c_str());

        sqlite3_free(errorMessage);
        sqlite3_close(db);
        return false;
    }

    const char * insertSQL =
        "INSERT OR REPLACE INTO EditorOptions (Key, Value) "
        "VALUES (?, ?);";

    sqlite3_stmt * statement = nullptr;

    if (sqlite3_prepare_v2(db, insertSQL, -1, &statement, nullptr) != SQLITE_OK)
    {
        LOG_ERROR(fmt::sprintf("Failed to prepare EditorOptions INSERT: %s", sqlite3_errmsg(db)).c_str());

        sqlite3_close(db);
        return false;
    }

    auto insertOption = [&](const char * key, const string & value)
        {
            sqlite3_reset(statement);
            sqlite3_clear_bindings(statement);

            if (sqlite3_bind_text(statement, 1, key, -1, SQLITE_STATIC) != SQLITE_OK)
            {
                LOG_ERROR(fmt::sprintf("Failed to bind EditorOptions key '%s': %s", key, sqlite3_errmsg(db)).c_str());
                return false;
            }

            if (sqlite3_bind_text(statement, 2, value.c_str(), -1, SQLITE_TRANSIENT) != SQLITE_OK)
            {
                LOG_ERROR(fmt::sprintf("Failed to bind EditorOptions value for '%s': %s", key, sqlite3_errmsg(db)).c_str());
                return false;
            }

            if (sqlite3_step(statement) != SQLITE_DONE)
            {
                LOG_ERROR(fmt::sprintf("Failed to save EditorOption '%s': %s", key, sqlite3_errmsg(db)).c_str());
                return false;
            }

            return true;
        };

    bool success = true;

    success &= insertOption("EditorGlobalScale", fmt::sprintf("%f", m_editorGlobalScale));
    success &= insertOption("ElevationStep", fmt::sprintf("%d", m_elevationStep));
    success &= insertOption("SnowStep", fmt::sprintf("%d", m_snowStep));
    success &= insertOption("UseModdedCivsForTSL", m_useModdedCivsForTSL ? "1" : "0");

    sqlite3_finalize(statement);
    sqlite3_close(db);

    return success;
}

#pragma once

#include "tile/civ7tile.h"

class UndoRedoEvent
{
public:
    virtual void Reset() = 0;
    virtual void Undo() = 0;
    virtual void Redo() = 0;
};

class UndoRedoStack
{
public:
    UndoRedoStack();
    ~UndoRedoStack();

    static bool Undo();
    static bool Redo();

    static void add(UndoRedoEvent * _event);

private:
    static vector<UndoRedoEvent *>   m_doneStack;
    static vector<UndoRedoEvent *>   m_undoneStack;
};
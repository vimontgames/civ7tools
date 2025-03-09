#include "UndoRedo.h"

vector<UndoRedoEvent *> UndoRedoStack::m_doneStack;
vector<UndoRedoEvent *> UndoRedoStack::m_undoneStack;

//--------------------------------------------------------------------------------------
void UndoRedoStack::add(UndoRedoEvent * _event)
{
    m_doneStack.push_back(_event);
    m_undoneStack.clear();
}

//--------------------------------------------------------------------------------------
bool UndoRedoStack::Undo()
{
    if (!m_doneStack.empty())
    {
        if (m_doneStack.empty())
            return false;

        UndoRedoEvent * entry = m_doneStack.back();
        m_doneStack.pop_back();

        entry->Undo();
        m_undoneStack.push_back(entry);
        return true;
    }

    return false;
}

//--------------------------------------------------------------------------------------
bool UndoRedoStack::Redo()
{
    if (!m_undoneStack.empty())
    {
        UndoRedoEvent * entry = m_undoneStack.back();
        m_undoneStack.pop_back();

        entry->Redo();
        m_doneStack.push_back(entry);

        return true;
    }

    return false;
}
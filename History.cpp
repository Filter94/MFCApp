#include "stdafx.h"
#include "History.h"

void History::makeUndoRecord(HistoryRecord record)
{
	UndoStack.push(record);
}

void History::makeRedoRecord(HistoryRecord record)
{
	RedoStack.push(record);
}

History::HistoryRecord History::makeUndo()
{
	History::HistoryRecord undo = UndoStack.top();
	UndoStack.pop();
	return undo;
}

History::HistoryRecord History::makeRedo()
{
	History::HistoryRecord redo = RedoStack.top();
	RedoStack.pop();
	return redo;
}

bool History::canUndo()
{
	return !UndoStack.empty();
}

bool History::canRedo()
{
	return !RedoStack.empty();
}

void History::flushHistory()
{
	while(!UndoStack.empty())
	{
		UndoStack.pop();
	}
	while (!RedoStack.empty())
	{
		RedoStack.pop();
	}
}

void History::flushRedo()
{
	while (!RedoStack.empty())
	{
		RedoStack.pop();
	}
}

void saveStack(MemoryFile& ar, stack<History::HistoryRecord> someStack)
// Save reversed stack to ar
// Stack on input will be emptied
{
	stack<History::HistoryRecord> stackBuf;
	while (someStack.size())
	{
		stackBuf.push(someStack.top());
		someStack.pop();
	}
	ar << stackBuf.size();
	while (stackBuf.size())
	{
		ar << stackBuf.top();
		stackBuf.pop();
	}
}

MemoryFile& operator << (MemoryFile& memFile, History history)
{
	stack<History::HistoryRecord> undoStackBuf(history.UndoStack),
		redoStackBuf(history.RedoStack);
	saveStack(memFile, undoStackBuf);
	saveStack(memFile, redoStackBuf);
	return memFile;
}

MemoryFile& operator >> (MemoryFile& ar, History& history)
{
	int undoStackElems;
	ar >> undoStackElems;
	for (int i = 0; i < undoStackElems; i++)
	{

		History::HistoryRecord historyBuf;
		ar >> historyBuf;
		history.UndoStack.push(historyBuf);
	}
	int redoStackElems;
	ar >> redoStackElems;
	for (int i = 0; i < redoStackElems; i++)
	{
		History::HistoryRecord historyBuf;
		ar >> historyBuf;
		history.RedoStack.push(historyBuf);
	}
	return ar;
}

MemoryFile& operator >> (MemoryFile& stream, History::HistoryRecord& record)
{
	stream >> record.before >> record.after;
	return stream;
}


MemoryFile& operator << (MemoryFile& stream, const History::HistoryRecord record)
{
	stream << record.before << record.after;
	return stream;
}

MemoryFile& operator >> (MemoryFile& stream, History::Cell& cell)
{
	stream >> cell.x >> cell.y;
	return stream;
}


MemoryFile& operator << (MemoryFile& stream, const History::Cell cell)
{
	stream << cell.x << cell.y;
	return stream;
}

int History::getSize()
{
	return (RedoStack.size() + UndoStack.size() * sizeof(HistoryRecord));
}

stack<History::HistoryRecord> History::getUndoStackReversedCopy(){
	stack<History::HistoryRecord> undoCopy(UndoStack);
	stack<History::HistoryRecord> stackBuf;
	while (undoCopy.size())
	{
		stackBuf.push(undoCopy.top());
		undoCopy.pop();
	}
	return stackBuf;
}
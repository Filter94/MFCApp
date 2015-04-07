#pragma once
#include "stdafx.h"
#include <stack>
#include "MemoryFile.h"

using namespace std;
class History
{
public:
	class HistoryException : public exception{};
	struct Cell
	{
		int x;
		int y;
		friend MemoryFile& operator << (MemoryFile&, const Cell);
		friend MemoryFile& operator >> (MemoryFile&, Cell&);
		Cell(){};
		Cell(int X, int Y)
		{
			x = X;
			y = Y;
		}
	};
	struct HistoryRecord
	{
		Cell before;
		Cell after;
		HistoryRecord(){};
		HistoryRecord(Cell Before, Cell After)
		{
			before = Before;
			after = After;
		}
		friend MemoryFile& operator << (MemoryFile&, const HistoryRecord);
		friend MemoryFile& operator >> (MemoryFile&, HistoryRecord&);
	};
	History(){};
	void makeUndoRecord(HistoryRecord);
	void makeRedoRecord(HistoryRecord);
	HistoryRecord makeUndo();
	HistoryRecord makeRedo();
	bool canUndo();
	bool canRedo();
	void flushHistory();
	void flushRedo();
	int getSize();
	stack<HistoryRecord> getUndoStackReversedCopy();
	friend MemoryFile& operator << (MemoryFile&, const History);
	friend MemoryFile& operator >> (MemoryFile&, History&);
private:
	stack<HistoryRecord> RedoStack;
	stack<HistoryRecord> UndoStack;
};

#include <stack>
#include <iterator>
#include <iostream>
#include "Resource.h"
#pragma once
using namespace std;
class History
{
public:
	class HistoryException : public exception{};
	struct Cell
	{
		int x;
		int y;
		friend CArchive& operator << (CArchive&, const Cell);
		friend CArchive& operator >> (CArchive&, Cell&);
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
		friend CArchive& operator << (CArchive&, const HistoryRecord);
		friend CArchive& operator >> (CArchive&, HistoryRecord&);
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
	stack<HistoryRecord> getUndoStackReversedCopy();
	friend CArchive& operator << (CArchive&, const History);
	friend CArchive& operator >> (CArchive&, History&);
private:
	stack<HistoryRecord> RedoStack;
	stack<HistoryRecord> UndoStack;
};

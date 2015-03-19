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
	struct HistoryRecord{
		int matrix[DOC_Y][DOC_X];

		HistoryRecord(){};
		HistoryRecord(int array[DOC_Y][DOC_X]){
			for (int i = 0; i < DOC_Y; i++)
				for (int j = 0; j< DOC_X; j++){
					matrix[i][j] = array[i][j];
				}
			}
		friend CArchive& operator << (CArchive&, const HistoryRecord);
		friend CArchive& operator >> (CArchive&, const HistoryRecord);
	};
	stack<HistoryRecord> RedoStack;
	stack<HistoryRecord> UndoStack;
	History(){};
	void makeUndoRecord(HistoryRecord);
	void makeRedoRecord(HistoryRecord);
	HistoryRecord makeUndo();
	HistoryRecord makeRedo();
	bool canUndo();
	bool canRedo();
	void flushHistory();
	void flushRedo();
	void save(CArchive& ar);
	void load(CArchive& ar);
	friend CArchive& operator << (CArchive&, const History);
	friend CArchive& operator >> (CArchive&, History);
};

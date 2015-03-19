#include "stdafx.h"
#include "History.h"

void History::makeUndoRecord(HistoryRecord record){
	UndoStack.push(record);
}

void History::makeRedoRecord(HistoryRecord record){
	RedoStack.push(record);
}

History::HistoryRecord History::makeUndo(){
	History::HistoryRecord undo = UndoStack.top();
	UndoStack.pop();
	return undo;
}

History::HistoryRecord History::makeRedo(){
	History::HistoryRecord redo = RedoStack.top();
	RedoStack.pop();
	return redo;
}

bool History::canUndo(){
	return !UndoStack.empty();
}

bool History::canRedo(){
	return !RedoStack.empty();
}

void History::flushHistory(){
	while(!UndoStack.empty()){
		UndoStack.pop();
	}
	while (!RedoStack.empty()){
		RedoStack.pop();
	}
}

void History::flushRedo(){
	while (!RedoStack.empty()){
		RedoStack.pop();
	}
}

CArchive& operator << (CArchive& ar, History history){
	//list<History::HistoryRecord>::iterator it;
	//for (it = history.HistoryList.begin(); it != history.HistoryList.end(); it++){
	//	ar << *it;
	//}
	return ar;
}

CArchive& operator >> (CArchive& ar, History history){
	/*while (!ar.IsBufferEmpty()){
		History::HistoryRecord buf;
		ar >> buf;
		history.HistoryList.push_back(buf);
	}*/
	return ar;
}

CArchive& operator >> (CArchive& stream, History::HistoryRecord record){
	for (int i = 0; i < DOC_Y; i++)
		for (int j = 0; j< DOC_X; j++){
			stream >> record.matrix[i][j];
		}
	return stream;
}


CArchive& operator << (CArchive& stream, const History::HistoryRecord record){
	for (int i = 0; i < DOC_Y; i++)
		for (int j = 0; j< DOC_X; j++){
			stream << record.matrix[i][j];
		}
	return stream;
}
#pragma once
#include "SFML_Lib.h"

class HighScoreManager {
public:
	HighScoreManager();
	void AddScore(string name, int mins, int secs);
private:
	void RemoveScore(int index);
	void InsertScore(string name, int mins, int secs, int index);
	unsigned int CalculateChecksum(string name, int mins, int secs);
	void SaveScores();

	string m_Names[5];
	unsigned int m_Mins[5] = {0, 0, 0, 0, 0};
	unsigned int m_Secs[5] = {0, 0, 0, 0, 0};
	unsigned int m_CheckSum[5] = { 0, 0, 0, 0, 0 };
	bool m_Populated[5] = { false, false, false, false, false };
	string m_SavefileName;
};
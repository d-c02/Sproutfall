#include "HighScoreManager.h"

HighScoreManager::HighScoreManager()
{
	m_SavefileName = "Info/highscores.bin";
	ifstream file(m_SavefileName, ios::in | ios::binary);

	int curLine = 0;
	//DATA FORMAT: NameMins:Secs:Checksum
	enum isParsing{p_name, p_mins, p_secs, p_checksum, p_null};

	if (file.is_open())
	{
		string line;
		while (curLine < 5 && getline(file, line))
		{
			//cout << line << endl;
			int curParse = p_name;
				for (int i = 0; i < line.size(); i++)
				{
					if (curParse == p_name)
					{
						if (i <= 2 && isalpha(line[i]))
						{
							m_Names[curLine].push_back(toupper(line[i]));
						}
						else
						{
							curParse = p_mins;
						}
					}
					if (curParse == p_mins)
					{
						if (isdigit(line[i]))
						{
							m_Mins[curLine] *= 10;
							m_Mins[curLine] += line[i] - '0';
						}
						else
						{
							curParse = p_secs;
						}
					}
					else if (curParse == p_secs)
					{
						if (isdigit(line[i]))
						{
							m_Secs[curLine] *= 10;
							m_Secs[curLine] += line[i] - '0';
						}
						else
						{
							curParse = p_checksum;
						}
					}
					else if (curParse == p_checksum)
					{
						if (isdigit(line[i]))
						{
							m_CheckSum[curLine] *= 10;
							m_CheckSum[curLine] += line[i] - '0';
						}
						else
						{
							curParse = p_null;
						}
					}
				}
				m_Populated[curLine] = true;
				curLine++;
		}
		file.close();
	}
	else
	{
		cout << "Failed to open high scores" << endl;
	}

	int i = 0;
	while (i < 5)
	{
		if (m_Populated[i])
		{
			if (CalculateChecksum(m_Names[i], m_Mins[i], m_Secs[i]) == m_CheckSum[i])
			{
				i++;
			}
			else
			{
				RemoveScore(i);
				SaveScores();
			}
		}
		else
		{
			i++;
		}
	}

	cout << "High scores:" << endl;
	for (int i = 0; i < 5; i++)
	{
		if (m_Populated[i])
		{
			string opt = "";
			if (m_Secs[i] < 10)
			{
				opt = "0";
			}
			cout << m_Names[i] << " has a high score of " << m_Mins[i] << ":" << opt << m_Secs[i] << endl;
		}
	}
}



void HighScoreManager::AddScore(string name, int mins, int secs)
{
	for (int i = 0; i < 5; i++)
	{
		if (m_Populated[i])
		{
			if (mins < m_Mins[i])
			{
				InsertScore(name, mins, secs, i);
				break;
			}
			else if (mins == m_Mins[i])
			{
				if (secs < m_Secs[i])
				{
					InsertScore(name, mins, secs, i);
					break;
				}
			}
		}
		else
		{
			InsertScore(name, mins, secs, i);
			break;
		}
	}
	SaveScores();
}

string HighScoreManager::getName(int index)
{
	return m_Names[index];
}

unsigned int HighScoreManager::getMins(int index)
{
	return m_Mins[index];
}

unsigned int HighScoreManager::getSecs(int index)
{
	return m_Secs[index];
}

bool HighScoreManager::getPopulated(int index)
{
	return m_Populated[index];
}

void HighScoreManager::RemoveScore(int index)
{
	for (int i = index; i < 4; i++)
	{
		m_Names[i] = m_Names[i + 1];
		m_Mins[i] = m_Mins[i + 1];
		m_Secs[i] = m_Secs[i + 1];
		m_Populated[i] = m_Populated[i + 1];
		m_CheckSum[i] = m_CheckSum[i + 1];
	}
	m_Names[4] = "";
	m_Mins[4] = 0;
	m_Secs[4] = 0;
	m_CheckSum[4] = 0;
	m_Populated[4] = false;
}

void HighScoreManager::InsertScore(string name, int mins, int secs, int index)
{
	for (int i = 4; i > index; i--)
	{
		m_Names[i] = m_Names[i - 1];
		m_Mins[i] = m_Mins[i - 1];
		m_Secs[i] = m_Secs[i - 1];
		m_Populated[i] = m_Populated[i - 1];
		m_CheckSum[i] = m_CheckSum[i - 1];
	}
	m_Names[index] = name;
	m_Mins[index] = mins;
	m_Secs[index] = secs;
	m_CheckSum[index] = CalculateChecksum(name, mins, secs);
	m_Populated[index] = true;
}

unsigned int HighScoreManager::CalculateChecksum(string name, int mins, int secs)
{
	unsigned int checksum = 0;
	for (int i = 0; i < name.size(); i++)
	{
		checksum += name[i];
	}

	/*
	while (mins > 0)
	{
		checksum += mins % 10;
		mins /= 10;
	}


	while (secs > 0)
	{
		checksum += secs % 10;
		secs /= 10;
	}
	*/
	checksum += mins * 60;
	checksum += secs;

	checksum = ~checksum;
	return checksum;
}

void HighScoreManager::SaveScores()
{
	ofstream file(m_SavefileName, ios_base::binary);

	for (int i = 0; i < 5; i++)
	{
		if (m_Populated[i])
		{
			file.write(m_Names[i].c_str(), strlen(m_Names[i].c_str()) * sizeof(char));
			file.write(to_string(m_Mins[i]).c_str(), sizeof(char) * strlen(to_string(m_Mins[i]).c_str()));
			file.write(":", sizeof(char));
			file.write(to_string(m_Secs[i]).c_str(), sizeof(char) * strlen(to_string(m_Secs[i]).c_str()));
			file.write(":", sizeof(char));
			file.write(to_string(m_CheckSum[i]).c_str(), sizeof(char) * strlen(to_string(m_CheckSum[i]).c_str()));
			file.write("\n", sizeof(char));
		}
	}

	file.close();
}


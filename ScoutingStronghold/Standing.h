#pragma once
#include <afxdb.h>
#include <SFML/Graphics.hpp>
#include <string>

class Standing
{
private:
	sf::RectangleShape box;
	sf::Text categoryNameLabel;
	sf::Text team1Name;
	sf::Text team2Name;
	sf::Text team3Name;
	sf::Text team4Name;
	sf::Text team5Name;
	sf::Text team1Value;
	sf::Text team2Value;
	sf::Text team3Value;
	sf::Text team4Value;
	sf::Text team5Value;

	std::string categoryName;

	CString team1NameC;
	CString team2NameC;
	CString team3NameC;
	CString team4NameC;
	CString team5NameC;
	CString team1ValueC;
	CString team2ValueC;
	CString team3ValueC;
	CString team4ValueC;
	CString team5ValueC;
public:
	void setCategoryName(std::string x) { categoryName = x; }

	void setTeam1NameC(CString x) { team1NameC = x; }
	void setTeam2NameC(CString x) { team2NameC = x; }
	void setTeam3NameC(CString x) { team3NameC = x; }
	void setTeam4NameC(CString x) { team4NameC = x; }
	void setTeam5NameC(CString x) { team5NameC = x; }

	void setTeam1ValueC(CString x) { team1ValueC = x; }
	void setTeam2ValueC(CString x) { team2ValueC = x; }
	void setTeam3ValueC(CString x) { team3ValueC = x; }
	void setTeam4ValueC(CString x) { team4ValueC = x; }
	void setTeam5ValueC(CString x) { team5ValueC = x; }

	std::string getCategoryName() { return categoryName; }

	std::string getTeam1NameStr() { return CW2A(team1NameC); }
	std::string getTeam2NameStr() { return CW2A(team2NameC); }
	std::string getTeam3NameStr() { return CW2A(team3NameC); }
	std::string getTeam4NameStr() { return CW2A(team4NameC); }
	std::string getTeam5NameStr() { return CW2A(team5NameC); }

	std::string getTeam1ValueStr() { return CW2A(team1ValueC); }
	std::string getTeam2ValueStr() { return CW2A(team2ValueC); }
	std::string getTeam3ValueStr() { return CW2A(team3ValueC); }
	std::string getTeam4ValueStr() { return CW2A(team4ValueC); }
	std::string getTeam5ValueStr() { return CW2A(team5ValueC); }

	sf::RectangleShape& getBox() { return box; }
	sf::Text& getCategoryNameLabel() { return categoryNameLabel; }
	sf::Text& getTeam1Name() { return team1Name; }
	sf::Text& getTeam2Name() { return team2Name; }
	sf::Text& getTeam3Name() { return team3Name; }
	sf::Text& getTeam4Name() { return team4Name; }
	sf::Text& getTeam5Name() { return team5Name; }
	sf::Text& getTeam1Value() { return team1Value; }
	sf::Text& getTeam2Value() { return team2Value; }
	sf::Text& getTeam3Value() { return team3Value; }
	sf::Text& getTeam4Value() { return team4Value; }
	sf::Text& getTeam5Value() { return team5Value; }
}; 
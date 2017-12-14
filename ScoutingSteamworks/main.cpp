#include <iostream>
#include <string>
#include <afxdb.h>					// For the database
#include <SFML/Graphics.hpp>		// For the joysticks and window
#include <atlstr.h>
#include "Team.h";
#include "Standing.h";
#include "SFMLCode.h"

enum overallState { PLAYING, HOLD, NEXT, GET_RANK, STANDINGS } programState;

int main()
{
	programState = NEXT;

	std::vector<Team> teams(6);
	std::vector<Standing> standings(6);
	CString totals[6] = { "avgAutonBalls", "perAutonGearAttempted", "perAutonGearTotal", "avgTeleopBalls", "avgTeleopGears", "perClimb" };

	CDatabase database;
	CRecordset results(&database);
	CString query;
	CString totalsQuery;
	Team tempTeam;
	CString tempC;
	std::string tempStr;
	const char * tempChar;
	float numerator;
	float denominator;
	float fraction;
	CString matchNumber;

	autonGearEnum tempAutonGear;
	climbEnum tempClimb;
	int tempInt;
	bool autonSet = false;
	bool competitionDone = false;
	CString file = L"..\\scout.accdb";
	CString driver = L"MICROSOFT ACCESS DRIVER (*.mdb, *.accdb)";
	CString dSN;
	dSN.Format(L"ODBC;DRIVER={%s};DSN='';DBQ=%s", driver, file);


	// Make a new window that you can close and has a titlebar
	sf::RenderWindow window(sf::VideoMode(SFMLCode::WIN_WIDTH, SFMLCode::WIN_HEIGHT), "Scouting Program", sf::Style::Close | sf::Style::Titlebar);
	window.setJoystickThreshold(50);

	// for loop that runs 8 times (8 joysticks possible with SFML)
	for (int i = 0; i < 8; i++)
	{
		// Prints label for each Joystick 0-7
		std::cout << "Joystick " << i << " : ";
		// if statement testing if joystick is connected
		if (sf::Joystick::isConnected(i))
			// if connected, print true
			std::cout << "true\n";
		else
			// if not connected, print false
			std::cout << "false\n";
	} // END of for loop for joystick connections

	// Event object that can be used in a switch statement
	sf::Event event;
	// while loop that runs as long as the window is open
	while (window.isOpen())
	{
		for (int i = 0; i < 6; i++)
		{
			teams[i].setJoystick(sf::Joystick::isConnected(i));
		}

		if (programState == NEXT)
		{
			database.Open(NULL, false, false, dSN);
			query = "SELECT TOP 1 matchNumber, red1, red2, red3, blue1, blue2, blue3 FROM matches WHERE hasPlayed=false ORDER BY matchNumber ASC";
			results.Open(CRecordset::forwardOnly, query, CRecordset::readOnly);

			if (results.IsBOF() == 0)
			{
				results.GetFieldValue(L"matchNumber", matchNumber);

				results.GetFieldValue(L"red1", tempC);
				teams[0].setNumber(tempC);
				teams[0].reset();

				results.GetFieldValue(L"red2", tempC);
				teams[1].setNumber(tempC);
				teams[1].reset();

				results.GetFieldValue(L"red3", tempC);
				teams[2].setNumber(tempC);
				teams[2].reset();

				results.GetFieldValue(L"blue1", tempC);
				teams[3].setNumber(tempC);
				teams[3].reset();

				results.GetFieldValue(L"blue2", tempC);
				teams[4].setNumber(tempC);
				teams[4].reset();

				results.GetFieldValue(L"blue3", tempC);
				teams[5].setNumber(tempC);
				teams[5].reset();

				results.Close();

				for (int i = 0; i < 6; i++)
				{
					tempStr = teams[i].getNumber();
					tempC = tempStr.c_str();

					query = "SELECT TOP 1 match FROM " + tempC + " WHERE hasPlayed=false ORDER BY match ASC";
					results.Open(CRecordset::forwardOnly, query, CRecordset::readOnly);
					results.GetFieldValue(L"match", tempC);
					teams[i].setTeamMatch(tempC);
					results.Close();
				}

				database.Close();
				programState = PLAYING;
			}
			else
			{
				results.Close();
				database.Close();
				programState = GET_RANK;
				competitionDone = true;
			}
		}

		// while loop that runs as long as there are pending events
		while (window.pollEvent(event))
		{
			// switch statement that depends on the event type
			switch (event.type)
			{
				// if window X clicked, close window
				case sf::Event::Closed: window.close(); break;
				case sf::Event::KeyPressed:
					if (programState == HOLD)
					{
						if (event.key.code == sf::Keyboard::Return)
							programState = NEXT;
						else if (event.key.code == sf::Keyboard::Tab)
							programState = GET_RANK;
					}
					else if (programState == STANDINGS && !competitionDone)
					{
						if (event.key.code == sf::Keyboard::Escape)
							programState = NEXT;
					}
					else if (programState == PLAYING)
					{
						if (event.key.code == sf::Keyboard::BackSpace)
						{
							for (int i = 0; i < 6; i++)
							{
								teams[i].reset();
							}
						}
					}
					break;
				case sf::Event::JoystickButtonPressed:
					if (teams[event.joystickButton.joystickId].getState() == GAME && programState == PLAYING)
					{
						if (teams[event.joystickButton.joystickId].getClimb() == climbEnum::UNDEF)
						{
							if (event.joystickButton.button == 4)
								teams[event.joystickButton.joystickId].setClimb(YES);
							else if (event.joystickButton.button == 5)
								teams[event.joystickButton.joystickId].setClimb(NO);
						}
						if (event.joystickButton.button == 0)
							teams[event.joystickButton.joystickId].setAutonBalls(teams[event.joystickButton.joystickId].getAutonBalls() + 1);
						if (event.joystickButton.button == 1)
							teams[event.joystickButton.joystickId].setTeleopBalls(teams[event.joystickButton.joystickId].getTeleopBalls() + 1);
						if (event.joystickButton.button == 3)
							teams[event.joystickButton.joystickId].setTeleopGears(teams[event.joystickButton.joystickId].getTeleopGears() + 1);
						if (event.joystickButton.button == 7)
						{
							//teams[event.joystickButton.joystickId].setState(EDIT);
							for (int i = 0; i < 6; i++)
							{
								teams[i].setState(EDIT);
							}
						}
							
					}
					else if (teams[event.joystickButton.joystickId].getState() == EDIT && programState == PLAYING)
					{
						if (event.joystickButton.button == 7)
						{
							//teams[event.joystickButton.joystickId].setState(WAIT);
							for (int i = 0; i < 6; i++)
							{
								teams[i].setState(WAIT);
							}
						}
						if (event.joystickButton.button == 8)
							teams[event.joystickButton.joystickId].setEditSelection(A_GEAR);
						if (event.joystickButton.button == 4 || event.joystickButton.button == 5)
								teams[event.joystickButton.joystickId].setEditSelection(CLIMB);
						if (event.joystickButton.button == 0)
							teams[event.joystickButton.joystickId].setEditSelection(A_BALLS);
						if (event.joystickButton.button == 1)
							teams[event.joystickButton.joystickId].setEditSelection(T_BALLS);
						if (event.joystickButton.button == 3)
							teams[event.joystickButton.joystickId].setEditSelection(T_GEARS);
					}
					
					break;
				case sf::Event::JoystickMoved:
					if (teams[event.joystickMove.joystickId].getState() == GAME && programState == PLAYING)
					{
						tempAutonGear = teams[event.joystickMove.joystickId].getAutonGear();
						autonSet = (tempAutonGear != autonGearEnum::UNSET);
						if (!autonSet)
						{
							if (event.joystickMove.axis == 0 && event.joystickMove.position < 0 && abs(event.joystickMove.position) > 50)
								teams[event.joystickMove.joystickId].setAutonGear(SIDE);
							else if (event.joystickMove.axis == 0 && event.joystickMove.position > 0 && abs(event.joystickMove.position) > 50)
								teams[event.joystickMove.joystickId].setAutonGear(MISS);
							else if (event.joystickMove.axis == 1 && event.joystickMove.position < 0 && abs(event.joystickMove.position) > 50)
								teams[event.joystickMove.joystickId].setAutonGear(MIDDLE);
							else if (event.joystickMove.axis == 1 && event.joystickMove.position > 0 && abs(event.joystickMove.position) > 50)
								teams[event.joystickMove.joystickId].setAutonGear(NONE);
						}
					}
					else if (teams[event.joystickMove.joystickId].getState() == EDIT && programState == PLAYING)
					{
						if (event.joystickMove.axis == 7)
						{
							switch (teams[event.joystickMove.joystickId].getEditSelection())
							{
								case A_GEAR:
									tempAutonGear = teams[event.joystickMove.joystickId].getAutonGear();
									autonSet = (tempAutonGear != UNSET);
									if (!autonSet)
									{
										teams[event.joystickMove.joystickId].setAutonGear(MIDDLE);
										tempAutonGear = MIDDLE;
									}
									switch (tempAutonGear)
									{
										case MIDDLE: tempInt = 0; break;
										case SIDE: tempInt = 1; break;
										case MISS: tempInt = 2; break;
										case NONE: tempInt = 3; break;
									}
								
									if (event.joystickMove.position == 100)
									{
										tempInt++;
										if (tempInt == 4)
											tempInt = 0;
									}
									else if (event.joystickMove.position == -100)
									{
										tempInt--;
										if (tempInt == -1)
											tempInt = 3;
									}
									teams[event.joystickMove.joystickId].setAutonGear(static_cast<autonGearEnum>(tempInt));
									break;
								case A_BALLS:
									tempInt = teams[event.joystickMove.joystickId].getAutonBalls();
									if (event.joystickMove.position == 100)
										tempInt++;
									else if(event.joystickMove.position == -100)
									{
										tempInt--;
										if (tempInt == -1)
											tempInt = 0;
									}
									teams[event.joystickMove.joystickId].setAutonBalls(tempInt);
									break;
								case T_GEARS:
									tempInt = teams[event.joystickMove.joystickId].getTeleopGears();
									if (event.joystickMove.position == 100)
										tempInt++;
									else if (event.joystickMove.position == -100)
									{
										tempInt--;
										if (tempInt == -1)
											tempInt = 0;
									}
									teams[event.joystickMove.joystickId].setTeleopGears(tempInt);
									break;
								case T_BALLS:
									tempInt = teams[event.joystickMove.joystickId].getTeleopBalls();
									if (event.joystickMove.position == 100)
										tempInt++;
									else if (event.joystickMove.position == -100)
									{
										tempInt--;
										if (tempInt == -1)
											tempInt = 0;
									}
									teams[event.joystickMove.joystickId].setTeleopBalls(tempInt);
									break;
								case CLIMB:
									tempClimb = teams[event.joystickMove.joystickId].getClimb();
									if (tempClimb != NO && tempClimb != YES)
									{
										teams[event.joystickMove.joystickId].setClimb(NO);
										tempClimb = NO;
									}
									switch (tempClimb)
									{
										case NO: tempInt = 0; break;
										case YES: tempInt = 1; break;
									}

									if (event.joystickMove.position == 100 || event.joystickMove.position == -100)
									{
										if (tempInt == 0)
											tempInt = 1;
										else
											tempInt = 0;
									}
									teams[event.joystickMove.joystickId].setClimb(static_cast<climbEnum>(tempInt));
									break;
							}
						}
					}
					break;
			} // END of event switch statement
		} // END of while loop when there are pending events

		if (programState == NEXT)
		{
			for (int i = 0; i < 6; i++)
			{
				teams[i].setState(GAME);
				teams[i].setSaved(false);
			}
		}

		if (programState == PLAYING)
		{
			for (int i = 0; i < 6; i++)
			{
				if (teams[i].getState() == WAIT && !teams[i].getSaved())
				{
					database.Open(NULL, false, false, dSN);

					tempStr = teams[i].getNumber();
					tempC = tempStr.c_str();
					query = "UPDATE " + tempC;
					switch (teams[i].getAutonGear())
					{
					case autonGearEnum::MIDDLE: tempC = "Middle"; break;
					case autonGearEnum::SIDE: tempC = "Side"; break;
					case autonGearEnum::MISS: tempC = "Miss"; break;
					case autonGearEnum::NONE: tempC = "None"; break;
					default: tempC = "Unset"; break;
					}
					query += " SET hasPlayed=true, autonGear='" + tempC;
					tempStr = std::to_string(teams[i].getAutonBalls());
					tempC = tempStr.c_str();
					query += "', autonBalls=" + tempC;
					tempStr = std::to_string(teams[i].getTeleopGears());
					tempC = tempStr.c_str();
					query += ", teleopGears=" + tempC;
					tempStr = std::to_string(teams[i].getTeleopBalls());
					tempC = tempStr.c_str();
					query += ", teleopBalls=" + tempC;
					switch (teams[i].getClimb())
					{
					case YES: tempC = "true";  break;
					case NO: default: tempC = "false";
					}
					query += ", climb=" + tempC + " WHERE match=" + teams[i].getTeamMatch();
					database.ExecuteSQL(query);

					tempStr = teams[i].getNumber();
					tempC = tempStr.c_str();
					query = "SELECT COUNT(*) AS count FROM " + tempC + " WHERE hasPlayed=true";
					results.Open(CRecordset::forwardOnly, query, CRecordset::readOnly);
					results.GetFieldValue(L"count", tempC);
					tempStr = CW2A(tempC);
					tempChar = tempStr.c_str();
					denominator = atof(tempChar);
					results.Close();

					tempStr = teams[i].getNumber();
					tempC = tempStr.c_str();
					query = "SELECT autonGear, autonBalls, teleopBalls, teleopGears FROM " + tempC + " WHERE hasPlayed=true";
					results.Open(CRecordset::snapshot, query, CRecordset::readOnly);
					numerator = 0;
					while (!results.IsEOF())
					{
						results.GetFieldValue(L"autonBalls", tempC);
						tempStr = CW2A(tempC);
						tempChar = tempStr.c_str();
						numerator += atof(tempChar);
						results.MoveNext();
					}
					fraction = numerator / denominator;
					tempStr = std::to_string(fraction);
					tempC = tempStr.c_str();
					totalsQuery = "UPDATE totals SET avgAutonBalls=" + tempC;

					results.MoveFirst();
					numerator = 0;
					while (!results.IsEOF())
					{
						results.GetFieldValue(L"teleopBalls", tempC);
						tempStr = CW2A(tempC);
						tempChar = tempStr.c_str();
						numerator += atof(tempChar);
						results.MoveNext();
					}
					fraction = numerator / denominator;
					tempStr = std::to_string(fraction);
					tempC = tempStr.c_str();
					totalsQuery += ", avgTeleopBalls=" + tempC;

					results.MoveFirst();
					numerator = 0;
					while (!results.IsEOF())
					{
						results.GetFieldValue(L"teleopGears", tempC);
						tempStr = CW2A(tempC);
						tempChar = tempStr.c_str();
						numerator += atof(tempChar);
						results.MoveNext();
					}
					fraction = numerator / denominator;
					tempStr = std::to_string(fraction);
					tempC = tempStr.c_str();
					totalsQuery += ", avgTeleopGears=" + tempC;
					results.Close();

					tempStr = teams[i].getNumber();
					tempC = tempStr.c_str();
					query = "SELECT COUNT(*) AS count FROM " + tempC + " WHERE hasPlayed=true AND climb=true";
					results.Open(CRecordset::forwardOnly, query, CRecordset::readOnly);
					results.GetFieldValue(L"count", tempC);
					tempStr = CW2A(tempC);
					tempChar = tempStr.c_str();
					numerator = atof(tempChar);
					results.Close();
					fraction = numerator / denominator;
					fraction *= 100;
					tempStr = std::to_string(fraction);
					tempC = tempStr.c_str();
					totalsQuery += ", perClimb=" + tempC;

					tempStr = teams[i].getNumber();
					tempC = tempStr.c_str();
					query = "SELECT COUNT(*) AS count FROM " + tempC + " WHERE hasPlayed=true AND (autonGear='Middle' OR autonGear='Side')";
					results.Open(CRecordset::forwardOnly, query, CRecordset::readOnly);
					results.GetFieldValue(L"count", tempC);
					tempStr = CW2A(tempC);
					tempChar = tempStr.c_str();
					numerator = atof(tempChar);
					results.Close();
					fraction = numerator / denominator;
					fraction *= 100;
					tempStr = std::to_string(fraction);
					tempC = tempStr.c_str();
					totalsQuery += ", perAutonGearTotal=" + tempC;

					tempStr = teams[i].getNumber();
					tempC = tempStr.c_str();
					query = "SELECT COUNT(*) AS count FROM " + tempC + " WHERE hasPlayed=true AND autonGear<>'None'";
					results.Open(CRecordset::forwardOnly, query, CRecordset::readOnly);
					results.GetFieldValue(L"count", tempC);
					tempStr = CW2A(tempC);
					tempChar = tempStr.c_str();
					denominator = atof(tempChar);
					results.Close();
					fraction = numerator / denominator;
					fraction *= 100;
					tempStr = std::to_string(fraction);
					tempC = tempStr.c_str();
					totalsQuery += ", perAutonGearAttempted=" + tempC;

					tempStr = teams[i].getNumber();
					tempC = tempStr.c_str();
					totalsQuery += " WHERE teamNumber=" + tempC;
					database.ExecuteSQL(totalsQuery);

					teams[i].setSaved(true);
					database.Close();
				}
			}
		}

		if (teams[0].getState() == WAIT && teams[1].getState() == WAIT && teams[2].getState() == WAIT &&
			teams[3].getState() == WAIT && teams[4].getState() == WAIT && teams[5].getState() == WAIT && programState == PLAYING)
		{
			programState = HOLD;
			try 
			{
				database.Open(NULL, false, false, dSN);
				query = "UPDATE matches SET hasPlayed=true WHERE matchNumber=" + matchNumber;
				database.ExecuteSQL(query);
				database.Close();
			}
			catch (CDBException* e)
			{
				e->ReportError();
			}
		}

		if (programState == GET_RANK)
		{
			for (int i = 0; i < 6; i++)
			{
				database.Open(NULL, false, false, dSN);
				query = "SELECT TOP 5 teamNumber, " + totals[i] + " FROM totals ORDER BY " + totals[i] + " DESC";
				results.Open(CRecordset::forwardOnly, query, CRecordset::readOnly);
				if (results.IsBOF() == 0)
				{
					for (int j = 0; j < 5; j++)
					{
						results.GetFieldValue(totals[i], tempC);
						if (i == 1 || i == 2 || i == 5)
							tempC += "%";
						switch (j)
						{
						case 0: standings[i].setTeam1ValueC(tempC); break;
						case 1: standings[i].setTeam2ValueC(tempC); break;
						case 2: standings[i].setTeam3ValueC(tempC); break;
						case 3: standings[i].setTeam4ValueC(tempC); break;
						case 4: standings[i].setTeam5ValueC(tempC); break;
						}
						results.GetFieldValue(L"teamNumber", tempC);
						switch (j)
						{
						case 0: standings[i].setTeam1NameC(tempC); break;
						case 1: standings[i].setTeam2NameC(tempC); break;
						case 2: standings[i].setTeam3NameC(tempC); break;
						case 3: standings[i].setTeam4NameC(tempC); break;
						case 4: standings[i].setTeam5NameC(tempC); break;
						}
						results.MoveNext();
					}
					results.Close();
					switch (i)
					{
					case 0: standings[i].setCategoryName("Avg. Auton Balls"); break;
					case 1: standings[i].setCategoryName("Auton Gear Success (Attempted)"); break;
					case 2: standings[i].setCategoryName("Auton Gear Success (Total)"); break;
					case 3: standings[i].setCategoryName("Avg. Teleop Balls"); break;
					case 4: standings[i].setCategoryName("Avg. Teleop Gears"); break;
					case 5: standings[i].setCategoryName("Climb Success"); break;
					}
				}

				database.Close();
				programState = STANDINGS;
			}
		}
		// Clear the window where blank space will be blue
		window.clear(sf::Color::Blue);

		switch (programState)
		{
			case HOLD:
			case PLAYING: SFMLCode::drawAll(window, teams, matchNumber); break;
			case STANDINGS: SFMLCode::drawAll(window, standings); break;
			case NEXT: break;
			case GET_RANK: break;
		}
	} // END of while loop when window is open
	
	if (database.IsOpen())
		database.Close();
	return 0;
}

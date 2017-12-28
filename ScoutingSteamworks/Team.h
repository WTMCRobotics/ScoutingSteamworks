#pragma once
#include <afxdb.h>
#include <SFML/Graphics.hpp>

// Enumerator for the autonomous gear.
enum autonGearEnum { MIDDLE = 0, SIDE = 1, MISS = 2, NONE = 3, UNSET = 4 };
// Enumerator for climbing. (UNDEF = unset, but needed another word because UNSET is defined in autonGearEnum.)
enum climbEnum { NO = 0, YES = 1, UNDEF = 2 };

// Enumerator for the state of an individual team.
enum teamState { GAME, EDIT, WAIT };
// Enumerator for the selection in edit mode.
enum editSelection { A_GEAR, A_BALLS, T_GEARS, T_BALLS, CLIMB, NO_EDIT };

class Team
{
private:
	// Team number
	CString number;
	// Joystick plugged in: true = yes; false = no
	bool isJoystick;
	// Autonomous gear value
	autonGearEnum autonGear;
	// Match for team (1 through 12)
	CString teamMatch;
	// Autonomous balls value
	int autonBalls;
	// Teleop gears value
	int teleopGears;
	// Teleop balls value
	int teleopBalls;
	// Climb value
	climbEnum climb;
	// State: GAME = match in progress; EDIT = change values; WAIT = submitted and waiting for next match
	teamState state;
	editSelection editSelect;
	sf::RectangleShape joystickBox;
	sf::RectangleShape box;
	sf::RectangleShape submit;
	sf::Text title;
	sf::Text autonLabel;
	sf::Text autonGearLabel;
	sf::Text autonGearValue;
	sf::Text autonBallsLabel;
	sf::Text autonBallsValue;
	sf::Text teleopLabel;
	sf::Text teleopGearsLabel;
	sf::Text teleopGearsValue;
	sf::Text teleopBallsLabel;
	sf::Text teleopBallsValue;
	sf::Text climbLabel;
	sf::Text climbValue;
	sf::Text submitStatus;
	sf::Text joystickText;
	bool saved;
public:
	Team() : state(GAME), autonBalls(0), teleopGears(0), teleopBalls(0), saved(false) { joystickText.setString("Joystick Missing"); }
	void setNumber(CString x) { number = x; };
	void setJoystick(bool x) { isJoystick = x; }
	void setTeamMatch(CString x) { teamMatch = x; }
	void setAutonGear(autonGearEnum x) { autonGear = x; }
	void setAutonBalls(int x) { autonBalls = x; }
	void setTeleopGears(int x) { teleopGears = x; }
	void setTeleopBalls(int x) { teleopBalls = x; }
	void setClimb(climbEnum x) { climb = x; }
	void setState(teamState x) { state = x; }
	void setEditSelection(editSelection x) { editSelect = x; }
	void setSaved(bool x) { saved = x; }

	sf::Text& getAutonGearValue() { return autonGearValue; }
	sf::Text& getAutonBallsValue() { return autonBallsValue; }
	sf::Text& getTeleopGearsValue() { return teleopGearsValue; }
	sf::Text& getTeleopBallsValue() { return teleopBallsValue; }
	sf::Text& getClimbValue() { return climbValue; }

	sf::RectangleShape& getJoystickBox() { return joystickBox; }
	sf::RectangleShape& getBox() { return box; }
	sf::Text& getTitle() { return title; }
	sf::Text& getAutonLabel() { return autonLabel; }
	sf::Text& getAutonGearLabel() { return autonGearLabel; }
	sf::Text& getAutonBallsLabel() { return autonBallsLabel; }
	sf::Text& getTeleopLabel() { return teleopLabel; }
	sf::Text& getTeleopGearsLabel() { return teleopGearsLabel; }
	sf::Text& getTeleopBallsLabel() { return teleopBallsLabel; }
	sf::Text& getClimbLabel() { return climbLabel; }
	sf::RectangleShape& getSubmit() { return submit; }
	sf::Text& getSubmitStatus() { return submitStatus; }
	sf::Text& getJoystickText() { return joystickText; }
	bool getSaved() { return saved; }

	teamState getState() { return state; }
	editSelection getEditSelection() { return editSelect; }
	CString getNumberC() { return number; }
	std::string getNumber() { return CW2A(number); }
	bool getJoystick() { return isJoystick; }
	CString getTeamMatch() { return teamMatch; }
	autonGearEnum getAutonGear() { return autonGear; }
	int getAutonBalls() { return autonBalls; }
	int getTeleopGears() { return teleopGears; }
	int getTeleopBalls() { return teleopBalls; }
	climbEnum getClimb() { return climb; }

	void reset()
	{
		autonGear = autonGearEnum::UNSET;
		autonBalls = 0;
		teleopGears = 0;
		teleopBalls = 0;
		climb = climbEnum::UNDEF;
		editSelect = NO_EDIT;
	}
};

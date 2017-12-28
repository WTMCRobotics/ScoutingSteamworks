#include <iostream>
#include <string>
#include <afxdb.h>					// For the database
#include <SFML/Graphics.hpp>		// For the joysticks and window
#include <atlstr.h>					// For type conversions involving CString
#include "Team.h";
#include "Standing.h";
#include "SFMLCode.h"

// States for the program state
//		PLAYING = Game in progress. Some teams may be submitted, but others are still going.
//		HOLD = All the teams have submitted. Can either go to next match or see standings.
//		NEXT = Pulls data for next match. Then, PLAYING, again.
//		GET_RANK = Pulls data for the standings. Then, STANDINGS.
//		STANDINGS = Displays the top teams in several categories.
enum overallState { PLAYING, HOLD, NEXT, GET_RANK, STANDINGS } programState;

// Values for the button and axis numbers of the controller
const int LEFT_BUMPER = 4;
const int RIGHT_BUMPER = 5;
const int A_BUTTON = 0;
const int B_BUTTON = 1;
const int Y_BUTTON = 3;
const int MENU_BUTTON = 7;
const int LEFT_STICK_BUTTON = 8;

const int DPAD_UP_DOWN_AXIS = 7;
const int LEFT_STICK_X_AXIS = 0;
const int LEFT_STICK_Y_AXIS = 1;

int main()
{
	// Start the program in the NEXT state. Will pull data for the match later.
	programState = NEXT;

	// Make 6 Team objects and 6 Standing objects (6 categories).
	std::vector<Team> teams(6);
	std::vector<Standing> standings(6);
	// Array of column names for the 6 team numbers as they appear in the database.
	CString teamNumberColumns[6] = { "red1", "red2", "red3", "blue1", "blue2", "blue3" };
	// Array of column names for the 6 standings categories as they appear in the database.
	CString totalsColumns[6] = { "avgAutonBalls", "perAutonGearAttempted", "perAutonGearTotal", "avgTeleopBalls", "avgTeleopGears", "perClimb" };
	// Create an object to access the database.
	CDatabase database;
	// Create an object to hold records from the database.
	CRecordset results(&database);
	// Create CString objects to store query statements.
	//		SQL Queries = SELECT and UPDATE [not used: INSERT, DELETE]
	CString query;
	CString totalsQuery;
	// Team object, CString, and string that don't need to be saved for long (temporary)
	Team tempTeam;
	CString tempC;
	std::string tempStr;
	// Character pointer that doesn't need to be saved for long (temporary)
	//		const char * means that you can change the location it points to, but not the value in that location.
	//		For converting CString to a double (CString --> string --> const char * --> double)
	const char * tempChar;
	// Doubles needed for calculating and saving totals
	double numerator;
	double denominator;
	double fraction;
	// CString to keep track of the match number
	CString matchNumber;

	// Variables of the user created enumerators that don't need to be saved for long (temporary)
	autonGearEnum tempAutonGear;
	climbEnum tempClimb;
	// Integer that doesn't need to saved for long (temporary)
	int tempInt;

	// Start with competition not being done
	bool competitionDone = false;

	// CString for the relative path for the Microsoft Access file.
	CString file = L"..\\scout.accdb";
	// CString for the driver being used to access the database.
	CString driver = L"MICROSOFT ACCESS DRIVER (*.mdb, *.accdb)";
	// CString for the connection.
	CString dSN;
	// Sets the value of the dSN variable. %s is replaced with the values of the additional arguments (in order).
	dSN.Format(L"ODBC;DRIVER={%s};DSN='';DBQ=%s", driver, file);


	// Make a new window.
	sf::RenderWindow window(sf::VideoMode(999, 650), "Scouting Program", sf::Style::Default);
	// Start window at the 999 x 650.
	SFMLCode::updateSizeValues(999, 650);
	// Joystick Threshold is the point where the absolute value of the axis position must be above to trigger the JoystickMoved event.
	//		Axis position must be below -50 or above 50.
	window.setJoystickThreshold(50);

	// for loop that runs 8 times (8 joysticks possible with SFML) [Print Joysticks]
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
		// for each of the 6 teams [Joystick states]
		for (int i = 0; i < 6; i++)
		{
			// Save the state of the joystick in the corresponding Team object.
			teams[i].setJoystick(sf::Joystick::isConnected(i));
		} // END of for loop for settting joystick states

		  // if NEXT [Pulling data for next match]
		if (programState == NEXT)
		{
			// for each of the 6 teams [Reset Team Mode and Save State]
			for (int i = 0; i < 6; i++)
			{
				// Set each team's state to GAME from WAIT.
				teams[i].setState(GAME);
				// Reset each team's save state to false.
				teams[i].setSaved(false);
			} // END of for loop for settting team mode and save status

			  // Open the database
			database.Open(NULL, false, false, dSN);
			// Set SQL query
			//		SELECT = Get records
			//		TOP 1 = Only one record (equivalent to LIMIT 1 at the end of a query statement for MySQL)
			//		matchNumber, red1, red3, blue1, blue2, blue3 = Name of the columns that you need values from
			//		FROM matches = Table to get record from
			//		WHERE hasPlayed=false = Record needs to have the condition that hasPlayed is false
			//		ORDER BY matchNumber ASC = Order the records in ascending order of the matchNumber column
			//			Insures that it will be the next match that hasn't been played.
			query = "SELECT TOP 1 matchNumber, red1, red2, red3, blue1, blue2, blue3 FROM matches WHERE hasPlayed=false ORDER BY matchNumber ASC";
			// Save the records in the results object.
			//		forwardOnly = Can only cycle through the records forward (Irrelevant = TOP 1)
			//		readOnly = Cannot change values, only read them
			results.Open(CRecordset::forwardOnly, query, CRecordset::readOnly);

			// if there are records in the results (unplayed match exists) [Match Number, Team Numbers, and Team Match Numbers]
			if (results.IsBOF() == 0)
			{
				// Save the value in the matchNumber column in the matchNumber CString variable.
				results.GetFieldValue(L"matchNumber", matchNumber);

				// for each of the 6 teams [Team Number]
				for (int i = 0; i < 6; i++)
				{
					// Save the value in the column teamNumberColumns[i] in the tempC CString variable.
					results.GetFieldValue(teamNumberColumns[i], tempC);
					// Set the team number for the corresponding Team object to the value of tempC.
					teams[i].setNumber(tempC);
					// Reset the game variables and editing variable for the Team objects.
					teams[i].reset();
				} // END of for loop for setting team numbers
				// Clear the results variable of the records.
				results.Close();

				// for each of the 6 teams [Team Match Number]
				for (int i = 0; i < 6; i++)
				{
					// Get the team number.
					tempC = teams[i].getNumberC();
					// Set SQL query
					//		tempC = Team number = Table to get records from
					//		Next match that hasn't been played for a specific team (1 through 12)
					query = "SELECT TOP 1 match FROM " + tempC + " WHERE hasPlayed=false ORDER BY match ASC";
					// Save the records in the results object.
					results.Open(CRecordset::forwardOnly, query, CRecordset::readOnly);
					// Save the value in the column match in  the tempC CString variable.
					results.GetFieldValue(L"match", tempC);
					// Set the team match for the corresponding Team object to the value of tempC.
					teams[i].setTeamMatch(tempC);
					// Clear the results variable of the records.
					results.Close();
				} // END of for loop for setting team match numbers

				  // Close connection to the database.
				database.Close();
				// Change the program state from NEXT to PLAYING.
				programState = PLAYING;
			} // END of if statement checking for nonempty results (unplayed match exists)
			  // else empty results (no unplayed matches) [Start Process for Viewing Standings]
			else
			{
				// Make sure results variable is cleared.
				results.Close();
				// Make sure database connection is closed.
				database.Close();
				// Change the program state from NEXT to GET_RANK.
				programState = GET_RANK;
				// Competition is over. No more matches.
				competitionDone = true;
			} // END of else empty results (no unplayed matches exist)
		} // END of if statement checking that program state is NEXT

		  // while loop that runs as long as there are pending events [Joystick and Keyboard Input]
		while (window.pollEvent(event))
		{
			// switch statement that depends on the event type.
			switch (event.type)
			{
			// if window is resized [Adjust Display Based on Window Size]
			case sf::Event::Resized:
			{
				// Make a rectangle with the size of the window.
				sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
				// Set the view to the area of the new window.
				window.setView(sf::View(visibleArea));
				// Update size values that will be used in drawing on the window.
				SFMLCode::updateSizeValues(event.size.width, event.size.height);
				// break for the Resized event.
				break;
			} // END of Resized case (braces needed to initialize the FloatRect in the switch statement)
			// if window X clicked, close window
			case sf::Event::Closed: window.close(); break;
			// if a key is pressed
			case sf::Event::KeyPressed:
				// if the program state is HOLD (all teams have submitted)
				if (programState == HOLD)
				{
					// if Return key is pressed
					if (event.key.code == sf::Keyboard::Return)
						// program state from HOLD to NEXT (start new match).
						programState = NEXT;
					// else if Tab key is pressed
					else if (event.key.code == sf::Keyboard::Tab)
						// program state from HOLD to GET_RANK (view standings).
						programState = GET_RANK;
				} // END of if statement checking that program state is HOLD
				  // else if program state is STANDINGS and the competition is not done
				else if (programState == STANDINGS && !competitionDone)
				{
					// if the Esc key is pressed
					if (event.key.code == sf::Keyboard::Escape)
						// program state from STANDINGS to NEXT (start new match).
						programState = NEXT;
				} // END of else statement checking that program state is STANDINGS and competition is not done
				  // else if program state is PLAYING
				else if (programState == PLAYING)
				{
					// if the Backspace key is pressed
					if (event.key.code == sf::Keyboard::BackSpace)
					{
						// for each of the 6 teams [Reset match]
						for (int i = 0; i < 6; i++)
						{
							// Reset the match variables for every team.
							teams[i].reset();
						} // END of for loop for resetting the match
					} // END of if statement checking if the backspace key is pressed
				} // END of else statement checking that program state is PLAYING

				  // break for the KeyPressed event
				break;
				// if a joystick button is pressed
			case sf::Event::JoystickButtonPressed:
				// if the state of the team that a button was pushed for is GAME and the program state is PLAYING
				if (teams[event.joystickButton.joystickId].getState() == GAME && programState == PLAYING)
				{
					// if the climbing variable is unset for the team that a button was pushed for
					if (teams[event.joystickButton.joystickId].getClimb() == climbEnum::UNDEF)
					{
						// if the button pushed is the Left Bumper
						if (event.joystickButton.button == LEFT_BUMPER)
							// Set the climbing variable to YES for the team that corresponds with the joystick.
							teams[event.joystickButton.joystickId].setClimb(YES);
						// else if the button pushed is the Right Bumper
						else if (event.joystickButton.button == RIGHT_BUMPER)
							// Set the climbing variable to NO for the team that corresponds with the joystick.
							teams[event.joystickButton.joystickId].setClimb(NO);
					} // END of if statement checking that the climbing variable for the corresponding team is UNDEF

					  // if the A Button is pressed
					if (event.joystickButton.button == A_BUTTON)
						// Set the autonomous balls variable to the current value plus 1 for the corresponding team.
						teams[event.joystickButton.joystickId].setAutonBalls(teams[event.joystickButton.joystickId].getAutonBalls() + 1);
					// if the B Button is pressed
					if (event.joystickButton.button == B_BUTTON)
						// Set the teleop balls variable to the current value plus 1 for the corresponding team.
						teams[event.joystickButton.joystickId].setTeleopBalls(teams[event.joystickButton.joystickId].getTeleopBalls() + 1);
					// if the Y Button is pressed
					if (event.joystickButton.button == Y_BUTTON)
						// Set the teleop gears variable to the current value plus 1 for the corresponding team.
						teams[event.joystickButton.joystickId].setTeleopGears(teams[event.joystickButton.joystickId].getTeleopGears() + 1);
					// if the Menu Button is pressed
					if (event.joystickButton.button == MENU_BUTTON)
						// Change state from GAME to EDIT for the corresponding team.
						//teams[event.joystickButton.joystickId].setState(EDIT);

						// Change every team's state from GAME to EDIT (for testing with one remote).
						for (int i = 0; i < 6; i++) { teams[i].setState(EDIT); }
				} // END of if statement checking that the corresponding team's state is GAME and the program state is PLAYING
				  // else if the state of the team that a button was pushed for is EDIT and the program state is PLAYING
				else if (teams[event.joystickButton.joystickId].getState() == EDIT && programState == PLAYING)
				{
					// if the Menu Button is pressed
					if (event.joystickButton.button == MENU_BUTTON)
						// Change state from EDIT to WAIT for the corresponding team.
						//teams[event.joystickButton.joystickId].setState(WAIT);

						// Change every team's state from EDIT to WAIT (for testing with one remote).
						for (int i = 0; i < 6; i++) { teams[i].setState(WAIT); }
					// if the Left Stick Button is pressed
					if (event.joystickButton.button == LEFT_STICK_BUTTON)
						// Set the edit selection variable to A_GEAR for the corresponding team.
						teams[event.joystickButton.joystickId].setEditSelection(A_GEAR);
					// if the Left Bumper or Right Bumper is presssed
					if (event.joystickButton.button == LEFT_BUMPER || event.joystickButton.button == RIGHT_BUMPER)
						// Set the edit selection variable to CLIMB for the corresponding team.
						teams[event.joystickButton.joystickId].setEditSelection(CLIMB);
					// if the A Button is pressed
					if (event.joystickButton.button == A_BUTTON)
						// Set the edit selection variable to A_BALLS for the corresponding team.
						teams[event.joystickButton.joystickId].setEditSelection(A_BALLS);
					// if the B Button is pressed
					if (event.joystickButton.button == B_BUTTON)
						// Set the edit selection variable to T_BALLS for the corresponding team.
						teams[event.joystickButton.joystickId].setEditSelection(T_BALLS);
					// if the Y Button is pressed
					if (event.joystickButton.button == Y_BUTTON)
						// Set the edit selection variable to T_GEARS for the corresponding team.
						teams[event.joystickButton.joystickId].setEditSelection(T_GEARS);
				} // END of else statement checking that the corresponding team's state is EDIT and the program state is PLAYING

				  // break the JoystickButtonPressed event
				break;
				// if a joystick axis is moved
			case sf::Event::JoystickMoved:
				// if the state of the team that a button was pushed for is GAME and the program state is PLAYING
				if (teams[event.joystickMove.joystickId].getState() == GAME && programState == PLAYING)
				{
					// Save the current value of the autonomous gear for the corresponding team.
					tempAutonGear = teams[event.joystickMove.joystickId].getAutonGear();
					// if the autonomous gear variable is unset for the team that the axis was pushed for
					if (tempAutonGear == autonGearEnum::UNSET)
					{
						// if the Left Stick is moved about the x axis to a position at least 50 to the left
						if (event.joystickMove.axis == LEFT_STICK_X_AXIS && event.joystickMove.position < 0 && abs(event.joystickMove.position) > 50)
							// Set the autonomous gear variable to SIDE for the corresponding team.
							teams[event.joystickMove.joystickId].setAutonGear(SIDE);
						// else if the Left Stick is moved about the x axis to a position at least 50 to the right
						else if (event.joystickMove.axis == LEFT_STICK_X_AXIS && event.joystickMove.position > 0 && abs(event.joystickMove.position) > 50)
							// Set the autonomous gear variable to MISS for the corresponding team.
							teams[event.joystickMove.joystickId].setAutonGear(MISS);
						// else if the Left Stick is moved about the y axis to a position at least 50 up
						else if (event.joystickMove.axis == LEFT_STICK_Y_AXIS && event.joystickMove.position < 0 && abs(event.joystickMove.position) > 50)
							// Set the autonomous gear variable to MIDDLE for the corresponding team.
							teams[event.joystickMove.joystickId].setAutonGear(MIDDLE);
						// else if the Left Stick is moved about the y axis to a position at least 50 down
						else if (event.joystickMove.axis == LEFT_STICK_Y_AXIS && event.joystickMove.position > 0 && abs(event.joystickMove.position) > 50)
							// Set the autonomous gear variable to NONE for the corresponding team.
							teams[event.joystickMove.joystickId].setAutonGear(NONE);
					} // END of if the autonomous gear variable is unset
				} // END of if statement checking that the corresponding team's state is GAME and the program state is PLAYING
				  // else if the state of the team that a button was pushed for is EDIT and the program state is PLAYING
				else if (teams[event.joystickMove.joystickId].getState() == EDIT && programState == PLAYING)
				{
					// if the axis is the D-pad y axis
					if (event.joystickMove.axis == DPAD_UP_DOWN_AXIS)
					{
						// if the edit selection is A_GEAR for the corresponing team
						if (teams[event.joystickMove.joystickId].getEditSelection() == A_GEAR)
						{
							// Save the current value of the autonomous gear for the corresponding team.
							tempAutonGear = teams[event.joystickMove.joystickId].getAutonGear();
							// if the autonomous gear variable is unset for the team that the button was pushed for
							if (tempAutonGear == autonGearEnum::UNSET)
							{
								// Set the autonomous gear variable to MIDDLE for the corresponding team.
								teams[event.joystickMove.joystickId].setAutonGear(MIDDLE);
								// Update the temporary autonomous gear variable.
								tempAutonGear = MIDDLE;
							} // END of if statement checking that the autonomous gear variable for the corresponding team is UNSET

							  // Convert the enumerator value to an integer.
							tempInt = static_cast<int>(tempAutonGear);

							// if the position of the axis is 100 (up button on D-pad)
							if (event.joystickMove.position == 100)
							{
								// Increment the integer so it corresponds to the next enumerator value.
								tempInt++;
								// if the integer value corresponding to the enumerator value is 4 (if it is at the UNSET value)
								if (tempInt == 4)
									// Reset the integer value to 0, the beginning
									tempInt = 0;
							} // END of if statement checking that the position of the axis is 100
							  // else if the position of the axis is -100 (down button on D-pad)
							else if (event.joystickMove.position == -100)
							{
								// Decrement the integer so it corresponds to the previous enumerator value.
								tempInt--;
								// if the integer value corresponding to the enumerator value is -1 (past the beginning of the enumerator)
								if (tempInt == -1)
									// Reset the integer value to 3, the end (other than UNSET)
									tempInt = 3;
							} // END of else statement checking that the position of the axis is -100

							  // Set the autonomous gear variable to the converted enumerator value from the integer for the corresponding team.
							teams[event.joystickMove.joystickId].setAutonGear(static_cast<autonGearEnum>(tempInt));
						} // END of if statement checking that the edit selection is A_GEAR for the corresponing team
						  // else if the edit selection is CLIMB for the corresponing team
						else if (teams[event.joystickMove.joystickId].getEditSelection() == CLIMB)
						{
							// Save the current value of the climb variable for the corresponding team.
							tempClimb = teams[event.joystickMove.joystickId].getClimb();
							// if the climb variable is UNDEF
							if (tempClimb == UNDEF)
							{
								// Set the climb variable to NO for the corresponding team.
								teams[event.joystickMove.joystickId].setClimb(NO);
								// Update the temporary climb variable.
								tempClimb = NO;
							} // END of if statement checking that the climb variable is UNDEF

							// Convert the enumerator value to an integer.
							tempInt = static_cast<int>(tempClimb);

							// if the position of the axis is 100 or -100 (up or down on the D-pad)
							if (event.joystickMove.position == 100 || event.joystickMove.position == -100)
							{
								// if the integer value is 0
								if (tempInt == 0)
									// Set the integer value to 1
									tempInt = 1;
								// else (the integer value is 1)
								else
									// Set the integer value to 0
									tempInt = 0;
							} // END of if statement checking that the position of the axis is 100 or -100

							  // Set the climb variable to the converted enumerator value from the integer for the corresponding team.
							teams[event.joystickMove.joystickId].setClimb(static_cast<climbEnum>(tempInt));
						} // END of else statement checking that the edit selection is CLIMB for the corresponing team
						// else the edit selection is A_BALLS, T_GEARS, or T_BALLS
						else
						{
							// swtich statement that depends on the edit selection of the corresponding team. [Get Original Value]
							switch (teams[event.joystickMove.joystickId].getEditSelection())
							{
							case A_BALLS:
								// Save the current value of the autonomous balls for the corresponding team.
								tempInt = teams[event.joystickMove.joystickId].getAutonBalls();
								// break the A_BALLS edit selection
								break;
								// if the edit selection variable is T_GEARS for the corresponding team
							case T_GEARS:
								// Save the current value of the teleop gears for the corresponding team.
								tempInt = teams[event.joystickMove.joystickId].getTeleopGears();
								// break the T_GEARS edit selection
								break;
							case T_BALLS:
								// Save the current value of the teleop gears for the corresponding team.
								tempInt = teams[event.joystickMove.joystickId].getTeleopBalls();
								// break the T_BALLS edit selection
								break;
							} // END of switch statement that gets the original value depending on the edit selection

							  // if the position of the axis is 100 (up button on D-pad)
							if (event.joystickMove.position == 100)
								// Increment the integer value.
								tempInt++;
							// else if the position of the axis is -100 (down button on D-pad)
							else if (event.joystickMove.position == -100)
							{
								// Decrement the integer value.
								tempInt--;
								// if the integer value has fallen below 0
								if (tempInt == -1)
									// Reset the integer value to 0.
									tempInt = 0;
							} // END of else statement checking that the position of the axis is -100

							  // swtich statement that depends on the edit selection of the corresponding team. [Save New Value]
							switch (teams[event.joystickMove.joystickId].getEditSelection())
							{
							case A_BALLS:
								// Set the autonomous balls variable to the integer for the corresponding team.
								teams[event.joystickMove.joystickId].setAutonBalls(tempInt);
								// break the A_BALLS edit selection
								break;
								// if the edit selection variable is T_GEARS for the corresponding team
							case T_GEARS:
								// Set the teleop gears variable to the integer for the corresponding team.
								teams[event.joystickMove.joystickId].setTeleopGears(tempInt);
								// break the T_GEARS edit selection
								break;
							case T_BALLS:
								teams[event.joystickMove.joystickId].setTeleopBalls(tempInt);
								// break the T_BALLS edit selection
								break;
							} // END of switch statement that saves the new value depending on the edit selection
						} // END of else statement checking that the edit selection is  A_BALLS, T_GEARS, or T_BALLS
					} // END of if statement checking that the axis changed is the y axis of the D-Pad
				} // END of else statement checking that the corresponding team's state is EDIT and the program state is PLAYING

				  // break the JoystickMoved event
				break;
			} // END of event switch statement
		} // END of while loop when there are pending events

		  // if PLAYING
		if (programState == PLAYING)
		{
			// for each of the 6 teams [Check to Save the Data]
			for (int i = 0; i < 6; i++)
			{
				// if the corresponding team's state is WAIT (submitted) and has not been saved to the database [Save Data]
				if (teams[i].getState() == WAIT && !teams[i].getSaved())
				{
					// Open the database.
					database.Open(NULL, false, false, dSN);

					// Get the team number.
					tempC = teams[i].getNumberC();
					// Set SQL Query
					//		UPDATE [teamNumber]
					//		[teamNumber] = name of the table
					query = "UPDATE " + tempC;

					// switch statement that depends on the value of the autonomous gear variable. [Set CString to Proper Value]
					switch (teams[i].getAutonGear())
					{
					case autonGearEnum::MIDDLE: tempC = "Middle"; break;
					case autonGearEnum::SIDE: tempC = "Side"; break;
					case autonGearEnum::MISS: tempC = "Miss"; break;
					case autonGearEnum::NONE: tempC = "None"; break;
					case autonGearEnum::UNSET: tempC = "Unset"; break;
					} // END of switch statement that gets the proper CString based on the autonomous gear value

					  // Set SQL Query
					  //		UPDATE statement that updates values on the team's table.
					query += " SET hasPlayed=true, autonGear='" + tempC;
					// Convert the autonomous balls integer to a string.
					tempStr = std::to_string(teams[i].getAutonBalls());
					// Convert the autonomous balls string to a CString.
					tempC = tempStr.c_str();
					// Add to SQL Query.
					query += "', autonBalls=" + tempC;
					// Convert the teleop gears integer to a string.
					tempStr = std::to_string(teams[i].getTeleopGears());
					// Convert the teleop gears string to a CString.
					tempC = tempStr.c_str();
					// Add to SQL Query.
					query += ", teleopGears=" + tempC;
					// Convert the teleop balls integer to a string.
					tempStr = std::to_string(teams[i].getTeleopBalls());
					// Convert the teleop balls string to a CString.
					tempC = tempStr.c_str();
					// Add to SQL Query.
					query += ", teleopBalls=" + tempC;

					// switch statement that depends on the value of the climb variable. [Set CString to Proper Value]
					switch (teams[i].getClimb())
					{
					case climbEnum::YES: tempC = "true";  break;
					case climbEnum::NO: case climbEnum::UNDEF: tempC = "false";
					} // END of switch statement that gets the proper CString based on the climb value

					  // Final SQL Query
					  //		UPDATE [teamNumber] SET hasPlayed=true, autonGear='[Middle, Side, Miss, None, Unset]', autonBalls=[autonBallsValue],
					  //			teleopGears=[teleopGearsValue], teleopBalls=[teleopBallsValue], climb=[true, false] WHERE match=[teamMatch]
					query += ", climb=" + tempC + " WHERE match=" + teams[i].getTeamMatch();
					// Execute the SQL query.
					database.ExecuteSQL(query);

					// Get the team number.
					tempC = teams[i].getNumberC();
					// Set SQL Query
					//		SELECT COUNT(*) AS count FROM [teamNumber] WHERE hasPlayed=true
					//		Get the number of matches a team has played.
					query = "SELECT COUNT(*) AS count FROM " + tempC + " WHERE hasPlayed=true";
					// Save the record in the results object.
					results.Open(CRecordset::forwardOnly, query, CRecordset::readOnly);
					// Get the number of matches a team has played using the field value "count."
					results.GetFieldValue(L"count", tempC);
					// Convert the count as CString into a string.
					tempStr = CW2A(tempC);
					// Convert the count as a string into const char* string.
					tempChar = tempStr.c_str();
					// Convert the count as a const char* string into a float and save it as the denominator.
					denominator = atof(tempChar);
					
					// Clear the results variable of the records.
					results.Close();

					// Get the team number.
					tempC = teams[i].getNumberC();
					// Set SQL Query
					//		SELECT autonGear, autonBalls, teleopBalls, teleopGears FROM [teamNumber] WHERE hasPlayed=true
					query = "SELECT autonGear, autonBalls, teleopBalls, teleopGears FROM " + tempC + " WHERE hasPlayed=true";
					// Save the records in the results object.
					//		snapshot = Can cycle through the records forward and backward
					//		readOnly = Cannot change values, only read them
					results.Open(CRecordset::snapshot, query, CRecordset::readOnly);

					// Start the numerator at 0.
					numerator = 0;
					// while there are still records in the results
					while (!results.IsEOF())
					{
						// Get the value from the column autonBalls.
						results.GetFieldValue(L"autonBalls", tempC);
						// Convert the autonomous balls CString into a string.
						tempStr = CW2A(tempC);
						// Convert the autonomous balls string into a const char*.
						tempChar = tempStr.c_str();
						// Convert the autonomous balls const char* into a float and add it to the numerator.
						numerator += atof(tempChar);
						// Go to the next record.
						results.MoveNext();
					} // END of while there are still records in the results
					// Calculate a fraction of all the autonomous balls from the played matches divided by the number of played matches.
					fraction = numerator / denominator;
					// Convert this fraction into a string.
					tempStr = std::to_string(fraction);
					// Convert the fraction string into a CString.
					tempC = tempStr.c_str();
					// Set SQL Query
					//		totals = table for the average values for all the teams
					//		tempC = fraction of total autonomous balls / matches played
					totalsQuery = "UPDATE totals SET avgAutonBalls=" + tempC;

					// Go back to the first record in the results.
					results.MoveFirst();
					// Reset the numerator to 0.
					numerator = 0;
					// while there are still records in the results
					while (!results.IsEOF())
					{
						// Get the value from the column teleopBalls.
						results.GetFieldValue(L"teleopBalls", tempC);
						// Convert the teleop balls CString into a string.
						tempStr = CW2A(tempC);
						// Convert the teleop balls string into a const char*.
						tempChar = tempStr.c_str();
						// Convert the teleop balls const char* into a float and add it to the numerator.
						numerator += atof(tempChar);
						// Go to the next record.
						results.MoveNext();
					} // END of while there are still records in the results
					// Calculate fraction. Numerator = total teleop balls from matches played. Denominator = matches played.
					fraction = numerator / denominator;
					// Convert the fraction into a string.
					tempStr = std::to_string(fraction);
					// Convert the string into a CString.
					tempC = tempStr.c_str();
					// Add to SQL Query.
					totalsQuery += ", avgTeleopBalls=" + tempC;

					// Go back to the first record in the results.
					results.MoveFirst();
					// Reset the numerator to 0.
					numerator = 0;
					// while there are still records in the results.
					while (!results.IsEOF())
					{
						// Get the value from the column teleopGears.
						results.GetFieldValue(L"teleopGears", tempC);
						// Convert the teleop gears CString into a string.
						tempStr = CW2A(tempC);
						// Convert the teleop gears string into a const char*.
						tempChar = tempStr.c_str();
						// Convert the teleop gears const char* into a float and add it to the numerator.
						numerator += atof(tempChar);
						// Go to the next record.
						results.MoveNext();
					} // END of while there are still records in the results
					// Calculate fraction. Numerator = total teleop gears from matches played. Denominator = matches played.
					fraction = numerator / denominator;
					// Convert the fraction into a string.
					tempStr = std::to_string(fraction);
					// Convert the string into a CString.
					tempC = tempStr.c_str();
					// Add to SQL Query.
					totalsQuery += ", avgTeleopGears=" + tempC;
					// Clear the results variable of the records.
					results.Close();

					// Get the team number.
					tempC = teams[i].getNumberC();
					// Set SQL Query
					//		tempC = team number = table
					//		Get the number of times a team has climbed.
					query = "SELECT COUNT(*) AS count FROM " + tempC + " WHERE hasPlayed=true AND climb=true";
					// Save the records in results.
					results.Open(CRecordset::forwardOnly, query, CRecordset::readOnly);
					// Get the value from count.
					results.GetFieldValue(L"count", tempC);
					// Convert the count CString into a string.
					tempStr = CW2A(tempC);
					// Convert the count string into a const char*.
					tempChar = tempStr.c_str();
					// Convert the count const char* into a float and set it as the numerator.
					numerator = atof(tempChar);
					// Clear the results variable of the records.
					results.Close();
					// Calculate fraction. Numerator = number of climbs for a team. Denominator = matcehs played.
					fraction = numerator / denominator;
					// Multiply the fraction by 100 to get a percentage.
					fraction *= 100;
					// Convert the percentage into a string.
					tempStr = std::to_string(fraction);
					// Convert the percentage string into a CString.
					tempC = tempStr.c_str();
					// Add to SQL Query.
					totalsQuery += ", perClimb=" + tempC;

					// Get the team number.
					tempC = teams[i].getNumberC();
					// Set SQL Query
					//		tempC = team number = table
					//		Get the number of times a team has scored a gear in the middle or side during autonomous.
					query = "SELECT COUNT(*) AS count FROM " + tempC + " WHERE hasPlayed=true AND (autonGear='Middle' OR autonGear='Side')";
					// Save the records in results.
					results.Open(CRecordset::forwardOnly, query, CRecordset::readOnly);
					// Get the value from count.
					results.GetFieldValue(L"count", tempC);
					// Convert the count CString into a string.
					tempStr = CW2A(tempC);
					// Convert the count string into a const char*.
					tempChar = tempStr.c_str();
					// Convert the count const char* into a float and set it as the numerator.
					numerator = atof(tempChar);
					// Clear the results variable of the records.
					results.Close();
					// Calculate fraction. Numerator = number of gears scored in autonomous. Denominator = matches played.
					fraction = numerator / denominator;
					// Multiply the fraction by 100 to get a percentage.
					fraction *= 100;
					// Convert the percentage into a string.
					tempStr = std::to_string(fraction);
					// Convert the percentage string into a CString.
					tempC = tempStr.c_str();
					// Add to SQL Query.
					totalsQuery += ", perAutonGearTotal=" + tempC;

					// Get the team number.
					tempC = teams[i].getNumberC();
					// Set SQL Query
					//		tempC = team number = table
					//		Get the number of times a team has attempted to score a gear in autonomous (missed or scored in the middle or side).
					query = "SELECT COUNT(*) AS count FROM " + tempC + " WHERE hasPlayed=true AND (autonGear='Miss' OR autonGear='Middle' OR autonGear='Side')";
					// Save the records in results.
					results.Open(CRecordset::forwardOnly, query, CRecordset::readOnly);
					// Get the value from count.
					results.GetFieldValue(L"count", tempC);
					// Convert the count CString into a string.
					tempStr = CW2A(tempC);
					// Convert the count string into a const char*.
					tempChar = tempStr.c_str();
					// Convert the count const char* into a float and set it as the denominator.
					denominator = atof(tempChar);
					// Clear the results variable of the records.
					results.Close();
					// if number of times attempted is 0
					if (denominator == 0)
						// Set the value to 0 in the perAutonGearAttempted column
						totalsQuery += ", perAutonGearAttempted=0";
					else
					{
						// Calculate fraction. Numerator = number of gears scored in autonomous. Denoinator = number of times attempted to score gear in autonomous. 
						fraction = numerator / denominator;
						// Multiply the fraction by 100 to get a percentage.
						fraction *= 100;
						// Convert the percentage into a string.
						tempStr = std::to_string(fraction);
						// Convert the percentage string into a CString.
						tempC = tempStr.c_str();
						// Add to SQL Query.
						totalsQuery += ", perAutonGearAttempted=" + tempC;
					} // END of else statement checking that the denominator is not 0
					// Get the team number.
					tempC = teams[i].getNumberC();
					// Final SQL Query
					//		UPDATE totals SET avgAutonBalls=[autonBallsAvg], avgTeleopBalls=[teleopBallsAvg], 
					//			avgTeleopGears=[teleopGearsAvg], perClimb=[climbPercentage], perAutonGearTotal=[autonGearTotalPercentage], 
					//			perAutonGearAttempted=[autonGearAttemptedPercentage] WHERE teamNumber=[teamNumber]
					totalsQuery += " WHERE teamNumber=" + tempC;

					// Execute SQL Query.
					database.ExecuteSQL(totalsQuery);
					// Set the saved state to true.
					teams[i].setSaved(true);
					// Close the connection to the database.
					database.Close();
				} // END of if statement checking that the team's state is WAIT and data not saved
			} // END of for loop that will test each team to see if it's data needs to be saved
		} // END of if statement checking that the program state is PLAYING

		// if all the teams are in the WAIT state and the program state is PLAYING [End Match]
		if (teams[0].getState() == WAIT && teams[1].getState() == WAIT && teams[2].getState() == WAIT &&
			teams[3].getState() == WAIT && teams[4].getState() == WAIT && teams[5].getState() == WAIT && programState == PLAYING)
		{
			// Set the program state to HOLD.
			programState = HOLD;

			// Open the database.
			database.Open(NULL, false, false, dSN);
			// Update the matches table so the current match has the hasPlayed value of true.
			query = "UPDATE matches SET hasPlayed=true WHERE matchNumber=" + matchNumber;
			// Execute the SQL Query.
			database.ExecuteSQL(query);
			// Close the connection to the database.
			database.Close();
		} // END of if statement checking that all the team's are in the WAIT state and the program state is PLAYING [End Match]

		// if the program state is GET_RANK
		if (programState == GET_RANK)
		{
			// Open the database.
			database.Open(NULL, false, false, dSN);

			// for each of the 6 categories [Get the 5 Teams and Values]
			for (int i = 0; i < 6; i++)
			{
				// Set SQL Query
				//		TOP 5 = 5 teams
				//		teamNumber, totalsColumns[i] = Get the team numbers and values from the respective columns.
				//		ORDER BY totalsColumns[i] DESC = Get the first 5 teams that have the greatest values in the respective column.
				query = "SELECT TOP 5 teamNumber, " + totalsColumns[i] + " FROM totals ORDER BY " + totalsColumns[i] + " DESC";
				// Save the records in results.
				results.Open(CRecordset::forwardOnly, query, CRecordset::readOnly);
				// if there are records in the results variable
				if (results.IsBOF() == 0)
				{
					// for each of the 5 teams [Set the Team Names and Values]
					for (int j = 0; j < 5; j++)
					{
						// Get the value from the column based on the outer loop index (categories).
						results.GetFieldValue(totalsColumns[i], tempC);
						/// if the outer loop index (categories) is 1, 2, or 5
						if (i == 1 || i == 2 || i == 5)
							// Add a percent sign to the end of the value
							tempC += "%";

						// switch statement based on the index of the inner loop (teams). [Set Team Values]
						switch (j)
						{
						case 0: standings[i].setTeam1ValueC(tempC); break;
						case 1: standings[i].setTeam2ValueC(tempC); break;
						case 2: standings[i].setTeam3ValueC(tempC); break;
						case 3: standings[i].setTeam4ValueC(tempC); break;
						case 4: standings[i].setTeam5ValueC(tempC); break;
						} // END of switch statement that sets the values.
						
						// Get the value from the teamNumber column.
						results.GetFieldValue(L"teamNumber", tempC);

						// switch statement based on the index of the inner loop (teams). [Set Team Numbers]
						switch (j)
						{
						case 0: standings[i].setTeam1NameC(tempC); break;
						case 1: standings[i].setTeam2NameC(tempC); break;
						case 2: standings[i].setTeam3NameC(tempC); break;
						case 3: standings[i].setTeam4NameC(tempC); break;
						case 4: standings[i].setTeam5NameC(tempC); break;
						} // END of switch statement that sets the team numbers.
						
						// Move to the next record (team).
						results.MoveNext();
					} // END of for loop that sets the team numbers and values.

					// Clear the results variable of the records.
					results.Close();

					// switch statement based on the index of the outer loop (categories). [Set Category Name]
					switch (i)
					{
					case 0: standings[i].setCategoryName("Avg. Auton Balls"); break;
					case 1: standings[i].setCategoryName("Auton Gear Success (Attempted)"); break;
					case 2: standings[i].setCategoryName("Auton Gear Success (Total)"); break;
					case 3: standings[i].setCategoryName("Avg. Teleop Balls"); break;
					case 4: standings[i].setCategoryName("Avg. Teleop Gears"); break;
					case 5: standings[i].setCategoryName("Climb Success"); break;
					} // END of switch statement that sets the category name.
				} // END of if statement checking that the results variable is not empty

				// Set the program state to STANDINGS from GET_RANK.
				programState = STANDINGS;

			} // END of for loop that goes through all the categories  and sets the team numbers, team values, and the category name.
			
			// Close connection to the database.
			database.Close();
		} // END of if statement checking that the program state is GET_RANK

		// Clear the window where blank space will be blue
		window.clear(sf::Color::Blue);

		// switch statement based on the program state [Display]
		switch (programState)
		{
		case HOLD:
		case PLAYING: SFMLCode::drawAll(window, teams, matchNumber); break;
		case STANDINGS: SFMLCode::drawAll(window, standings); break;
		case NEXT: break;
		case GET_RANK: break;
		} // END of switch statement that determines what to display
	} // END of while loop when window is open

	// if the database is open
	if (database.IsOpen())
		// Close the connection to the database.
		database.Close();


	return 0;
} // END of int main()

#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Text.hpp>
#include "Team.h"
#include "Standing.h"

class SFMLCode abstract
{
private:
	static void drawBoxes(std::vector<Team>& teamsVector, sf::RenderWindow& win);
	static void drawBoxes(std::vector<Standing>& standingVector, sf::RenderWindow& win);
	static void drawHeader(sf::Font& font, sf::RenderWindow& win, CString match);
	static void drawHeader(sf::Font& font, sf::RenderWindow& win);
	static void drawTitles(std::vector<Team>& teamsVector, sf::Font& font, sf::RenderWindow& win);
	static void drawTitles(std::vector<Standing>& standingVector, sf::Font& font, sf::RenderWindow& win);
	static void drawLabels(std::vector<Team>& teamsVector, sf::Font& font, sf::RenderWindow& win);
	static void drawLabels(std::vector<Standing>& standingVector, sf::Font& font, sf::RenderWindow& win);
	static void drawSubmit(std::vector<Team>& teamsVector, sf::Font& font, sf::RenderWindow& win);
	static void drawValues(std::vector<Team>& teamsVector, sf::Font& font, sf::RenderWindow& win);
	static void drawValues(std::vector<Standing>& standingVector, sf::Font& font, sf::RenderWindow& win);
	static void drawJoystickMessage(std::vector<Team>& teamsVector, sf::Font& font, sf::RenderWindow& win);
public:
	static void updateSizeValues(double width, double height);
	static float xOffset;
	static float yOffset;
	static void getOffset(int index);

	static float WIN_WIDTH;
	static float WIN_HEIGHT;

	static float BORDER_WIDTH;

	static float HEAD_HEIGHT;
	static float BOX_HEIGHT;
	static float BOX_WIDTH;
	static float BOX_BUFFER;

	static float TITLE_SCALE;
	static float SCALE;

	const static std::string HEADER_GAME_TEXT;
	const static std::string HEADER_STANDINGS_TEXT;
	const static std::string TITLE_TEXT;
	const static std::string AUTON_LABEL;
	const static std::string AUTON_GEAR_LABEL;
	const static std::string AUTON_BALLS_LABEL;
	const static std::string TELEOP_LABEL;
	const static std::string TELEOP_GEARS_LABEL;
	const static std::string TELEOP_BALLS_LABEL;
	const static std::string CLIMB_LABEL;

	static void drawAll(sf::RenderWindow& win, std::vector<Team>& teamsVector, CString match);
	static void drawAll(sf::RenderWindow& win, std::vector<Standing>& standingVector);

};

const std::string SFMLCode::HEADER_STANDINGS_TEXT = "Competition Standings";
const std::string SFMLCode::HEADER_GAME_TEXT = "Match: ";
const std::string SFMLCode::TITLE_TEXT = "Team ";
const std::string SFMLCode::AUTON_LABEL = "Auton";
const std::string SFMLCode::AUTON_GEAR_LABEL = "Gear";
const std::string SFMLCode::AUTON_BALLS_LABEL = "Balls";
const std::string SFMLCode::TELEOP_LABEL = "Teleop";
const std::string SFMLCode::TELEOP_GEARS_LABEL = "Gears";
const std::string SFMLCode::TELEOP_BALLS_LABEL = "Balls";
const std::string SFMLCode::CLIMB_LABEL = "Climb";

float SFMLCode::xOffset = 0;
float SFMLCode::yOffset = 0;

float SFMLCode::WIN_WIDTH = 999;
float SFMLCode::WIN_HEIGHT = 650;
float SFMLCode::BORDER_WIDTH = 0;
float SFMLCode::HEAD_HEIGHT = 0;
float SFMLCode::BOX_HEIGHT = 0;
float SFMLCode::BOX_WIDTH = 0;
float SFMLCode::BOX_BUFFER = 0;

float SFMLCode::SCALE = .7;
float SFMLCode::TITLE_SCALE = 1;

void SFMLCode::getOffset(int index)
{
	switch (index)
	{
	case 0: xOffset = 0; yOffset = 0; break;
	case 1: xOffset = BOX_WIDTH; yOffset = 0; break;
	case 2: xOffset = (BOX_WIDTH * 2); yOffset = 0; break;
	case 3: xOffset = 0; yOffset = BOX_HEIGHT; break;
	case 4: xOffset = BOX_WIDTH; yOffset = BOX_HEIGHT; break;
	case 5: xOffset = (BOX_WIDTH * 2); yOffset = BOX_HEIGHT; break;
	}
}

void SFMLCode::updateSizeValues(double width, double height)
{
	WIN_WIDTH = width;
	WIN_HEIGHT = height;

	BORDER_WIDTH = height / 325;

	HEAD_HEIGHT = height / 16;

	BOX_HEIGHT = (WIN_HEIGHT - (HEAD_HEIGHT + (BORDER_WIDTH * 2))) / 2;
	BOX_WIDTH = WIN_WIDTH / 3 + (BORDER_WIDTH * 2);
	BOX_BUFFER = BORDER_WIDTH * 4;

	SCALE = height / 929;
	TITLE_SCALE = height / 650;
}

void SFMLCode::drawBoxes(std::vector<Team>& teamsVector, sf::RenderWindow& win)
{
	for (int i = 0; i < 6; i++)
	{
		teamsVector[i].getBox().setSize(sf::Vector2f(BOX_WIDTH, BOX_HEIGHT));
		teamsVector[i].getBox().setFillColor(sf::Color::Color(248, 248, 248));
		teamsVector[i].getBox().setOutlineColor(sf::Color::Black);
		teamsVector[i].getBox().setOutlineThickness(BORDER_WIDTH);
	}

	teamsVector[0].getBox().setPosition(sf::Vector2f(0, HEAD_HEIGHT + (BORDER_WIDTH * 2)));
	teamsVector[1].getBox().setPosition(sf::Vector2f(BOX_WIDTH + BORDER_WIDTH, HEAD_HEIGHT + (BORDER_WIDTH * 2)));
	teamsVector[2].getBox().setPosition(sf::Vector2f((BOX_WIDTH + BORDER_WIDTH) * 2, HEAD_HEIGHT + (BORDER_WIDTH * 2)));
	teamsVector[3].getBox().setPosition(sf::Vector2f(0, HEAD_HEIGHT + BOX_HEIGHT + (BORDER_WIDTH * 2)));
	teamsVector[4].getBox().setPosition(sf::Vector2f(BOX_WIDTH + BORDER_WIDTH, HEAD_HEIGHT + BOX_HEIGHT + (BORDER_WIDTH * 2)));
	teamsVector[5].getBox().setPosition(sf::Vector2f((BOX_WIDTH + BORDER_WIDTH) * 2, HEAD_HEIGHT + BOX_HEIGHT + (BORDER_WIDTH * 2)));

	for (int i = 0; i < 6; i++)
	{
		win.draw(teamsVector[i].getBox());
	}
}

void SFMLCode::drawBoxes(std::vector<Standing>& standingVector, sf::RenderWindow& win)
{
	for (int i = 0; i < 6; i++)
	{
		standingVector[i].getBox().setSize(sf::Vector2f(BOX_WIDTH, BOX_HEIGHT));
		standingVector[i].getBox().setFillColor(sf::Color::Color(248, 248, 248));
		standingVector[i].getBox().setOutlineColor(sf::Color::Black);
		standingVector[i].getBox().setOutlineThickness(BORDER_WIDTH);
	}

	standingVector[0].getBox().setPosition(sf::Vector2f(0, HEAD_HEIGHT + (BORDER_WIDTH * 2)));
	standingVector[1].getBox().setPosition(sf::Vector2f(BOX_WIDTH + BORDER_WIDTH, HEAD_HEIGHT + (BORDER_WIDTH * 2)));
	standingVector[2].getBox().setPosition(sf::Vector2f((BOX_WIDTH + BORDER_WIDTH) * 2, HEAD_HEIGHT + (BORDER_WIDTH * 2)));
	standingVector[3].getBox().setPosition(sf::Vector2f(0, HEAD_HEIGHT + BOX_HEIGHT + (BORDER_WIDTH * 2)));
	standingVector[4].getBox().setPosition(sf::Vector2f(BOX_WIDTH + BORDER_WIDTH, HEAD_HEIGHT + BOX_HEIGHT + (BORDER_WIDTH * 2)));
	standingVector[5].getBox().setPosition(sf::Vector2f((BOX_WIDTH + BORDER_WIDTH) * 2, HEAD_HEIGHT + BOX_HEIGHT + (BORDER_WIDTH * 2)));

	for (int i = 0; i < 6; i++)
	{
		win.draw(standingVector[i].getBox());
	}
}

void SFMLCode::drawHeader(sf::Font& font, sf::RenderWindow& win, CString match)
{
	// 0,0 is at the top left and the bottom right is WIN_WIDTH, WIN_HEIGHT
	// Create a rectangle called header that is a thin rectangle at the top of the window
	sf::RectangleShape header(sf::Vector2f(WIN_WIDTH, HEAD_HEIGHT));
	// Set the position of the rectangle to all the way left and down BORDER_WIDTH
	header.setPosition(sf::Vector2f(0, BORDER_WIDTH));
	// Set fill color to a gray (r,g,b)
	header.setFillColor(sf::Color::Color(248, 248, 248));
	// Set the outline color to black
	header.setOutlineColor(sf::Color::Black);
	// Set the outline thickness to BORDER_WIDTH
	header.setOutlineThickness(BORDER_WIDTH);

	sf::Text matchNumberText;
	std::string tempStr = CW2A(match);
	matchNumberText.setString(HEADER_GAME_TEXT + tempStr);
	matchNumberText.setFillColor(sf::Color::Black);
	matchNumberText.setFont(font);
	matchNumberText.setScale(TITLE_SCALE, TITLE_SCALE);
	matchNumberText.setOrigin(matchNumberText.getLocalBounds().width / 2, matchNumberText.getLocalBounds().height / 2);
	matchNumberText.setPosition(sf::Vector2f(WIN_WIDTH / 2, (HEAD_HEIGHT / 2) - (BORDER_WIDTH * 2)));

	win.draw(header);
	win.draw(matchNumberText);
}

void SFMLCode::drawHeader(sf::Font& font, sf::RenderWindow& win)
{
	// 0,0 is at the top left and the bottom right is WIN_WIDTH, WIN_HEIGHT
	// Create a rectangle called header that is a thin rectangle at the top of the window
	sf::RectangleShape header(sf::Vector2f(WIN_WIDTH, HEAD_HEIGHT));
	// Set the position of the rectangle to all the way left and down BORDER_WIDTH
	header.setPosition(sf::Vector2f(0, BORDER_WIDTH));
	// Set fill color to a gray (r,g,b)
	header.setFillColor(sf::Color::Color(248, 248, 248));
	// Set the outline color to black
	header.setOutlineColor(sf::Color::Black);
	// Set the outline thickness to BORDER_WIDTH
	header.setOutlineThickness(BORDER_WIDTH);

	sf::Text headerText;
	headerText.setString(HEADER_STANDINGS_TEXT);
	headerText.setFillColor(sf::Color::Black);
	headerText.setFont(font);
	headerText.setScale(TITLE_SCALE, TITLE_SCALE);
	headerText.setOrigin(headerText.getLocalBounds().width / 2, headerText.getLocalBounds().height / 2);
	headerText.setPosition(sf::Vector2f(WIN_WIDTH / 2, (HEAD_HEIGHT / 2) - (BORDER_WIDTH * 2)));

	win.draw(header);
	win.draw(headerText);
}

void SFMLCode::drawTitles(std::vector<Team>& teamsVector, sf::Font& font, sf::RenderWindow& win)
{
	sf::Text temp;
	for (int i = 0; i < 6; i++)
	{
		getOffset(i);
		temp = teamsVector[i].getTitle();
		temp.setString(TITLE_TEXT + teamsVector[i].getNumber());
		if(i == 0 || i == 1 || i == 2)
			temp.setFillColor(sf::Color::Red);
		else
			temp.setFillColor(sf::Color::Blue);
		temp.setFont(font);
		temp.setScale(TITLE_SCALE, TITLE_SCALE);
		temp.setOrigin(temp.getLocalBounds().width / 2, temp.getLocalBounds().height / 2);
		temp.setPosition(sf::Vector2f(-BORDER_WIDTH + (BOX_WIDTH / 2) + xOffset, HEAD_HEIGHT + (BORDER_WIDTH * 2) + BOX_BUFFER + yOffset));
		win.draw(temp);
	}
}

void SFMLCode::drawTitles(std::vector<Standing>& standingVector, sf::Font& font, sf::RenderWindow& win)
{
	sf::Text temp;
	for (int i = 0; i < 6; i++)
	{
		getOffset(i);
		temp = standingVector[i].getCategoryNameLabel();
		temp.setString(standingVector[i].getCategoryName());
		temp.setFillColor(sf::Color::Black);
		temp.setFont(font);
		temp.setScale(SCALE, SCALE);
		temp.setOrigin(temp.getLocalBounds().width / 2, temp.getLocalBounds().height / 2);
		temp.setPosition(sf::Vector2f(-BORDER_WIDTH + (BOX_WIDTH / 2) + xOffset, HEAD_HEIGHT + (BORDER_WIDTH * 2) + BOX_BUFFER + yOffset));
		win.draw(temp);
	}
}

void SFMLCode::drawLabels(std::vector<Team>& teamsVector, sf::Font& font, sf::RenderWindow& win)
{
	sf::Text temp;
	for (int i = 0; i < 6; i++)
	{
		getOffset(i);
		//if (teamsVector[i].getJoystick())
		//{
			temp = teamsVector[i].getAutonLabel();
			temp.setString(AUTON_LABEL);
			temp.setFillColor(sf::Color::Black);
			temp.setFont(font);
			temp.setScale(SCALE, SCALE);
			temp.setOrigin(temp.getLocalBounds().left, temp.getLocalBounds().height / 2);
			temp.setPosition(sf::Vector2f(BORDER_WIDTH + (BOX_WIDTH * (1.0 / 15)) + xOffset, HEAD_HEIGHT + (BORDER_WIDTH * 2) + BOX_BUFFER + (BOX_HEIGHT * .1) + yOffset));
			win.draw(temp);

			temp = teamsVector[i].getAutonGearLabel();
			temp.setString(AUTON_GEAR_LABEL);
			temp.setFillColor(sf::Color::Black);
			temp.setFont(font);
			temp.setScale(SCALE, SCALE);
			temp.setOrigin(temp.getLocalBounds().left, temp.getLocalBounds().height / 2);
			temp.setPosition(sf::Vector2f(BORDER_WIDTH + (BOX_WIDTH * (1.0 / 5) + xOffset), HEAD_HEIGHT + (BORDER_WIDTH * 2) + BOX_BUFFER + (BOX_HEIGHT * .2) + yOffset));
			win.draw(temp);

			temp = teamsVector[i].getAutonBallsLabel();
			temp.setString(AUTON_BALLS_LABEL);
			temp.setFillColor(sf::Color::Black);
			temp.setFont(font);
			temp.setScale(SCALE, SCALE);
			temp.setOrigin(temp.getLocalBounds().left, temp.getLocalBounds().height / 2);
			temp.setPosition(sf::Vector2f(BORDER_WIDTH + (BOX_WIDTH * (1.0 / 5)) + xOffset, HEAD_HEIGHT + (BORDER_WIDTH * 2) + BOX_BUFFER + (BOX_HEIGHT * .3) + yOffset));
			win.draw(temp);

			temp = teamsVector[i].getTeleopLabel();
			temp.setString(TELEOP_LABEL);
			temp.setFillColor(sf::Color::Black);
			temp.setFont(font);
			temp.setScale(SCALE, SCALE);
			temp.setOrigin(temp.getLocalBounds().left, temp.getLocalBounds().height / 2);
			temp.setPosition(sf::Vector2f(BORDER_WIDTH + (BOX_WIDTH * (1.0 / 15) + xOffset), HEAD_HEIGHT + (BORDER_WIDTH * 2) + BOX_BUFFER + (BOX_HEIGHT * .4) + yOffset));
			win.draw(temp);

			temp = teamsVector[i].getTeleopGearsLabel();
			temp.setString(TELEOP_GEARS_LABEL);
			temp.setFillColor(sf::Color::Black);
			temp.setFont(font);
			temp.setScale(SCALE, SCALE);
			temp.setOrigin(temp.getLocalBounds().left, temp.getLocalBounds().height / 2);
			temp.setPosition(sf::Vector2f(BORDER_WIDTH + (BOX_WIDTH * (1.0 / 5) + xOffset), HEAD_HEIGHT + (BORDER_WIDTH * 2) + BOX_BUFFER + (BOX_HEIGHT * .5) + yOffset));
			win.draw(temp);

			temp = teamsVector[i].getTeleopBallsLabel();
			temp.setString(TELEOP_BALLS_LABEL);
			temp.setFillColor(sf::Color::Black);
			temp.setFont(font);
			temp.setScale(SCALE, SCALE);
			temp.setOrigin(temp.getLocalBounds().left, temp.getLocalBounds().height / 2);
			temp.setPosition(sf::Vector2f(BORDER_WIDTH + (BOX_WIDTH * (1.0 / 5) + xOffset), HEAD_HEIGHT + (BORDER_WIDTH * 2) + BOX_BUFFER + (BOX_HEIGHT * .6) + yOffset));
			win.draw(temp);

			temp = teamsVector[i].getClimbLabel();
			temp.setString(CLIMB_LABEL);
			temp.setFillColor(sf::Color::Black);
			temp.setFont(font);
			temp.setScale(SCALE, SCALE);
			temp.setOrigin(temp.getLocalBounds().left, temp.getLocalBounds().height / 2);
			temp.setPosition(sf::Vector2f(BORDER_WIDTH + (BOX_WIDTH * (1.0 / 5) + xOffset), HEAD_HEIGHT + (BORDER_WIDTH * 2) + BOX_BUFFER + (BOX_HEIGHT * .7) + yOffset));
			win.draw(temp);
		//}
		
	}

}

void SFMLCode::drawLabels(std::vector<Standing>& standingVector, sf::Font& font, sf::RenderWindow& win)
{
	sf::Text temp;
	for (int i = 0; i < 6; i++)
	{
		getOffset(i);
		temp = standingVector[i].getTeam1Name();
		temp.setString(standingVector[i].getTeam1NameStr());
		temp.setFillColor(sf::Color::Black);
		temp.setFont(font);
		temp.setScale(SCALE, SCALE);
		temp.setOrigin(temp.getLocalBounds().width / 2, temp.getLocalBounds().height / 2);
		temp.setPosition(sf::Vector2f(BORDER_WIDTH + (BOX_WIDTH * (1.0 / 4)) + xOffset, HEAD_HEIGHT + (BORDER_WIDTH * 2) + BOX_BUFFER + (BOX_HEIGHT * .15) + yOffset));
		win.draw(temp);

		temp = standingVector[i].getTeam2Name();
		temp.setString(standingVector[i].getTeam2NameStr());
		temp.setFillColor(sf::Color::Black);
		temp.setFont(font);
		temp.setScale(SCALE, SCALE);
		temp.setOrigin(temp.getLocalBounds().width / 2, temp.getLocalBounds().height / 2);
		temp.setPosition(sf::Vector2f(BORDER_WIDTH + (BOX_WIDTH * (1.0 / 4) + xOffset), HEAD_HEIGHT + (BORDER_WIDTH * 2) + BOX_BUFFER + (BOX_HEIGHT * .3) + yOffset));
		win.draw(temp);

		temp = standingVector[i].getTeam3Name();
		temp.setString(standingVector[i].getTeam3NameStr());
		temp.setFillColor(sf::Color::Black);
		temp.setFont(font);
		temp.setScale(SCALE, SCALE);
		temp.setOrigin(temp.getLocalBounds().width / 2, temp.getLocalBounds().height / 2);
		temp.setPosition(sf::Vector2f(BORDER_WIDTH + (BOX_WIDTH * (1.0 / 4)) + xOffset, HEAD_HEIGHT + (BORDER_WIDTH * 2) + BOX_BUFFER + (BOX_HEIGHT * .45) + yOffset));
		win.draw(temp);

		temp = standingVector[i].getTeam4Name();
		temp.setString(standingVector[i].getTeam4NameStr());
		temp.setFillColor(sf::Color::Black);
		temp.setFont(font);
		temp.setScale(SCALE, SCALE);
		temp.setOrigin(temp.getLocalBounds().width / 2, temp.getLocalBounds().height / 2);
		temp.setPosition(sf::Vector2f(BORDER_WIDTH + (BOX_WIDTH * (1.0 / 4) + xOffset), HEAD_HEIGHT + (BORDER_WIDTH * 2) + BOX_BUFFER + (BOX_HEIGHT * .6) + yOffset));
		win.draw(temp);

		temp = standingVector[i].getTeam5Name();
		temp.setString(standingVector[i].getTeam5NameStr());
		temp.setFillColor(sf::Color::Black);
		temp.setFont(font);
		temp.setScale(SCALE, SCALE);
		temp.setOrigin(temp.getLocalBounds().width / 2, temp.getLocalBounds().height / 2);
		temp.setPosition(sf::Vector2f(BORDER_WIDTH + (BOX_WIDTH * (1.0 / 4) + xOffset), HEAD_HEIGHT + (BORDER_WIDTH * 2) + BOX_BUFFER + (BOX_HEIGHT * .75) + yOffset));
		win.draw(temp);
	}
}

void SFMLCode::drawSubmit(std::vector<Team>& teamsVector, sf::Font& font, sf::RenderWindow& win)
{
	sf::Text tempText;
	teamState tempState;
	for (int i = 0; i < 6; i++)
	{
		//if (teamsVector[i].getJoystick())
		//{
			tempState = teamsVector[i].getState();
			getOffset(i);
			teamsVector[i].getSubmit().setSize(sf::Vector2f(BOX_WIDTH * .75, BOX_HEIGHT * .1));
			teamsVector[i].getSubmit().setOrigin(teamsVector[i].getSubmit().getLocalBounds().width / 2, teamsVector[i].getSubmit().getLocalBounds().height / 2);
			teamsVector[i].getSubmit().setPosition(sf::Vector2f(BORDER_WIDTH + (BOX_WIDTH * .5) + xOffset, HEAD_HEIGHT + BOX_BUFFER + (BORDER_WIDTH * 2) + (BOX_HEIGHT * .85) + yOffset));
			if (tempState == GAME)
				teamsVector[i].getSubmit().setFillColor(sf::Color::White);
			else if (tempState == EDIT)
				teamsVector[i].getSubmit().setFillColor(sf::Color::Color(237, 41, 57));
			else if (tempState == WAIT)
				teamsVector[i].getSubmit().setFillColor(sf::Color::Green);
			teamsVector[i].getSubmit().setOutlineColor(sf::Color::Black);
			teamsVector[i].getSubmit().setOutlineThickness(BORDER_WIDTH);
			win.draw(teamsVector[i].getSubmit());

			tempText = teamsVector[i].getSubmitStatus();
			if (tempState == GAME)
				tempText.setString("Submit with SELECT");
			else if (tempState == EDIT)
				tempText.setString("<EDIT>");
			else if (tempState == WAIT)
				tempText.setString("Submitted");
			tempText.setFillColor(sf::Color::Black);
			tempText.setFont(font);
			tempText.setScale(SCALE, SCALE);
			tempText.setOrigin(tempText.getLocalBounds().width / 2, tempText.getLocalBounds().height / 2);
			tempText.setPosition(sf::Vector2f(-BORDER_WIDTH + (BOX_WIDTH * .5) + xOffset, HEAD_HEIGHT + BOX_BUFFER - (BORDER_WIDTH * 2) + (BOX_HEIGHT * .85) + yOffset));
			win.draw(tempText);
		//}
	}
}

void SFMLCode::drawValues(std::vector<Team>& teamsVector, sf::Font& font, sf::RenderWindow& win)
{		
	sf::Text temp;
	std::string tempStr;
	for (int i = 0; i < 6; i++)
	{
		//if (teamsVector[i].getJoystick())
		//{
			getOffset(i);

			temp = teamsVector[i].getAutonGearValue();
			switch (teamsVector[i].getAutonGear())
			{
				case autonGearEnum::MIDDLE: tempStr = "Middle"; break;
				case autonGearEnum::SIDE: tempStr = "Side"; break;
				case autonGearEnum::MISS: tempStr = "Miss"; break;
				case autonGearEnum::NONE: tempStr = "None"; break;
				case autonGearEnum::UNSET: tempStr = "<unset>"; break;
			}

			temp.setString(tempStr);
			(teamsVector[i].getEditSelection() == A_GEAR) ? temp.setFillColor(sf::Color::Red) : temp.setFillColor(sf::Color::Black);
			temp.setFont(font);
			temp.setScale(SCALE, SCALE);
			temp.setOrigin(temp.getLocalBounds().width / 2, temp.getLocalBounds().height / 2);
			temp.setPosition(sf::Vector2f(BORDER_WIDTH + (BOX_WIDTH * (3.0 / 4) + xOffset), HEAD_HEIGHT + (BORDER_WIDTH * 2) + BOX_BUFFER + (BOX_HEIGHT * .2) + yOffset));
			win.draw(temp);

			temp = teamsVector[i].getAutonBallsValue();
			temp.setString(std::to_string(teamsVector[i].getAutonBalls()));
			(teamsVector[i].getEditSelection() == A_BALLS) ? temp.setFillColor(sf::Color::Red) : temp.setFillColor(sf::Color::Black);
			temp.setFont(font);
			temp.setScale(SCALE, SCALE);
			temp.setOrigin(temp.getLocalBounds().width / 2, temp.getLocalBounds().height / 2);
			temp.setPosition(sf::Vector2f(BORDER_WIDTH + (BOX_WIDTH * (3.0 / 4)) + xOffset, HEAD_HEIGHT + (BORDER_WIDTH * 2) + BOX_BUFFER + (BOX_HEIGHT * .3) + yOffset));
			win.draw(temp);

			temp = teamsVector[i].getTeleopGearsValue();
			temp.setString(std::to_string(teamsVector[i].getTeleopGears()));
			(teamsVector[i].getEditSelection() == T_GEARS) ? temp.setFillColor(sf::Color::Red) : temp.setFillColor(sf::Color::Black);
			temp.setFont(font);
			temp.setScale(SCALE, SCALE);
			temp.setOrigin(temp.getLocalBounds().width / 2, temp.getLocalBounds().height / 2);
			temp.setPosition(sf::Vector2f(BORDER_WIDTH + (BOX_WIDTH * (3.0 / 4) + xOffset), HEAD_HEIGHT + (BORDER_WIDTH * 2) + BOX_BUFFER + (BOX_HEIGHT * .5) + yOffset));
			win.draw(temp);

			temp = teamsVector[i].getTeleopBallsValue();
			temp.setString(std::to_string(teamsVector[i].getTeleopBalls()));
			(teamsVector[i].getEditSelection() == T_BALLS) ? temp.setFillColor(sf::Color::Red) : temp.setFillColor(sf::Color::Black);
			temp.setFont(font);
			temp.setScale(SCALE, SCALE);
			temp.setOrigin(temp.getLocalBounds().width / 2, temp.getLocalBounds().height / 2);
			temp.setPosition(sf::Vector2f(BORDER_WIDTH + (BOX_WIDTH * (3.0 / 4) + xOffset), HEAD_HEIGHT + (BORDER_WIDTH * 2) + BOX_BUFFER + (BOX_HEIGHT * .6) + yOffset));
			win.draw(temp);

			switch (teamsVector[i].getClimb())
			{
				case YES: tempStr = "Yes"; break;
				case NO: tempStr = "No"; break;
				case UNDEF: tempStr = "<unset>"; break;
			}

			temp = teamsVector[i].getClimbValue();
			temp.setString(tempStr);
			(teamsVector[i].getEditSelection() == CLIMB) ? temp.setFillColor(sf::Color::Red) : temp.setFillColor(sf::Color::Black);
			temp.setFont(font);
			temp.setScale(SCALE, SCALE);
			temp.setOrigin(temp.getLocalBounds().width / 2, temp.getLocalBounds().height / 2);
			temp.setPosition(sf::Vector2f(BORDER_WIDTH + (BOX_WIDTH * (3.0 / 4) + xOffset), HEAD_HEIGHT + (BORDER_WIDTH * 2) + BOX_BUFFER + (BOX_HEIGHT * .7) + yOffset));
			win.draw(temp);
		//}
	}
}

void SFMLCode::drawValues(std::vector<Standing>& standingVector, sf::Font& font, sf::RenderWindow& win)
{
	sf::Text temp;
	for (int i = 0; i < 6; i++)
	{
		getOffset(i);
		temp = standingVector[i].getTeam1Value();
		temp.setString(standingVector[i].getTeam1ValueStr());
		temp.setFillColor(sf::Color::Black);
		temp.setFont(font);
		temp.setScale(SCALE, SCALE);
		temp.setOrigin(temp.getLocalBounds().left, temp.getLocalBounds().height / 2);
		temp.setPosition(sf::Vector2f(BORDER_WIDTH + (BOX_WIDTH * (3.0 / 5)) + xOffset, HEAD_HEIGHT + (BORDER_WIDTH * 2) + BOX_BUFFER + (BOX_HEIGHT * .15) + yOffset));
		win.draw(temp);

		temp = standingVector[i].getTeam2Value();
		temp.setString(standingVector[i].getTeam2ValueStr());
		temp.setFillColor(sf::Color::Black);
		temp.setFont(font);
		temp.setScale(SCALE, SCALE);
		temp.setOrigin(temp.getLocalBounds().left, temp.getLocalBounds().height / 2);
		temp.setPosition(sf::Vector2f(BORDER_WIDTH + (BOX_WIDTH * (3.0 / 5) + xOffset), HEAD_HEIGHT + (BORDER_WIDTH * 2) + BOX_BUFFER + (BOX_HEIGHT * .3) + yOffset));
		win.draw(temp);

		temp = standingVector[i].getTeam3Value();
		temp.setString(standingVector[i].getTeam3ValueStr());
		temp.setFillColor(sf::Color::Black);
		temp.setFont(font);
		temp.setScale(SCALE, SCALE);
		temp.setOrigin(temp.getLocalBounds().left, temp.getLocalBounds().height / 2);
		temp.setPosition(sf::Vector2f(BORDER_WIDTH + (BOX_WIDTH * (3.0 / 5)) + xOffset, HEAD_HEIGHT + (BORDER_WIDTH * 2) + BOX_BUFFER + (BOX_HEIGHT * .45) + yOffset));
		win.draw(temp);

		temp = standingVector[i].getTeam4Value();
		temp.setString(standingVector[i].getTeam4ValueStr());
		temp.setFillColor(sf::Color::Black);
		temp.setFont(font);
		temp.setScale(SCALE, SCALE);
		temp.setOrigin(temp.getLocalBounds().left, temp.getLocalBounds().height / 2);
		temp.setPosition(sf::Vector2f(BORDER_WIDTH + (BOX_WIDTH * (3.0 / 5) + xOffset), HEAD_HEIGHT + (BORDER_WIDTH * 2) + BOX_BUFFER + (BOX_HEIGHT * .6) + yOffset));
		win.draw(temp);

		temp = standingVector[i].getTeam5Value();
		temp.setString(standingVector[i].getTeam5ValueStr());
		temp.setFillColor(sf::Color::Black);
		temp.setFont(font);
		temp.setScale(SCALE, SCALE);
		temp.setOrigin(temp.getLocalBounds().left, temp.getLocalBounds().height / 2);
		temp.setPosition(sf::Vector2f(BORDER_WIDTH + (BOX_WIDTH * (3.0 / 5) + xOffset), HEAD_HEIGHT + (BORDER_WIDTH * 2) + BOX_BUFFER + (BOX_HEIGHT * .75) + yOffset));
		win.draw(temp);
	}
}

void SFMLCode::drawJoystickMessage(std::vector<Team>& teamsVector, sf::Font& font, sf::RenderWindow& win)
{
	sf::Text temp;
	for (int i = 0; i < 6; i++)
	{
		if (!teamsVector[i].getJoystick())
		{
			getOffset(i);
			teamsVector[i].getJoystickBox().setSize(sf::Vector2f(BOX_WIDTH / 2.0, BOX_HEIGHT / 3.0));
			teamsVector[i].getJoystickBox().setFillColor(sf::Color::White);
			teamsVector[i].getJoystickBox().setOutlineColor(sf::Color::Black);
			teamsVector[i].getJoystickBox().setOutlineThickness(BORDER_WIDTH);
			teamsVector[i].getJoystickBox().setOrigin(teamsVector[i].getJoystickBox().getLocalBounds().width / 2, teamsVector[i].getJoystickBox().getLocalBounds().height / 2);
			teamsVector[i].getJoystickBox().setPosition(sf::Vector2f(BORDER_WIDTH + BOX_WIDTH * .5 + xOffset, HEAD_HEIGHT + (BOX_HEIGHT * .5) + yOffset));
			win.draw(teamsVector[i].getJoystickBox());

			temp = teamsVector[i].getJoystickText();
			temp.setFillColor(sf::Color::Red);
			temp.setFont(font);
			temp.setScale(SCALE, SCALE);
			temp.setOrigin(temp.getLocalBounds().width / 2, temp.getLocalBounds().height / 2);
			temp.setPosition(sf::Vector2f(BOX_WIDTH * .5 - BORDER_WIDTH + xOffset, HEAD_HEIGHT + (BOX_HEIGHT * .5) - (BORDER_WIDTH * 4) + yOffset));
			win.draw(temp);
		}
	}
}

void SFMLCode::drawAll(sf::RenderWindow& win, std::vector<Team>& teamsVector, CString match)
{
	sf::Font font;
	if (!font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf"))
	{
		std::cout << "BAD\n";
		system("pause");
	}
	drawBoxes(teamsVector, win);
	drawHeader(font, win, match);
	drawTitles(teamsVector, font, win);
	drawSubmit(teamsVector, font, win);
	drawValues(teamsVector, font, win);
	//drawJoystickMessage(teamsVector, font, win);
	drawLabels(teamsVector, font, win);

	win.display();
}

void SFMLCode::drawAll(sf::RenderWindow& win, std::vector<Standing>& standingVector)
{
	sf::Font font;
	if (!font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf"))
	{
		std::cout << "BAD\n";
		system("pause");
	}
	drawHeader(font, win);
	drawBoxes(standingVector, win);
	drawTitles(standingVector, font, win);
	drawLabels(standingVector, font, win);
	drawValues(standingVector, font, win);

	win.display();
}
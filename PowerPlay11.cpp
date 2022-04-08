//
//  main.cpp
//
//  Project: UALR - Programming 2 - Spring 22 - Power Play 11
//  Created by: Mark McCorkle on 20220406
//  Based on: Code Provided by Sean Orme
// 
//  IDE: CLion 2021.2.3     - UNVERIFIED
//  IDE: XCode              - UNVERIFIED
//  IDE: Visual Studio 2022 - VERIFIED
//  IDE: Linux via g++      - UNVERIFIED
//
#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <algorithm>
#include <fstream>
#include <map>
#include <memory>

#include "Item.h"
#include "Object.h"
#include "Player.h"
#include "Monster.h"

void createMonsters(std::vector<std::unique_ptr<Object>>& objects);

void displayBattle(const std::vector<std::unique_ptr<Object>>& objects);
void bringOutYourDead(std::vector<std::unique_ptr<Object>>& objects);


int main()
{
	/*
		Rule of 5 Test Examples:
	*/
	{

		// 1) Create a default Player object, player1:
		Player player1{ };

		// 2) Copy the player1 into player2 preserving player1 using Copy Constructor Test:
		Player player2{ player1 };

		// 3) Move player2 into player3 eleminating player2 using Move Copy Constructor:
		Player player3{ std::move(player2) };

		// 4) Copy player3 back into player2, preserving player3 using Assignment Operator Overload:
		player2 = player3;

		// 5) Move player3 into player4, eliminating player3 using Move Assignment Operator Overload:
		Player player4{ };
		player4 = std::move(player3);

		// 6) Try to copy player4 into itself using Assignment Operator Overload:
		player4 = player4;

		// 7) Try to move player4 into itself using Move Assignment Operator Overload:
		player4 = std::move(player4);

		system("pause");

	}

	std::vector<std::unique_ptr<Object>> objects;
	objects.push_back(std::make_unique<Player>());

	while (objects.front()->getName() == Object::Type::player)
	{
		createMonsters(objects);

		std::cout << objects.size()-1 << " monster(s) approaches!!" << std::endl;
		/*system("pause");
		system("cls");*/

		while (objects.size() > 1 
			&& objects.front()->getName() == Object::Type::player)
		{
			bringOutYourDead(objects);
			displayBattle(objects);				
			std::cout << std::endl;
			std::for_each(objects.begin(), objects.end(), [&](std::unique_ptr<Object>& object)
				{
					object->update(objects);
				});
			
			system("PAUSE");
			system("CLS");
		}
	}

	if (objects.size() == 0
		|| objects.front()->getName() != Object::Type::player)
	{
		std::cout << "You Have Died" << std::endl;
	}
	if (objects.size() == 0)
	{
		std::cout << "BUT" << std::endl;
	}
	if (objects.size() == 0
		|| objects.front()->getName() == Object::Type::player)
	{
		std::cout << "You have killed the monsters!!!" << std::endl;
	}
	system("PAUSE");

}



void displayBattle(const std::vector<std::unique_ptr<Object>>& objects)
{
	Object::nameOnly = false;
	std::cout << objects.front() << std::endl;
	std::cout << std::endl << "  Monsters: " << std::endl;
	{
		int i{ 1 };
		std::for_each(objects.begin()+1, objects.end(), [&](const std::unique_ptr<Object>& monster)
			{
				std::cout << "   " << i << ". " << monster << std::endl;

				i++;
			});
	}
}

void createMonsters(std::vector<std::unique_ptr<Object>>& objects)
{
	std::normal_distribution<double> randomNumMonsters((double)objects.front()->getLevel(), objects.front()->getLevel() / 2.0);
	objects.resize(std::max(2, (int)randomNumMonsters(Object::engine)));
	std::generate(objects.begin()+1, objects.end(), [&]()
		{
			return std::make_unique<Monster>(objects.front());
		});
}

void bringOutYourDead(std::vector<std::unique_ptr<Object>>& objects)
{
	Object::nameOnly = true;
	objects.erase(
		std::remove_if(objects.begin(), objects.end(),
			[](const std::unique_ptr<Object>& object)
			{
				if (object->isDead())
				{

					std::cout << object << " has died!!!" << std::endl << std::endl;
					return true;
				}
				return false;
			}),
		objects.end());

}

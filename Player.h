#ifndef PLAYER_H
#define PLAYER_H

	#include <memory>

	#include "Object.h"
	#include "Item.h"

	class Player: public Object
	{
	public:
		// Rule of 5
		//	   To promote deep copying and efficiency of moving resources
		// 
		// Rule of 3:
		//     Copy Constructor
		//     Overload Assignment Operator
		//     Destructor (That can be virtual)
		// 
		// Rule of 5 (Addition of the following two):
		//     Move Copy Constructor
		//     Move Assignment Operator
		// 
		//     Note: The last two were added as an effeciency measure, when source 
		//           object needed copying and assigning, but the source is not 
		//           needed afterwards.

		// Default Constructor:
		Player();

		// Copy Constructor:
		Player( const Player& src ) noexcept;

		// Move Copy Constructor:
		Player( Player&& src ) noexcept;

		// Assignment Operator Overload:
		Player& operator=( const Player& src ) noexcept;
		
		// Move Assignment Operator Overload:
		Player& operator=( Player&& ) noexcept;

		// Destructor
		~Player();

		void levelUp();

		void update(std::vector<std::unique_ptr<Object>>& objects) override;

		int getLevel() const;
		int getSP() const;

		int attack() const override;
		void defend(int damage) override;
		void print(std::ostream& o) const override;	
		void heal();


	private:
		std::map<Item::Type, std::unique_ptr<Item>> inventory;
		int SP{ 0 };

	};

	std::ostream& operator<< (std::ostream& o, const std::map<Item::Type, std::unique_ptr<Item>>& src);

#endif // !PLAYER_H
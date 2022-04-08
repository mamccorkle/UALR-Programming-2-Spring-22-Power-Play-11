#include <random>
#include <iostream>

#include "Player.h"
#include "Monster.h"

// Default Constructor:
Player::Player(): Object(Object::Type::player, 0, 1, 0)
{
	std::cout << "** Default Constructor...\n";

	levelUp();
}

// Copy Constructor:
// To copy the Player class object we have three elements that we
// need to work with: Object, SP, and Inventory. The Object class
// is inherited and doesnt make use of pointers. The spell points 
// (SP) is the same in that it doesnt use pointers. So, those are 
// copied straight over. The inventory map does make use of 
// pointers for the Item object. To recreate the map using
// unique pointers a loop was created to iterate through each pair
// in the inventory and makes a new unique pointer for each Item.
Player::Player( const Player& src ) noexcept
	: Object( src.name, src.strength, src.health, src.level ),
	SP{ src.SP }
{
	std::cout << "** Copy Constructor...\n";

	// Copy the pointer items into new memory locations for this object:
	for (const auto& item : src.inventory)
	{
		inventory.emplace(std::make_pair(item.first, std::make_unique<Item>(*item.second)));
	}
}

// Move Copy Constructor:
// Like the Copy constructor the Object class and SP are copied
// over and reset within the body of the constructor to a default
// value of 0. The inventory is moved directly over without 
// any modification
Player::Player( Player&& src ) noexcept
	: Object( src.name, src.strength, src.health, src.level ),
	inventory{ std::move(src.inventory) },
	SP{ src.SP }
{
	std::cout << "** Move Copy Constructor...\n";

	// Destroyed the link to the original inventory items by 
	// using the std::move command. Next, reset the remaining 
	// nonpointer based members.
	src.name = Object::Type::player;
	src.strength = 0;
	src.health = 0;
	src.level = 0;
	src.SP = 0;
}

// Assignment Operator Overload:
// This is a copy assignment from one memory location to
// another much like the Copy Constructor. The difference
// here is that we check if the input source is the same
// as the class object we are working on, if it is just
// return itself.
Player& Player::operator=( const Player& src ) noexcept
{
	std::cout << "** Assignment Operator Overload...\n";

	// Are we trying to copy ourselves:
	if (this == &src)
		return *this;

	// Copy the nonpointers:
	name = src.name;
	strength = src.strength;
	health = src.health;
	level = src.level;
	SP = src.SP;

	// Copy the pointer items into new memory locations for this object:
	for (const auto& item : src.inventory)
	{
		inventory.emplace(std::make_pair(item.first, std::make_unique<Item>(*item.second)));
	}
	
	return *this;
}

// Move Assignment Operator Overload:
//
// This is a move assignment from one memory location to
// another, much like the Move Copy Constructor. The 
// difference here is that we check if the input source 
// is the same as the class object we are working on, if
// it is just return itself.
Player& Player::operator=( Player&& src ) noexcept
{
	std::cout << "** Move Assignment Operator Overload...\n";

	// Are we trying to copy ourselves:
	if (this == &src)
		return *this;

	// Copy the nonpointers:
	name = src.name;
	strength = src.strength;
	health = src.health;
	level = src.level;
	SP = src.SP;

	// Move the inventory over:
	inventory = std::move(src.inventory);

	// Reset to default values:
	src.name = Object::Type::player;
	src.strength = 0;
	src.health = 0;
	src.level = 0;

	return *this;
}

// Destructor:
Player::~Player()
{
	// We are using smart pointers so nothing to be done here...

	std::cout << "** Destructor...\n";
}

void Player::levelUp()
{
	level++;
	std::normal_distribution<double> randomHealth(20.0 + level * 5, 5.0);
	health += std::max(1, (int)randomHealth(engine));

	std::normal_distribution<double> randomStrength(3.0 + level, 1.0);
	strength += std::max(1, (int)randomStrength(engine));
	SP += std::max(1, (int)randomStrength(engine));

	//grab new item.
	std::uniform_int_distribution<int> randomItem(0, (int)Item::Type::numTypes - 1);
	std::normal_distribution<double> randomBonus((double)level, (double)level / 2);
	std::unique_ptr<Item> found{ std::make_unique<Item>((Item::Type)randomItem(engine), std::max(1, (int)randomBonus(engine)))} ;

	std::cout << "You found a " << *found << "!!!!" << std::endl;
	
	if (
		auto haveOne{ inventory.find(found->getClassification()) };
		haveOne == inventory.end()
		|| (*inventory[found->getClassification()]) < *found
		)
	{
		std::cout << "You keep the shiny new toy!" << std::endl;
		inventory.insert_or_assign(found->getClassification(), std::move(found));
	}
	else
	{
		std::cout << "You toss aside the ugly old thing!" << std::endl;
	}
}

void Player::update(std::vector<std::unique_ptr<Object>>& objects)
{
	
	if (objects.size() == 1)
	{
		levelUp();
		return;
	}
	std::cout << "What do you do? (a)ttack (h)eal ";
	char command{ 'x' };
	std::cin >> command;
	switch (command)
	{
	case 'a':
	{
		int monsterNum{ 1 };
		if (objects.size() > 2)
		{
			std::cout << "Which Monster: ";
			std::cin >> monsterNum;
		}
		if (monsterNum > 0 && monsterNum <= objects.size()-1)
		{

			objects[monsterNum]->defend(objects.front()->attack());
		}
		break;
	}
	case 'h':
		heal();
		break;
	default:
		std::cout << "please enter a or h" << std::endl;
		break;
	}
	
}

int Player::getLevel() const
{
	return level;
}

int Player::getSP() const
{
	return SP;
}

int Player::attack() const
{
	if (auto sword{ inventory.find(Item::Type::sword) }; sword != inventory.end())
	{
		return damageDone(sword->second->getBonusValue());
	}
	else
	{
		return damageDone(0);
	}
}

void Player::defend(int damage)
{
	int AC{ 0 };
	if (auto armor{ inventory.find(Item::Type::armor) }; armor != inventory.end())
	{
		AC += *(armor->second);
	}
	if (auto shield{ inventory.find(Item::Type::shield) }; shield != inventory.end())
	{
		AC += *(shield->second);
	}
	damageTaken(damage, AC);
}

void Player::heal()
{
	Object::nameOnly = true;
	if (SP >= 2)
	{
		std::normal_distribution<double> randomHeal(strength*2.0, 20.0*(1.0/level));
		int  amountHealed{ std::max(1, (int)randomHeal(engine)) };

		SP -= 2;
		std::cout << *this << " is healed by " << amountHealed << "hp!" << std::endl;
		health += amountHealed;
	}
	else
	{
		std::cout << "Not enough SP!!!" << std::endl;
	}

}

void Player::print(std::ostream& o) const
{
	
	if (nameOnly) Object::print(o);
	else
	{
		o << "L:" << getLevel() << " ";
		Object::print(o);
		o << " h:" << getHealth() << ", SP:" << getSP() << std::endl;
		o << inventory;
	}
}

std::ostream& operator<<(std::ostream& o, const std::map<Item::Type, std::unique_ptr<Item>>& src)
{
	for(const auto& [type, item] : src )
		{
			o << "  " << *item << std::endl;
		};
	return o;
}
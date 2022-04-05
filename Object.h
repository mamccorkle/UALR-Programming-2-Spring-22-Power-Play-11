#ifndef OBJECT_H
#define OBJECT_H

#include <map>
#include <random>
#include <memory>

class Object
{
public:
	static std::random_device seed;
	static std::default_random_engine engine;
	static bool nameOnly;
	enum class Type { player, slime, orc, sprite, dragon, numTypes };

	Object() {}
	Object(Type name, int strength, int health, int level);
	

	virtual int attack() const = 0;
	virtual void defend(int damage) = 0;
	virtual void update(std::vector<std::unique_ptr<Object>>& objects) = 0;
	virtual void print(std::ostream& o) const;
	bool isDead();
	Type getName() const;
	int getLevel() const;
	int getHealth() const;

protected:
	int damageDone(int modification) const;
	int damageTaken(int damageDone, int AC);
	Type name{ Type::numTypes };
	int strength{ 0 };
	int health{ 0 };
	int level{ 0 };

};

std::ostream& operator<< (std::ostream& o, const Object& src);
std::ostream& operator<< (std::ostream& o, const std::unique_ptr<Object>& src);
#endif // !OBJECT_H


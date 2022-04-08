#ifndef ITEM_H
#define ITEM_H


class Item
{
public:
	Item() {};
	
	enum class Type { sword, armor, shield, numTypes };
	Item(Type classification, int bonusValue);

	Type getClassification() const;
	int getBonusValue() const;
private:
	Type classification{ Type::numTypes };
	int bonusValue{ 0 };
};

std::ostream& operator<< (std::ostream& o, const Item& src);
bool operator< (const Item& srcL, const Item& srcR);
int& operator+=(int& srcL, const Item& srcR);

#endif // !ITEM_H
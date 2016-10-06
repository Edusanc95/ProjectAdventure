
#include "GameObject.h"

GameObject::GameObject(string name, double damage){
	_name = name;
	_damage = damage;

	cout << "GameObject: "<< _name << " created" << endl;
}

GameObject::~GameObject(){
	cout << "GameObject: "<< _name << " deleted" << endl;
}

std::string GameObject::getName(){
	return _name;
}

double GameObject::getDamage(){
	return _damage;
}


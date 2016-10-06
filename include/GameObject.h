/*
 * GameObject.h
 *
 *  Created on: May 27, 2016
 *      Author: edu
 */

#ifndef INCLUDE_GAMEOBJECT_H_
#define INCLUDE_GAMEOBJECT_H_

#include <iostream>
using namespace std;

class GameObject{

public:
	GameObject(string name, double damage);
	~GameObject();
	string getName();
	double getDamage();


protected:
	string _name;
	double _damage;

};

#endif /* INCLUDE_GAMEOBJECT_H_ */

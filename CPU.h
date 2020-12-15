#pragma once
#ifndef CPU_H
#define CPU_H

#include <string>
using namespace std;

class CPU {
private:
	int currentHealth, maxHealth; 
	int healsRemain, bleedStacks;
	bool stunned, bleeding, guarding; 
	string attacks[6] = {"punch", "kick", "stab", "shoot", "heal", "guard"};

public:
	CPU();
	void setcurrentHealth(int health); //health
	int getcurrentHealth() const;

	void setmaxHealth(int health);
	int getmaxHealth() const; 

	void sethealsRemain(int numHeals); //If a heal is used, set this to be 
	int getHealsRemain() const;

	void setbleedStacks(int bleedStacks); //decreases once per turn. 
	int getBleedStacks() const;

	void takeDamage(int damage); //subtract from health

	void setStunStatus(bool status);
	bool getStunStatus() const;

	void setBleedingStatus(bool status);
	bool getBleedingStatus() const;

	void setGuardingStatus(bool status);
	bool getGuardingStatus() const;

	std::string getAttack(int diceRoll); //dice roll number dictates which string is returned.
};
#endif
#include <iostream>
#include <string>
#include <random>
#include "CPU.h"
using namespace std; 

CPU::CPU() {
	currentHealth = 100;
	maxHealth = 100;
	healsRemain = 3;
	bleedStacks = 0;
	stunned = false;
	bleeding = false;
	guarding = false;
}

void CPU::setcurrentHealth(int health) {
	if (health > maxHealth) { //check to see if health is > max Health
		this->currentHealth = maxHealth;
	}

	else { //else just set to whatever is inputted
		this->currentHealth = health;
	}
}

int CPU::getcurrentHealth() const {
	return currentHealth;
} 

void CPU::setmaxHealth(int health) {
	this->maxHealth = health;
}

int CPU::getmaxHealth() const {
	return maxHealth;
}

void CPU::sethealsRemain(int healsRemain) {
	this->healsRemain = healsRemain;
}

int CPU::getHealsRemain() const { 
	return healsRemain;
}

void CPU::setbleedStacks(int bleedStacks) { //decreases once per turn while > 0, damage per turn is 3 * bleedStacks
	this->bleedStacks = bleedStacks;
}

int CPU::getBleedStacks() const {
	return bleedStacks;
}

void CPU::setStunStatus(bool status) { //lasts for 1 turn
	this->stunned = status;
}
bool CPU::getStunStatus() const {
	return stunned;
}

void CPU::setBleedingStatus(bool status) {
	this->bleeding = status;
}
bool CPU::getBleedingStatus() const {
	return bleeding;
}

void CPU::setGuardingStatus(bool status) {
	this->guarding = status;
}
bool CPU::getGuardingStatus() const {
	return guarding;
}

void CPU::takeDamage(int damage) {
	if (currentHealth - damage < 0) { //if CPU takes damage that puts them below 0 hp
		currentHealth = 0; //just set hp to 0
	}

	else {
		currentHealth = currentHealth - damage;
	}
}

std::string CPU::getAttack(int diceRoll) {
	/*
	* Attack rates :
	* Punch - 30%
	* Kick - 20%, stuns for 1 turn
	* Stab - 15%, Inflicts bleeding for 5 turns (-3 hp each turn), MAX 3 stack currently.
	* Shoot - 5%, Inflicts Bleeding for 10 turns, MAX 1 stack.
	* Guard - 15%
	* Heal - 15%, increased to 30% if health is equal to or below 25% (Heals 30% of max HP).
	*/

	if (diceRoll > 70 && diceRoll <= 85 && currentHealth == maxHealth && healsRemain > 0) { //if full HP, can heal, but is full HP
		return "fhp"; //full hp
	}

	else if (diceRoll > 70 && diceRoll <= 100 && currentHealth <= (currentHealth * 0.25) && healsRemain > 0) { //special heal condition, if less than or equal to 25% hp, and can heal
		return attacks[4]; //heal
	}

	else if (diceRoll > 70 && diceRoll <= 85 && currentHealth < maxHealth && healsRemain == 0) { //is not full hp, but cannot heal
		return "nhr"; //no heal remains
	}

	else if(diceRoll > 70 && diceRoll <= 100 && currentHealth <= (currentHealth * 0.25) && healsRemain == 0) { //special heal condition, if less than or equal to 25% hp, and can't heal
		return "nhr"; //no heal remains
	}

	else if (diceRoll >= 0 && diceRoll <= 30) {
		return attacks[0]; //punch
	}

	else if (diceRoll > 30 && diceRoll <= 50) {
		return attacks[1]; //kick
	}

	else if (diceRoll > 50 && diceRoll <= 65) {
		return attacks[2]; //stab
	}

	else if (diceRoll > 65 && diceRoll <= 70) {
		return attacks[3]; //shoot
	}

	else if (diceRoll > 70 && diceRoll <= 85) {
		return attacks[4]; //heal
	}

	else if (diceRoll > 85 && diceRoll <= 100) {
		return attacks[5]; //guard
	}

	else {
		return " ";
	}

}

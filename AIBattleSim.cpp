/*
* DEVELOPED BY FELIX CHOY 12/15/2020
*/

#include <iostream>
#include <time.h>
#include <random>
#include <limits>
#include "CPU.h"
#include <windows.h>
#undef max

using namespace std;

/* --- GAME LOOP ---
* Menu :
* Enter HP for each CPU
* Roll a dice to see who goes first
*
* Battle Simulator :
* While one of the CPU's current health != 0
* Roll dice for CPU's turn
* CPU does a move
* If CPU cannot perform the move (Heal when full hp), reroll the dice and return a new move.
* Deal damage to OTHER CPU or Block
* Next CPU's turn
*/
void printcurrentCPUHealth(CPU* aCPU, int index); //used to print the current HP of the OTHER CPU after they take damage.

int main()
{
	random_device rand_seed; //seed used to seed the random num generator
	mt19937 randEngine_main(rand_seed()); //random number generator

	uniform_int_distribution<int> dicerollDist(0, 100); //assigns dist as a uniform int distr from [0, 100] 
	uniform_int_distribution<int> second_dicerollDist(30, 100); //use for if heal under 25% fails
	uniform_int_distribution<int> punchDist(10, 15);
	uniform_int_distribution<int> kickDist(15, 20);
	uniform_int_distribution<int> stabDist(20, 25);
	uniform_int_distribution<int> shootDist(35, 35);

	CPU* newCPU = new CPU[2]; //create 2 CPUs
	int health_one, health_two;
	int cpu_one_diceRoll = 0, cpu_two_diceRoll = 0;
	int moveDiceRoll;
	int takeDamage = 0;
	string cpuAttack;
	bool invalidMove = true;
	int turn = 0;
	int otherPlayer = 0;
	bool isDead = false;
	bool input_one_valid = false;
	bool input_two_valid = false;

	cout << "~ AI BATTLE SIMULATOR ~ " << endl;
	cout << "Developed by Felix Choy " << endl << endl;

	while (input_one_valid == false) {
		cout << "Please enter the health of CPU 1: ";
		cin >> health_one;
		if (cin.fail()) {
			cout << "Invalid input. Please try again." << endl << endl;
			cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
		else {
			newCPU[0].setmaxHealth(health_one);
			newCPU[0].setcurrentHealth(health_one);
			cout << endl;
			input_one_valid = true;
		}
	}

	while (input_two_valid == false) {
		cout << "Please enter the health of CPU 2: ";
		cin >> health_two;
		if (cin.fail()) {
			cout << "Invalid input. Please try again." << endl << endl;
			cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
		else {
			newCPU[1].setmaxHealth(health_two);
			newCPU[1].setcurrentHealth(health_two);
			cout << endl;
			input_two_valid = true;
		}
	}

	cin.ignore();

	//roll dice to see which CPU goes first.
	cpu_one_diceRoll = dicerollDist(randEngine_main);
	cpu_two_diceRoll = dicerollDist(randEngine_main);

	cout << "A dice roll will determine which CPU goes first." << endl;
	cout << "Press Enter to Continue" << flush;
	cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
	cout << endl;

	cout << "CPU #1 rolls: " << cpu_one_diceRoll << endl;
	cout << "Press Enter to Continue" << flush;
	cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
	cout << endl;
	
	cout << "CPU #2 rolls: " << cpu_two_diceRoll << endl;
	cout << "Press Enter to Continue" << flush;
	cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
	cout << endl;

	if (cpu_one_diceRoll < cpu_two_diceRoll) {
		turn = 1;
		cout << "CPU #2 will go first." << endl << endl;
	}
	else {
		turn = 0;
		cout << "CPU #1 will go first." << endl << endl;
	}
	
	if (turn == 0) { //If CPU 1 goes first
		while (newCPU[0].getcurrentHealth() != 0 || newCPU[1].getcurrentHealth() != 0) { //while neither CPU is dead
			for (int i = 0; i <= 1; i++) {

				if (newCPU[0].getcurrentHealth() == 0) {
					cout << "CPU #1 is dead. " << endl;
					cout << "CPU #2 wins. " << endl;
					cout << "Press Enter to Continue" << flush;
					cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
					delete[] newCPU;
					exit(1);
				}

				else if (newCPU[1].getcurrentHealth() == 0) {
					cout << "CPU #2 is dead. " << endl;
					cout << "CPU #1 wins. " << endl;
					cout << "Press Enter to Continue" << flush;
					cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
					delete[] newCPU;
					exit(1);
				}

				invalidMove = true;

				if (i == 0)
					otherPlayer = 1;
				else
					otherPlayer = 0;

				cout << "It is currently CPU " << i + 1 << "'s turn." << endl;
				cout << endl;
				printcurrentCPUHealth(newCPU, i);

				//press enter to continue.
				cout << "Press Enter to Continue" << flush;
				cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
				cout << endl;

				if (newCPU[i].getStunStatus() == true) { //If stunned, do nothing but take bleed damage (if CPU has bleed damage), skip turn
					cout << "CPU " << i + 1 << " is stunned. Their turn will be skipped." << endl;

					//press enter to continue.
					cout << "Press Enter to Continue" << endl;
					cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
					cout << endl;

					if (newCPU[i].getBleedingStatus() == true) {
						cout << "CPU " << i + 1 << " is bleeding. They take " << newCPU[i].getBleedStacks() * 3 << " damage." << endl;
						newCPU[i].takeDamage(newCPU[i].getBleedStacks() * 3); //take damage based on bleed stacks
						cout << endl;

						//if CPU dies here
						if (newCPU[i].getcurrentHealth() == 0) { //break from loop
							//isDead = true;
							break;
						}

						newCPU[i].setbleedStacks(newCPU[i].getBleedStacks() - 1);
						cout << "CPU " << i + 1 << " has " << newCPU[i].getBleedStacks() << " bleed stacks remaining." << endl;
						printcurrentCPUHealth(newCPU, i);
						cout << "Press Enter to Continue" << endl;
						cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
						cout << endl;

						if (newCPU[i].getBleedStacks() == 0) { //if no more bleed stacks
							newCPU[i].setBleedingStatus(false);
						}
					}
					newCPU[i].setStunStatus(false); //reset stun status
				}

				else { //if not stunned
					//check bleed status
					if (newCPU[i].getBleedingStatus() == true) {
						cout << "CPU " << i + 1 << " is bleeding. They take " << newCPU[i].getBleedStacks() * 3 << " damage." << endl;
						newCPU[i].takeDamage(newCPU[i].getBleedStacks() * 3); //take damage based on bleed stacks
						cout << endl;

						//if CPU dies here
						if (newCPU[i].getcurrentHealth() == 0) { //break from loop
							//isDead = true;
							break;
						}

						else { //else, sub 1 from bleed stacks & print out remaining bleed stacks
							newCPU[i].setbleedStacks(newCPU[i].getBleedStacks() - 1);
							cout << "CPU " << i + 1 << " has " << newCPU[i].getBleedStacks() << " bleed stacks remaining." << endl;
							printcurrentCPUHealth(newCPU, i);
							cout << endl;
							cout << "Press Enter to Continue" << endl;
							cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
							cout << endl;

							if (newCPU[i].getBleedStacks() == 0) { //if no more bleed stacks
								newCPU[i].setBleedingStatus(false);
								cout << "CPU " << i + 1 << " is no longer bleeding." << endl;
								cout << endl;
								cout << "Press Enter to Continue" << endl;
								cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
								cout << endl;
							}
						}
					}

					while (invalidMove == true) {
						moveDiceRoll = dicerollDist(randEngine_main); //roll dice
						cpuAttack = newCPU[i].getAttack(moveDiceRoll); //return an attack

						if (cpuAttack == "punch") {
							invalidMove = false;
							//other player takes damage (if blocking status is true, take 50% less damage)
							cout << "CPU #" << i + 1 << " punches CPU " << otherPlayer + 1 << endl;

							if (newCPU[otherPlayer].getGuardingStatus() == true) {
								//take floor of half damage
								takeDamage = static_cast<int>(floor(punchDist(randEngine_main) / 2));
								cout << "They deal " << takeDamage << " damage to CPU " << i + 1 << "." << endl;
								newCPU[otherPlayer].takeDamage(takeDamage);
								printcurrentCPUHealth(newCPU, otherPlayer);
								cout << endl;
								cout << "Press Enter to Continue" << endl;
								cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
								cout << endl;
								if (newCPU[otherPlayer].getcurrentHealth() == 0) { //break from loop
									//isDead = true;
									break;
								}
							}
							else {
								//take full damage
								takeDamage = punchDist(randEngine_main);
								cout << "They deal " << takeDamage << " damage to CPU " << i + 1 << "." << endl;
								newCPU[otherPlayer].takeDamage(takeDamage);
								printcurrentCPUHealth(newCPU, otherPlayer);
								cout << endl;
								cout << "Press Enter to Continue" << endl;
								cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
								cout << endl;
								//if CPU dies here
								if (newCPU[otherPlayer].getcurrentHealth() == 0) { //break from loop
									//isDead = true;
									break;
								}

							}
						}

						else if (cpuAttack == "kick") {
							invalidMove = false;
							cout << "CPU #" << i + 1 << " kicks CPU " << otherPlayer + 1 << endl;

							//other player takes damage and gets stunned for 1 turn (if blocking status is true, take 50% less damage)
							//other player takes damage (if blocking status is true, take 50% less damage)
							if (newCPU[otherPlayer].getGuardingStatus() == true) {
								//take floor of half damage & no stun
								takeDamage = static_cast<int>(floor(kickDist(randEngine_main) / 2));
								cout << "They deal " << takeDamage << " damage to CPU " << i + 1 << "." << endl;
								newCPU[otherPlayer].takeDamage(takeDamage);
								printcurrentCPUHealth(newCPU, otherPlayer);
								cout << endl;
								cout << "Press Enter to Continue" << endl;
								cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
								cout << endl;
								if (newCPU[otherPlayer].getcurrentHealth() == 0) { //break from loop
									//isDead = true;
									break;
								}

							}
							else {
								//take full damage & stun
								newCPU[otherPlayer].setStunStatus(true);
								takeDamage = kickDist(randEngine_main);
								cout << "They deal " << takeDamage << " damage to CPU " << i + 1 << "." << endl;
								newCPU[otherPlayer].takeDamage(takeDamage);
								printcurrentCPUHealth(newCPU, otherPlayer);
								cout << endl;
								cout << "Press Enter to Continue" << endl;
								cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
								cout << endl;
								if (newCPU[otherPlayer].getcurrentHealth() == 0) { //break from loop
									//isDead = true;
									break;
								}

							}
						}


						else if (cpuAttack == "stab") {
							invalidMove = false;
							cout << "CPU #" << i + 1 << " stabs CPU " << otherPlayer + 1 << endl;

							//other player takes damage and gets 4 bleed stacks (if blocking status is true, take 50% less damage)
							if (newCPU[otherPlayer].getGuardingStatus() == true) {
								//take floor of half damage & half bleed stacks
								takeDamage = static_cast<int>(floor(stabDist(randEngine_main) / 2));
								cout << "They deal " << takeDamage << " damage to CPU " << i + 1 << "." << endl;
								newCPU[otherPlayer].takeDamage(takeDamage);
								printcurrentCPUHealth(newCPU, otherPlayer);
								cout << endl;
								cout << "Press Enter to Continue" << endl;
								cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
								cout << endl;
								if (newCPU[otherPlayer].getcurrentHealth() == 0) { //break from loop
									//isDead = true;
									break;
								}
								newCPU[otherPlayer].setBleedingStatus(true);
								newCPU[otherPlayer].setbleedStacks(2);
							}
							else {
								//take full damage and bleed stacks
								takeDamage = stabDist(randEngine_main);
								cout << "They deal " << takeDamage << " damage to CPU " << i + 1 << "." << endl;
								newCPU[otherPlayer].takeDamage(takeDamage);
								printcurrentCPUHealth(newCPU, otherPlayer);
								cout << endl;
								cout << "Press Enter to Continue" << endl;
								cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
								cout << endl;
								if (newCPU[otherPlayer].getcurrentHealth() == 0) { //break from loop
									//isDead = true;
									break;
								}
								newCPU[otherPlayer].setBleedingStatus(true);
								newCPU[otherPlayer].setbleedStacks(4);
							}
						}


						else if (cpuAttack == "shoot") {
							invalidMove = false;
							cout << "CPU #" << i + 1 << " shoots CPU " << otherPlayer + 1 << endl;

							if (newCPU[otherPlayer].getGuardingStatus() == true) {
								//take floor of half damage & half bleed stacks
								takeDamage = static_cast<int>(floor(shootDist(randEngine_main) / 2));
								cout << "They deal " << takeDamage << " damage to CPU " << i + 1 << "." << endl;
								newCPU[otherPlayer].takeDamage(takeDamage);
								printcurrentCPUHealth(newCPU, otherPlayer);

								if (newCPU[otherPlayer].getcurrentHealth() == 0) { //break from loop
									//isDead = true;
									break;
								}

								newCPU[otherPlayer].setBleedingStatus(true);
								newCPU[otherPlayer].setbleedStacks(5);
								cout << endl;
								cout << "Press Enter to Continue" << endl;
								cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
								cout << endl;
							}
							else {
								//take full damage and bleed stacks
								takeDamage = shootDist(randEngine_main);
								cout << "They deal " << takeDamage << " damage to CPU " << i + 1 << "." << endl;
								newCPU[otherPlayer].takeDamage(takeDamage);
								printcurrentCPUHealth(newCPU, otherPlayer);

								if (newCPU[otherPlayer].getcurrentHealth() == 0) { //break from loop
									//isDead = true;
									break;

								}
								newCPU[otherPlayer].setBleedingStatus(true);
								newCPU[otherPlayer].setbleedStacks(10);
								cout << endl;
								cout << "Press Enter to Continue" << endl;
								cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
								cout << endl;

							}
						}


						else if (cpuAttack == "guard") {
							invalidMove = false;
							cout << "CPU #" << i + 1 << " guards. The next attack will deal half damage to them. " << endl;

							//press enter to continue.
							cout << "Press Enter to Continue" << endl;
							cin.ignore(std::numeric_limits<streamsize>::max(), '\n');

							newCPU[i].setGuardingStatus(true);
							cout << endl;
						}


						else if (cpuAttack == "heal") {
							invalidMove = false;
							cout << "CPU #" << i + 1 << " heals. They heal for " << static_cast<int>(ceil(newCPU[i].getmaxHealth() * 0.25)) << " health." << endl;

							//press enter to continue.
							cout << "Press Enter to Continue" << endl;
							cin.ignore(std::numeric_limits<streamsize>::max(), '\n');

							newCPU[i].sethealsRemain(newCPU[i].getHealsRemain() - 1);
							cout << "CPU #" << i + 1 << " has " << newCPU[i].getHealsRemain() << " / 3 remaining." << endl;
							newCPU[i].setcurrentHealth(static_cast<int>(newCPU[i].getcurrentHealth() + ceil(newCPU[i].getmaxHealth() * 0.25))); //heal hp by 25% (rounded to the greater integer)
							printcurrentCPUHealth(newCPU, i);
							cout << endl;
						}

						else if (cpuAttack == "fhp") {
							invalidMove = true;

							cout << "This CPU cannot heal, as they are full HP. They will roll another move." << endl;
							//press enter to continue.
							cout << "Press Enter to Continue" << endl;
							cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
							cout << endl;
						}

						else if (cpuAttack == "nhr") {
							invalidMove = true;
							cout << "This CPU cannot heal, as they have " << newCPU[i].getHealsRemain() << " / 3 heals remaining. They will roll another move." << endl;
							//press enter to continue.
							cout << "Press Enter to Continue" << endl;
							cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
							cout << endl;
						}

						else {
							invalidMove = true;
							cout << "Invalid move. Another move will be rolled." << endl;
							cout << endl;
							cout << "Press Enter to Continue" << endl;
							cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
							cout << endl;
						}
					}
				}
			}
		}
	}

	else if (turn == 1) {
		while (newCPU[0].getcurrentHealth() != 0 || newCPU[1].getcurrentHealth() != 0) { //while neither CPU is dead
			for (int i = 1; i >= 0; i--) {

				if (newCPU[0].getcurrentHealth() == 0) {
					cout << "CPU #1 is dead. " << endl;
					cout << "CPU #2 wins. " << endl;
					cout << "Press Enter to Continue" << flush;
					cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
					exit(1);
				}

				else if (newCPU[1].getcurrentHealth() == 0) {
					cout << "CPU #2 is dead. " << endl;
					cout << "CPU #1 wins. " << endl;
					cout << "Press Enter to Continue" << flush;
					cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
					exit(1);
				}

				invalidMove = true;

				if (i == 0)
					otherPlayer = 1;
				else
					otherPlayer = 0;

				cout << "It is currently CPU " << i + 1 << "'s turn." << endl;
				printcurrentCPUHealth(newCPU, i);

				//press enter to continue.
				cout << "Press Enter to Continue" << flush;
				cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
				cout << endl;

				if (newCPU[i].getStunStatus() == true) { //If stunned, do nothing but take bleed damage (if CPU has bleed damage), skip turn
					cout << "CPU " << i + 1 << " is stunned. Their turn will be skipped." << endl;

					//press enter to continue.
					cout << "Press Enter to Continue" << endl;
					cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
					cout << endl;

					if (newCPU[i].getBleedingStatus() == true) {
						cout << "CPU " << i + 1 << " is bleeding. They take " << newCPU[i].getBleedStacks() * 3 << " damage." << endl;
						newCPU[i].takeDamage(newCPU[i].getBleedStacks() * 3); //take damage based on bleed stacks
						cout << endl;

						//if CPU dies here
						if (newCPU[i].getcurrentHealth() == 0) { //break from loop
							//isDead = true;
							break;
						}

						newCPU[i].setbleedStacks(newCPU[i].getBleedStacks() - 1);
						cout << "CPU " << i + 1 << " has " << newCPU[i].getBleedStacks() << " bleed stacks remaining." << endl;
						printcurrentCPUHealth(newCPU, i);
						cout << "Press Enter to Continue" << endl;
						cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
						cout << endl;

						if (newCPU[i].getBleedStacks() == 0) { //if no more bleed stacks
							newCPU[i].setBleedingStatus(false);
						}
					}
					newCPU[i].setStunStatus(false); //reset stun status
				}

				else { //if not stunned
					//check bleed status
					if (newCPU[i].getBleedingStatus() == true) {
						cout << "CPU " << i + 1 << " is bleeding. They take " << newCPU[i].getBleedStacks() * 3 << " damage." << endl;
						newCPU[i].takeDamage(newCPU[i].getBleedStacks() * 3); //take damage based on bleed stacks
						cout << endl;

						//if CPU dies here
						if (newCPU[i].getcurrentHealth() == 0) { //break from loop
							//isDead = true;
							break;
						}

						else { //else, sub 1 from bleed stacks & print out remaining bleed stacks
							newCPU[i].setbleedStacks(newCPU[i].getBleedStacks() - 1);
							cout << "CPU " << i + 1 << " has " << newCPU[i].getBleedStacks() << " bleed stacks remaining." << endl;
							printcurrentCPUHealth(newCPU, i);
							cout << endl;
							cout << "Press Enter to Continue" << endl;
							cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
							cout << endl;

							if (newCPU[i].getBleedStacks() == 0) { //if no more bleed stacks
								newCPU[i].setBleedingStatus(false);
								cout << "CPU " << i + 1 << " is no longer bleeding." << endl;
								cout << endl;
								cout << "Press Enter to Continue" << endl;
								cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
								cout << endl;
							}
						}
					}

					while (invalidMove == true) {
						moveDiceRoll = dicerollDist(randEngine_main); //roll dice
						cpuAttack = newCPU[i].getAttack(moveDiceRoll); //return an attack

						if (cpuAttack == "punch") {
							invalidMove = false;
							//other player takes damage (if blocking status is true, take 50% less damage)
							cout << "CPU #" << i + 1 << " punches CPU " << otherPlayer + 1 << endl;

							if (newCPU[otherPlayer].getGuardingStatus() == true) {
								//take floor of half damage
								takeDamage = static_cast<int>(floor(punchDist(randEngine_main) / 2));
								cout << "They deal " << takeDamage << " damage to CPU " << i + 1 << "." << endl;
								newCPU[otherPlayer].takeDamage(takeDamage);
								printcurrentCPUHealth(newCPU, otherPlayer);
								cout << endl;
								cout << "Press Enter to Continue" << endl;
								cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
								cout << endl;
								if (newCPU[otherPlayer].getcurrentHealth() == 0) { //break from loop
									isDead = true;
									break;
								}
							}
							else {
								//take full damage
								takeDamage = punchDist(randEngine_main);
								cout << "They deal " << takeDamage << " damage to CPU " << i + 1 << "." << endl;
								newCPU[otherPlayer].takeDamage(takeDamage);
								printcurrentCPUHealth(newCPU, otherPlayer);
								cout << endl;
								cout << "Press Enter to Continue" << endl;
								cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
								cout << endl;
								//if CPU dies here
								if (newCPU[otherPlayer].getcurrentHealth() == 0) { //break from loop
									//isDead = true;
									break;
								}
								
							}
						}

						else if (cpuAttack == "kick") {
							invalidMove = false;
							cout << "CPU #" << i + 1 << " kicks CPU " << otherPlayer + 1 << endl;

							//other player takes damage and gets stunned for 1 turn (if blocking status is true, take 50% less damage)
							//other player takes damage (if blocking status is true, take 50% less damage)
							if (newCPU[otherPlayer].getGuardingStatus() == true) {
								//take floor of half damage & no stun
								takeDamage = static_cast<int>(floor(kickDist(randEngine_main) / 2));
								cout << "They deal " << takeDamage << " damage to CPU " << i + 1 << "." << endl;
								newCPU[otherPlayer].takeDamage(takeDamage);
								printcurrentCPUHealth(newCPU, otherPlayer);
								cout << endl;
								cout << "Press Enter to Continue" << endl;
								cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
								cout << endl;
								if (newCPU[otherPlayer].getcurrentHealth() == 0) { //break from loop
									//isDead = true;
									break;
								}

							}
							else {
								//take full damage & stun
								newCPU[otherPlayer].setStunStatus(true);
								takeDamage = kickDist(randEngine_main);
								cout << "They deal " << takeDamage << " damage to CPU " << i + 1 << "." << endl;
								newCPU[otherPlayer].takeDamage(takeDamage);
								printcurrentCPUHealth(newCPU, otherPlayer);
								cout << endl;
								cout << "Press Enter to Continue" << endl;
								cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
								cout << endl;
								if (newCPU[otherPlayer].getcurrentHealth() == 0) { //break from loop
									//isDead = true;
									break;
								}

							}
						}


						else if (cpuAttack == "stab") {
							invalidMove = false;
							cout << "CPU #" << i + 1 << " stabs CPU " << otherPlayer + 1 << endl;

							//other player takes damage and gets 4 bleed stacks (if blocking status is true, take 50% less damage)
							if (newCPU[otherPlayer].getGuardingStatus() == true) {
								//take floor of half damage & half bleed stacks
								takeDamage = static_cast<int>(floor(stabDist(randEngine_main) / 2));
								cout << "They deal " << takeDamage << " damage to CPU " << i + 1 << "." << endl;
								newCPU[otherPlayer].takeDamage(takeDamage);
								printcurrentCPUHealth(newCPU, otherPlayer);
								cout << endl;
								cout << "Press Enter to Continue" << endl;
								cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
								cout << endl;
								if (newCPU[otherPlayer].getcurrentHealth() == 0) { //break from loop
									//isDead = true;
									break;
								}
								newCPU[otherPlayer].setBleedingStatus(true);
								newCPU[otherPlayer].setbleedStacks(2);
							}
							else {
								//take full damage and bleed stacks
								takeDamage = stabDist(randEngine_main);
								cout << "They deal " << takeDamage << " damage to CPU " << i + 1 << "." << endl;
								newCPU[otherPlayer].takeDamage(takeDamage);
								printcurrentCPUHealth(newCPU, otherPlayer);
								cout << endl;
								cout << "Press Enter to Continue" << endl;
								cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
								cout << endl;
								if (newCPU[otherPlayer].getcurrentHealth() == 0) { //break from loop
									//isDead = true;
									break;
								}
								newCPU[otherPlayer].setBleedingStatus(true);
								newCPU[otherPlayer].setbleedStacks(4);
							}
						}


						else if (cpuAttack == "shoot") {
							invalidMove = false;
							cout << "CPU #" << i + 1 << " shoots CPU " << otherPlayer + 1 << endl;

							if (newCPU[otherPlayer].getGuardingStatus() == true) {
								//take floor of half damage & half bleed stacks
								takeDamage = static_cast<int>(floor(shootDist(randEngine_main) / 2));
								cout << "They deal " << takeDamage << " damage to CPU " << i + 1 << "." << endl;
								newCPU[otherPlayer].takeDamage(takeDamage);
								printcurrentCPUHealth(newCPU, otherPlayer);

								if (newCPU[otherPlayer].getcurrentHealth() == 0) { //break from loop
									//isDead = true;
									break;
								}

								newCPU[otherPlayer].setBleedingStatus(true);
								newCPU[otherPlayer].setbleedStacks(5);
								cout << endl;
								cout << "Press Enter to Continue" << endl;
								cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
								cout << endl;
							}
							else {
								//take full damage and bleed stacks
								takeDamage = shootDist(randEngine_main);
								cout << "They deal " << takeDamage << " damage to CPU " << i + 1 << "." << endl;
								newCPU[otherPlayer].takeDamage(takeDamage);
								printcurrentCPUHealth(newCPU, otherPlayer);

								if (newCPU[otherPlayer].getcurrentHealth() == 0) { //break from loop
									//isDead = true;
									break;

								}
								newCPU[otherPlayer].setBleedingStatus(true);
								newCPU[otherPlayer].setbleedStacks(10);
								cout << endl;
								cout << "Press Enter to Continue" << endl;
								cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
								cout << endl;

							}
						}


						else if (cpuAttack == "guard") {
							invalidMove = false;
							cout << "CPU #" << i + 1 << " guards. The next attack will deal half damage to them. " << endl;

							//press enter to continue.
							cout << "Press Enter to Continue" << endl;
							cin.ignore(std::numeric_limits<streamsize>::max(), '\n');

							newCPU[i].setGuardingStatus(true);
							cout << endl;
						}


						else if (cpuAttack == "heal") {
							invalidMove = false;
							cout << "CPU #" << i + 1 << " heals. They heal for " << static_cast<int>(ceil(newCPU[i].getmaxHealth() * 0.25)) << " health." << endl;
							cout << "CPU #" << i + 1 << " has " << newCPU[i].getHealsRemain() << " / 3 remaining." << endl;

							//press enter to continue.
							cout << "Press Enter to Continue" << endl;
							cin.ignore(std::numeric_limits<streamsize>::max(), '\n');

							newCPU[i].sethealsRemain(newCPU[i].getHealsRemain() - 1);
							newCPU[i].setcurrentHealth(static_cast<int>(newCPU[i].getcurrentHealth() + ceil(newCPU[i].getmaxHealth() * 0.25))); //heal hp by 25% (rounded to the greater integer)
							printcurrentCPUHealth(newCPU, i);
							cout << endl;
						}

						else if (cpuAttack == "fhp") {
							invalidMove = true;

							cout << "This CPU cannot heal, as they are full HP. They will roll another move." << endl;
							//press enter to continue.
							cout << "Press Enter to Continue" << endl;
							cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
							cout << endl;
						}

						else if (cpuAttack == "nhr") {
							invalidMove = true;
							cout << "This CPU cannot heal, as they have " << newCPU[i].getHealsRemain() << " / 3 heals remaining. They will roll another move." << endl;
							//press enter to continue.
							cout << "Press Enter to Continue" << endl;
							cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
							cout << endl;
						}

						else {
							invalidMove = true;
							cout << "Invalid move. Another move will be rolled." << endl;
							cout << endl;
							cout << "Press Enter to Continue" << endl;
							cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
							cout << endl;
						}
					}
				}
			}
		}
	}
}


void printcurrentCPUHealth(CPU* aCPU, int index) {
	cout << "CPU " << index + 1 << " currently has " << aCPU[index].getcurrentHealth() << " / " << aCPU[index].getmaxHealth() << " health remaining." << endl;

}
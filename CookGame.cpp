#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <vector>
#include <iomanip>
#include <ctime>
#include <stdlib.h>
using namespace std;

/**
 * class for rank
 */
class Rank {
private:
	string time;
	string username;
	int score;

public:
	Rank(string username, int score) :
			username(username), score(score) {
		//read system time
		time_t now = std::time(NULL);
		tm* t = localtime(&now);

		//create time string in format:  2019/4/18 11:58
		ostringstream os;
		os << t->tm_year + 1900 << "/" << t->tm_mon + 1 << "/" << t->tm_mday
				<< " " << setw(2) << setfill('0') << t->tm_min << ":" << setw(2)
				<< setfill('0') << t->tm_sec;
		time = os.str();
	}

	friend ostream& operator<<(ostream& os, const Rank& rank) {
		os << "User: " << rank.username << "\n" << "Score: " << rank.score
				<< "\n" << "time: " << rank.time << endl;
		return os;
	}

	//override '<', used for sorting by score
	bool operator<(const Rank& rank) {
		return score < rank.score;
	}
};

/**
 * class for Ingredient
 */
class Ingredient {
private:
	string name; //the name of Ingredient, e.g., "Bread"
	char symbol; //the symbol of Ingredient, e.g., "b"
	string label; //the lable for Ingredient, e.g. "[B]read"
public:
	/**
	 * constructor of Ingredient
	 */
	Ingredient(string name, char symbol, string label) :
			name(name), symbol(symbol), label(label) {
	}

	//get name of Ingredient
	string getName() const {
		return name;
	}

	//get label of Ingredient
	string getLabel() const {
		return label;
	}

	//get the symbol with lower case
	char getLowercaseSymbol() const {
		return symbol;
	}

	//get the symbol with upper case
	char getUppercaseSymbol() const {
		return symbol + 'A' - 'a';
	}

	//override << for printing a Ingredient, e.g., Bread               B/b
	friend ostream& operator<<(ostream& os, const Ingredient& ingre) {
		os.setf(ios::left);
		os << setw(20) << ingre.getName();
		os << ingre.getUppercaseSymbol() << "/" << ingre.getLowercaseSymbol();
		os.unsetf(ios::left);
		return os;
	}
};

/**
 * class for Burger
 */
class Burger {
private:
	string name; //name of Burger
	vector<Ingredient> ingres; //ingredients of Burger
	int cookingTime; //the length of cooking time
public:

	/**
	 * constructor
	 */
	Burger(string name, int cookingTime) :
			name(name), cookingTime(cookingTime) {
	}

	//get ingredient list of burger, e.g.,  [B]read, [C]heese, [T]omato, Bee[f], [L]ettuce, [B]read
	string getIngredientList() {
		ostringstream os;
		for (size_t i = 0; i < ingres.size(); i++) {
			os << ingres[i].getLabel();
			if (i != ingres.size() - 1) {
				os << ", ";
			}
		}
		return os.str();
	}

	//get the key list of burger, e.g., bctflb
	string getKeyList() {
		ostringstream os;
		for (size_t i = 0; i < ingres.size(); i++) {
			os << ingres[i].getLowercaseSymbol();
		}
		return os.str();
	}

	//validate the key list user input
	bool checkKeyList(string input) {
		string key = getKeyList();
		if (key.length() != input.length()) {
			return false;
		}

		for (size_t i = 0; i < input.size(); i++) {
			char ch = input[i];
			//convert to lower case
			if (ch >= 'A' && ch <= 'Z') {
				ch += 'a' - 'A';
			}
			if (ch != key[i]) {
				return false;
			}
		}
		return true;
	}

	//get name of burger
	string getName() const {
		return name;
	}

	//get cooking time of burger
	int getCookingTime() const {
		return cookingTime;
	}

	//get the list of ingredients, e.g., Bread, cheese, beef, lettuce, bread
	string getIngredientsString() const {
		ostringstream os;
		for (size_t i = 0; i < ingres.size(); i++) {
			os << ingres[i].getName();
			if (i != ingres.size() - 1) {
				os << ", ";
			}
		}
		return os.str();
	}

	//add a ingredient
	void addIngredient(Ingredient ingre) {
		ingres.push_back(ingre);
	}

	//override << for printing a burger, e.g., Cheese burger 		Bread, cheese, beef, lettuce, bread          10
	friend ostream& operator<<(ostream& os, const Burger& burger) {
		os.setf(ios::left);
		os << setw(20) << burger.getName();
		os << setw(50) << burger.getIngredientsString();
		os << burger.getCookingTime();
		os.unsetf(ios::left);
		return os;
	}
};

/**
 * the class for Order
 */
class Order {
private:
	int id; //the id of order, 1,2,3...
	Burger burger; //the burger
	int status; //1 for preparing, 2 for cooking, 3 for ready to serve
	time_t startTime; //the time of start order
	time_t cookTime; //the time for start cooking
public:
	/**
	 * constructor
	 */
	Order(int id, Burger burger) :
			id(id), burger(burger) {
		status = 1;
		startTime = time(NULL);
		cookTime = time(NULL);
	}

	//check if this order is overdue, player will reduce 5 scores if this is true
	bool isOverdue(time_t timeLimit) {
		time_t remain = timeLimit - (time(NULL) - startTime);
		return remain <= 0;
	}

	//update id
	void setId(int id) {
		this->id = id;
	}

	//get id of order
	int getId() {
		return id;
	}

	//get burger of order
	Burger getBurger() {
		return burger;
	}

	//get status of order
	string getStatus() {
		if (status == 1) {
			return "preparing";
		} else if (status == 2) {
			return "cooking";
		} else {
			return "ready to serve";
		}
	}

	//get remaining time for cooking
	string getCookingRemainingTime() {
		time_t remain = burger.getCookingTime() - (time(NULL) - cookTime);
		if (remain < 0) {
			remain = 0;
		}
		time_t minite = remain / 60;
		time_t seconds = remain % 60;

		ostringstream os;
		os << minite << "'" << seconds << "\"";
		return os.str();
	}

	//get remaining time
	string getRemainingTime(int timeLimit) {
		if (status == 2) {
			//it's cooking now, return cooking time remained
			return getCookingRemainingTime();
		} else {
			//the status is preparing
			time_t remain = timeLimit - (time(NULL) - startTime);
			if (remain < 0) {
				remain = 0;
			}
			time_t minite = remain / 60;
			time_t seconds = remain % 60;

			ostringstream os;
			os << minite << "'" << seconds << "\"";
			return os.str();
		}
	}

	//make this order start cooking
	void startCooking() {
		status++;
		cookTime = time(NULL);
	}

	//refresh to check the cooking is finished
	void refresh() {
		if (status == 2 && getCookingRemainingTime() == "0'0\"") {
			status = 3;
		}
	}

	//check if the status is "ready to serve"
	bool readyToServer() {
		return status == 3;
	}
};

/**
 * class for game
 */
class CookGame {
private:
	time_t timeLimit; //time total time limit, default is 40
	int maxOrder; //the max number of order, default is 5
	vector<Ingredient> ingres; //the list of all ingredients
	vector<Burger> burgers; //the list of all burgers

	int nextId; //next order id
	int score; //current score of user
	vector<Order> orders; //current orders
	vector<Rank> ranks; //the records of player
private:
	//generate new orders
	void generateOrders();
	//check order list, remove the orders ready to server and overdue, then print order list
	void checkOrderList();
	//process order
	void processOrder(Order& order);
public:
	CookGame();

	//show global menu
	void menu();
	//start new game
	void startGame();
	//do settings
	void settings();
	//show burgers and ingredients
	void burgerMenus();
	//show instructions
	void instructions();
	//show group members
	void credits();
	//show ranks
	void showRanks();
};

int main() {
	CookGame game;
	game.menu();
	return 0;
}

CookGame::CookGame() {

	timeLimit = 40;
	maxOrder = 5;
	nextId = 1;
	score = 10;

	//create all ingredients
	ingres.push_back(Ingredient("Bread", 'b', "[B]read"));
	ingres.push_back(Ingredient("Cheese", 'c', "[C]heese"));
	ingres.push_back(Ingredient("Beef", 'f', "Bee[f]"));
	ingres.push_back(Ingredient("Lettuce", 'l', "[L]ettuce"));
	ingres.push_back(Ingredient("Tomato", 't', "[T]omato"));
	ingres.push_back(Ingredient("Mushroom", 'm', "[M]ushroom"));
	ingres.push_back(Ingredient("Egg", 'e', "[E]gg"));
	ingres.push_back(Ingredient("Salmon", 's', "[S]almon"));

	//create all burgers
	Burger cheeseBurger("Cheese burger", 10);
	cheeseBurger.addIngredient(ingres[0]);
	cheeseBurger.addIngredient(ingres[1]);
	cheeseBurger.addIngredient(ingres[2]);
	cheeseBurger.addIngredient(ingres[3]);
	cheeseBurger.addIngredient(ingres[0]);
	burgers.push_back(cheeseBurger);

	Burger beefBurger("Beef burger", 10);
	beefBurger.addIngredient(ingres[0]);
	beefBurger.addIngredient(ingres[1]);
	beefBurger.addIngredient(ingres[4]);
	beefBurger.addIngredient(ingres[2]);
	beefBurger.addIngredient(ingres[3]);
	beefBurger.addIngredient(ingres[0]);
	burgers.push_back(beefBurger);

	Burger mushroomBurger("Mushroom burger", 15);
	mushroomBurger.addIngredient(ingres[0]);
	mushroomBurger.addIngredient(ingres[1]);
	mushroomBurger.addIngredient(ingres[2]);
	mushroomBurger.addIngredient(ingres[5]);
	mushroomBurger.addIngredient(ingres[0]);
	burgers.push_back(mushroomBurger);

	Burger veggieBurger("Veggie burger", 10);
	veggieBurger.addIngredient(ingres[0]);
	veggieBurger.addIngredient(ingres[4]);
	veggieBurger.addIngredient(ingres[6]);
	veggieBurger.addIngredient(ingres[3]);
	veggieBurger.addIngredient(ingres[0]);
	burgers.push_back(veggieBurger);

	Burger salmonBurger("Salmon burger", 15);
	salmonBurger.addIngredient(ingres[0]);
	salmonBurger.addIngredient(ingres[1]);
	salmonBurger.addIngredient(ingres[2]);
	salmonBurger.addIngredient(ingres[7]);
	salmonBurger.addIngredient(ingres[0]);
	burgers.push_back(salmonBurger);
}

void CookGame::menu() {
	cout << "Welcome Message designed by your group" << endl;
	cout << endl;

	bool quit = false;
	while (!quit) {
		system("cls");

		cout << "*** Game Menu ***" << endl;
		cout << "[1] Start Game" << endl;
		cout << "[2] Settings" << endl;
		cout << "[3] Burger Menus" << endl;
		cout << "[4] Instructions" << endl;
		cout << "[5] Credits" << endl;
		cout << "[6] Rank" << endl;
		cout << "[7] Exit" << endl;
		cout << "*****************" << endl;
		cout << "Option (1 - 7): ";

		string option;
		cin >> option;

		if (option == "1") {
			startGame();
		} else if (option == "2") {
			settings();
		} else if (option == "3") {
			burgerMenus();
		} else if (option == "4") {
			instructions();
		} else if (option == "5") {
			credits();
		} else if (option == "6") {
			showRanks();
		} else if (option == "7") {
			while (true) {
				cout << "confirm to exit?(y/n): ";
				cin >> option;
				if (option == "y" || option == "Y") {
					quit = true;
					break;
				} else if (option == "n" || option == "N") {
					quit = false;
					break;
				} else {
					cout << "invalid option" << endl;
				}
			}
		} else {
			cout << "invalid option" << endl;
		}

		if (!quit) {
			system("pause");
		}
	}
}

void CookGame::checkOrderList() {
	cout << "*** Order list ***" << endl;

	vector<Order> temp;

	for (size_t i = 0; i < orders.size(); i++) {
		orders[i].refresh();
		if (orders[i].getStatus() != "ready to serve") {
			if (orders[i].isOverdue(timeLimit)) {
				//the order is over due
				score -= 5;
			} else {
				//the order is not over due
				orders[i].setId(temp.size() + 1);
				temp.push_back(orders[i]);
			}
		} else {
			//the order is ready to serve
			//score += 10;
			temp.push_back(orders[i]);
		}
	}

	//only keep the orders not over due
	orders = temp;

	//have 70 percent probability to fulfill orders list
	if (orders.size() < maxOrder) {
		double probility = 70;
		if (rand() % 100 < 70) {
			generateOrders();
		}
	}

	//print current orders
	for (size_t i = 0; i < orders.size(); i++) {
		cout << "Order #" << (i + 1) << ": ";
		cout << orders[i].getBurger().getName() << ", ";
		cout << orders[i].getStatus() << ", ";
		cout << orders[i].getRemainingTime(timeLimit);
		cout << endl;
	}
	cout << "-----------------------------------------------" << endl;
	cout << "Score: " << score << endl;
	cout << "Enter [U] for update, [Q] for Quit, or [1-" << orders.size()
			<< "] for order: ";
}

void CookGame::startGame() {
	//reset game status
	orders.clear();
	score = 10;
	nextId = 1;

	string username;
	cout << "enter your name: ";
	cin.ignore(100,'\n');
	getline(cin, username);

	generateOrders();
	bool quit = false;
	while (!quit) {
		system("cls");

		checkOrderList();

		//if score < 0, game is over
		if (score < 0) {
			cout << endl;
			cout << "your got negative score, game is over." << endl;

			//add record
			Rank rank(username, score);
			ranks.push_back(rank);

			system("pause");
			break;
		}

		string option;
		cin >> option;
		if (option == "U" || option == "u") {
			//update
		} else if (option == "Q" || option == "q") {
			while (true) {
				cout << "confirm to quit?(y/n): ";
				cin >> option;
				if (option == "y" || option == "Y") {

					//add record
					Rank rank(username, score);
					ranks.push_back(rank);

					quit = true;
					break;
				} else if (option == "n" || option == "N") {
					quit = false;
					break;
				} else {
					cout << "invalid option" << endl;
				}
			}
		} else {
			int index = atoi(option.c_str());
			if (index >= 1 && index <= orders.size()) {

				/**
				 * If the player chooses the order with status "cooking", message should be prompted
				 to the player to let him/her know that the burger is not yet done.
				 */
				if (orders[index - 1].getStatus() == "cooking") {
					cout << "the burger is not yet done." << endl;
				} else if (orders[index - 1].getStatus() == "ready to serve") {
					cout << "the burger is done, you go 10 score" << endl;
					score += 10;
					//remove this order
					orders.erase(orders.begin() + index - 1);
				} else {
					processOrder(orders[index - 1]);
				}
			} else {
				cout << "invalid option" << endl;
			}
		}

		system("pause");
	}
}

void CookGame::processOrder(Order& order) {

	while (true) {
		system("cls");

		cout << "*** Process Order ***" << endl;
		cout.setf(ios::left);
		int width = 40;
		cout << setw(width) << "Order #" << ": " << order.getId() << endl;
		cout << setw(width) << "Burger" << ": " << order.getBurger().getName()
				<< endl;
		cout << setw(width) << "Status" << ": " << order.getStatus() << endl;
		cout << setw(width) << "Remaining Time" << ": "
				<< order.getRemainingTime(timeLimit) << endl;
		cout << setw(width) << "Burger Ingredient List" << ": "
				<< order.getBurger().getIngredientList() << endl;
		cout << setw(width) << "Burger Key List" << ": "
				<< order.getBurger().getKeyList() << endl;
		cout << endl;

		cout << "Please choose [U] for update, [R] for return, or" << endl;
		cout << "type correct key list to start cooking: ";

		string option;
		cin >> option;
		if (option == "U" || option == "u") {
			//update
		} else if (option == "R" || option == "r") {
			break;
		} else if (order.getBurger().checkKeyList(option)) {
			//user input the correct key list, start cooking
			order.startCooking();
			break;
		} else {
			cout << "invalid option" << endl;
			system("pause");
		}
	}
}

//show setting menu
void CookGame::settings() {
	bool quit = false;
	while (!quit) {
		system("cls");

		cout << "*** Settings Menu ***" << endl;
		cout << "[1] Time limit [" << timeLimit << " sec]" << endl;
		cout << "[2] Max number of orders [" << maxOrder << "]" << endl;
		cout << "[3] Return to Game Menu" << endl;
		cout << "*********************" << endl;
		cout << "Option (1 - 3): ";

		string option;
		cin >> option;

		if (option == "1") {
			//update time limit
			cout << "current time limit: " << timeLimit << endl;
			cout << "enter new time limit: ";
			time_t limit;
			cin >> limit;
			if (cin.good()) {
				if (limit > 10 && limit <= 300) {
					timeLimit = limit;
					cout << "time limit updated" << endl;
				} else {
					cout << "invalid time limit number" << endl;
				}
			} else {
				cout << "please input a integer" << endl;
				cin.clear();
			}
		} else if (option == "2") {
			//update max number of order
			cout << "current max number of order: " << maxOrder << endl;
			cout << "enter new max number of order: ";
			int number;
			cin >> number;
			if (cin.good()) {
				if (number >= 1 && number <= 50) {
					maxOrder = number;
					cout << "max number of order updated" << endl;
				} else {
					cout << "invalid max number of order" << endl;
				}
			} else {
				cout << "please input a integer" << endl;
				cin.clear();
			}
		} else if (option == "3") {
			quit = true;
		} else {
			cout << "invalid option" << endl;
		}

		if (!quit) {
			system("pause");
		}
	}
}

//show burger menu
void CookGame::burgerMenus() {
	bool quit = false;
	while (!quit) {
		system("cls");

		cout << "**** Burger Menu *****" << endl;
		cout << "[1] Types of Burger" << endl;
		cout << "[2] Ingredient Symbols" << endl;
		cout << "[3] Return to Game Menu" << endl;
		cout << "*********************" << endl;
		cout << "Option (1 - 3): ";

		string option;
		cin >> option;

		if (option == "1") {
			//print table title

			cout.setf(ios::left);
			cout << setw(20) << "Type of burgers";
			cout << setw(50) << "Ingredients orders";
			cout << "Cooking time";
			cout.unsetf(ios::left);
			cout << endl;

			//print all burgers
			for (size_t i = 0; i < burgers.size(); i++) {
				cout << burgers[i] << endl;
			}
		} else if (option == "2") {
			//print all ingredients
			for (size_t i = 0; i < ingres.size(); i++) {
				cout << ingres[i] << endl;
			}
		} else if (option == "3") {
			//3 for quit
			quit = true;
		} else {
			cout << "invalid option" << endl;
		}

		if (!quit) {
			system("pause");
		}
	}
}

//print instructions
void CookGame::instructions() {
	cout << "When the game starts, the player will initially have 10 marks."
			<< endl;
	cout << "If the order is served on time, 10 marks will be awarded." << endl;
	cout
			<< "When the order cannot be served within the time limit, 5 marks will be deducted."
			<< endl;
	cout
			<< "The game finishes when the score < 0. If the game finishes, the system should notice"
			<< endl;
	cout << "the player, and the program returns to the Game Menu." << endl;
}

//print group members
void CookGame::credits() {
	cout << "name" << "\t" << "id" << "\t" << "class" << "\t" << "group"
			<< endl;
	cout << "name" << "\t" << "id" << "\t" << "class" << "\t" << "group"
			<< endl;
	cout << "name" << "\t" << "id" << "\t" << "class" << "\t" << "group"
			<< endl;
}

void CookGame::showRanks() {
	if (ranks.size() == 0) {
		cout << "no records now" << endl;
	} else {
		//sort records by score ascending
		sort(ranks.begin(), ranks.end());
		//reverse list
		reverse(ranks.begin(), ranks.end());
		//print all records
		for (size_t i = 0; i < ranks.size(); i++) {
			cout << "Rank " << i+1 << endl;
			cout << ranks[i] << endl;
		}
		cout << endl;
	}
}

//generate new orders
void CookGame::generateOrders() {
	while (orders.size() < maxOrder) {
		orders.push_back(Order(nextId++, burgers[rand() % burgers.size()]));
	}
}

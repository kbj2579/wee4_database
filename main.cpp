#include "database.h"
#include <iostream>
#include <string>


int main() {
	std::string commandType = "command (list, add, get, del, exit): ";
	std::string command, key, typeInput;
	int int_value;;
	double dou_value;
	std::string str_value;
	void* value = &int_value;


	Database database;
	Database* database_p = &database;
	Entry entry;
	Entry* entry_p = &entry;
	Type type = INT;
	Array arr;
	init(database);
	
	int cnt = 0;
	while (cnt!=4) {
		cnt++;
		std::cout << commandType;
		std::cin >> command;
		
		if (command == "list") {
			list(database);
		}

		else if (command == "add") {
			
			checkType(database, entry_p);

			if (database.size == 0) {
				database.db_array[0] = *entry_p;
				database.size++;
			} else {
				add(database, entry_p);
			}
			
		}
	}
	
}


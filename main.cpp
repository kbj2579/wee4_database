#include "database.h"
#include <iostream>
#include <string>


int main() {
	std::string commandType = "command (list, add, get, del, exit): ";
	std::string command, key, typeInput;
	int int_value;;
	double dou_value;
	void* value = &int_value;

	Database database;
	Entry entry;
	Entry* entry_p = &entry;
	Type type = INT;
	init(database);


	while (true) {
		std::cout << commandType;
		std::cin >> command;

		// command 가 list 인 경우
		if (command == "list") {
			list(database);
		}

		// command 가 add 인 경우
		else if (command == "add") {
			std::cout << "key: "; // key 입력
			std::cin >> key;
			std::cout << "type (int, double, string, array): ";
			std::cin >> typeInput; // type 입력
			std::cout << "value: ";
			if (typeInput == "int") {// 타입이 int인 경우
				type = INT;
				std::cin >> int_value;
				if (exception_handling()) continue; // 예외처리
				value = &int_value;
				entry_p = create(type, key, value);
			}
			else if (typeInput == "double") { // 타입이 double인 경우
				type = DOUBLE;
				std::cin >> dou_value;
				if (exception_handling()) continue; // 예외처리
				value = &dou_value;
				entry_p = create(type, key, value);
			}
			else if (typeInput == "string") {	// 타입이 string인 경우
				type = STRING;
				std::cin.ignore(); // 입력 버퍼 비우기
				std::string str_input;
				std::getline(std::cin, str_input);
				if (exception_handling()) continue; // 예외처리
				value = new std::string(str_input); // 문자열을 동적할당하여 value에 저장
				entry_p = create(type, key, value);
			}
			else if (typeInput == "array") {	// 타입이 array인 경우
				Array* arr_p = new Array;
				arr_p->size = 0;
				arr_p->type = INT;
				arr_p->items = nullptr;
				initArray(arr_p);
				value = static_cast<Array*>(arr_p);
				entry_p = create(ARRAY, key, value);
			}
			else { // 주어지지 않은 type인 경우
				std::cout << "invalid type" << '\n';
				continue;
			}


			if (database.size == 0) { // database의 크기가 0인경우 
				database.db_array[0] = *entry_p;
				database.size++;
			}
			else { // database의 크기가 1 이상인 경우
				add(database, entry_p);
			}

		}
		else if (command == "get") {
			std::cout << "key: ";
			std::cin >> key;
			entry_p = get(database, key);
			if (entry_p->key == key) {
				std::cout << entry_p->key << ": ";

				// value의 타입을 확인
				if (entry_p->type == INT) {
					std::cout << *(int*)(entry_p->value);
				}
				else if (entry_p->type == DOUBLE) {
					std::cout << *(double*)(entry_p->value);
				}
				else if (entry_p->type == STRING) {
					std::cout << "\"" << *(std::string*)(entry_p->value) << "\"";
				}
				else if (entry_p->type == ARRAY) {
					Array* arr = static_cast<Array*>(entry_p->value);
					listArray(*arr);
				}
				std::cout << '\n';
			}
			else { // key에 해당하는 값을 못찾으면 문구출력
				std::cout << "not found" << '\n';
			}

		}
		else if (command == "del") {
			std::cout << "key: ";
			std::cin >> key;
			remove(database, key);
		}
		else if (command == "exit") {
			destroy(database);
			exit(0);
		}
		else {
			std::cout << "invalid command" << '\n';
		}
		std::cout << '\n';
	}

}


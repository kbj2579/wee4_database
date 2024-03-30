#include <string>
#include <iostream>
enum Type { INT, DOUBLE, STRING, ARRAY };

struct Array {
	int size;
	Type type;
	void* items;
};

struct Entry {
	Type type;
	std::string key;
	void* value;
};

struct Database {
	int size;
	Entry* db_array; // Entry type 배열
	Array arr; // ARRAY type 배열
};

// 엔트리를 생성한다.
Entry* create(Type type, std::string key, void* value) {
	Entry* entry = new Entry;
	entry->type = type;
	entry->key = key;
	entry->value = value;
	return entry;
}

// 데이터베이스를 초기화한다.
void init(Database& database) {
	database.size = 0;
	database.db_array = new Entry[1];
	database.arr.size = 0; 
	database.arr.items = nullptr;
}


// 데이터베이스에 엔트리를 추가한다.
void add(Database& database, Entry* entry) {
	Entry* new_array = new Entry[database.size + 1];
	for (int i = 0; i < database.size; i++) {
		new_array[i] = database.db_array[i];
	}
	new_array[database.size] = *entry;
	delete[] database.db_array;

	database.db_array = new_array;
	database.size++;
}	


// 데이터베이스에 리스트를 출력한다.
void list(Database& database) {
	for (int i = 0; i < database.size; i++) {
		std::cout << database.db_array[i].key << ": ";
		// value의 데이터 형식에 따라 출력을 변경
		if (database.db_array[i].type == INT) {
			std::cout << *(int*)(database.db_array[i].value);
		}
		else if (database.db_array[i].type == DOUBLE) {
			std::cout << *(double*)(database.db_array[i].value);
		}
		else if (database.db_array[i].type == STRING) {
			std::cout << *(std::string*)(database.db_array[i].value);
		}
		else if (database.db_array[i].type == ARRAY) {
		}
		else {
			std::cout << "invalid command";
		}
		std::cout << '\n';
	}
}

void checkType(Database& database, Entry* entry) {
	int int_value;;
	double dou_value;
	std::string key, str_value, typeInput;
	void* value = &int_value;
	Type type = INT;
	std::cout << "key: "; // key 입력
	std::cin >> key;
	std::cout << "type (int, double, string, array): ";
	std::cin >> typeInput; // type 입력
	std::cout << "value: ";
	if (typeInput == "int") {
		type = INT;
		std::cin >> int_value;
		value = &int_value;
	}
	else if (typeInput == "double") {
		type = DOUBLE;
		std::cin >> dou_value;
		value = &dou_value;
	}
	else if (typeInput == "string") {
		type = STRING;
		std::cin.ignore();
		std::getline(std::cin, str_value);
		value = &str_value;
	}
	else if (typeInput == "array") {
		/////
	}
	else {
		std::cout << "invalid command";
	}
	entry = create(type, key, value);
}
void initArray(Array& array) {
	int size;
	std::string typeInput;
	std::cout << "size: ";
	std::cin >> size;
	array.size = size;
	std::cout << "type (int, double, string, array): ";
	std::cin >> typeInput;
	if (typeInput == "int") {
		array.type = INT;
		array.items = new int[array.size];
	}
	else if (typeInput == "double") {
		array.type = DOUBLE;
		array.items = new double[array.size];
	}
	else if (typeInput == "string") {
		array.type = STRING;
		array.items = new std::string[array.size];
	}
	else {
		std::cout << "Invalid type input. Array creation failed." << std::endl;
		array.size = 0; // 배열 크기를 0으로 설정하여 무효화
		array.items = nullptr; // 배열 포인터를 null로 설정하여 무효화
	}
}








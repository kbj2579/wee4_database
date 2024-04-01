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
	// 문자열인 경우 복사본을 생성하여 저장
	if (type == STRING) {
		std::string* str_value = static_cast<std::string*>(value);
		entry->value = new std::string(*str_value);
	}
	else { // 문자열이 아닌경우
		entry->value = value;
	}
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
	// key 값이 중복된 경우 값을 덮어씀   
	for (int i = 0; i < database.size; i++) {
		if (database.db_array[i].key == entry->key) {
			database.db_array[i] = *entry;
			return;
		}
	}
	// key 값이 중복되지 않은 경우, 새로운 엔트리를 만들고, db_array를 해제후, 동적할당하여 추가
	Entry* new_array = new Entry[database.size + 1];
	for (int i = 0; i < database.size; i++) {
		new_array[i] = database.db_array[i];
	}
	Entry* new_entry = create(entry->type, entry->key, entry->value);

	// 새로운 엔트리를 추가
	new_array[database.size] = *new_entry;
	delete[] database.db_array;
	database.db_array = new_array;
	database.size++;
}

// 데이터베이스에서 키에 해당하는 엔트리를 찾는다.
Entry* get(Database& database, std::string& key) {
	Entry* entry = new Entry;
	for (int i = 0; i < database.size; i++) {
		if (database.db_array[i].key == key) {
			entry->key = key;
			entry->type = database.db_array[i].type;
			entry->value = database.db_array[i].value;
			return entry;
		}
	}
	entry->key = "invalid";
	return entry;
}

// 데이터베이스에서 키에 해당하는 엔트리를 제거한다.
void remove(Database& database, std::string& key) {
	// Database의 크기가 0인경우, 문구출력하고 return
	if (database.size == 0) {
		std::cout << "not found" << '\n';
		return;
	}
	int key_index = -1;
	Entry* new_array = new Entry[database.size - 1];
	for (int i = 0; i < database.size; i++) { // 입력받은 key 와 일치하는 데이터베이스 db_array key값의 index 를 찾는다.
		if (database.db_array[i].key == key) {
			key_index = i;
		}
	}

	// key_index를 못찾았을경우, 문구출력하고 return
	if (key_index == -1) {
		std::cout << "invalid command" << '\n';
	}

	if (database.size == 1 && key_index == 0) {
		delete[] database.db_array;
		init(database);
		return;
	}
	for (int j = 0; j < key_index; j++) { // 해당하는 key와 value 를 제거하고, 새로 동적할당한다.
		new_array[j] = database.db_array[j];
	}
	for (int k = key_index; k < database.size - 1; k++) {
		new_array[k] = database.db_array[k + 1];
	}
	delete[] database.db_array;
	database.db_array = new_array;
	database.size--;
}

// 데이터베이스를 해제한다.
void destroy(Database& database) {
	if (database.size != 0) {
		delete[] database.db_array;
	}
}


void listArray(Array& array);
// 데이터베이스에 리스트를 출력한다.
void list(Database& database) {
	for (int i = 0; i < database.size; i++) {
		std::cout << database.db_array[i].key << ": ";

		// value의 타입을 확인
		if (database.db_array[i].type == INT) {
			std::cout << *(int*)(database.db_array[i].value);
		}
		else if (database.db_array[i].type == DOUBLE) {
			std::cout << *(double*)(database.db_array[i].value);
		}
		else if (database.db_array[i].type == STRING) {
			std::cout << "\"" << *(std::string*)(database.db_array[i].value) << "\"";
		}
		else if (database.db_array[i].type == ARRAY) {
			Array* arr = static_cast<Array*>(database.db_array[i].value);
			listArray(*arr);
		}
		else {
			std::cout << "invalid command";
		}
		std::cout << '\n';
	}
}

bool exception_handling();
bool exception_handling(std::string str);
// value 타입이 array 일때 초기화하고, 필요하다면 중첩배열을 만든다.
void initArray(Array* array) {
	int size;
	std::string typeInput;
	std::cout << "type (int, double, string, array): ";
	std::cin >> typeInput;
	if (typeInput == "int") {
		array->type = INT;
		std::cout << "size: ";
		std::cin >> size;
		if (exception_handling("size")) return; // 예외처리
		array->size = size;
		array->items = new int[array->size];
		for (int i = 0; i < array->size; i++) {
			std::cout << "item[" << i << "]: ";
			std::cin >> static_cast<int*>(array->items)[i];
			if (exception_handling(typeInput)) return; // 예외처리
		}
	}
	else if (typeInput == "double") {
		array->type = DOUBLE;
		std::cout << "size: ";
		std::cin >> size;
		if (exception_handling("size")) return; // 예외처리
		array->size = size;
		array->items = new double[array->size];
		for (int i = 0; i < array->size; i++) {
			std::cout << "item[" << i << "]: ";
			std::cin >> static_cast<double*>(array->items)[i];
			if (exception_handling(typeInput)) return; // 예외처리
		}
	}
	else if (typeInput == "string") {
		array->type = STRING;
		std::cout << "size: ";
		std::cin >> size;
		if (exception_handling("size")) return; // 예외처리
		array->size = size;
		array->items = new std::string[array->size];
		std::cin.ignore();
		for (int i = 0; i < array->size; i++) {
			std::cout << "item[" << i << "]: ";
			std::getline(std::cin, static_cast<std::string*>(array->items)[i]);
			if (exception_handling(typeInput)) return; // 예외처리
		}
	}
	else if (typeInput == "array") {
		array->type = ARRAY;
		std::cout << "size: ";
		std::cin >> size;
		if (exception_handling(typeInput)) return; // 예외처리
		array->size = size;
		array->items = new Array[array->size];
		for (int i = 0; i < array->size; i++) {
			std::cout << "item[" << i << "]: ";
			Array* item = &(static_cast<Array*>(array->items)[i]);
			initArray(item);
			if (exception_handling(typeInput)) return; // 예외처리
		}
	}
	else {
		std::cout << "invalid type" << '\n';
		array->size = 0; // 배열 크기 0으로 설정
		array->items = nullptr; // 배열 초기화
	}
}

// 데이터 베이스 list 를 출력할때 해당 타입이 array 일때 이를 출력한다.
void listArray(Array& array) {
	std::cout << "[";
	for (int i = 0; i < array.size; i++) {
		if (array.type == INT) {
			std::cout << static_cast<int*>(array.items)[i];
		}
		else if (array.type == DOUBLE) {
			std::cout << static_cast<double*>(array.items)[i];
		}
		else if (array.type == STRING) {
			std::cout << "\"" << static_cast<std::string*>(array.items)[i] << "\"";
		}
		else if (array.type == ARRAY) {
			listArray(static_cast<Array*>(array.items)[i]);
		}
		else {
			std::cout << "invalid command";
		}

		if (i != array.size - 1) std::cout << ", ";
	}
	std::cout << "]";
}

// 입력받으려는 변수의 타입과 입력받은값의 타입이 다른 경우 오류문구를 출력한다.
bool exception_handling() {
	if (std::cin.fail()) { // 입력이 실패
		std::cin.clear(); // 입력 버퍼를 초기화하여 오류를 제거
		std::cin.ignore();
		std::cout << "Invalid type" << '\n';
		return true;
	}
	return false;
}


bool exception_handling(std::string str) {
	if (std::cin.fail()) { // 입력이 실패
		std::cin.clear(); // 입력 버퍼를 초기화하여 오류를 제거
		std::cin.ignore();
		std::cout << "Invalid " << str << '\n';
		return true;
	}
	return false;
}







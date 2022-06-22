// Project: libv, File: sandbox/libv_sqlite3_main.cpp

// std
#include <fmt/format.h>
#include <fmt/printf.h>
#include <sqlite3/sqlite3.h>


static int callback(void* userdata, int argc, char** argv, char** column_names) {
	if (argc == 0)
		return 0;

	bool& is_first = *reinterpret_cast<bool*>(userdata);

	if (is_first) {
		is_first = false;

		for (int i = 0; i < argc - 1; i++)
			fmt::print("{:^20}|", column_names[i]);
		fmt::print("{:^20}", column_names[argc - 1]);
		fmt::print("\n");

		std::string dashes(20, '-');
		for (int i = 0; i < argc - 1; i++)
			fmt::print("{}+", dashes);
		fmt::print("{}\n", dashes);
	}

	for (int i = 0; i < argc - 1; i++)
		fmt::print("{:>20}|", argv[i] ? argv[i] : "NULL");
	fmt::print("{:>20}", argv[argc - 1] ? argv[argc - 1] : "NULL");

	fmt::print("\n");
	return 0;
}

int main(int argc, char** argv) {
//	if (argc != 3) {
//		argc = 3;
//		argv[1] = "test.db";
//		argv[2] = "select * from test";
//	}

	if (argc != 3) {
		fmt::print(stderr, "Usage: {} DATABASE SQL-STATEMENT\n", argv[0]);
		return (1);
	}

	sqlite3* db;
	int rc = sqlite3_open(argv[1], &db);
	if (rc) {
		fmt::print(stderr, "Can't open database: {}\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		return (1);
	}

	char* error_message = nullptr;
	bool is_first = true;
	rc = sqlite3_exec(db, argv[2], callback, &is_first, &error_message);
	if (rc != SQLITE_OK) {
		fmt::print(stderr, "SQL error: {}\n", error_message);
		sqlite3_free(error_message);
	}

	sqlite3_close(db);
	return 0;
}

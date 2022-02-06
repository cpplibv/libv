// Project: libv, File: sandbox/libv_reflection2_main.cpp

// libv
#include <libv/meta/for_constexpr.hpp>
#include <libv/meta/reflection.hpp>
#include <libv/serial/reflection.hpp>
#include <libv/serial/serial.hpp>
// std
#include <functional>
#include <iomanip>
#include <iostream>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>


// -------------------------------------------------------------------------------------------------

template <typename T> using UpdateField = std::optional<T>;
template <typename T> using StateField = T;

template <template <template <typename> class> class FieldSet>
struct Update : FieldSet<UpdateField> {
};

template <template <template <typename> class> class FieldSet>
struct State : FieldSet<StateField> {
	State& operator+=(const Update<FieldSet>& update) {
		libv::meta::interleave_member_reference([](auto& lhs, const auto& rhs) {
			if (rhs)
				lhs = *rhs;
		}, *this, update);
		return *this;
	}

	Update<FieldSet> operator-(const State<FieldSet>& rhsState) {
		Update<FieldSet> resultUpdate;
		libv::meta::interleave_member_reference([](auto& result, const auto& lhs, const auto& rhs) {
			if (lhs != rhs)
				result = lhs;
		}, resultUpdate, *this, rhsState);
		return resultUpdate;
	}
};

// -------------------------------------------------------------------------------------------------

template <template <typename> class Field>
struct ServerFieldSet {
	Field<bool> has_mod{};
	Field<bool> has_password{};
	Field<std::string> map{};
	Field<std::string> name{};
	Field<uint16_t> players_current{};
	Field<uint16_t> players_limit{};

	LIBV_REFLECTION_ACCESS(has_mod);
	LIBV_REFLECTION_ACCESS(has_password);
	LIBV_REFLECTION_ACCESS(map);
	LIBV_REFLECTION_ACCESS(name);
	LIBV_REFLECTION_ACCESS(players_current);
	LIBV_REFLECTION_ACCESS(players_limit);
	LIBV_SERIALIZATION_ENABLE_REFLECTION();
};

// -------------------------------------------------------------------------------------------------

template <typename T>
auto printHeader = [] () {
	std::cout << "       ";
	libv::meta::foreach_static_member_name<T>([] (const auto& name) {
		std::cout << "| " << std::setw(11) << std::string_view(name).substr(0, 11) << ' ';
	}); std::cout << '\n';
	std::cout << "-------";
	libv::meta::for_constexpr<0, libv::meta::member_count_v<T>>([] {
		std::cout << "+-------------";
	}); std::cout << '\n';
};

auto printRow = [] (const auto& obj) {
	libv::meta::foreach_member_reference(obj, [](const auto& reference) {
		std::cout << "| \u001B[36m" << std::setw(11) << reference << "\u001B[0m ";
	}); std::cout << '\n';
};

auto printUpdate = [] (const auto& obj) {
	libv::meta::foreach_member_reference(obj, [](const auto& reference) {
		 if (reference)
			std::cout << "| \u001B[33m" << std::setw(11) << *reference << "\u001B[0m ";
		 else
			std::cout << "|      ~      ";
	}); std::cout << '\n';
};






template <typename Key, template <template <typename> class> class FieldSet>
class FieldSetTable {
private:
	struct PackedState {
		std::size_t changeID;
		State<FieldSet> state;

		LIBV_REFLECTION_ACCESS(changeID);
		LIBV_REFLECTION_ACCESS(state);
		LIBV_SERIALIZATION_ENABLE_REFLECTION();
	};
	struct PackedUpdate {
		Key key;
		Update<FieldSet> update;

		LIBV_REFLECTION_ACCESS(key);
		LIBV_REFLECTION_ACCESS(update);
		LIBV_SERIALIZATION_ENABLE_REFLECTION();
	};

private:
	std::unordered_map<Key, PackedState> map;
	std::vector<PackedUpdate> changes;

public:
	std::vector<PackedUpdate> fetchChanges() {
		decltype(changes) result;
		std::swap(result, changes);
		return result;
	}

	void applyChanges(const std::vector<PackedUpdate>& packedUpdates) {
		for (const auto& packedUpdate : packedUpdates) {
			auto& packedState = map[packedUpdate.key];
			packedState.state += packedUpdate.update;

			if (packedState.changeID == 0) {
				changes.emplace_back(packedUpdate);
			}


		}
//		map[change.key].changeID = 0;
//		changes.clear();
	}

public:

//	const State<FieldSet>& operator[](const Key& key) const {
//		return map[key];
//	}
//
//	const State<FieldSet>& get(const Key& key) const {
//		return map[key];
//	}
//
//	void emplace(Key key, State<FieldSet> state) {
//		map.emplace(std::move(key), std::move(state));
//	}
//
//	void update(const Key& key, const Update<FieldSet>& update_) {
//		map[key] += update_;
//	}
//
//	void print() {
//		printHeader<State<FieldSet>>();
//		for (const auto& [key, row] : map) {
//			std::cout << std::setw(11) << key << ' ';
//			printRow(row);
//		}
//	}

};



using ServerID = uint64_t;

struct BatchUpdate {
	std::vector<ServerID> add;
	std::vector<ServerID> remove;
	std::vector<std::pair<ServerID, Update<ServerFieldSet>>> updates;
};

class Master {
	FieldSetTable<ServerID, ServerFieldSet> servers;

	std::function<void(BatchUpdate&)> send;
};
class Server {
	ServerID id;
	State<ServerFieldSet> state;

	std::function<void(Update<ServerFieldSet>&)> send;
};
class Client {
	FieldSetTable<ServerID, ServerFieldSet> servers;
};


int main(int, char**) {
	std::cout << std::boolalpha;

	Update<ServerFieldSet> update00;
	update00.has_mod = true;
	update00.has_password = false;
	update00.map = "Disk v1";
	update00.name = "Liviotora";
	update00.players_current = 4;

	Update<ServerFieldSet> update01;
	update01.map = "Spiral v1";
	update01.players_current = 6;
	update01.players_limit = 8;


	std::vector<Client> clients;
	std::vector<Server> servers;
	Master master;



//	master.send = [&](auto&& updates) {
//		for (auto& client : clients)
//			client.receive(updates);
//	};
//	for (auto& server : servers) {
//		server.send = [&](auto&& updates) {
//			master.receive(updates);
//		};
//	}

//	table.emplace(29444, State<ServerFieldSet>{});
//	table.update(29444, update00);
//	table.update(29444, update01);
//
//	table.emplace(29443, State<ServerFieldSet>{});
//	table.update(29443, update01);
//
//	table.emplace(29445, State<ServerFieldSet>{});
//	table.update(29445, update01);
//
//	table.emplace(29446, State<ServerFieldSet>{});
//	table.update(29445, update01);
//	table.update(29445, update00);

//	table.print();

	return 0;
}

// =================================================================================================

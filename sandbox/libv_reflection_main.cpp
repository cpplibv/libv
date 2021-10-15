// Project: libv, File: sandbox/libv_reflection_main.cpp

// libv
#include <libv/meta/for_constexpr.hpp>
#include <libv/meta/reflection.hpp>
#include <libv/serial/reflection.hpp>
#include <libv/serial/serial.hpp>
#include <libv/utility/member_offset.hpp>
// std
#include <iomanip>
#include <iostream>
#include <optional>
#include <string>
#include <string_view>


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
	Field<uint16_t> players_current{};
	Field<uint16_t> players_limit{};
	Field<std::string> map{};
	Field<std::string> name{};

	LIBV_REFLECTION_ACCESS(has_mod);
	LIBV_REFLECTION_ACCESS(has_password);
	LIBV_REFLECTION_ACCESS(players_current);
	LIBV_REFLECTION_ACCESS(players_limit);
	LIBV_REFLECTION_ACCESS(map);
	LIBV_REFLECTION_ACCESS(name);
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
	libv::meta::for_constexpr<0, libv::meta::member_count_v<T>>([](auto) {
		std::cout << "+-------------";
	}); std::cout << '\n';
};

auto printRow = [] (const auto& obj) {
	std::cout << "State  ";
	libv::meta::foreach_member_reference(obj, [] (const auto& value) {
		std::cout << "| \u001B[36m" << std::setw(11) << value << "\u001B[0m ";
	}); std::cout << '\n';
};

auto printUpdate = [] (const auto& obj) {
	std::cout << "Update ";
	libv::meta::foreach_member_reference(obj, [] (const auto& value) {
		 if (value)
			std::cout << "| \u001B[33m" << std::setw(11) << *value << "\u001B[0m ";
		 else
			std::cout << "|      ~      ";
	}); std::cout << '\n';
};

int main(int, char**) {
//	using ServerID = uint64_t;
//	const auto serverID = ServerID{29444};
//
//	libv::meta::Table<ServerID, ServerFieldSet> table;

	std::cout << std::boolalpha;
	printHeader<State<ServerFieldSet>>();

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

	State<ServerFieldSet> row1;

	printRow(row1);
	printUpdate(update00);

	row1 += update00;

	printRow(row1);
	printUpdate(update01);

	row1 += update01;

	printRow(row1);

	std::cout << "\n\n"; // -----

	Update<ServerFieldSet> update10;
	update00.has_password = true;
	update10.map = "Spiral v2";

	State<ServerFieldSet> row2;
	printRow(row2);
	printUpdate(update10);

	row2 += update10;

	printRow(row2);

	auto diff = row2 - row1;

	printUpdate(diff);

	row1 += diff;
	auto diff2 = row2 - row1;

	printUpdate(diff2);

	// TODO P5: create a proper unit test for reflection

	libv::meta::foreach_static_member_pointer<decltype(row1)>([](auto&& member_pointer) {
		std::cout << libv::member_offset(member_pointer) << ", ";
	});

	std::cout << std::endl;

	libv::meta::foreach_static_member_pointer<decltype(update00)>([](auto&& member_pointer) {
		std::cout << libv::member_offset(member_pointer) << ", ";
	});

	std::cout << std::endl;

	constexpr int num_uint16_t_members = [] {
		int i = 0;
		libv::meta::foreach_static_member<State<ServerFieldSet>>([&i](auto member) {
			if (std::is_same_v<typename decltype(member)::value_type, uint16_t>)
				++i;
		});
		return i;
	}();

	std::cout << "num_uint16_t_members: " << num_uint16_t_members << std::endl;

	return 0;
}

// =================================================================================================

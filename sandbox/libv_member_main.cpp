// File: meta_main.cpp Author: Vader Created on 2017. június 16., 6:42

#define LIBV_USE_SERIALIZATION_CEREAL

// libv
#include <libv/meta/member.hpp>
#include <libv/serialization/member.hpp>
#include <libv/serialization/serialization.hpp>
// std
#include <iostream>
#include <optional>
#include <sstream>


// -------------------------------------------------------------------------------------------------

template <typename Key, template <typename> class FieldSet>
struct Table {
	// ...
};

template <typename T> using UpdateField = std::optional<T>;
template <typename T> using StateField = T;

template <template <template <typename> class> class FieldSet>
struct Update : FieldSet<UpdateField> {
};

template <template <template <typename> class> class FieldSet>
struct State : FieldSet<StateField> {
	State& operator+=(const Update<FieldSet>& update) {
		libv::meta::interleave_member_value([](auto& lhs, const auto& rhs) {
			if (rhs)
				lhs = *rhs;
		}, *this, update);
		return *this;
	}

	Update<FieldSet> operator-(const State<FieldSet>& rhsState) {
		Update<FieldSet> resultUpdate;
		libv::meta::interleave_member_value([](auto& result, const auto& lhs, const auto& rhs) {
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

	LIBV_META_MEMBER_LIST {
		LIBV_META_MEMBER(0, has_mod);
		LIBV_META_MEMBER(1, has_password);
		LIBV_META_MEMBER(2, map);
		LIBV_META_MEMBER(3, name);
		LIBV_META_MEMBER(4, players_current);
		LIBV_META_MEMBER(5, players_limit);
	}

	LIBV_SERIALIAZTION_ENABLE_MEMBER();
};

// -------------------------------------------------------------------------------------------------

int main(int, char**) {
//	using ServerID = uint64_t;
//	const auto serverID = ServerID{29444};
//
//	libv::meta::Table<ServerID, ServerFieldSet> table;

	Update<ServerFieldSet> update00;
	update00.has_mod = false;
	update00.has_password = false;
	update00.map = "Disk v1";
	update00.name = "Liviotora";
	update00.players_current = 4;

	Update<ServerFieldSet> update01;
	update01.map = "Spiral v1";
	update01.players_current = 6;
	update01.players_limit = 8;

	State<ServerFieldSet> row1;
	libv::meta::foreach_member_nvp(row1, [](const auto& name, const auto& value) { std::cout << name << ":\u001B[36m" << value << "\u001B[0m "; }); std::cout << std::endl;
	std::cout << "\n - "; libv::meta::foreach_member_nvp(update00, [](const auto& name, const auto& value) { if (value) std::cout << name << ":\u001B[36m" << *value << "\u001B[0m "; }); std::cout << std::endl;
	row1 += update00;
	libv::meta::foreach_member_nvp(row1, [](const auto& name, const auto& value) { std::cout << name << ":\u001B[36m" << value << "\u001B[0m "; }); std::cout << std::endl;
	std::cout << "\n - "; libv::meta::foreach_member_nvp(update01, [](const auto& name, const auto& value) { if (value) std::cout << name << ":\u001B[36m" << *value << "\u001B[0m "; }); std::cout << std::endl;
	row1 += update01;
	libv::meta::foreach_member_nvp(row1, [](const auto& name, const auto& value) { std::cout << name << ":\u001B[36m" << value << "\u001B[0m "; }); std::cout << std::endl;

	Update<ServerFieldSet> update10;
	update00.has_password = true;
	update10.map = "Spiral v2";

	State<ServerFieldSet> row2;
	libv::meta::foreach_member_nvp(row2, [](const auto& name, const auto& value) { std::cout << name << ":\u001B[36m" << value << "\u001B[0m "; }); std::cout << std::endl;
	std::cout << "\n - "; libv::meta::foreach_member_nvp(update10, [](const auto& name, const auto& value) { if (value) std::cout << name << ":\u001B[36m" << *value << "\u001B[0m "; }); std::cout << std::endl;
	row2 += update10;
	libv::meta::foreach_member_nvp(row2, [](const auto& name, const auto& value) { std::cout << name << ":\u001B[36m" << value << "\u001B[0m "; }); std::cout << std::endl;

	auto diff = row2 - row1;
	std::cout << "\n - "; libv::meta::foreach_member_nvp(diff, [](const auto& name, const auto& value) { if (value) std::cout << name << ":\u001B[36m" << *value << "\u001B[0m "; }); std::cout << std::endl;

	row1 += diff;
	auto diff2 = row2 - row1;
	std::cout << "\n - "; libv::meta::foreach_member_nvp(diff2, [](const auto& name, const auto& value) { if (value) std::cout << name << ":\u001B[36m" << *value << "\u001B[0m "; }); std::cout << std::endl;

	return 0;
}

// =================================================================================================





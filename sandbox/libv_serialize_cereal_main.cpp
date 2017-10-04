// File: vserialize_main.cpp, Created on 2017. 04. 14. 15:09, Author: Vader

// ext
#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/archives/portable_binary.hpp>
#include <cereal/archives/xml.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/vector.hpp>
// libv
#include <libv/serialization/archive/binary.hpp>
#include <libv/serialization/reflection.hpp>
#include <libv/serialization/memberwise.hpp>
#include <libv/serialization/serialization.hpp>
#include <libv/serialization/types/boost_flat_map.hpp>
#include <libv/serialization/types/std_variant.hpp>
// std
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>


// -------------------------------------------------------------------------------------------------

struct SimpleServerFieldSet {
	bool has_mod{};
	std::string name{};
	uint16_t players_current{};

	LIBV_REFLECTION_ACCESS(has_mod);
	LIBV_REFLECTION_ACCESS(name);
	LIBV_REFLECTION_ACCESS(players_current);
	LIBV_SERIALIAZTION_ENABLE_REFLECTION();
};

struct Memberwise {
	LIBV_SERIALIAZTION_ENABLE_MEMBERWISE();

	std::string a;
	double b;
	std::variant<int, double, std::string> c;
};

struct Inner {
	std::string x;

	Inner() { }
	Inner(std::string xv) : x(xv) { }

	template <class Archive>
	inline void serialize(Archive& ar) {
		ar & LIBV_NVP(x);
	}
};

struct Test {
	std::string name = "xz";
	std::shared_ptr<Inner> shader;
	std::vector<double> data;
	std::variant<int, double, std::string> variant;
	boost::container::flat_map<int, std::string> flat_map;
	boost::container::flat_map<int, double> flat_map_empty;
	bool dead = false;
	int32_t size = 500100;
	Memberwise memberwise;
	SimpleServerFieldSet server;

	template <class Archive>
	inline void serialize(Archive& ar) {
		ar & LIBV_NVP(name);
		ar & LIBV_NVP(shader);
		ar & LIBV_NVP(data);
		ar & LIBV_NVP(variant);
		ar & LIBV_NVP(flat_map);
		ar & LIBV_NVP(flat_map_empty);
		ar & LIBV_NVP(dead);
		ar & LIBV_NVP(size);
		ar & LIBV_NVP(memberwise);
		ar & LIBV_NVP(server);
	}

	bool operator==(const Test& rhs) const {
		return name == rhs.name &&
				data == rhs.data &&
				(shader && rhs.shader ? shader->x == rhs.shader->x : shader == rhs.shader) &&
				variant == rhs.variant &&
				flat_map == rhs.flat_map &&
				flat_map_empty == rhs.flat_map_empty &&
				dead == rhs.dead &&
				size == rhs.size &&
				memberwise.a == rhs.memberwise.a &&
				memberwise.b == rhs.memberwise.b &&
				memberwise.c == rhs.memberwise.c &&
				server.has_mod == rhs.server.has_mod &&
				server.name == rhs.server.name &&
				server.players_current == rhs.server.players_current;
	}
};

inline std::string hex_dump(const std::string& s) {
	std::ostringstream ss;
	ss << std::dec << s.size() << " - ";

	for (size_t i = 0; i < s.size(); ++i) {
		uint16_t c = static_cast<unsigned char>(s[i]);
		ss << std::hex << std::setfill('0') << std::setw(2) << std::uppercase << c << ' ';
	}

	auto result = ss.str();
	result.pop_back();
	return result;
}

// TODO P5: Basically this file could be a unit test

int main() {
	Test object_out;
	object_out.name = "name";
	object_out.dead = true;
	object_out.size = 0x01020304;
	object_out.data.emplace_back(2);
	object_out.data.emplace_back(3);
	object_out.data.emplace_back(4.36535435);
	object_out.data.emplace_back(5.23653463);
	object_out.variant = "variant_value";
	object_out.flat_map.emplace(11, "5.11");
	object_out.flat_map.emplace(12, "5.12");
	object_out.flat_map.emplace(13, "5.13");
	object_out.flat_map.emplace(14, "5.14");
	object_out.shader = std::make_shared<Inner>("inner_value");
	object_out.memberwise.a = "memberwise.a value";
	object_out.memberwise.b = 43;
	object_out.memberwise.c = "memberwise variant";
	object_out.server.has_mod = true;
	object_out.server.name = "member string";
	object_out.server.players_current = 6398;

	Test object_in;

	{
		std::stringstream ss(std::ios::in | std::ios::out | std::ios::binary);

		{
			libv::archive::BinaryOutput oar(ss);
			oar << cereal::make_nvp<libv::archive::BinaryOutput>("object", object_out);
		}

		std::cout << std::endl;
		std::cout << hex_dump(ss.str()) << std::endl;
		std::cout << std::endl;

		{
			libv::archive::BinaryInput iar(ss);
			iar >> cereal::make_nvp<libv::archive::BinaryInput>("object", object_in);
		}
	}

	std::cout << (object_out == object_in) << std::endl;

	{
		std::stringstream ss(std::ios::in | std::ios::out | std::ios::binary);

		{
			cereal::PortableBinaryOutputArchive oar(ss);
			oar << cereal::make_nvp<cereal::PortableBinaryOutputArchive>("object", object_out);
		}

		std::cout << std::endl;
		std::cout << hex_dump(ss.str()) << std::endl;
		std::cout << std::endl;

		{
			cereal::PortableBinaryInputArchive iar(ss);
			iar >> cereal::make_nvp<cereal::PortableBinaryInputArchive>("object", object_in);
		}
	}

	std::cout << (object_out == object_in) << std::endl;

	{
		std::stringstream ss;

		{
			cereal::BinaryOutputArchive oar(ss);
			oar << cereal::make_nvp<cereal::BinaryOutputArchive>("object", object_out);
		}

		std::cout << std::endl;
		std::cout << hex_dump(ss.str()) << std::endl;
		std::cout << std::endl;

		{
			cereal::BinaryInputArchive iar(ss);
			iar >> cereal::make_nvp<cereal::BinaryInputArchive>("object", object_in);
		}
	}

	std::cout << (object_out == object_in) << std::endl;

	{
		std::stringstream ss;

		{
			cereal::JSONOutputArchive oar(ss);
			oar << cereal::make_nvp<cereal::JSONOutputArchive>("object", object_out);
		}

		std::cout << std::endl;
		std::cout << ss.str() << std::endl;
		std::cout << std::endl;

		{
			cereal::JSONInputArchive iar(ss);
			iar >> cereal::make_nvp<cereal::JSONInputArchive>("object", object_in);
		}
	}

	std::cout << (object_out == object_in) << std::endl;

	{
		std::stringstream ss;

		{
			cereal::XMLOutputArchive oar(ss);
			oar << cereal::make_nvp<cereal::XMLOutputArchive>("object", object_out);
		}

		std::cout << std::endl;
		std::cout << ss.str() << std::endl;
		std::cout << std::endl;

		{
			cereal::XMLInputArchive iar(ss);
			iar >> cereal::make_nvp<cereal::XMLInputArchive>("object", object_in);
		}
	}

	std::cout << (object_out == object_in) << std::endl;

	return 0;
}

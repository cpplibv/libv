// Project: libv, File: sandbox/libv_serialize_cereal_main.cpp

// ext
#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/archives/portable_binary.hpp>
#include <cereal/archives/xml.hpp>
// libv
#include <libv/serial/archive/binary.hpp>
#include <libv/serial/memberwise.hpp>
#include <libv/serial/reflection.hpp>
#include <libv/serial/serial.hpp>
#include <libv/serial/types/boost_flat_map.hpp>
#include <libv/serial/types/std_map.hpp>
#include <libv/serial/types/std_memory.hpp>
#include <libv/serial/types/std_string.hpp>
#include <libv/serial/types/std_variant.hpp>
#include <libv/serial/types/std_vector.hpp>
#include <libv/utility/hex_dump.hpp>
// std
#include <iomanip>
#include <iostream>
#include <map>
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
	LIBV_SERIALIZATION_ENABLE_REFLECTION();
};

struct Memberwise {
	LIBV_SERIALIZATION_ENABLE_MEMBERWISE();

	std::string a;
	double b;
	std::variant<int, double, std::string> c;
};

struct Inner {
	std::string x;

	Inner() { }
	explicit Inner(std::string xv) : x(xv) { }

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
	std::map<int, std::string> std_map;
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
		ar & LIBV_NVP(std_map);
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
				std_map == rhs.std_map &&
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
	object_out.std_map.emplace(11, "5.11");
	object_out.std_map.emplace(12, "5.12");
	object_out.std_map.emplace(13, "5.13");
	object_out.std_map.emplace(14, "5.14");
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
			oar << LIBV_NVP_NAMED("object", object_out);
		}

		std::cout << std::endl;
		std::cout << ss.str().size() << " - " << libv::hex_dump(ss.str()) << std::endl;
		std::cout << std::endl;

		{
			libv::archive::BinaryInput iar(ss);
			iar >> LIBV_NVP_NAMED("object", object_in);
		}
	}

	std::cout << (object_out == object_in) << std::endl;

	{
		std::stringstream ss(std::ios::in | std::ios::out | std::ios::binary);

		{
			cereal::PortableBinaryOutputArchive oar(ss);
			oar << LIBV_NVP_NAMED("object", object_out);
		}

		std::cout << std::endl;
		std::cout << ss.str().size() << " - " << libv::hex_dump(ss.str()) << std::endl;
		std::cout << std::endl;

		{
			cereal::PortableBinaryInputArchive iar(ss);
			iar >> LIBV_NVP_NAMED("object", object_in);
		}
	}

	std::cout << (object_out == object_in) << std::endl;

	{
		std::stringstream ss;

		{
			cereal::BinaryOutputArchive oar(ss);
			oar << LIBV_NVP_NAMED("object", object_out);
		}

		std::cout << std::endl;
		std::cout << ss.str().size() << " - " << libv::hex_dump(ss.str()) << std::endl;
		std::cout << std::endl;

		{
			cereal::BinaryInputArchive iar(ss);
			iar >> LIBV_NVP_NAMED("object", object_in);
		}
	}

	std::cout << (object_out == object_in) << std::endl;

	{
		std::stringstream ss;

		{
			cereal::JSONOutputArchive oar(ss);
			oar << LIBV_NVP_NAMED("object", object_out);
		}

		std::cout << std::endl;
		std::cout << ss.str() << std::endl;
		std::cout << std::endl;

		{
			cereal::JSONInputArchive iar(ss);
			iar >> LIBV_NVP_NAMED("object", object_in);
		}
	}

	std::cout << (object_out == object_in) << std::endl;

	{
		std::stringstream ss;

		{
			cereal::XMLOutputArchive oar(ss);
			oar << LIBV_NVP_NAMED("object", object_out);
		}

		std::cout << std::endl;
		std::cout << ss.str() << std::endl;
		std::cout << std::endl;

		{
			cereal::XMLInputArchive iar(ss);
			iar >> LIBV_NVP_NAMED("object", object_in);
		}
	}

	std::cout << (object_out == object_in) << std::endl;

	return 0;
}

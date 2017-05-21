// File: vserialize_main.cpp, Created on 2017. 04. 14. 15:09, Author: Vader

// ext
#include <cereal/cereal.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/archives/xml.hpp>
#include <cereal/archives/portable_binary.hpp>
#include <cereal/archives/binary.hpp>
// libv
#include <libv/serialization/archive/cereal_binary_portable.hpp>
// std
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>


#define LIBV_NVP(N) CEREAL_NVP(N)
#define LIBV_NVP2(N, V) CEREAL_NVP_(N, V)

// -------------------------------------------------------------------------------------------------

struct Inner {
	std::string x;

	Inner() { }
	Inner(std::string x) : x(x) { }

	template <class Archive>
	inline void serialize(Archive& ar) {
		ar & LIBV_NVP(x);
	}
};

struct Test {
	std::string name = "xz";
	std::shared_ptr<Inner> shader;
	std::vector<double> data;
	bool dead = false;
	int32_t size = 500100;

	template <class Archive>
	inline void serialize(Archive& ar) {
		ar & LIBV_NVP(name);
		ar & LIBV_NVP(shader);
		ar & LIBV_NVP(data);
		ar & LIBV_NVP(dead);
		ar & LIBV_NVP(size);
	}

	bool operator==(const Test& rhs) const {
		return name == rhs.name &&
				data == rhs.data &&
				(shader && rhs.shader ? shader->x == rhs.shader->x : shader == rhs.shader) &&
				dead == rhs.dead &&
				size == rhs.size;
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

int main() {
	Test object_out;
	object_out.name = "name";
	object_out.dead = true;
	object_out.size = 0x01020304;
	object_out.data.emplace_back(2);
	object_out.data.emplace_back(3);
	object_out.data.emplace_back(4.36535435);
	object_out.data.emplace_back(5.23653463);
	object_out.shader = std::make_shared<Inner>("inner_value");

	Test object_in;

	{
		std::stringstream ss(std::ios::in | std::ios::out | std::ios::binary);

		{
			libv::archive::CerealPortableBinaryOutput oar(ss);
			oar << cereal::make_nvp<libv::archive::CerealPortableBinaryOutput>("object", object_out);
		}

		std::cout << std::endl;
		std::cout << hex_dump(ss.str()) << std::endl;
		std::cout << std::endl;

		{
			libv::archive::CerealPortableBinaryInput iar(ss);
			iar >> cereal::make_nvp<libv::archive::CerealPortableBinaryInput>("object", object_in);
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
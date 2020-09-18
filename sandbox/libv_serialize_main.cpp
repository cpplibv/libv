// Project: libv, File: sandbox/libv_serialize_main.cpp, Author: Császár Mátyás [Vader]

// libv
#include <libv/serialization/archive/binary.hpp>
#include <libv/serialization/archive/xml.hpp>
#include <libv/serialization/serialization.hpp>
#include <libv/serialization/types/std_memory.hpp>
#include <libv/serialization/types/std_vector.hpp>
// std
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>


// -------------------------------------------------------------------------------------------------

struct Inner {
	std::string x;

	Inner() { }

	Inner(std::string x) : x(x) { }

	template <class Archive>
	inline void serialize(Archive& ar, const unsigned int) {
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
	inline void serialize(Archive& ar, const unsigned int) {
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

int main(int, char **) {
	Test object_out;
	object_out.name = "name";
	object_out.dead = true;
	object_out.size = 45365;
	object_out.data.emplace_back(2);
	object_out.data.emplace_back(3);
	object_out.data.emplace_back(4.36535435);
	object_out.data.emplace_back(5.23653463);
	object_out.shader = std::make_shared<Inner>("xz");

	Test object_in;

	{
		std::ofstream ofs("test_file_xml");
		libv::archive::XMLOutput oar(ofs);
		oar << LIBV_NVP_NAMED("object", object_out);
	}
	{
		std::ifstream ifs("test_file_xml");
		libv::archive::XMLInput iar(ifs);
		iar >> LIBV_NVP_NAMED("object", object_in);
	}

	std::cout << (object_in == object_out) << std::endl;

	{
		std::ofstream ofs("test_file_bin");
		libv::archive::BinaryOutput oar(ofs);
		oar << LIBV_NVP_NAMED("object", object_out);
	}
	{
		std::ifstream ifs("test_file_bin");
		libv::archive::BinaryInput iar(ifs);
		iar >> LIBV_NVP_NAMED("object", object_in);
	}


	std::cout << (object_in == object_out) << std::endl;

	return 0;
}



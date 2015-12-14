// File: MaterialTest.cpp, Created on 2015. february 2. 08:18, Author: Vader

#include <catch.hpp>

#include <boost/serialization/serialization.hpp>
#include <boost/archive/portable_iarchive.hpp>
#include <boost/archive/portable_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>

#include <sstream>

#include <libv/vm3/serialization/material.hpp>
#include <libv/vm3/material_property_visitor.hpp>

using namespace libv;
using namespace libv::vm3;

TEST_CASE("materialBasic") {
	Material material0;
	CHECK(material0.getName() == "--unnamed--");
	CHECK(material0.getShader() == "--unknown--");

	Material material1("Metal", "Phong-E-v2");
	CHECK(material1.getName() == "Metal");
	CHECK(material1.getShader() == "Phong-E-v2");
}

TEST_CASE("materialPropertySetGet") {
	Material material("Metal", "Phong-E-v2");

	std::string result0;
	int result1 = 0;
	double result2 = 0;
	material.set<std::string>("diffuseTexture", "diffuseTexture.dds");
	material.set("numDim", 3);

	CHECK(material.get("diffuseTexture", result0));
	CHECK(result0 == "diffuseTexture.dds");
	CHECK(material.get("numDim", result1));
	CHECK(result1 == 3);
	CHECK(!material.get("notSet", result2));
}

TEST_CASE("materialPropertyReset") {
	Material material("Metal", "Phong-E-v2");

	std::string result;
	material.set<std::string>("diffuseTexture", "diffuseTexture.dds");
	material.set<std::string>("diffuseTexture", "diffuseTexture2.dds");

	CHECK(material.get("diffuseTexture", result));
	CHECK(result == "diffuseTexture2.dds");
}

TEST_CASE("materialPropertyGetProxy") {
	Material material("Metal", "Phong-E-v2");
	material.set<std::string>("diffuseTexture", "diffuseTexture.dds");

	auto resultBad = material.get<std::string>("notSet");
	auto resultGood = material.get<std::string>("diffuseTexture");

	CHECK_FALSE(resultBad);
	CHECK(resultGood);
	CHECK(*resultGood == "diffuseTexture.dds");
}

// --- Serialization -------------------------------------------------------------------------------

template<typename OArchive, typename IArchive>
void materialArchiveTester(std::iostream& stream) {
	Material material_orig("Metal", "Phong-E-v2");
	Material material_target;

	material_orig.set("numDim", 3);
	material_orig.set<std::string>("diffuseTexture", "diffuseTexture.dds");

	OArchive oar(stream);
	oar << BOOST_SERIALIZATION_NVP(material_orig);

	IArchive iar(stream);
	iar >> BOOST_SERIALIZATION_NVP(material_target);

	CHECK((material_orig.getName() == material_target.getName()));
	CHECK((material_orig.getShader() == material_target.getShader()));

	CHECK(!material_target.get<std::string>("notSet"));
	CHECK(material_target.get<int>("numDim"));
	CHECK((*material_target.get<int>("numDim") == 3));
	CHECK(material_target.get<std::string>("diffuseTexture"));
	CHECK((*material_target.get<std::string>("diffuseTexture") == "diffuseTexture.dds"));
}

TEST_CASE("materialSerializationWithArchives") {
#ifndef VM3_NO_BINARY_ARCHIVE
	std::stringstream ssb(std::stringstream::binary | std::ios_base::out | std::ios_base::in);
	materialArchiveTester<
			boost::archive::binary_oarchive,
			boost::archive::binary_iarchive>(ssb);
#endif

#ifndef VM3_NO_PORTABLE_BINARY_ARCHIVE
	std::stringstream sspb(std::stringstream::binary | std::ios_base::out | std::ios_base::in);
	materialArchiveTester<
			eos::portable_oarchive,
			eos::portable_iarchive>(sspb);
#endif

#ifndef VM3_NO_TEXT_ARCHIVE
	std::stringstream sst;
	materialArchiveTester<
			boost::archive::text_oarchive,
			boost::archive::text_iarchive>(sst);
#endif

#ifndef VM3_NO_XML_ARCHIVE
	std::stringstream ssx;
	materialArchiveTester<
			boost::archive::xml_oarchive,
			boost::archive::xml_iarchive>(ssx);
#endif
}

TEST_CASE("materialVisitor") {
	Material material;
	material.set("numDim0", 3);
	material.set("numDim1", 4);
	material.set("color", vec4(0,1,2,1));
	material.set<std::string>("diffuseTexture", "diffuseTexture.dds");

	MaterialPropertyCounter counter;
	material.accept(counter);

	CHECK(counter.numInt() == 2);
	CHECK(counter.numFloat() == 0);
	CHECK(counter.numDouble() == 0);
	CHECK(counter.numString() == 1);
	CHECK(counter.numVec4() == 1);
}

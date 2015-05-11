// File: ModelTest.cpp, Created on 2015. február 14. 18:51, Author: Vader

#include <catch.hpp>

#include <boost/serialization/vector.hpp>
#include <boost/serialization/export.hpp>
#include <boost/serialization/unique_ptr.hpp>
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

#include "vl/vm3/serialization/model.hpp"

using namespace vl;
using namespace vl::vm3;

TEST_CASE("modelBasic") {
	Model model;
//	CHECK(model.getName() == "unnamed");
//	CHECK(model.getFilePath() == "unknown");
}

// --- Serialization -------------------------------------------------------------------------------

template<typename OArchive, typename IArchive>
void modelArchiveTester(std::iostream& stream) {
	Model model_orig;
	Model model_target;
	
//	model_orig.set("numDim", 3);
//	model_orig.set<std::string>("diffuseTexture", "diffuseTexture.dds");

	OArchive oar(stream);
	oar << BOOST_SERIALIZATION_NVP(model_orig);

	IArchive iar(stream);
	iar >> BOOST_SERIALIZATION_NVP(model_target);

	CHECK(model_orig.getName() == model_target.getName());

//	CHECK(!model_target.get<std::string>("notSet"));
//	CHECK(model_target.get<int>("numDim"));
//	CHECK(*model_target.get<int>("numDim") == 3);
//	CHECK(model_target.get<std::string>("diffuseTexture"));
//	CHECK(*model_target.get<std::string>("diffuseTexture") == "diffuseTexture.dds");
}

TEST_CASE("modelSerializationWithArchives") {
#ifndef VM3_NO_BINARY_ARCHIVE
	std::stringstream ssb(std::stringstream::binary | std::ios_base::out | std::ios_base::in);
	modelArchiveTester<
			boost::archive::binary_oarchive,
			boost::archive::binary_iarchive>(ssb);
#endif

#ifndef VM3_NO_PORTABLE_BINARY_ARCHIVE
	std::stringstream sspb(std::stringstream::binary | std::ios_base::out | std::ios_base::in);
	modelArchiveTester<
			eos::portable_oarchive,
			eos::portable_iarchive>(sspb);
#endif

#ifndef VM3_NO_TEXT_ARCHIVE
	std::stringstream sst;
	modelArchiveTester<
			boost::archive::text_oarchive,
			boost::archive::text_iarchive>(sst);
#endif

#ifndef VM3_NO_XML_ARCHIVE
	std::stringstream ssx;
	modelArchiveTester<
			boost::archive::xml_oarchive,
			boost::archive::xml_iarchive>(ssx);
#endif
}


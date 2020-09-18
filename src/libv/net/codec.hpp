// File: packetizer.hpp Author: Vader Created on 2017. május 13., 20:26

#pragma once

// std
//#include <optional>
//#include <sstream>


namespace libv {
namespace net {

// -------------------------------------------------------------------------------------------------

//template <typename OArchive>
//class Packetizer {
//private:
//	std::ostringstream ss;
//	// TODO P2: Use a proper in memory stream that can yield memory in a proper way. The current packet() will not do it.
//	//			Maybe an function with input param like use/write/prep/init(~memory~)
//	std::optional<OArchive> oarchive;
//
//public:
//	Packetizer() :
//		ss(std::ios::out | std::ios::binary),
//		oarchive{ss} {
//		reset();
//	}
//
//	template <typename T>
//	Packetizer& encode(T&& t) {
//		*oarchive << t;
//		return *this;
//	}
//
//	std::string packet() {
//		oarchive.reset();
//		const auto p = ss.tellp();
//		auto result = ss.str();
//		result.resize(p);
//		reset();
//		return result;
//	}
//
//private:
//	void reset() {
//		ss.clear();
//		ss.str("");
//		oarchive = OArchive{ss};
//	}
//};

// -------------------------------------------------------------------------------------------------

} // namespace net
} // namespace libv

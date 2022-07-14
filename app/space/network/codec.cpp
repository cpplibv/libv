// Project: libv, File: app/space/network/codec.cpp

// hpp
#include <space/network/codec.hpp>
// ext
#include <fmt/format.h>
// libv
#include <libv/serial/archive/binary.hpp>
#include <libv/serial/archive/json_any.hpp>


namespace space {

// -------------------------------------------------------------------------------------------------

std::string debug_binary_as_json(std::span<const std::byte> message) {
	std::string result;

	try {
//		libv::archive::Binary::input iar(message);
//		libv::archive::JSONAny::output oar(result);

//		SnapshotArchive<libv::archive::BasicBinaryInput> iar(false, message);
//		SnapshotArchive<libv::archive::BasicJSONAnyOutput> oar(false, result);

		// TODO P1: app.space:

//		libv::archive::BinaryInput iar(message);
//		libv::archive::JSONAnyOutput oar(result);
//
//		network_codec.decode(iar, [&oar](const auto& object) {
//			network_codec.encode(oar, object);
//		});

	} catch (const Codec::UnexpectedIDException& e) {
		if (!message.empty())
			result = fmt::format("<Unknown type ID: {} (0x{:02X})", +e.id, +e.id);
		else
			result = "<Empty>";
	} catch (const std::exception& e) {
		if (message.empty())
			result = fmt::format("<Exception during transcoding empty: {}", e.what());
		else
			result = fmt::format("<Exception during transcoding {} (0x{:02X}): {}", static_cast<int>(message[0]), static_cast<int>(message[0]), e.what());
	}

	return result;
}

// -------------------------------------------------------------------------------------------------

} // namespace space

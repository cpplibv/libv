// Project: libv.serial, File: test/libv/serial/test_codec3.cpp

// hpp
#include <catch/catch.hpp>
// libv
#include <libv/serial/archive/binary.hpp>
#include <libv/serial/archive/json_any.hpp>
#include <libv/serial/codec3.hpp>
#include <libv/serial/enable.hpp>
#include <libv/serial/reflection.hpp>
#include <libv/serial/types/std_string.hpp>
#include <libv/serial/types/std_vector.hpp>
#include <libv/utility/hex_dump.hpp>


// -------------------------------------------------------------------------------------------------

using MessageID = libv::serial::CodecMessageID;

struct CTO_Introduction {
	static constexpr MessageID id{0x10};

	std::string player_name;
	uint64_t version;

	LIBV_REFLECTION_ACCESS(player_name);
	LIBV_REFLECTION_ACCESS(version);
	LIBV_SERIALIZATION_ENABLE_REFLECTION();
};

struct CTO_ChatMessage {
	static constexpr MessageID id{0x11};

	std::string player_name;
	std::string message;

	LIBV_REFLECTION_ACCESS(player_name);
	LIBV_REFLECTION_ACCESS(message);
	LIBV_SERIALIZATION_ENABLE_REFLECTION();
};

struct CTO_PlayerJoined {
	static constexpr MessageID id{0x12};

	std::string player_name;

	LIBV_REFLECTION_ACCESS(player_name);
	LIBV_SERIALIZATION_ENABLE_REFLECTION();
};

// ---

struct TestCodec : libv::serial::Codec3<TestCodec> {
	template <typename Ar, typename F> struct gen_table {
		static constexpr TypeAccessEntry types[] = {
				create_entry<Ar, F, CTO_Introduction>(),
				create_entry<Ar, F, CTO_ChatMessage>(),
				create_entry<Ar, F, CTO_PlayerJoined>(),
		};
	};
};

// -------------------------------------------------------------------------------------------------

TEST_CASE("Codec3 encode/decode one object", "[libv.serial.codec]") {
	TestCodec codec;
	std::vector<std::byte> buffer;

	{
		libv::archive::Binary::output oar(buffer);
		codec.encode(oar, CTO_Introduction{"Vader", 202110131011});
	}

	CHECK(libv::hex_dump(buffer) == "10 00 00 00 05 56 61 64  65 72 00 00 00 2F 0E B3  DB 43");
	//								 ^^ ID = 0x10   ^^^^^^^^^^^^^^^ Vader
	//								    ^^^^^^^^^^^ Length = 5      ^^^^^^^^^^^^^^^^^^^^^^^^ Version

	libv::archive::Binary::input iar(buffer);

	SECTION("Expected type const&") {
		bool decode_handler_called = false;
		codec.decode(iar, [&](const CTO_Introduction& message) {
			decode_handler_called = true;
			CHECK(message.player_name == "Vader");
			CHECK(message.version == 202110131011);
		});
		CHECK(decode_handler_called);
	}

	SECTION("Expected type &&") {
		bool decode_handler_called = false;
		codec.decode(iar, [&](CTO_Introduction&& message) {
			decode_handler_called = true;
			CHECK(message.player_name == "Vader");
			CHECK(message.version == 202110131011);
		});
		CHECK(decode_handler_called);
	}

	SECTION("Generic type const&") {
		bool decode_handler_called = false;
		codec.decode(iar, [&](const auto& message) {
			decode_handler_called = true;
			CHECK(std::is_same_v<decltype(message), const CTO_Introduction&>);
		});
		CHECK(decode_handler_called);
	}

	SECTION("Generic type &&") {
		bool decode_handler_called = false;
		codec.decode(iar, [&](auto&& message) {
			decode_handler_called = true;
			CHECK(std::is_same_v<decltype(message), CTO_Introduction&&>);
		});
		CHECK(decode_handler_called);
	}

	SECTION("Expected type with Expected and Unexpected types in Generic const&") {
		bool decode_handler_called = false;
		const auto handler = [&](const auto& message) {
			decode_handler_called = true;

			if constexpr (std::is_same_v<std::remove_cvref_t<decltype(message)>, CTO_ChatMessage>)
				return TestCodec::RejectMessage{};
			else
				(void) message;
		};
		codec.decode(iar, handler);
		CHECK(decode_handler_called);
	}

	SECTION("Unexpected type with Expected and Unexpected types in Generic const&") {
		bool decode_handler_called = false;
		const auto handler = [&](const auto& message) {
			decode_handler_called = true;
			CHECK(false); // Handler should not be called

			if constexpr (std::is_same_v<std::remove_cvref_t<decltype(message)>, CTO_Introduction>)
				return TestCodec::RejectMessage{};
			else
				(void) message;
		};
		CHECK_THROWS_AS(codec.decode(iar, handler), TestCodec::UnexpectedIDException);
		CHECK(not decode_handler_called);
	}

	SECTION("Not Expected type with Expected and Unexpected types in Generic const&") {
		bool decode_handler_called = false;
		const auto handler = [&](const auto& message) {
			decode_handler_called = true;
			CHECK(false); // Handler should not be called

			if constexpr (!std::is_same_v<std::remove_cvref_t<decltype(message)>, CTO_ChatMessage>)
				return TestCodec::RejectMessage{};
			else
				(void) message;
		};
		CHECK_THROWS_AS(codec.decode(iar, handler), TestCodec::UnexpectedIDException);
		CHECK(not decode_handler_called);
	}

	SECTION("Not Unexpected type with Expected and Unexpected types in Generic const&") {
		bool decode_handler_called = false;
		const auto handler = [&](const auto& message) {
			decode_handler_called = true;

			if constexpr (!std::is_same_v<std::remove_cvref_t<decltype(message)>, CTO_Introduction>)
				return TestCodec::RejectMessage{};
			else
				(void) message;
		};
		codec.decode(iar, handler);
		CHECK(decode_handler_called);
	}

	SECTION("Unexpected type by signature const&") {
		auto handler = [&](const CTO_ChatMessage&) {
			CHECK(false); // Handler should not be called
		};
		CHECK_THROWS_AS(codec.decode(iar, handler), TestCodec::UnexpectedIDException);
	}

	SECTION("Unexpected type by signature &&") {
		auto handler = [&](CTO_ChatMessage&&) {
			CHECK(false); // Handler should not be called
		};
		CHECK_THROWS_AS(codec.decode(iar, handler), TestCodec::UnexpectedIDException);
	}

	SECTION("Unexpected type by reject return type && early") {
		bool decode_handler_called = false;
		const auto handler = [&](auto&& message) {
			if constexpr (!std::is_same_v<std::remove_cvref_t<decltype(message)>, CTO_ChatMessage>)
				return TestCodec::RejectMessage{};

			decode_handler_called = true;
			CHECK(false); // Handler should not be called
		};
		CHECK_THROWS_AS(codec.decode(iar, handler), TestCodec::UnexpectedIDException);
		CHECK(not decode_handler_called);
	}

	SECTION("Unexpected type by reject return type && early") {
		bool decode_handler_called = false;
		const auto handler = [&](auto&& message) {
			if constexpr (!std::is_same_v<std::remove_cvref_t<decltype(message)>, CTO_ChatMessage>)
				return TestCodec::RejectMessage{};

			decode_handler_called = true;
			CHECK(false); // Handler should not be called
		};
		CHECK_THROWS_AS(codec.decode(iar, handler), TestCodec::UnexpectedIDException);
		CHECK(not decode_handler_called);
	}

	SECTION("Unexpected type by reject return type const& late") {
		bool decode_handler_called = false;
		const auto handler = [&](const auto& message) {
			decode_handler_called = true;
			CHECK(false); // Handler should not be called

			if constexpr (!std::is_same_v<std::remove_cvref_t<decltype(message)>, CTO_ChatMessage>)
				return TestCodec::RejectMessage{};
		};
		CHECK_THROWS_AS(codec.decode(iar, handler), TestCodec::UnexpectedIDException);
		CHECK(not decode_handler_called);
	}

	SECTION("Unexpected type by reject return type const& late") {
		bool decode_handler_called = false;
		const auto handler = [&](const auto& message) {
			decode_handler_called = true;
			CHECK(false); // Handler should not be called

			if constexpr (!std::is_same_v<std::remove_cvref_t<decltype(message)>, CTO_ChatMessage>)
				return TestCodec::RejectMessage{};
		};
		CHECK_THROWS_AS(codec.decode(iar, handler), TestCodec::UnexpectedIDException);
		CHECK(not decode_handler_called);
	}

	// -------------------------------------------------------------------------------------------------

	SECTION("const Handler with Accept") {
		bool decode_handler_called = false;
		const auto handler = [&](CTO_Introduction&&) {
			decode_handler_called = true;
		};
		codec.decode(iar, handler);
		CHECK(decode_handler_called);
	}
	SECTION("const Handler with Reject") {
		const auto handler = [&](CTO_ChatMessage&&) {
			CHECK(false); // Handler should not be called
		};
		CHECK_THROWS_AS(codec.decode(iar, handler), TestCodec::UnexpectedIDException);
	}
	SECTION("const Handler with Generic") {
		bool decode_handler_called = false;
		const auto handler = [&](auto&&) {
			decode_handler_called = true;
		};
		codec.decode(iar, handler);
		CHECK(decode_handler_called);
	}
}

// -------------------------------------------------------------------------------------------------

TEST_CASE("Codec3 encode/decode multiple object", "[libv.serial.codec]") {
	TestCodec codec;
	std::vector<std::byte> buffer;

	{
		libv::archive::Binary::output oar(buffer);
		codec.encode(oar, CTO_Introduction{"Vader", 202110131011});
		codec.encode(oar, CTO_ChatMessage{"Vader", "Hello!"});
	}

	CHECK(libv::hex_dump(buffer) ==
			"10 00 00 00 05 56 61 64  65 72 00 00 00 2F 0E B3  DB 43 "
//			 ^^ ID = 0x10   ^^^^^^^^^^^^^^^^^^^^^^^^ Version
//				^^^^^^^^^^^ Length = 5               ^^^^^^^^^^^^^^^ Vader
			"11 00 00 00 05 56  61 64 65 72 00 00 00 06\n48 65 6C 6C 6F 21");
//			 ^^ ID = 0x11   ^^^^^^^^^^^^^^^ Vader        ^^^^^^^^^^^^^^^^^ Hello!
//				^^^^^^^^^^^ Length = 5      ^^^^^^^^^^^ Length = 6

	libv::archive::Binary::input iar(buffer);

	int decode_handler_call = 0;
	codec.decode(iar, [&](const CTO_Introduction& message) {
		decode_handler_call++;
		CHECK(message.player_name == "Vader");
		CHECK(message.version == 202110131011);
	});
	codec.decode(iar, [&](const CTO_ChatMessage& message) {
		decode_handler_call++;
		CHECK(message.player_name == "Vader");
		CHECK(message.message == "Hello!");
	});

	CHECK(decode_handler_call == 2);

	// Try over-read
	CHECK_THROWS_AS(codec.decode(iar, [&](const auto&) {}), cereal::Exception);
}

TEST_CASE("Codec3 encode/decode multiple object JSON", "[libv.serial.codec]") {
	TestCodec codec;
	std::string buffer;

	{
		libv::archive::JSONAny::output oar(buffer);
		codec.encode(oar, CTO_Introduction{"Vader", 202110131011});
		codec.encode(oar, CTO_ChatMessage{"Vader", "Hello!"});
	}

	CHECK(buffer ==
R"({
    "type": 16,
    "message": {
        "player_name": "Vader",
        "version": 202110131011
    },
    "type": 17,
    "message": {
        "player_name": "Vader",
        "message": "Hello!"
    }
})");

	libv::archive::JSONAny::input iar(buffer);

	bool decode_handler_call_0 = false;
	bool decode_handler_call_1 = false;
	codec.decode(iar, [&](const CTO_Introduction& message) {
		decode_handler_call_0 = true;
		CHECK(message.player_name == "Vader");
		CHECK(message.version == 202110131011);
	});
	codec.decode(iar, [&](const CTO_ChatMessage& message) {
		decode_handler_call_1 = true;
		CHECK(message.player_name == "Vader");
		CHECK(message.message == "Hello!");
	});

	CHECK(decode_handler_call_0);
	CHECK(decode_handler_call_1);

	// Try over-read
	// Currently the JSON archive restart when they reach the end, so no over-read exception
	//	CHECK_THROWS_AS(codec.decode(iar, [&](const auto&) {}), cereal::Exception);
}

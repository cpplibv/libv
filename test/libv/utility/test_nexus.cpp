// Project: libv.utility, File: test/libv/utility/test_entity_ptr.cpp

// hpp
#include <catch2/catch_test_macros.hpp>
// std
#include <sstream>
// pro
#include <libv/utility/nexus.hpp>


// -------------------------------------------------------------------------------------------------

class MockCallbackStream {
	std::ostringstream ss;
	std::string separator;
	std::shared_ptr<bool> callback_counter = std::make_shared<bool>();

public:
	explicit inline MockCallbackStream(std::string separator) :
		separator(std::move(separator)) {}

	template <typename... Args2>
	inline auto callback(Args2&&... args2) {
		return [this, ...args_capture = std::forward<Args2>(args2), cc = callback_counter](auto&&... args) mutable {
			if (ss.tellp() != 0)
				ss << separator;
			(ss << ... << args_capture);
			(ss << ... << args);
		};
	}

	std::string consume() {
		auto result = std::move(ss).str();
		ss.str("");
		return result;
	}

	int num_alive_callback() {
		return callback_counter.use_count() - 1;
	}
};

// -------------------------------------------------------------------------------------------------

TEST_CASE("Test Nexus lifecycle", "[libv.utility.nexus]") {
	using EventType = int;

	libv::Nexus nexus;
	MockCallbackStream queue{", "};

	const void* signal = reinterpret_cast<void*>(0x11111111);
	const void* slot = reinterpret_cast<void*>(0x22222222);
	const void* signal_unused = reinterpret_cast<void*>(0x33333333);

	nexus.connect_global<EventType>(queue.callback("global"));
	CHECK(queue.num_alive_callback() == 1);
	CHECK(nexus.num_channel() == 1);
	CHECK(nexus.num_tracked() == 1);
	nexus.connect_global<EventType>(slot, queue.callback("slot"));
	CHECK(queue.num_alive_callback() == 2);
	CHECK(nexus.num_channel() == 1);
	CHECK(nexus.num_tracked() == 2);
	nexus.connect_channel<EventType>(signal, queue.callback("signal"));
	CHECK(queue.num_alive_callback() == 3);
	CHECK(nexus.num_channel() == 2);
	CHECK(nexus.num_tracked() == 3);
	nexus.connect_channel<EventType>(signal, slot, queue.callback("both"));
	CHECK(queue.num_alive_callback() == 4);
	CHECK(nexus.num_channel() == 2);
	CHECK(nexus.num_tracked() == 3);
	// nullptr -> nullptr
	// nullptr -> slot
	// signal  -> nullptr
	// signal  -> slot
	// 2 Channel is created: null (global) and signal
	// 3 Object is tracked: null (global), slot and signal

	SECTION("broadcast") {
		nexus.broadcast_global<EventType>(1);
		CHECK(queue.consume() == "global1, slot1");

		nexus.broadcast_channel<EventType>(signal, 2);
		CHECK(queue.consume() == "signal2, both2");

		nexus.broadcast_channel<EventType>(signal_unused, 3);
		CHECK(queue.consume() == "");

		nexus.broadcast_channel<EventType>(slot, 4);
		CHECK(queue.consume() == "");
	}

	SECTION("disconnect_slot") {
		SECTION("type") {
			nexus.disconnect_slot<EventType>(slot);
		}
		SECTION("all") {
			nexus.disconnect_slot_all(slot);
		}

		CHECK(queue.num_alive_callback() == 2);
		CHECK(nexus.num_channel() == 2);
		CHECK(nexus.num_tracked() == 2);

		nexus.broadcast_global<EventType>(5);
		CHECK(queue.consume() == "global5");

		nexus.broadcast_channel<EventType>(signal, 6);
		CHECK(queue.consume() == "signal6");
	}

	SECTION("disconnect_channel") {
		SECTION("type") {
			nexus.disconnect_channel<EventType>(signal);
		}
		SECTION("all") {
			nexus.disconnect_channel_all(signal);
		}

		CHECK(queue.num_alive_callback() == 2);
		CHECK(nexus.num_channel() == 1);
		CHECK(nexus.num_tracked() == 2);

		nexus.broadcast_global<EventType>(5);
		CHECK(queue.consume() == "global5, slot5");

		nexus.broadcast_channel<EventType>(signal, 6);
		CHECK(queue.consume() == "");
	}

	SECTION("disconnect_all slot") {
		nexus.disconnect_all(slot);

		CHECK(queue.num_alive_callback() == 2);
		CHECK(nexus.num_channel() == 2);
		CHECK(nexus.num_tracked() == 2);

		nexus.broadcast_global<EventType>(5);
		CHECK(queue.consume() == "global5");

		nexus.broadcast_channel<EventType>(signal, 6);
		CHECK(queue.consume() == "signal6");
	}

	SECTION("disconnect_all signal") {
		nexus.disconnect_all(signal);

		CHECK(queue.num_alive_callback() == 2);
		CHECK(nexus.num_channel() == 1);
		CHECK(nexus.num_tracked() == 2);

		nexus.broadcast_global<EventType>(5);
		CHECK(queue.consume() == "global5, slot5");

		nexus.broadcast_channel<EventType>(signal, 6);
		CHECK(queue.consume() == "");
	}

	SECTION("disconnect_all unused") {
		nexus.disconnect_all(signal_unused);

		CHECK(queue.num_alive_callback() == 4);
		CHECK(nexus.num_channel() == 2);
		CHECK(nexus.num_tracked() == 3);
	}

	SECTION("disconnect nullptr") {
		nexus.disconnect_all(nullptr);

		CHECK(queue.num_alive_callback() == 1);
		CHECK(nexus.num_channel() == 1);
		CHECK(nexus.num_tracked() == 2);

		nexus.broadcast_global<EventType>(5);
		CHECK(queue.consume() == "");

		nexus.broadcast_channel<EventType>(signal, 6);
		CHECK(queue.consume() == "both6");
	}

	SECTION("disconnect_channel nullptr") {
		SECTION("type") {
			nexus.disconnect_channel<EventType>(nullptr);
		}
		SECTION("all") {
			nexus.disconnect_channel_all(nullptr);
		}

		CHECK(queue.num_alive_callback() == 2);
		CHECK(nexus.num_channel() == 1);
		CHECK(nexus.num_tracked() == 3);

		nexus.broadcast_global<EventType>(5);
		CHECK(queue.consume() == "");

		nexus.broadcast_channel<EventType>(signal, 6);
		CHECK(queue.consume() == "signal6, both6");
	}

	SECTION("disconnect_slot nullptr") {
		SECTION("type") {
			nexus.disconnect_slot<EventType>(nullptr);
		}
		SECTION("all") {
			nexus.disconnect_slot_all(nullptr);
		}

		CHECK(queue.num_alive_callback() == 2);
		CHECK(nexus.num_channel() == 2);
		CHECK(nexus.num_tracked() == 3);

		nexus.broadcast_global<EventType>(5);
		CHECK(queue.consume() == "slot5");

		nexus.broadcast_channel<EventType>(signal, 6);
		CHECK(queue.consume() == "both6");
	}
}

TEST_CASE("Test Nexus multi connect", "[libv.utility.nexus]") {
	using EventType = int;

	libv::Nexus nexus;
	MockCallbackStream queue{", "};

	const void* signal = reinterpret_cast<void*>(0x11111111);
	const void* slot = reinterpret_cast<void*>(0x22222222);
	const void* signal_unused = reinterpret_cast<void*>(0x33333333);

	nexus.connect_global<EventType>(queue.callback("globalA"));
	CHECK(queue.num_alive_callback() == 1);
	CHECK(nexus.num_channel() == 1);
	CHECK(nexus.num_tracked() == 1);
	nexus.connect_global<EventType>(queue.callback("globalB"));
	CHECK(queue.num_alive_callback() == 2);
	CHECK(nexus.num_channel() == 1);
	CHECK(nexus.num_tracked() == 1);
	nexus.connect_global<EventType>(slot, queue.callback("slotA"));
	CHECK(queue.num_alive_callback() == 3);
	CHECK(nexus.num_channel() == 1);
	CHECK(nexus.num_tracked() == 2);
	nexus.connect_global<EventType>(slot, queue.callback("slotB"));
	CHECK(queue.num_alive_callback() == 4);
	CHECK(nexus.num_channel() == 1);
	CHECK(nexus.num_tracked() == 2);
	nexus.connect_channel<EventType>(signal, queue.callback("signalA"));
	CHECK(queue.num_alive_callback() == 5);
	CHECK(nexus.num_channel() == 2);
	CHECK(nexus.num_tracked() == 3);
	nexus.connect_channel<EventType>(signal, queue.callback("signalB"));
	CHECK(queue.num_alive_callback() == 6);
	CHECK(nexus.num_channel() == 2);
	CHECK(nexus.num_tracked() == 3);
	nexus.connect_channel<EventType>(signal, slot, queue.callback("bothA"));
	CHECK(queue.num_alive_callback() == 7);
	CHECK(nexus.num_channel() == 2);
	CHECK(nexus.num_tracked() == 3);
	nexus.connect_channel<EventType>(signal, slot, queue.callback("bothB"));
	CHECK(queue.num_alive_callback() == 8);
	CHECK(nexus.num_channel() == 2);
	CHECK(nexus.num_tracked() == 3);

	SECTION("broadcast") {
		nexus.broadcast_global<EventType>(1);
		CHECK(queue.consume() == "globalA1, globalB1, slotA1, slotB1");

		nexus.broadcast_channel<EventType>(signal, 2);
		CHECK(queue.consume() == "signalA2, signalB2, bothA2, bothB2");

		nexus.broadcast_channel<EventType>(signal_unused, 3);
		CHECK(queue.consume() == "");

		nexus.broadcast_channel<EventType>(slot, 4);
		CHECK(queue.consume() == "");
	}

	SECTION("disconnect_slot") {
		SECTION("type") {
			nexus.disconnect_slot<EventType>(slot);
		}
		SECTION("all") {
			nexus.disconnect_slot_all(slot);
		}

		CHECK(queue.num_alive_callback() == 4);
		CHECK(nexus.num_channel() == 2);
		CHECK(nexus.num_tracked() == 2);

		nexus.broadcast_global<EventType>(5);
		CHECK(queue.consume() == "globalA5, globalB5");

		nexus.broadcast_channel<EventType>(signal, 6);
		CHECK(queue.consume() == "signalA6, signalB6");
	}

	SECTION("disconnect_channel") {
		SECTION("type") {
			nexus.disconnect_channel<EventType>(signal);
		}
		SECTION("all") {
			nexus.disconnect_channel_all(signal);
		}

		CHECK(queue.num_alive_callback() == 4);
		CHECK(nexus.num_channel() == 1);
		CHECK(nexus.num_tracked() == 2);

		nexus.broadcast_global<EventType>(5);
		CHECK(queue.consume() == "globalA5, globalB5, slotA5, slotB5");

		nexus.broadcast_channel<EventType>(signal, 6);
		CHECK(queue.consume() == "");
	}

	SECTION("disconnect_all slot") {
		nexus.disconnect_all(slot);

		CHECK(queue.num_alive_callback() == 4);
		CHECK(nexus.num_channel() == 2);
		CHECK(nexus.num_tracked() == 2);

		nexus.broadcast_global<EventType>(5);
		CHECK(queue.consume() == "globalA5, globalB5");

		nexus.broadcast_channel<EventType>(signal, 6);
		CHECK(queue.consume() == "signalA6, signalB6");
	}

	SECTION("disconnect_all signal") {
		nexus.disconnect_all(signal);

		CHECK(queue.num_alive_callback() == 4);
		CHECK(nexus.num_channel() == 1);
		CHECK(nexus.num_tracked() == 2);

		nexus.broadcast_global<EventType>(5);
		CHECK(queue.consume() == "globalA5, globalB5, slotA5, slotB5");

		nexus.broadcast_channel<EventType>(signal, 6);
		CHECK(queue.consume() == "");
	}

	SECTION("disconnect_all unused") {
		nexus.disconnect_all(signal_unused);

		CHECK(queue.num_alive_callback() == 8);
		CHECK(nexus.num_channel() == 2);
		CHECK(nexus.num_tracked() == 3);
	}

	SECTION("disconnect nullptr") {
		nexus.disconnect_all(nullptr);

		CHECK(queue.num_alive_callback() == 2);
		CHECK(nexus.num_channel() == 1);
		CHECK(nexus.num_tracked() == 2);

		nexus.broadcast_global<EventType>(5);
		CHECK(queue.consume() == "");

		nexus.broadcast_channel<EventType>(signal, 6);
		CHECK(queue.consume() == "bothA6, bothB6");
	}

	SECTION("disconnect_channel nullptr") {
		SECTION("type") {
			nexus.disconnect_channel<EventType>(nullptr);
		}
		SECTION("all") {
			nexus.disconnect_channel_all(nullptr);
		}

		CHECK(queue.num_alive_callback() == 4);
		CHECK(nexus.num_channel() == 1);
		CHECK(nexus.num_tracked() == 3);

		nexus.broadcast_global<EventType>(5);
		CHECK(queue.consume() == "");

		nexus.broadcast_channel<EventType>(signal, 6);
		CHECK(queue.consume() == "signalA6, signalB6, bothA6, bothB6");
	}

	SECTION("disconnect_slot nullptr") {
		SECTION("type") {
			nexus.disconnect_slot<EventType>(nullptr);
		}
		SECTION("all") {
			nexus.disconnect_slot_all(nullptr);
		}

		CHECK(queue.num_alive_callback() == 4);
		CHECK(nexus.num_channel() == 2);
		CHECK(nexus.num_tracked() == 3);

		nexus.broadcast_global<EventType>(5);
		CHECK(queue.consume() == "slotA5, slotB5");

		nexus.broadcast_channel<EventType>(signal, 6);
		CHECK(queue.consume() == "bothA6, bothB6");
	}
}

TEST_CASE("Test Nexus connect ordering and front", "[libv.utility.nexus]") {
	using EventType = int;

	const void* slot = reinterpret_cast<void*>(0x22222222);

	libv::Nexus nexus;
	MockCallbackStream queue{", "};

	SECTION("B B B") {
		nexus.connect_global<EventType>(queue.callback("A"));
		nexus.connect_global<EventType>(queue.callback("B"));
		nexus.connect_global<EventType>(queue.callback("C"));

		nexus.broadcast_global<EventType>(1);
		CHECK(queue.consume() == "A1, B1, C1");
	}

	SECTION("F F F") {
		nexus.connect_global_front<EventType>(queue.callback("A"));
		nexus.connect_global_front<EventType>(queue.callback("B"));
		nexus.connect_global_front<EventType>(queue.callback("C"));

		nexus.broadcast_global<EventType>(1);
		CHECK(queue.consume() == "C1, B1, A1");
	}

	SECTION("B F B") {
		nexus.connect_global<EventType>(queue.callback("A"));
		nexus.connect_global_front<EventType>(queue.callback("B"));
		nexus.connect_global<EventType>(queue.callback("C"));

		nexus.broadcast_global<EventType>(1);
		CHECK(queue.consume() == "B1, A1, C1");
	}

	SECTION("F B F") {
		nexus.connect_global_front<EventType>(queue.callback("A"));
		nexus.connect_global<EventType>(queue.callback("B"));
		nexus.connect_global_front<EventType>(queue.callback("C"));

		nexus.broadcast_global<EventType>(1);
		CHECK(queue.consume() == "C1, A1, B1");
	}

	// ---

	SECTION("disconnect ordering 1") {
		nexus.connect_global_front<EventType>(slot, queue.callback("A"));
		nexus.connect_global_front<EventType>(queue.callback("B"));
		nexus.connect_global_front<EventType>(queue.callback("C"));
		nexus.connect_global_front<EventType>(queue.callback("D"));

		SECTION("disconnect_slot") {
			nexus.disconnect_slot<EventType>(slot);
		}
		SECTION("disconnect_slot_all") {
			nexus.disconnect_slot_all(slot);
		}
		SECTION("disconnect_all") {
			nexus.disconnect_all(slot);
		}

		nexus.broadcast_global<EventType>(1);
		CHECK(queue.consume() == "D1, C1, B1");
	}

	SECTION("disconnect ordering 2") {
		nexus.connect_global_front<EventType>(queue.callback("A"));
		nexus.connect_global_front<EventType>(slot, queue.callback("B"));
		nexus.connect_global_front<EventType>(queue.callback("C"));
		nexus.connect_global_front<EventType>(queue.callback("D"));

		SECTION("disconnect_slot") {
			nexus.disconnect_slot<EventType>(slot);
		}
		SECTION("disconnect_slot_all") {
			nexus.disconnect_slot_all(slot);
		}
		SECTION("disconnect_all") {
			nexus.disconnect_all(slot);
		}

		nexus.broadcast_global<EventType>(1);
		CHECK(queue.consume() == "D1, C1, A1");
	}

	SECTION("disconnect ordering 3") {
		nexus.connect_global_front<EventType>(queue.callback("A"));
		nexus.connect_global_front<EventType>(queue.callback("B"));
		nexus.connect_global_front<EventType>(slot, queue.callback("C"));
		nexus.connect_global_front<EventType>(queue.callback("D"));

		SECTION("disconnect_slot") {
			nexus.disconnect_slot<EventType>(slot);
		}
		SECTION("disconnect_slot_all") {
			nexus.disconnect_slot_all(slot);
		}
		SECTION("disconnect_all") {
			nexus.disconnect_all(slot);
		}

		nexus.broadcast_global<EventType>(1);
		CHECK(queue.consume() == "D1, B1, A1");
	}

	// ---

	SECTION("disconnect multi ordering 1") {
		nexus.connect_global_front<EventType>(slot, queue.callback("A"));
		nexus.connect_global_front<EventType>(slot, queue.callback("B"));
		nexus.connect_global_front<EventType>(queue.callback("C"));
		nexus.connect_global_front<EventType>(queue.callback("D"));

		SECTION("disconnect_slot") {
			nexus.disconnect_slot<EventType>(slot);
		}
		SECTION("disconnect_slot_all") {
			nexus.disconnect_slot_all(slot);
		}
		SECTION("disconnect_all") {
			nexus.disconnect_all(slot);
		}

		nexus.broadcast_global<EventType>(1);
		CHECK(queue.consume() == "D1, C1");
	}

	SECTION("disconnect multi ordering 2") {
		nexus.connect_global_front<EventType>(queue.callback("A"));
		nexus.connect_global_front<EventType>(slot, queue.callback("B"));
		nexus.connect_global_front<EventType>(slot, queue.callback("C"));
		nexus.connect_global_front<EventType>(queue.callback("D"));

		SECTION("disconnect_slot") {
			nexus.disconnect_slot<EventType>(slot);
		}
		SECTION("disconnect_slot_all") {
			nexus.disconnect_slot_all(slot);
		}
		SECTION("disconnect_all") {
			nexus.disconnect_all(slot);
		}

		nexus.broadcast_global<EventType>(1);
		CHECK(queue.consume() == "D1, A1");
	}

	SECTION("disconnect multi ordering 3") {
		nexus.connect_global_front<EventType>(slot, queue.callback("A"));
		nexus.connect_global_front<EventType>(queue.callback("B"));
		nexus.connect_global_front<EventType>(slot, queue.callback("C"));
		nexus.connect_global_front<EventType>(queue.callback("D"));

		SECTION("disconnect_slot") {
			nexus.disconnect_slot<EventType>(slot);
		}
		SECTION("disconnect_slot_all") {
			nexus.disconnect_slot_all(slot);
		}
		SECTION("disconnect_all") {
			nexus.disconnect_all(slot);
		}

		nexus.broadcast_global<EventType>(1);
		CHECK(queue.consume() == "D1, B1");
	}
}

TEST_CASE("Test Nexus broadcasted arguments", "[libv.utility.nexus]") {
	using EventType = int;

	const int channelA = 0;
	int channelB = 0;

	libv::Nexus nexus;
	int callCount = 0;

	nexus.connect_channel<EventType>(&channelA, [&](const int*, const EventType&) { ++callCount; });
	nexus.connect_channel<EventType>(&channelA, [&](const EventType&) { ++callCount; });
	nexus.connect_channel<EventType>(&channelA, [&](const int*) { ++callCount; });
	nexus.connect_channel<EventType>(&channelA, [&]() { ++callCount; });
	// These should fail to compile due to const correctness:
	//	nexus.connect_channel<EventType>(&channelA, [&](const int*, EventType&) { ++callCount; });
	//	nexus.connect_channel<EventType>(&channelA, [&](EventType&) { ++callCount; });
	//	nexus.connect_channel<EventType>(&channelA, [&](int*, const EventType&) { ++callCount; });
	//	nexus.connect_channel<EventType>(&channelB, [&](int*, EventType&) { ++callCount; });
	//	nexus.connect_channel<EventType>(&channelA, [&](int*) { ++callCount; });
	nexus.broadcast_channel<EventType>(&channelA, 1);
	CHECK(std::exchange(callCount, 0) == 4);

	nexus.connect_channel<EventType>(&channelB, [&](const int*, const EventType&) { ++callCount; });
	nexus.connect_channel<EventType>(&channelB, [&](int*, const EventType&) { ++callCount; });
	nexus.connect_channel<EventType>(&channelB, [&](const EventType&) { ++callCount; });
	nexus.connect_channel<EventType>(&channelB, [&](const int*) { ++callCount; });
	nexus.connect_channel<EventType>(&channelB, [&](int*) { ++callCount; });
	nexus.connect_channel<EventType>(&channelB, [&]() { ++callCount; });
	// These should fail to compile due to const correctness:
	//	nexus.connect_channel<EventType>(&channelB, [&](const int*, EventType&) { ++callCount; });
	//	nexus.connect_channel<EventType>(&channelB, [&](int*, EventType&) { ++callCount; });
	//	nexus.connect_channel<EventType>(&channelB, [&](EventType&) { ++callCount; });
	nexus.broadcast_channel<EventType>(&channelB, 1);

	CHECK(std::exchange(callCount, 0) == 6);
}

// -------------------------------------------------------------------------------------------------

TEST_CASE("Test Nexus simple set/get/remove", "[libv.utility.nexus]") {
	libv::Nexus nexus;

	int objectA = 0;
	int objectB = 0;

	CHECK(nexus.num_object() == 0);
	CHECK(nexus.object_view_get<int>() == nullptr);

	nexus.object_view_set<int>(&objectA);
	CHECK(nexus.num_object() == 1);
	CHECK(nexus.object_view_get<int>() == &objectA);

	nexus.object_view_set<int>(&objectB);
	CHECK(nexus.num_object() == 1);
	CHECK(nexus.object_view_get<int>() == &objectB);

	nexus.object_view_remove<int>();
	CHECK(nexus.num_object() == 0);
	CHECK(nexus.object_view_get<int>() == nullptr);
}

TEST_CASE("Test Nexus addressed set/get/remove", "[libv.utility.nexus]") {
	libv::Nexus nexus;

	int objectA = 0;
	int objectB = 0;

	CHECK(nexus.num_object() == 0);
	CHECK(nexus.object_view_get<int>("A") == nullptr);
	CHECK(nexus.object_view_get<int>("B") == nullptr);

	nexus.object_view_set<int>("A", &objectA);
	CHECK(nexus.num_object() == 1);
	CHECK(nexus.object_view_get<int>("A") == &objectA);
	CHECK(nexus.object_view_get<int>("B") == nullptr);

	nexus.object_view_set<int>("B", &objectB);
	CHECK(nexus.num_object() == 2);
	CHECK(nexus.object_view_get<int>("A") == &objectA);
	CHECK(nexus.object_view_get<int>("B") == &objectB);

	SECTION("Without simple names") {
		// Noop
	}
	SECTION("With simple names") {
		CHECK(nexus.num_object() == 2);
		CHECK(nexus.object_view_get<int>() == nullptr);

		nexus.object_view_set<int>(&objectA);
		CHECK(nexus.num_object() == 3);
		CHECK(nexus.object_view_get<int>() == &objectA);

		nexus.object_view_set<int>(&objectB);
		CHECK(nexus.num_object() == 3);
		CHECK(nexus.object_view_get<int>() == &objectB);

		nexus.object_view_remove<int>();
		CHECK(nexus.num_object() == 2);
		CHECK(nexus.object_view_get<int>() == nullptr);
	}

	nexus.object_view_set<int>("A", &objectB);
	CHECK(nexus.num_object() == 2);
	CHECK(nexus.object_view_get<int>("A") == &objectB);
	CHECK(nexus.object_view_get<int>("B") == &objectB);

	nexus.object_view_remove<int>("A");
	CHECK(nexus.num_object() == 1);
	CHECK(nexus.object_view_get<int>("A") == nullptr);
	CHECK(nexus.object_view_get<int>("B") == &objectB);

	nexus.object_view_remove<int>("B");
	CHECK(nexus.num_object() == 0);
	CHECK(nexus.object_view_get<int>("A") == nullptr);
	CHECK(nexus.object_view_get<int>("B") == nullptr);
}


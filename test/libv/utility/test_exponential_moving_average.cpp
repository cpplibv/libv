// Project: libv.utility, File: test/libv/utility/test_exponential_moving_average.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <catch/catch.hpp>
// pro
#include <libv/utility/approx.hpp>
#include <libv/utility/exponential_moving_average.hpp>


//#include <random>
//#include <iostream>
//#include <libv/utility/uniform_distribution.hpp>

// -------------------------------------------------------------------------------------------------

TEST_CASE("exponential_moving_average", "[libv.utility.exponential_moving_average]") {
	libv::exponential_moving_average<double, 10> avg01{0};
	libv::exponential_moving_average<double, 10> avg02{0};
	libv::exponential_moving_average<double, 10> avg05{0};
	libv::exponential_moving_average<double, 10> avg10{0};
	libv::exponential_moving_average<double, 10> avg20{0};
	libv::exponential_moving_average<double, 10> avg50{0};
//	libv::exponential_moving_average<double, 10.0, 0.8> avg01{0};
//	libv::exponential_moving_average<double, 10.0, 0.8> avg02{0};
//	libv::exponential_moving_average<double, 10.0, 0.8> avg05{0};
//	libv::exponential_moving_average<double, 10.0, 0.8> avg10{0};
//	libv::exponential_moving_average<double, 10.0, 0.8> avg20{0};
//	libv::exponential_moving_average<double, 10.0, 0.8> avg50{0};

	CHECK(avg01.value() == libv::approx(0.0));
	CHECK(avg02.value() == libv::approx(0.0));
	CHECK(avg05.value() == libv::approx(0.0));
	CHECK(avg10.value() == libv::approx(0.0));
	CHECK(avg20.value() == libv::approx(0.0));
	CHECK(avg50.value() == libv::approx(0.0));

	for (int i = 0; i < 100; ++i) 	avg01.add(1.0, 0.1);
	for (int i = 0; i < 50; ++i) 	avg02.add(1.0, 0.2);
	for (int i = 0; i < 20; ++i) 	avg05.add(1.0, 0.5);
	for (int i = 0; i < 10; ++i) 	avg10.add(1.0, 1.0);
	for (int i = 0; i < 5; ++i) 	avg20.add(1.0, 2.0);
	for (int i = 0; i < 2; ++i) 	avg50.add(1.0, 5.0);

	CHECK(avg01.value() == libv::approx(0.8));
	CHECK(avg02.value() == libv::approx(0.8));
	CHECK(avg05.value() == libv::approx(0.8));
	CHECK(avg10.value() == libv::approx(0.8));
	CHECK(avg20.value() == libv::approx(0.8));
	CHECK(avg50.value() == libv::approx(0.8));

	for (int i = 0; i < 100; ++i) 	avg01.add(1.0, 0.1);
	for (int i = 0; i < 50; ++i) 	avg02.add(1.0, 0.2);
	for (int i = 0; i < 20; ++i) 	avg05.add(1.0, 0.5);
	for (int i = 0; i < 10; ++i) 	avg10.add(1.0, 1.0);
	for (int i = 0; i < 5; ++i) 	avg20.add(1.0, 2.0);
	for (int i = 0; i < 2; ++i) 	avg50.add(1.0, 5.0);

	CHECK(avg01.value() == libv::approx(0.96));
	CHECK(avg02.value() == libv::approx(0.96));
	CHECK(avg05.value() == libv::approx(0.96));
	CHECK(avg10.value() == libv::approx(0.96));
	CHECK(avg20.value() == libv::approx(0.96));
	CHECK(avg50.value() == libv::approx(0.96));

	// --- Test data set for single impulse ------------------------------------------------------------

//	avg01.add(1024 * 1024, 0.1);
//	avg02.add(1024 * 1024, 0.2);
//	avg05.add(1024 * 1024, 0.5);
//	avg10.add(1024 * 1024, 1.0);
//	avg20.add(1024 * 1024, 2.0);
//	avg50.add(1024 * 1024, 5.0);
//	std::cout << 1024*1024
//			<< "\t" << avg01.value()
//			<< "\t" << avg02.value()
//			<< "\t" << avg05.value()
//			<< "\t" << avg10.value()
//			<< "\t" << avg20.value()
//			<< "\t" << avg50.value()
//			<< std::endl;
//
//	for (int i = 0; i < 1000; ++i) {
//		const auto v = 0;
//		avg01.add(v, 0.1);
//		avg02.add(v, 0.2);
//		avg05.add(v, 0.5);
//		avg10.add(v, 1.0);
//		avg20.add(v, 2.0);
//		avg50.add(v, 5.0);
//		std::cout << v
//				<< "\t" << avg01.value()
//				<< "\t" << avg02.value()
//				<< "\t" << avg05.value()
//				<< "\t" << avg10.value()
//				<< "\t" << avg20.value()
//				<< "\t" << avg50.value()
//				<< std::endl;
//	}

	// --- Test data set for random signal -------------------------------------------------------------

//	libv::exponential_moving_average<double, 10> avg01;
//	libv::exponential_moving_average<double, 10> avg02;
//	libv::exponential_moving_average<double, 10> avg05;
//	libv::exponential_moving_average<double, 10> avg10;
//	libv::exponential_moving_average<double, 10> avg20;
//	libv::exponential_moving_average<double, 10> avg50;
//	std::mt19937_64 rd(0x5EED);
//
//	for (int i = 0; i < 100; ++i) {
//		const auto b = libv::make_uniform_distribution_inclusive(10, 90)(rd);
//		const auto r = libv::make_uniform_distribution_inclusive(0, 25)(rd);
//		for (int j = 0; j < r; ++j) {
//			const auto v = b + libv::make_uniform_distribution_inclusive(-10, 10)(rd);
//			avg01.add(v, 0.1);
//			avg02.add(v, 0.2);
//			avg05.add(v, 0.5);
//			avg10.add(v, 1.0);
//			avg20.add(v, 2.0);
//			avg50.add(v, 5.0);
//			std::cout << v
//					<< "\t" << avg01.value()
//					<< "\t" << avg02.value()
//					<< "\t" << avg05.value()
//					<< "\t" << avg10.value()
//					<< "\t" << avg20.value()
//					<< "\t" << avg50.value()
//					<< std::endl;
//		}
//	}

	// --- Test data set for square signal -------------------------------------------------------------

//	for (int i = 0; i < 100; ++i) {
//		const auto v = 1;
//		avg01.add(v, 0.1);
//		avg02.add(v, 0.2);
//		avg05.add(v, 0.5);
//		avg10.add(v, 1.0);
//		avg20.add(v, 2.0);
//		avg50.add(v, 5.0);
//		std::cout << v
//				<< "\t" << avg01.value()
//				<< "\t" << avg02.value()
//				<< "\t" << avg05.value()
//				<< "\t" << avg10.value()
//				<< "\t" << avg20.value()
//				<< "\t" << avg50.value()
//				<< std::endl;
//	}
//
//	for (int i = 0; i < 100; ++i) {
//		const auto v = 0;
//		avg01.add(v, 0.1);
//		avg02.add(v, 0.2);
//		avg05.add(v, 0.5);
//		avg10.add(v, 1.0);
//		avg20.add(v, 2.0);
//		avg50.add(v, 5.0);
//		std::cout << v
//				<< "\t" << avg01.value()
//				<< "\t" << avg02.value()
//				<< "\t" << avg05.value()
//				<< "\t" << avg10.value()
//				<< "\t" << avg20.value()
//				<< "\t" << avg50.value()
//				<< std::endl;
//	}
//
//	for (int i = 0; i < 100; ++i) {
//		const auto v = 1;
//		avg01.add(v, 0.1);
//		avg02.add(v, 0.2);
//		avg05.add(v, 0.5);
//		avg10.add(v, 1.0);
//		avg20.add(v, 2.0);
//		avg50.add(v, 5.0);
//		std::cout << v
//				<< "\t" << avg01.value()
//				<< "\t" << avg02.value()
//				<< "\t" << avg05.value()
//				<< "\t" << avg10.value()
//				<< "\t" << avg20.value()
//				<< "\t" << avg50.value()
//				<< std::endl;
//	}
//
//	for (int i = 0; i < 100; ++i) {
//		const auto v = 3;
//		avg01.add(v, 0.1);
//		avg02.add(v, 0.2);
//		avg05.add(v, 0.5);
//		avg10.add(v, 1.0);
//		avg20.add(v, 2.0);
//		avg50.add(v, 5.0);
//		std::cout << v
//				<< "\t" << avg01.value()
//				<< "\t" << avg02.value()
//				<< "\t" << avg05.value()
//				<< "\t" << avg10.value()
//				<< "\t" << avg20.value()
//				<< "\t" << avg50.value()
//				<< std::endl;
//	}
//
//	for (int i = 0; i < 100; ++i) {
//		const auto v = 4;
//		avg01.add(v, 0.1);
//		avg02.add(v, 0.2);
//		avg05.add(v, 0.5);
//		avg10.add(v, 1.0);
//		avg20.add(v, 2.0);
//		avg50.add(v, 5.0);
//		std::cout << v
//				<< "\t" << avg01.value()
//				<< "\t" << avg02.value()
//				<< "\t" << avg05.value()
//				<< "\t" << avg10.value()
//				<< "\t" << avg20.value()
//				<< "\t" << avg50.value()
//				<< std::endl;
//	}
//
//	for (int i = 0; i < 100; ++i) {
//		const auto v = 1;
//		avg01.add(v, 0.1);
//		avg02.add(v, 0.2);
//		avg05.add(v, 0.5);
//		avg10.add(v, 1.0);
//		avg20.add(v, 2.0);
//		avg50.add(v, 5.0);
//		std::cout << v
//				<< "\t" << avg01.value()
//				<< "\t" << avg02.value()
//				<< "\t" << avg05.value()
//				<< "\t" << avg10.value()
//				<< "\t" << avg20.value()
//				<< "\t" << avg50.value()
//				<< std::endl;
//	}
}

// -------------------------------------------------------------------------------------------------

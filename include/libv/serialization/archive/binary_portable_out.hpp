
#pragma once

// ext
#include <boost/archive/basic_binary_oarchive.hpp>
#include <boost/archive/basic_binary_oprimitive.hpp>
#include <boost/archive/detail/register_archive.hpp>

#include <boost/spirit/home/support/detail/endian/endian.hpp>
#include <boost/spirit/home/support/detail/math/fpclassify.hpp>

#include <boost/type_traits/is_arithmetic.hpp>
#include <boost/type_traits/is_floating_point.hpp>
#include <boost/type_traits/is_integral.hpp>
#include <boost/type_traits/is_signed.hpp>

#include <boost/version.hpp>
// std
#include <ostream>
// pro
#include <libv/serialization/archive/binary_portable_exception.hpp>

// namespace alias
namespace fp = boost::spirit::math;

namespace libv {
namespace archive {

// -------------------------------------------------------------------------------------------------

class BinaryPortableOut;

using portable_oprimitive =
		boost::archive::basic_binary_oprimitive<BinaryPortableOut, std::ostream::char_type, std::ostream::traits_type>;

/**
 * \brief Portable binary output archive using little endian format.
 *
 * This archive addresses integer size, endianness and floating point types so
 * that data can be transferred across different systems. The archive consists
 * primarily of three different save implementations for integral types,
 * floating point types and string types.
 */
class BinaryPortableOut :
public portable_oprimitive,
public boost::archive::basic_binary_oarchive<BinaryPortableOut> {

	template <int>
	struct ignore {
		ignore(int) { }
	};

	void save_signed_char(const signed char& c) {
		portable_oprimitive::save(c);
	}

	void init(unsigned flags) {
		if (flags & boost::archive::no_header) { // if we don't have header we assume boost 1.33
			BOOST_ASSERT(archive_version == 3);
			return;
		}

		// write our minimalistic header (magic byte plus version)
		// the boost archives write a string instead - by calling
		// boost::archive::basic_binary_oarchive<derived_t>::init()
		operator<<(portable_binary_signature);
		operator<<(archive_version);
	}

public:
	template <class C, class T, class A>
	void save(const std::basic_string<C, T, A>& s) {
		BOOST_STATIC_ASSERT(sizeof (C) == sizeof (char));
		portable_oprimitive::save(s);
	}
	void save(bool b) {
		 // Intended conversion from bool to bool to avoid non 0 or 1 value to be serialized
		save_signed_char(b ? true : false);
	}
	/**
	 * \brief Save integer types.
	 *
	 * First we save the size information ie. the number of bytes that hold the
	 * actual data. We subsequently transform the data using store_little_endian
	 * and store non-zero bytes to the stream.
	 */
	template <typename T>
	typename std::enable_if<boost::is_integral<T>::value>::type
	save(const T& t, ignore<2> = 0) {
		if (T temp = t) {
			// examine the number of bytes
			// needed to represent the number
			int8_t size = 0;
			do {
				temp = static_cast<T> (temp >> CHAR_BIT);
				++size;
				//			} while (temp != 0 && temp != {-1});
			} while (temp != 0 && temp != static_cast<T> (-1));

			// encode the sign bit into the size
			save_signed_char(t > 0 ? size : static_cast<int8_t> (-size));
			BOOST_ASSERT(t > 0 || boost::is_signed<T>::value);

			// we choose to use little endian because this way we just
			// save the first size bytes to the stream and skip the rest
			boost::spirit::detail::store_little_endian<T, sizeof (T)>(&temp, t);
			save_binary(&temp, size);
		} else
			save_signed_char(0); // zero optimization
	}
	/**
	 * \brief Save floating point types.
	 *
	 * We simply rely on fp_traits to extract the bit pattern into an (unsigned)
	 * integral type and store that into the stream. Francois Mauger provided
	 * standardized behaviour for special values like inf and NaN, that need to
	 * be serialized in his application.
	 *
	 * \note by Johan Rade (author of the floating point utilities library):
	 * Be warned that the math::detail::fp_traits<T>::type::get_bits() function
	 * is *not* guaranteed to give you all bits of the floating point number. It
	 * will give you all bits if and only if there is an integer type that has
	 * the same size as the floating point you are copying from. It will not
	 * give you all bits for double if there is no uint64_t. It will not give
	 * you all bits for long double if sizeof(long double) > 8 or there is no
	 * uint64_t.
	 *
	 * The member fp_traits<T>::type::coverage will tell you whether all bits
	 * are copied. This is a typedef for either math::detail::all_bits or
	 * math::detail::not_all_bits.
	 *
	 * If the function does not copy all bits, then it will copy the most
	 * significant bits. So if you serialize and deserialize the way you
	 * describe, and fp_traits<T>::type::coverage is math::detail::not_all_bits,
	 * then your floating point numbers will be truncated. This will introduce
	 * small rounding off errors.
	 */
	template <typename T>
	typename std::enable_if<boost::is_floating_point<T>::value>::type
	save(const T& t, ignore<3> = 0) {
		typedef typename fp::detail::fp_traits<T>::type traits;

		// if the no_infnan flag is set we must throw here
		if (get_flags() & no_infnan && !fp::isfinite(t))
			throw IllegalFloatException(t);

		// if you end here there are three possibilities:
		// 1. you're serializing a long double which is not portable
		// 2. you're serializing a double but have no 64 bit integer
		// 3. your machine is using an unknown floating point format
		// after reading the note above you still might decide to
		// deactivate this static assert and try if it works out.
		typename traits::bits bits;
		BOOST_STATIC_ASSERT(sizeof (bits) == sizeof (T));
		BOOST_STATIC_ASSERT(std::numeric_limits<T>::is_iec559);

		// examine value closely
		switch (fp::fpclassify(t)) {
		case FP_ZERO: bits = 0;
			break;
		case FP_NAN: bits = traits::exponent | traits::mantissa;
			break;
		case FP_INFINITE: bits = traits::exponent | (t < 0) * traits::sign;
			break;
		case FP_SUBNORMAL: assert(std::numeric_limits<T>::has_denorm);
		case FP_NORMAL: traits::get_bits(t, bits); break;
		default:
			throw IllegalFloatException(t);
		}

		save(bits);
	}
	BinaryPortableOut(std::ostream& os, unsigned flags = 0) :
		portable_oprimitive(*os.rdbuf(), flags & boost::archive::no_codecvt),
		boost::archive::basic_binary_oarchive<BinaryPortableOut>(flags) {
		init(flags);
	}
	BinaryPortableOut(std::streambuf& sb, unsigned flags = 0) :
		portable_oprimitive(sb, flags & boost::archive::no_codecvt),
		boost::archive::basic_binary_oarchive<BinaryPortableOut>(flags) {
		init(flags);
	}
	void save(const boost::archive::library_version_type& version) {
		save(static_cast<uint_least16_t> (version));
	}
	void save(const boost::archive::class_id_type& class_id) {
		save(static_cast<int_least16_t> (class_id));
	}
	void save(const boost::serialization::item_version_type& class_id) {
		save(static_cast<uint_least32_t> (class_id));
	}
	void save(const boost::serialization::collection_size_type& class_id) {
		save(static_cast<size_t> (class_id));
	}
	void save(const boost::archive::object_id_type& object_id) {
		save(static_cast<uint_least32_t> (object_id));
	}
	void save(const boost::archive::version_type& version) {
		save(static_cast<uint_least32_t> (version));
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace archive
} // namespace libv

#include <boost/archive/impl/basic_binary_oarchive.ipp>
#include <boost/archive/impl/basic_binary_oprimitive.ipp>

#if !defined BOOST_ARCHIVE_SERIALIZER_INCLUDED
#    include <boost/archive/impl/archive_serializer_map.ipp>
#    define BOOST_ARCHIVE_SERIALIZER_INCLUDED
#endif

namespace boost {
namespace archive {

// explicitly instantiation
template class detail::archive_serializer_map<libv::archive::BinaryPortableOut>;

} // namespace archive
} // namespace boost

BOOST_SERIALIZATION_REGISTER_ARCHIVE(::libv::archive::BinaryPortableOut)

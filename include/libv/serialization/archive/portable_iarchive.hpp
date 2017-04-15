
#pragma once

// ext
#include <boost/archive/basic_binary_iarchive.hpp>
#include <boost/archive/basic_binary_iprimitive.hpp>
#include <boost/archive/detail/register_archive.hpp>

#include <boost/spirit/home/support/detail/endian/endian.hpp>
#include <boost/spirit/home/support/detail/math/fpclassify.hpp>

#include <boost/type_traits/is_arithmetic.hpp>
#include <boost/type_traits/is_floating_point.hpp>
#include <boost/type_traits/is_integral.hpp>
#include <boost/type_traits/is_unsigned.hpp>

#include <boost/version.hpp>
// std
#include <istream>
// pro
#include <libv/serialization/archive/portable_archive_exception.hpp>

// namespace alias
namespace fp = boost::spirit::math;

namespace libv {
namespace archive {

// -------------------------------------------------------------------------------------------------

class portable_iarchive;

using portable_iprimitive =
		boost::archive::basic_binary_iprimitive<portable_iarchive, std::istream::char_type, std::istream::traits_type>;

/**
 * \brief Portable binary input archive using little endian format.
 *
 * This archive addresses integer size, endianness and floating point types so
 * that data can be transferred across different systems. The archive consists
 * primarily of three different save implementations for integral types,
 * floating point types and string types.
 */
class portable_iarchive :
public portable_iprimitive,
public boost::archive::basic_binary_iarchive<portable_iarchive> {

	template <int> struct ignore {
		ignore(int) { }
	};

	signed char load_signed_char() {
		signed char c;
		portable_iprimitive::load(c);
		return c;
	}

	void init(unsigned flags) {
		using namespace boost::archive;
		boost::archive::library_version_type input_library_version;

		if (flags & no_header) { // if we don't have header we assume boost 1.33
			set_library_version(archive_version);
			return;
		}

		signature sig;
		operator>>(sig);
		if (sig != portable_binary_signature)
			throw archive_exception(archive_exception::invalid_signature);

		operator>>(input_library_version);

		if (archive_version < input_library_version)
			throw archive_exception(archive_exception::unsupported_version);
		else
			set_library_version(input_library_version);
	}

public:
	template <class C, class T, class A>
	void load(std::basic_string<C, T, A>& s) {
		BOOST_STATIC_ASSERT(sizeof (C) == sizeof (char));
		portable_iprimitive::load(s);
	}
	void load(bool& b) {
		b = load_signed_char();
	}
	/**
	 * \brief Load integer types.
	 *
	 * First we load the size information ie. the number of bytes that
	 * hold the actual data. Then we retrieve the data and transform it
	 * to the original value by using load_little_endian.
	 */
	template <typename T>
	typename std::enable_if<boost::is_integral<T>::value>::type
	load(T & t, ignore<2> = 0) {
		// get the number of bytes in the stream
		if (signed char size = load_signed_char()) {
			// check for negative value in unsigned type
			if (size < 0 && boost::is_unsigned<T>::value)
				throw NegativUnsignedException();

				// check that our type T is large enough
			else if (static_cast<unsigned> (abs(size)) > sizeof (T))
				throw IntegerSizeException(size);

			// reconstruct the value
			T temp = size < 0 ? -1 : 0;
			load_binary(&temp, abs(size));

			// load the value from little endian - is is then converted
			// to the target type T and fits it because size <= sizeof(T)
			t = boost::spirit::detail::load_little_endian<T, sizeof (T)>(&temp);
		} else
			t = 0; // zero optimization
	}
	/**
	 * \brief Load floating point types.
	 *
	 * We simply rely on fp_traits to set the bit pattern from the (unsigned)
	 * integral type that was stored in the stream. Francois Mauger provided
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
	load(T& t, ignore<3> = 0) {
		typedef typename fp::detail::fp_traits<T>::type traits;

		// if you end here there are three possibilities:
		// 1. you're serializing a long double which is not portable
		// 2. you're serializing a double but have no 64 bit integer
		// 3. your machine is using an unknown floating point format
		// after reading the note above you still might decide to
		// deactivate this static assert and try if it works out.
		typename traits::bits bits;
		BOOST_STATIC_ASSERT(sizeof (bits) == sizeof (T));
		BOOST_STATIC_ASSERT(std::numeric_limits<T>::is_iec559);

		load(bits);
		traits::set_bits(t, bits);

		// if the no_infnan flag is set we must throw here
		if (get_flags() & no_infnan && !fp::isfinite(t))
			throw IllegalFloatException(t);

		// if you end here your floating point type does not support
		// denormalized numbers. this might be the case even though
		// your type conforms to IEC 559 (and thus to IEEE 754)
		if (std::numeric_limits<T>::has_denorm == std::denorm_absent
				&& fp::fpclassify(t) == (int) FP_SUBNORMAL) // GCC4
			throw IllegalFloatException(t);
	}
	portable_iarchive(std::istream& is, unsigned flags = 0) :
		portable_iprimitive(*is.rdbuf(), flags & boost::archive::no_codecvt),
		boost::archive::basic_binary_iarchive<portable_iarchive>(flags) {
		init(flags);
	}
	portable_iarchive(std::streambuf& sb, unsigned flags = 0) :
		portable_iprimitive(sb, flags & boost::archive::no_codecvt),
		boost::archive::basic_binary_iarchive<portable_iarchive>(flags) {
		init(flags);
	}
	void load(boost::archive::library_version_type& version) {
		load(static_cast<uint_least16_t&> (version));
	}
	void load(boost::archive::class_id_type& class_id) {
		load(static_cast<int_least16_t&> (class_id));
	}
	void load(boost::serialization::item_version_type& version) {
		load(static_cast<uint_least32_t&> (version));
	}
	void load(boost::serialization::collection_size_type& version) {
		load(static_cast<size_t&> (version));
	}
	void load(boost::archive::object_id_type& object_id) {
		load(static_cast<uint_least32_t&> (object_id));
	}
	void load(boost::archive::version_type& version) {
		load(static_cast<uint_least32_t&> (version));
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace archive
} // namespace libv

#include <boost/archive/impl/basic_binary_iarchive.ipp>
#include <boost/archive/impl/basic_binary_iprimitive.ipp>

#if !defined BOOST_ARCHIVE_SERIALIZER_INCLUDED
#    include <boost/archive/impl/archive_serializer_map.ipp>
#    define BOOST_ARCHIVE_SERIALIZER_INCLUDED
#endif

namespace boost {
namespace archive {

// explicitly instantiation
template class detail::archive_serializer_map<libv::archive::portable_iarchive>;

} // namespace archive
} // namespace boost

BOOST_SERIALIZATION_REGISTER_ARCHIVE(::libv::archive::portable_iarchive)

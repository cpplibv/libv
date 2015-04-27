// File: Vec.inl, Created on 2014. november 28. 17:45, Author: Vader

#ifndef VLIB_VEC_INLINE
#    define VLIB_VEC_INLINE

// <editor-fold defaultstate="collapsed" desc="Generated Internal Macros: Custom vec helpers ...">
#    define __v2get(p1,p2)       /** Composite a new vector based on members @return The new vector */\
                       inline vec2_t<T> p1##p2() const noexcept {        \
                            return vec2_t<T>(p1,p2);} 
#    define __v3get(p1,p2,p3)    /** Composite a new vector based on members @return The new vector */\
                       inline vec3_t<T> p1##p2##p3() const noexcept {    \
                           return vec3_t<T>(p1,p2,p3);} 
#    define __v4get(p1,p2,p3,p4) /** Composite a new vector based on members @return The new vector */\
					   inline vec4_t<T> p1##p2##p3##p4() const noexcept {\
                           return vec4_t<T>(p1,p2,p3,p4);}

#    define __v22p1(p1)      __v2get(p1,x)       __v2get(p1,y)       
#    define __v22            __v22p1(x)          __v22p1(y)          
#    define __v23p2(p1,p2)   __v3get(p1,p2,x)    __v3get(p1,p2,y)    
#    define __v23p1(p1)      __v23p2(p1,x)       __v23p2(p1,y)       
#    define __v23            __v23p1(x)          __v23p1(y)          
#    define __v24p3(p1,p2,p3)__v4get(p1,p2,p3,x) __v4get(p1,p2,p3,y) 
#    define __v24p2(p1,p2)   __v24p3(p1,p2,x)    __v24p3(p1,p2,y)    
#    define __v24p1(p1)	     __v24p2(p1,x)       __v24p2(p1,y)       
#    define __v24            __v24p1(x)          __v24p1(y)          

#    define __v32p1(p1)      __v2get(p1,x)       __v2get(p1,y)       __v2get(p1,z)       
#    define __v32            __v32p1(x)          __v32p1(y)          __v32p1(z)          
#    define __v33p2(p1,p2)   __v3get(p1,p2,x)    __v3get(p1,p2,y)    __v3get(p1,p2,z)    
#    define __v33p1(p1)      __v33p2(p1,x)       __v33p2(p1,y)       __v33p2(p1,z)       
#    define __v33            __v33p1(x)          __v33p1(y)          __v33p1(z)          
#    define __v34p3(p1,p2,p3)__v4get(p1,p2,p3,x) __v4get(p1,p2,p3,y) __v4get(p1,p2,p3,z) 
#    define __v34p2(p1,p2)   __v34p3(p1,p2,x)    __v34p3(p1,p2,y)    __v34p3(p1,p2,z)    
#    define __v34p1(p1)	     __v34p2(p1,x)       __v34p2(p1,y)       __v34p2(p1,z)       
#    define __v34            __v34p1(x)          __v34p1(y)          __v34p1(z)          

#    define __v42p1(p1)      __v2get(p1,x)       __v2get(p1,y)       __v2get(p1,z)       __v2get(p1,w)       
#    define __v42            __v42p1(x)          __v42p1(y)          __v42p1(z)          __v42p1(w)          
#    define __v43p2(p1,p2)   __v3get(p1,p2,x)    __v3get(p1,p2,y)    __v3get(p1,p2,z)    __v3get(p1,p2,w)    
#    define __v43p1(p1)      __v43p2(p1,x)       __v43p2(p1,y)       __v43p2(p1,z)       __v43p2(p1,w)       
#    define __v43            __v43p1(x)          __v43p1(y)          __v43p1(z)          __v43p1(w)          
#    define __v44p3(p1,p2,p3)__v4get(p1,p2,p3,x) __v4get(p1,p2,p3,y) __v4get(p1,p2,p3,z) __v4get(p1,p2,p3,w) 
#    define __v44p2(p1,p2)   __v44p3(p1,p2,x)    __v44p3(p1,p2,y)    __v44p3(p1,p2,z)    __v44p3(p1,p2,w)    
#    define __v44p1(p1)	     __v44p2(p1,x)       __v44p2(p1,y)       __v44p2(p1,z)       __v44p2(p1,w)       
#    define __v44            __v44p1(x)          __v44p1(y)          __v44p1(z)          __v44p1(w)          
// </editor-fold>

#    define implement_vec2_custom_vec2_gets __v22
#    define implement_vec2_custom_vec3_gets __v23
#    define implement_vec2_custom_vec4_gets __v24
#    define implement_vec3_custom_vec2_gets __v32
#    define implement_vec3_custom_vec3_gets __v33
#    define implement_vec3_custom_vec4_gets __v34
#    define implement_vec4_custom_vec2_gets __v42
#    define implement_vec4_custom_vec3_gets __v43
#    define implement_vec4_custom_vec4_gets __v44
#endif

#ifndef COMMA
#    define COMMA ,
#endif

//---------------------------------------------------------------------------------------

#undef VEC_NAME_COMPOSER
#undef VEC_NAME_HELPER
#undef VEC_NAME

#define VEC_NAME_COMPOSER(DIM) vec##DIM##_t
#define VEC_NAME_HELPER(DIM) VEC_NAME_COMPOSER(DIM)
#define VEC_NAME VEC_NAME_HELPER(VEC_DIM)

//---------------------------------------------------------------------------------------

#if VEC_DIM == 2  //-------------------------------------------------------------

#    define VEC_DATA_STRUCTRE \
		union { T x, r, s, u; }; \
		union { T y, g, t, v; };

#    define VEC_OPERATOR(LHS, OPERATOR, RHS) \
		(LHS ptr[0]) OPERATOR (RHS ptr[0]); \
		(LHS ptr[1]) OPERATOR (RHS ptr[1]);

#    define VEC_LIST(VEC, SEPARATOR) \
		VEC ptr[0] SEPARATOR \
		VEC ptr[1]

#    define VEC_LIST_2(VEC, VEC2, SEPARATOR, OPERATOR) \
		VEC ptr[0] OPERATOR VEC2 ptr[0] SEPARATOR \
		VEC ptr[1] OPERATOR VEC2 ptr[1]

#    define VEC_LIST_3(VEC, SEPARATOR, CAST) \
		CAST (VEC ptr[0]) SEPARATOR \
		CAST (VEC ptr[1])

#elif VEC_DIM == 3 //-------------------------------------------------------------

#    define VEC_DATA_STRUCTRE \
		union { T x, r, s, u; }; \
		union { T y, g, t, v; }; \
		union { T z, b, p; };

#    define VEC_OPERATOR(LHS, OPERATOR, RHS) \
		(LHS ptr[0]) OPERATOR (RHS ptr[0]); \
		(LHS ptr[1]) OPERATOR (RHS ptr[1]); \
		(LHS ptr[2]) OPERATOR (RHS ptr[2]);

#    define VEC_LIST(VEC, SEPARATOR) \
		VEC ptr[0] SEPARATOR \
		VEC ptr[1] SEPARATOR \
		VEC ptr[2]

#    define VEC_LIST_2(VEC, VEC2, SEPARATOR, OPERATOR) \
		VEC ptr[0] OPERATOR VEC2 ptr[0] SEPARATOR \
		VEC ptr[1] OPERATOR VEC2 ptr[1] SEPARATOR \
		VEC ptr[2] OPERATOR VEC2 ptr[2]

#    define VEC_LIST_3(VEC, SEPARATOR, CAST) \
		CAST (VEC ptr[0]) SEPARATOR \
		CAST (VEC ptr[1]) SEPARATOR \
		CAST (VEC ptr[2])

#elif VEC_DIM == 4 //-------------------------------------------------------------

#    define VEC_DATA_STRUCTRE \
		union { T x, r, s, u; }; \
		union { T y, g, t, v; }; \
		union { T z, b, p; }; \
		union { T w, a, q; };

#    define VEC_OPERATOR(LHS, OPERATOR, RHS) \
		(LHS ptr[0]) OPERATOR (RHS ptr[0]); \
		(LHS ptr[1]) OPERATOR (RHS ptr[1]); \
		(LHS ptr[2]) OPERATOR (RHS ptr[2]); \
		(LHS ptr[3]) OPERATOR (RHS ptr[3]);

#    define VEC_LIST(VEC, SEPARATOR) \
		VEC ptr[0] SEPARATOR \
		VEC ptr[1] SEPARATOR \
		VEC ptr[2] SEPARATOR \
		VEC ptr[3]

#    define VEC_LIST_2(VEC, VEC2, SEPARATOR, OPERATOR) \
		VEC ptr[0] OPERATOR VEC2 ptr[0] SEPARATOR \
		VEC ptr[1] OPERATOR VEC2 ptr[1] SEPARATOR \
		VEC ptr[2] OPERATOR VEC2 ptr[2] SEPARATOR \
		VEC ptr[3] OPERATOR VEC2 ptr[3]

#    define VEC_LIST_3(VEC, SEPARATOR, CAST) \
		CAST (VEC ptr[0]) SEPARATOR \
		CAST (VEC ptr[1]) SEPARATOR \
		CAST (VEC ptr[2]) SEPARATOR \
		CAST (VEC ptr[3])
#endif

//---------------------------------------------------------------------------------------

template <typename T>
struct VEC_NAME {

	union {
		struct {
			VEC_DATA_STRUCTRE
		};
		T ptr[VEC_DIM];
	};

#if VEC_DIM == 2	
	explicit VEC_NAME(T x = 0, T y = 0) noexcept : x(x), y(y) { }
#elif VEC_DIM == 3
	explicit VEC_NAME(T x = 0, T y = 0, T z = 0) noexcept : x(x), y(y), z(z) { }
	explicit VEC_NAME(T x, const vec2_t<T>& yz) noexcept : x(x), y(yz.x), z(yz.y) { }
	explicit VEC_NAME(const vec2_t<T>& xy, T z = 0) noexcept : x(xy.x), y(xy.y), z(z) { }
#elif VEC_DIM == 4
	explicit VEC_NAME(T x = 0, T y = 0, T z = 0, T w = 0) noexcept : x(x), y(y), z(z), w(w) { }
	explicit VEC_NAME(const vec2_t<T>& xy, T z = 0, T w = 0) noexcept : x(xy.x), y(xy.y), z(z), w(w) { }
	explicit VEC_NAME(T x, const vec2_t<T>& yz, T w = 0) noexcept : x(x), y(yz.x), z(yz.y), w(w) { }
	explicit VEC_NAME(T x, T y, const vec2_t<T>& zw) noexcept : x(x), y(y), z(zw.x), w(zw.y) { }
	explicit VEC_NAME(const vec2_t<T>& xy, const vec2_t<T>& zw) noexcept : x(xy.x), y(xy.y), z(zw.x), w(zw.y) { }
	explicit VEC_NAME(const vec3_t<T>& xyz, T w = 0) noexcept : x(xyz.x), y(xyz.y), z(xyz.z), w(w) { }
	explicit VEC_NAME(T x, const vec3_t<T>& yzw) noexcept : x(x), y(yzw.x), z(yzw.y), w(yzw.z) { }
#endif
	
	VEC_NAME(const VEC_NAME& v) noexcept = default;
	VEC_NAME(VEC_NAME&& v) noexcept = default;
	template<typename K>
	explicit VEC_NAME(const VEC_NAME<K>& rhs) :
		ptr {VEC_LIST_3(rhs., COMMA, static_cast<T>)}{}
	inline VEC_NAME<T>& operator=(const VEC_NAME<T>& rhs) noexcept {
		VEC_OPERATOR(, =, rhs.)
		return *this;
	}
	template<typename K>
	inline VEC_NAME<T>& operator=(const VEC_NAME<K>& rhs) {
		VEC_OPERATOR(, = static_cast<T>, rhs.)
		return *this;
	}

	//==========================================================================
	template<typename K>
	inline VEC_NAME<T>& operator+=(const VEC_NAME<K>& rhs) noexcept {
		VEC_OPERATOR(, +=, rhs.)
		return *this;
	}
	template<typename K>
	inline VEC_NAME<T>& operator-=(const VEC_NAME<K>& rhs) noexcept {
		VEC_OPERATOR(, -=, rhs.)
		return *this;
	}
	template<typename K>
	inline VEC_NAME<T>& operator*=(const VEC_NAME<K>& rhs) noexcept {
		VEC_OPERATOR(, *=, rhs.)
		return *this;
	}
	template<typename K>
	inline VEC_NAME<T>& operator/=(const VEC_NAME<K>& rhs) noexcept {
		VEC_OPERATOR(, /=, rhs.)
		return *this;
	}

	//--------------------------------------------------------------------------
	template<typename K>
	inline VEC_NAME<T>& operator*=(const K& v) noexcept {
		VEC_LIST(, *= v;) *= v;
		return *this;
	}
	template<typename K>
	inline VEC_NAME<T>& operator/=(const K& v) noexcept {
		VEC_LIST(, /= v;) /= v;
		return *this;
	}

	//==========================================================================

	/** Return the length of the vector
	 * @note Does not change the original vector
	 * @template K The minimum precision type Default: T
	 * @return The vector */
	template<typename K = T>
	inline auto length() const noexcept -> decltype(ptr[0] / K()) {
		return sqrt(VEC_LIST_2(,, +, *));
	}

	/** Return the square length of the vector
	 * @note Does not change the original vector
	 * @template K The minimum precision type Default: T
	 * @return The vector */
	template<typename K = T>
	inline auto lengthSquare() const noexcept -> decltype(ptr[0] / K()) {
		return VEC_LIST_2(,, +, *);
	}
	/** Normalize the vector (by divide each component by the length)
	 * @return The vector */
	inline VEC_NAME<T>& normalize() noexcept {
		assert(lengthSquare() != 0);
		return operator/=(length());
	}

	/** Return the normalized vector (by divide each component by the length).
	 * @note Does not change the original vector
	 * @template K The minimum precision type Default: T
	 * @return The normalized vector */
	template<typename K = T>
	inline auto normalized() const noexcept
	-> decltype(VEC_NAME<decltype(ptr[0] / K())>()) {
		assert(lengthSquare() != 0);
		return *this / length();
	}

	//==========================================================================

#if VEC_DIM == 2	
	implement_vec2_custom_vec2_gets
	implement_vec2_custom_vec3_gets
	implement_vec2_custom_vec4_gets
#elif VEC_DIM == 3
	implement_vec3_custom_vec2_gets
	implement_vec3_custom_vec3_gets
	implement_vec3_custom_vec4_gets
#elif VEC_DIM == 4
	implement_vec4_custom_vec2_gets
	implement_vec4_custom_vec3_gets
	implement_vec4_custom_vec4_gets
#endif

	~VEC_NAME() noexcept { }
};

//==========================================================================

#define VEC_IMPLEMENT_OPERATOR(OPERATOR) \
		template<typename T, typename K> \
		inline const auto operator OPERATOR(const VEC_NAME<T>& lhs, const VEC_NAME<K>& rhs) noexcept \
		-> decltype(VEC_NAME<decltype(lhs.ptr[0] OPERATOR rhs.ptr[0])>()) { \
			return VEC_NAME<decltype(lhs.ptr[0] OPERATOR rhs.ptr[0])>( \
				VEC_LIST_2(lhs.,rhs.,COMMA,OPERATOR)); \
		}

VEC_IMPLEMENT_OPERATOR(-)
VEC_IMPLEMENT_OPERATOR(+)
VEC_IMPLEMENT_OPERATOR(*)
VEC_IMPLEMENT_OPERATOR( /)

template<typename T, typename K>
inline const auto operator*(const VEC_NAME<T>& lhs, const K& rhs) noexcept
-> decltype(VEC_NAME<decltype(lhs.ptr[0] * rhs)>()) {
	return VEC_NAME < decltype(lhs.ptr[0] * rhs)>(VEC_LIST(lhs., * rhs COMMA) * rhs);
}

template<typename T, typename K>
inline const auto operator/(const VEC_NAME<T>& lhs, const K& rhs) noexcept
-> decltype(VEC_NAME<decltype(lhs.ptr[0] / rhs)>()) {
	return VEC_NAME < decltype(lhs.ptr[0] / rhs)>(VEC_LIST(lhs., / rhs COMMA) / rhs);
}

//

template<typename T, typename K>
inline const auto operator*(const K& lhs, const VEC_NAME<T>& rhs) noexcept
-> decltype(VEC_NAME<decltype(lhs * rhs.ptr[0])>()) {
	return VEC_NAME < decltype(lhs * rhs.ptr[0])>(VEC_LIST(lhs * rhs., COMMA));
}

//==========================================================================

/** Determine two vector's equality by comparing each components equality
 * @return The result*/
template<typename T, typename K>
inline bool operator==(const VEC_NAME<T>& lhs, const VEC_NAME<K>& rhs) noexcept {
	return VEC_LIST_2(lhs., rhs., &&, ==);
}
/** Determine two vector's inequality by comparing each components inequality
 * @return The result*/
template<typename T, typename K>
inline bool operator!=(const VEC_NAME<T>& lhs, const VEC_NAME<K>& rhs) noexcept {
	return VEC_LIST_2(lhs., rhs., ||, !=);
}
/** Compare two vector's length for first less than second
 * @return The result*/
template<typename T, typename K>
inline bool operator<(const VEC_NAME<T>& lhs, const VEC_NAME<K>& rhs) noexcept {
	return lhs.lengthSquare() < rhs.lengthSquare();
}
/** Compare two vector's length for first less or equal than second
 * @return The result*/
template<typename T, typename K>
inline bool operator<=(const VEC_NAME<T>& lhs, const VEC_NAME<K>& rhs) noexcept {
	return lhs.lengthSquare() <= rhs.lengthSquare();
}
/** Compare two vector's length for first greater than second
 * @return The result*/
template<typename T, typename K>
inline bool operator>(const VEC_NAME<T>& lhs, const VEC_NAME<K>& rhs) noexcept {
	return lhs.lengthSquare() > rhs.lengthSquare();
}
/** Compare two vector's length for first greater or equal than second
 * @return The result*/
template<typename T, typename K>
inline bool operator>=(const VEC_NAME<T>& lhs, const VEC_NAME<K>& rhs) noexcept {
	return lhs.lengthSquare() >= rhs.lengthSquare();
}

//==========================================================================

template<typename T, typename K>
inline bool operator<(const VEC_NAME<T>& lhs, const K& rhs) noexcept {
	return lhs.length() < rhs;
}
template<typename T, typename K>
inline bool operator<=(const VEC_NAME<T>& lhs, const K& rhs) noexcept {
	return lhs.length() <= rhs;
}
template<typename T, typename K>
inline bool operator>(const VEC_NAME<T>& lhs, const K& rhs) noexcept {
	return lhs.length() > rhs;
}
template<typename T, typename K>
inline bool operator>=(const VEC_NAME<T>& lhs, const K& rhs) noexcept {
	return lhs.length() >= rhs;
}

//
template<typename T, typename K>
inline bool operator<(const K& lhs, const VEC_NAME<T>& rhs) noexcept {
	return lhs < rhs.length();
}
template<typename T, typename K>
inline bool operator<=(const K& lhs, const VEC_NAME<T>& rhs) noexcept {
	return lhs <= rhs.length();
}
template<typename T, typename K>
inline bool operator>(const K& lhs, const VEC_NAME<T>& rhs) noexcept {
	return lhs > rhs.length();
}
template<typename T, typename K>
inline bool operator>=(const K& lhs, const VEC_NAME<T>& rhs) noexcept {
	return lhs >= rhs.length();
}

//==========================================================================

template<typename T>
inline std::ostream& operator<<(std::ostream& os, const VEC_NAME<T> vec) {
	return os << VEC_LIST(vec., << " " <<);
}

//==========================================================================

#undef VEC_DATA_STRUCTRE
#undef VEC_OPERATOR
#undef VEC_LIST
#undef VEC_LIST_2
#undef VEC_LIST_3
#undef VEC_IMPLEMENT_OPERATOR
#undef VEC_NAME
#undef VEC_NAME_COMPOSER
#undef VEC_NAME_HELPER
// Project: libv.serialization, File: src/libv/serialization/enable.hpp, Author: Császár Mátyás [Vader]

#pragma once


#define LIBV_SERIALIAZTION_ENABLE_REFLECTION_SAVE() using __libv_serialiaztion_enable_reflection_save = void
#define LIBV_SERIALIAZTION_ENABLE_REFLECTION_LOAD() using __libv_serialiaztion_enable_reflection_load = void
#define LIBV_SERIALIAZTION_ENABLE_REFLECTION() LIBV_SERIALIAZTION_ENABLE_REFLECTION_SAVE(); LIBV_SERIALIAZTION_ENABLE_REFLECTION_LOAD()

#define LIBV_SERIALIAZTION_ENABLE_MEMBERWISE_SAVE() using __libv_serialiaztion_enable_memberwise_save = void
#define LIBV_SERIALIAZTION_ENABLE_MEMBERWISE_LOAD() using __libv_serialiaztion_enable_memberwise_load = void
#define LIBV_SERIALIAZTION_ENABLE_MEMBERWISE() LIBV_SERIALIAZTION_ENABLE_MEMBERWISE_SAVE(); LIBV_SERIALIAZTION_ENABLE_MEMBERWISE_LOAD()

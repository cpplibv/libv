// Project: libv.utility, File: src/libv/utility/cloud.cpp

// hpp
#include <libv/utility/cloud.hpp>
// libv
#include <libv/utility/hash.hpp>
// std
#include <mutex>
#include <unordered_map>


namespace libv {

// -------------------------------------------------------------------------------------------------

struct CloudObjectKeyView {
	std::string_view name;
	Cloud::type_uid type;
};

struct CloudObjectKey {
	std::string name;
	Cloud::type_uid type;

	[[nodiscard]] constexpr LIBV_FORCE_INLINE bool operator==(const CloudObjectKey&) const noexcept = default;
	[[nodiscard]] constexpr LIBV_FORCE_INLINE bool operator==(const CloudObjectKeyView& rhs) const noexcept {
		return type == rhs.type && name == rhs.name;
	}
};

} // namespace libv --------------------------------------------------------------------------------

LIBV_MAKE_HASHABLE(::libv::CloudObjectKey, t.name, t.type);
LIBV_MAKE_HASHABLE(::libv::CloudObjectKeyView, t.name, t.type);

namespace libv { // --------------------------------------------------------------------------------

struct CloudObjectKeyHasher {
	using is_transparent = void;

public:
	[[nodiscard]] LIBV_FORCE_INLINE std::size_t operator()(const CloudObjectKey& var) const noexcept {
		return std::hash<CloudObjectKey>{}(var);
	}
	[[nodiscard]] LIBV_FORCE_INLINE std::size_t operator()(const CloudObjectKeyView& var) const noexcept {
		return std::hash<CloudObjectKeyView>{}(var);
	}
};

// -------------------------------------------------------------------------------------------------

struct ImplCloud {
	using key_type = CloudObjectKey;

	std::mutex mutex;
	std::unordered_map<key_type, Cloud::object_ptr, CloudObjectKeyHasher, std::equal_to<>> objects;
};

// =================================================================================================

Cloud::Cloud() :
	self(std::make_shared<ImplCloud>()) { }

void Cloud::aux_set(Cloud::type_uid object_type, Cloud::object_ptr ptr) {
	auto lock = std::unique_lock(self->mutex);
	self->objects.insert_or_assign(CloudObjectKey{"", object_type}, ptr);
}

void Cloud::aux_set(std::string name, Cloud::type_uid object_type, Cloud::object_ptr ptr) {
	auto lock = std::unique_lock(self->mutex);
	self->objects.insert_or_assign(CloudObjectKey{std::move(name), object_type}, ptr);
}

Cloud::object_ptr Cloud::aux_get(Cloud::type_uid object_type) const noexcept {
	auto lock = std::unique_lock(self->mutex);
	const auto it = self->objects.find(CloudObjectKeyView{"", object_type});
	return it == self->objects.end() ? nullptr : it->second;
}

Cloud::object_ptr Cloud::aux_get(const std::string_view name, Cloud::type_uid object_type) const noexcept {
	auto lock = std::unique_lock(self->mutex);
	const auto it = self->objects.find(CloudObjectKeyView{name, object_type});
	return it == self->objects.end() ? nullptr : it->second;
}

void Cloud::aux_remove(Cloud::type_uid object_type) const noexcept {
	auto lock = std::unique_lock(self->mutex);
	self->objects.erase(CloudObjectKey{"", object_type});
}

void Cloud::aux_remove(const std::string_view name, Cloud::type_uid object_type) const noexcept {
	auto lock = std::unique_lock(self->mutex);
	const auto it = self->objects.find(CloudObjectKeyView{name, object_type});
	if (it != self->objects.end())
		self->objects.erase(it);
}

std::size_t Cloud::num_object() const noexcept {
	auto lock = std::unique_lock(self->mutex);
	return self->objects.size();
}

// -------------------------------------------------------------------------------------------------

} // namespace libv

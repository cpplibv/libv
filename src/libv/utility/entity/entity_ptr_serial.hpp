// Created by Vader on 2021.12.20..

#pragma once

// hpp
#include <libv/utility/entity/entity_ptr.hpp>
// std
#include <vector>
// libv
#include <libv/serial/size_tag.hpp>


namespace libv {

// -------------------------------------------------------------------------------------------------

template <typename T>
struct entity_primary_owner {
	libv::entity_store<T>& store;
	std::vector<libv::entity_ptr<T>>& container;

	template <typename Archive>
	inline void save(Archive& ar) const {
		ar(libv::serial::make_size_tag(static_cast<libv::serial::size_type>(container.size()))); // number of elements

		for (const auto& v : container)
			ar(*v);
	}

	template <typename Archive>
	inline void load(Archive& ar) {
		libv::serial::size_type size;
		ar(libv::serial::make_size_tag(size));
		container.resize(static_cast<std::size_t>(size));

		for (auto& v : container) {
			auto ptr = store.create();
			ar(*ptr);
			v = std::move(ptr);
		}
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace libv

// File: Model.cpp, Created on 2015. január 17. 7:21, Author: Vader

// hpp
#include <vl/vm3/serialization/model.hpp>
// ext
#include <boost/archive/portable_iarchive.hpp>
#include <boost/archive/portable_oarchive.hpp>
// std
#include <iostream>

namespace vl {
	namespace vm3 {

		Model::Model() : name("--unknown--") { }

		Model::Model(std::istream& is) {
			load(is);
		}

		bool Model::load(std::istream& is) {
			eos::portable_iarchive ar(is);
			ar >> VL_NVP_NAMED("model", *this);
			return true; //<<< Model load fail
		}
		
		bool Model::load(std::istream&& is) {
			return load(is);
		}

		bool Model::save(std::ostream& os) const {
			eos::portable_oarchive ar(os);
			ar << VL_NVP_NAMED("model", *this);
			return true; //<<< Model save fail
		}
		
		bool Model::save(std::ostream&& os) const {
			return save(os);
		}

	} //namespace gl
} //namespace vlib

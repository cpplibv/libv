// File: mesh.hpp - Created on 2017.11.25. 05:06 - Author: Vader

#pragma once

// ext
#include <boost/container/small_vector.hpp>
// libv
#include <libv/algorithm/associative.hpp>
#include <libv/algorithm/container.hpp>
#include <libv/gl/array_buffer_object.hpp>
#include <libv/gl/enum.hpp>
#include <libv/gl/vertex_array_object.hpp>
#include <libv/math/vec.hpp>
#include <libv/utility/observer_ptr.hpp>
#include <libv/utility/span.hpp>
// std
#include <memory>
#include <vector>


namespace libv {
namespace gl {

class GL;

} // namespace gl
namespace glr {

// -------------------------------------------------------------------------------------------------

using VertexIndex = uint32_t;
using AttributeChannel = int32_t;
struct Remote;
struct RemoteMesh;

// -------------------------------------------------------------------------------------------------

//struct SubMesh {
//	Mesh mesh;
//	uint32_t start;
//	uint32_t count;
//};

// -------------------------------------------------------------------------------------------------

class RemoteMeshAttribute {
public:
	libv::gl::ArrayBuffer buffer;
	AttributeChannel channel;
	libv::gl::AttributeType type;
	int dim;
	std::vector<std::byte> data;

public:
	RemoteMeshAttribute(AttributeChannel channel, libv::gl::AttributeType type, int dim) :
		channel(channel), type(type), dim(dim) { }

public:
	[[nodiscard]] inline bool operator==(AttributeChannel other) const noexcept {
		return channel == other;
	}

public:
	template <typename T>
	inline size_t size() const {
		return data.size() / sizeof(T);
	}

public:
	template <typename T>
	inline void reserve(size_t count) {
		data.reserve(sizeof(T) * count);
	}

	template <typename T>
	inline void resize(size_t count) {
		data.resize(sizeof(T) * count);
	}

	template <typename T>
	inline void clear() {
		data.clear();
	}

	template <typename T>
	inline void push_back(const T value) {
		const auto size = data.size();

		data.resize(size + sizeof(T));
		std::memcpy(data.data() + size, &value, sizeof(T));
	}

	template <typename T>
	inline void push_back_all(const libv::span<const T> values) {
		const auto size = data.size();
		const auto count = values.size();

		data.resize(size + sizeof(T) * count);
		std::memcpy(data.data() + size, values.data(), sizeof(T) * count);
	}

	template <typename T>
	inline T& at(size_t index) {
		return reinterpret_cast<T&>(data[sizeof(T) * index]);
	}
};
using RemoteMeshAttributes = boost::container::small_vector<RemoteMeshAttribute, 4>;

struct RemoteMeshIndecies {
	libv::gl::ArrayBuffer buffer;
	std::vector<VertexIndex> data;
};

struct RemoteMesh {
	bool dirty = false;
	bool created = false;

	//uint32_t ref_count = 0;

	VertexIndex start = 0;
	VertexIndex count = 0;

	libv::gl::VertexArray vao;
	RemoteMeshIndecies indices;
	RemoteMeshAttributes attributes;

	libv::gl::Primitive primitive = libv::gl::Primitive::Triangles;
	libv::gl::BufferUsage usage = libv::gl::BufferUsage::StaticDraw;

	/// Can be nullptr if the object is not yet associated with a remote OpenGL context
	libv::observer_ptr<Remote> remote;

private:
	void create(libv::gl::GL& gl, Remote& remote_) noexcept;
	void update(libv::gl::GL& gl, Remote& remote_) noexcept;

public:
	void render(libv::gl::GL& gl, Remote& remote_) noexcept;

	inline RemoteMesh(libv::gl::Primitive primitive, libv::gl::BufferUsage usage) noexcept :
		primitive(primitive),
		usage(usage) { }

	~RemoteMesh() noexcept;
};

// -------------------------------------------------------------------------------------------------

template <typename T, typename CRTP>
class BaseMeshAttribute {
public:
	using value_type = T;

private:
	RemoteMeshAttributes& ptr;
	size_t ptr_index;

public:
	BaseMeshAttribute(RemoteMeshAttributes& ptr, size_t index) :
		ptr(ptr),
		ptr_index(index) { }

public:
	inline CRTP& operator()(const T value) {
		ptr[ptr_index].push_back<T>(value);
		return static_cast<CRTP&>(*this);
	}
	inline CRTP& operator()(const libv::span<const T> values) {
		ptr[ptr_index].push_back_all<T>(values);
		return static_cast<CRTP&>(*this);
	}

	inline T& operator[](const size_t index) {
		return ptr[ptr_index].at<T>(index);
	}
	inline const T& operator[](const size_t index) const {
		return ptr[ptr_index].at<T>(index);
	}

public:
	inline size_t size() const {
		return ptr[ptr_index].size<T>();
	}
	inline void reserve(const size_t count) {
		ptr[ptr_index].reserve<T>(count);
	}
	inline void resize(const size_t count) {
		ptr[ptr_index].resize<T>(count);
	}
	inline void clear() {
		ptr[ptr_index].clear<T>();
	}
};

template <typename T>
struct MeshAttribute : BaseMeshAttribute<T, MeshAttribute<T>> {
	using BaseMeshAttribute<T, MeshAttribute<T>>::BaseMeshAttribute;
};

template <typename T>
struct MeshAttribute<libv::vec_t<1, T>> : BaseMeshAttribute<libv::vec_t<1, T>, MeshAttribute<libv::vec_t<1, T>>> {
	using BaseMeshAttribute<libv::vec_t<1, T>, MeshAttribute<libv::vec_t<1, T>>>::BaseMeshAttribute;
	using BaseMeshAttribute<libv::vec_t<1, T>, MeshAttribute<libv::vec_t<1, T>>>::operator();

	MeshAttribute& operator()(const T x) {
		return (*this)(libv::vec_t<1, T>{x});
	}
};

template <typename T>
struct MeshAttribute<libv::vec_t<2, T>> : BaseMeshAttribute<libv::vec_t<2, T>, MeshAttribute<libv::vec_t<2, T>>> {
	using BaseMeshAttribute<libv::vec_t<2, T>, MeshAttribute<libv::vec_t<2, T>>>::BaseMeshAttribute;
	using BaseMeshAttribute<libv::vec_t<2, T>, MeshAttribute<libv::vec_t<2, T>>>::operator();

	MeshAttribute& operator()(const T x, const T y) {
		return (*this)(libv::vec_t<2, T>{x, y});
	}
};

template <typename T>
struct MeshAttribute<libv::vec_t<3, T>> : BaseMeshAttribute<libv::vec_t<3, T>, MeshAttribute<libv::vec_t<3, T>>> {
	using BaseMeshAttribute<libv::vec_t<3, T>, MeshAttribute<libv::vec_t<3, T>>>::BaseMeshAttribute;
	using BaseMeshAttribute<libv::vec_t<3, T>, MeshAttribute<libv::vec_t<3, T>>>::operator();

	MeshAttribute& operator()(const T x, const T y, const T z) {
		return (*this)(libv::vec_t<3, T>{x, y, z});
	}
};

template <typename T>
struct MeshAttribute<libv::vec_t<4, T>> : BaseMeshAttribute<libv::vec_t<4, T>, MeshAttribute<libv::vec_t<4, T>>> {
	using BaseMeshAttribute<libv::vec_t<4, T>, MeshAttribute<libv::vec_t<4, T>>>::BaseMeshAttribute;
	using BaseMeshAttribute<libv::vec_t<4, T>, MeshAttribute<libv::vec_t<4, T>>>::operator();

	MeshAttribute& operator()(const T x, const T y, const T z, const T w) {
		return (*this)(libv::vec_t<4, T>{x, y, z, w});
	}
};

// -------------------------------------------------------------------------------------------------

class MeshIndices {
public:
	using value_type = VertexIndex;

private:
	RemoteMeshIndecies& ref;

public:
	MeshIndices(RemoteMeshIndecies& ref) : ref(ref) { }

public:
	inline MeshIndices& operator()(const VertexIndex i0) {
		ref.data.emplace_back(i0);
		return *this;
	}
	inline MeshIndices& operator()(const std::initializer_list<const VertexIndex> indices) {
		libv::insert_all(ref.data, indices);
		return *this;
	}
	inline MeshIndices& operator()(const libv::span<const VertexIndex> indices) {
		libv::insert_all(ref.data, indices);
		return *this;
	}

	inline MeshIndices& point(const VertexIndex i0) {
		ref.data.emplace_back(i0);
		return *this;
	}
	inline MeshIndices& line(const VertexIndex i0, const VertexIndex i1) {
		ref.data.emplace_back(i0);
		ref.data.emplace_back(i1);
		return *this;
	}
	inline MeshIndices& triangle(const VertexIndex i0, const VertexIndex i1, const VertexIndex i2) {
		ref.data.emplace_back(i0);
		ref.data.emplace_back(i1);
		ref.data.emplace_back(i2);
		return *this;
	}
	inline MeshIndices& quad(const VertexIndex i0, const VertexIndex i1, const VertexIndex i2, const VertexIndex i3) {
		ref.data.emplace_back(i0);
		ref.data.emplace_back(i1);
		ref.data.emplace_back(i2);
		ref.data.emplace_back(i2);
		ref.data.emplace_back(i3);
		ref.data.emplace_back(i0);
		return *this;
	}

public:
	inline libv::span<VertexIndex> view_last(const size_t count) {
		return {
				ref.data.data() + ref.data.size() - count,
				static_cast<libv::span<VertexIndex>::index_type>(count)
		};
	}

	inline VertexIndex& operator[](const size_t index) {
		return ref.data[index];
	}
	inline const VertexIndex& operator[](const size_t index) const {
		return ref.data[index];
	}

public:
	inline size_t size() const {
		return ref.data.size();
	}

public:
	inline void reserve(const size_t count) {
		ref.data.reserve(count);
	}

	inline void resize(const size_t count) {
		ref.data.resize(count);
	}

	inline void clear() {
		ref.data.clear();
	}
};

// -------------------------------------------------------------------------------------------------

class Queue;
class AttorneyMeshRemote;

class Mesh {
	friend class AttorneyMeshRemote;

private:
	std::shared_ptr<RemoteMesh> remote;

public:
	Mesh(libv::gl::Primitive primitive, libv::gl::BufferUsage usage) :
		remote(std::make_shared<RemoteMesh>(primitive, usage)) {
	}
	Mesh(const Mesh& orig) = default;
	Mesh(Mesh&& orig) = default;

public:
	template <typename T>
	[[nodiscard]] MeshAttribute<typename T::value_type> attribute(T) {
		const auto opt = libv::linear_find_optional(remote->attributes, T::channel);
		if (!opt) {
			using attribute_type = typename T::value_type;
			using underlying_type = typename attribute_type::value_type;

			static_assert(std::is_integral_v<underlying_type>
					|| std::is_same_v<underlying_type, float>
					|| std::is_same_v<underlying_type, double>,
					"bindAttribute underlying_type template parameter not supported. Expected float, double or integral");

			remote->attributes.emplace_back(
					T::channel,
					libv::gl::toAttributeType<underlying_type>(),
					attribute_type::dim);
		}

		remote->dirty = true;
		return MeshAttribute<typename T::value_type>{remote->attributes, remote->attributes.size() - 1};
	}

	[[nodiscard]] MeshIndices index() {
		remote->dirty = true;
		return MeshIndices{remote->indices};
	}

	void reserve(size_t vertex_count, size_t index_count) {
		for (RemoteMeshAttribute& attribute : remote->attributes)
			attribute.data.reserve(vertex_count);

		remote->indices.data.reserve(index_count);
	}

	void resize(size_t vertex_count, size_t index_count) {
		for (RemoteMeshAttribute& attribute : remote->attributes)
			attribute.data.resize(vertex_count);

		remote->indices.data.resize(index_count);
		remote->dirty = true;
	}
};

struct AttorneyMeshRemote {
	friend class Queue;
	inline static RemoteMesh& remote(const Mesh& mesh) {
		return *mesh.remote;
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace glr
} // namespace libv

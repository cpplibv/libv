// Project: libv.glr, File: src/libv/glr/mesh.hpp

#pragma once

// ext
#include <boost/align/aligned_allocator.hpp>
#include <boost/container/small_vector.hpp>
// libv
#include <libv/algo/container.hpp>
#include <libv/algo/linear_find.hpp>
#include <libv/gl/array_buffer_object.hpp>
#include <libv/gl/enum.hpp>
#include <libv/gl/gl_fwd.hpp>
#include <libv/gl/vertex_array_object.hpp>
#include <libv/math/vec.hpp>
#include <libv/utility/memory/observer_ptr.hpp>
// std
#include <memory>
#include <span>
#include <vector>


namespace libv {
namespace glr {

// -------------------------------------------------------------------------------------------------

using VertexIndex = uint32_t;
using AttributeChannel = uint32_t;
struct DestroyQueues;
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
	std::vector<std::byte, boost::alignment::aligned_allocator<std::byte, 32>> data_;

public:
	RemoteMeshAttribute(AttributeChannel channel, libv::gl::AttributeType type, int dim) :
		channel(channel), type(type), dim(dim) { }

public:
	template <typename T>
	inline T* data() {
		return reinterpret_cast<T*>(reinterpret_cast<void*>(data_.data()));
	}

	template <typename T>
	inline const T* data() const {
		return reinterpret_cast<const T*>(reinterpret_cast<const void*>(data_.data()));
	}

	template <typename T>
	inline std::size_t size() const {
		return data_.size() / sizeof(T);
	}

public:
	template <typename T>
	inline void reserve(std::size_t count) {
		data_.reserve(sizeof(T) * count);
	}

	template <typename T>
	inline void resize(std::size_t count) {
		data_.resize(sizeof(T) * count);
	}

	template <typename T>
	inline void push_back(const T value) {
		const auto size = data_.size();

		data_.resize(size + sizeof(T));
		std::memcpy(data_.data() + size, &value, sizeof(T));
	}

	template <typename T>
	inline T& at(std::size_t index) {
		return *(data<T>() + index);
	}
};
using RemoteMeshAttributes = boost::container::small_vector<RemoteMeshAttribute, 4>;

struct RemoteMeshIndices {
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
	RemoteMeshIndices indices;
	RemoteMeshAttributes attributes;

	libv::gl::Primitive primitive = libv::gl::Primitive::Triangles;
	libv::gl::BufferUsage usage = libv::gl::BufferUsage::StaticDraw;

	libv::observer_ptr<DestroyQueues> remote = nullptr;

private:
	void create(libv::gl::GL& gl, Remote& remote_) noexcept;
	void update(libv::gl::GL& gl, Remote& remote_) noexcept;

public:
	void render(libv::gl::GL& gl, Remote& remote_) noexcept;
	void render(libv::gl::GL& gl, Remote& remote_, VertexIndex baseVertex, VertexIndex baseIndex, VertexIndex numIndices) noexcept;

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
	std::size_t ptr_index;

public:
	BaseMeshAttribute(RemoteMeshAttributes& ptr, std::size_t index) :
		ptr(ptr),
		ptr_index(index) { }

public:
	inline CRTP& operator()(const T value) {
		ptr[ptr_index].push_back<T>(value);
		return static_cast<CRTP&>(*this);
	}
	inline CRTP& operator()(const std::span<const T> values) {
		this->append_from_range(values);
		return static_cast<CRTP&>(*this);
	}

	inline CRTP& set_from_range(const std::span<const value_type> range) {
		ptr[ptr_index].resize<T>(range.size());
		void* dst = ptr[ptr_index].data<T>();
		const void* src = range.data();
		std::memcpy(dst, src, range.size() * sizeof(value_type));
		return static_cast<CRTP&>(*this);
	}

	inline CRTP& append_from_range(const std::span<const value_type> range) {
		const auto originalSize = ptr[ptr_index].size<T>();
		ptr[ptr_index].resize<T>(originalSize + range.size());
		void* dst = ptr[ptr_index].data<T>() + originalSize;
		const void* src = range.data();
		std::memcpy(dst, src, range.size() * sizeof(value_type));
		return static_cast<CRTP&>(*this);
	}

	inline CRTP& append_n_times(std::size_t count, const value_type value) {
		const auto originalSize = ptr[ptr_index].size<T>();
		ptr[ptr_index].resize<T>(originalSize + count);
		T* dst = ptr[ptr_index].data<T>() + originalSize;
		const T* end = dst + count;
		for (; dst != end; ++dst)
			*dst = value;
		return static_cast<CRTP&>(*this);
	}

	inline T& operator[](const std::size_t index) {
		return ptr[ptr_index].at<T>(index);
	}
	inline const T& operator[](const std::size_t index) const {
		return ptr[ptr_index].at<T>(index);
	}

public:
	inline std::span<T> view_last(const std::size_t count) {
		return {
				ptr[ptr_index].data<T>() + ptr[ptr_index].size<T>() - count,
				static_cast<typename std::span<T>::index_type>(count)
		};
	}

public:
	inline std::size_t size() const {
		return ptr[ptr_index].size<T>();
	}
	inline void reserve(const std::size_t count) {
		ptr[ptr_index].reserve<T>(count);
	}
	inline void resize(const std::size_t count) {
		ptr[ptr_index].resize<T>(count);
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
	RemoteMeshIndices& ref;

public:
	explicit MeshIndices(RemoteMeshIndices& ref) : ref(ref) { }

public:
	inline MeshIndices& operator()(const VertexIndex i0) {
		ref.data.emplace_back(i0);
		return *this;
	}
	inline MeshIndices& operator()(const std::initializer_list<const VertexIndex> indices) {
		libv::insert_all(ref.data, indices);
		return *this;
	}
	inline MeshIndices& operator()(const std::span<const VertexIndex> indices) {
		this->append_from_range(indices);
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

	inline MeshIndices& set_from_range(const std::span<const value_type> range) {
		ref.data.resize(range.size());
		std::memcpy(ref.data.data(), range.data(), range.size() * sizeof(value_type));
		return *this;
	}

	inline MeshIndices& append_from_range(const std::span<const value_type> range) {
		const auto originalSize = ref.data.size();
		ref.data.resize(originalSize + range.size());
		std::memcpy(ref.data.data() + originalSize, range.data(), range.size() * sizeof(value_type));
		return *this;
	}

public:
	inline std::span<VertexIndex> view_last(const std::size_t count) {
		return {
				ref.data.data() + ref.data.size() - count,
				static_cast<std::span<VertexIndex>::size_type>(count)
		};
	}

	inline VertexIndex& operator[](const std::size_t index) {
		return ref.data[index];
	}
	inline const VertexIndex& operator[](const std::size_t index) const {
		return ref.data[index];
	}

public:
	inline std::size_t size() const {
		return ref.data.size();
	}

public:
	inline void reserve(const std::size_t count) {
		ref.data.reserve(count);
	}

	inline void resize(const std::size_t count) {
		ref.data.resize(count);
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
	inline Mesh(const Mesh&) noexcept = default;
	inline Mesh& operator=(const Mesh&) & noexcept = default;
	inline Mesh(Mesh&&) noexcept = default;
	inline Mesh& operator=(Mesh&&) & noexcept = default;

public:
	template <typename T>
	[[nodiscard]] MeshAttribute<typename T::value_type> attribute(T) {
		using attribute_type = typename T::value_type;
		using underlying_type = typename T::underlying_type;

		remote->dirty = true;
		auto it = libv::linear_find_iterator(remote->attributes, T::channel, &RemoteMeshAttribute::channel);

		if (it != remote->attributes.end())
			return MeshAttribute<attribute_type>{remote->attributes, static_cast<std::size_t>(std::distance(remote->attributes.begin(), it))};

		static_assert(std::is_integral_v<underlying_type>
				|| std::is_same_v<underlying_type, float>
				|| std::is_same_v<underlying_type, double>,
				"bindAttribute underlying_type template parameter not supported. Expected float, double or integral");

		remote->attributes.emplace_back(
				T::channel,
				libv::gl::toAttributeType<underlying_type>(),
				T::dim);

		return MeshAttribute<attribute_type>{remote->attributes, remote->attributes.size() - 1};
	}

	[[nodiscard]] MeshIndices index() {
		remote->dirty = true;
		return MeshIndices{remote->indices};
	}

	[[nodiscard]] std::size_t index_size() const noexcept {
		return remote->indices.data.size();
	}

	void clear() {
		// TODO P5: Think about if replacing is the best way to clear
		remote = std::make_shared<RemoteMesh>(remote->primitive, remote->usage);
	}

	void reserve(std::size_t vertex_count, std::size_t index_count) {
		for (RemoteMeshAttribute& attribute : remote->attributes)
			attribute.data_.reserve(vertex_count);

		remote->indices.data.reserve(index_count);
	}

	void resize(std::size_t vertex_count, std::size_t index_count) {
		for (RemoteMeshAttribute& attribute : remote->attributes)
			attribute.data_.resize(vertex_count);

		remote->indices.data.resize(index_count);
		remote->dirty = true;
	}
};

struct AttorneyMeshRemote {
	friend class Queue;
	static inline const std::shared_ptr<RemoteMesh>& remote(const Mesh& mesh) {
		return mesh.remote;
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace glr
} // namespace libv

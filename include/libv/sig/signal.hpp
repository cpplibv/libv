// File: Signal.hpp, Created on 2014. május 14. 20:05, Author: Vader

#pragma once

// TODO P0: Before merging to libv care about that capacitive signal changed multithreading behavior
// TODO P1: change const SignalOutput& orig -> to SignalOutput orig
// TODO P1: Use max history
// TODO P2: Branch signals into two: sync and async signals, this will ease my pain about them...
// TODO P2: review std::forward
// TODO P2: Add "Signal Hub" approach where signals are light weight and a main hub stores
//			everything and member signal "markers" (or what should i call them...) are just an
//			accessors to the hub wrapping the necessary minimal information.
//			SignalHub is a trackable
//			Both method should be kept
// TODO P4: HistorySignal:
//				- Dynamic history size
//				- Circular buffer
//				- Output auto-flush thread safety
// TODO P5: HistorySignal: multithread garantiee no slipping by events

// TODO P5: PrioritySignal - Modified capacitive where the storage is a priority que
// 			May consider a "predicate" function for generating priority
// TODO P5: PriorityOutSignal - The outputs has a priority which will be used on broadcast
//			Similar to routing as it stores addition parameter with outputs
// TODO P5: UniqueSignal - Modified capacitive where the storage is unique
// 			May consider a "compare" function for determining uniqueness
//			May consider merging and collapsing awaiting events buy a new one
//			This would be through some template extensibility...
// TODO P5: Merger signal - Stores last, compares and dont fire with same
//			In other name "sync unique signal"

// TODO P5: Another raw idea for light weight signals: SignalPtr which is only a pointer to the real
//			signal. It could also store some additional information such as FilterSignalPtr could
//			store the filter information that will be applied to the output. (Component::onClick
//			could be a FilterSignalPtr that points to the UI signal and stores the boundaries)
//			Or would a Signal Tunnel be a better name?

// NAME: Hub: SignalHub, SignalHost
// NAME: Proxy: SignalProxy, SignalMarker

#include <functional>
#include <memory>
#include <mutex>
#include <vector>

#include "module.hpp"
#include "thread_policy.hpp"
#include "trackable.hpp"
#include "type_traits.hpp"


namespace libv {
namespace sig {

// -------------------------------------------------------------------------------------------------

template <typename Signal>
struct Connection {
	libv::observer_ptr<Signal> signal;
};

template <typename Signature, typename Accumulator, typename ThreadPolicy>
struct Signal;

template <typename R, typename Args..., typename AccumulatorT = Accumulator<R>, typename ThreadPolicy = SingleThread>
struct Signal<R(Args...), AccumulatorT, ThreadPolicy> : private ThreadPolicy {
	struct Entry {
		libv::observer_ptr<Connection> connection;
		std::function<R(Args...)> func;
	};

	std::vector<Entry> connections;

	inline typename AccumulatorT::result_type fire(Args... args) {
		auto lock = read_lock_guard(static_cast<ThreadPolicy&>(*this));
//		return this->fireImpl(std::forward<Args>(args)...);
	}

	template <typename F>
	inline auto connect(Connection<Signal>& slot, F) {
		auto lock = read_lock_guard(static_cast<ThreadPolicy&>(*this));
//		return this->fireImpl(std::forward<Args>(args)...);
	}

//	inline typename base_type::result_type operator()(Args... args) {
//		return fire(std::forward<Args>(args)...);
//	}
};


// -------------------------------------------------------------------------------------------------

// --- Output Implementations ---
template <typename Signature>
struct SignalOutput;

template <typename RType, typename... Args>
struct SignalOutput<RType(Args...)> {
	template <typename F>
	using is_acceptable_func = decltype(std::function<RType(Args...)>(std::declval<F>()));

	std::unique_ptr<detail::TrackableConnectionBase> con;
	std::function<RType(Args...)> func;

	template <typename TP>
	static std::unique_ptr<detail::TrackableConnectionBase> makeConnection(TrackableThread<TP>& t) {
		return std::make_unique<detail::TrackableConnectionTrackable<TP>>(
				TrackableAccessor::getTrackingPoint(t));
	}
	static std::unique_ptr<detail::TrackableConnectionBase> makeConnection() {
		return std::make_unique<detail::TrackableConnectionNoTrackable>();
	}
	static std::unique_ptr<detail::TrackableConnectionBase> makeConnection(const std::weak_ptr<void>& wp) {
		return std::make_unique<detail::TrackableConnectionWeak>(wp);
	}

	SignalOutput(SignalOutput&& orig) = default;
	SignalOutput(const SignalOutput& orig) = default;
	SignalOutput& operator=(SignalOutput&& orig) = default;
	SignalOutput& operator=(const SignalOutput& orig) = default;

	template <typename Func, typename = is_acceptable_func<Func>>
	explicit SignalOutput(Func&& func) :
		con(makeConnection()),
		func(std::forward<Func>(func)) {
	}
	template <typename TP, typename Func, typename = is_acceptable_func<Func>>
	SignalOutput(TrackableThread<TP>& obj, Func&& func) :
		con(makeConnection(obj)),
		func(std::forward<Func>(func)) {
	}
	template <typename Derived, typename Object = Derived>
	explicit SignalOutput(Derived& obj, RType(Object::*memberFunc)(Args...) = &Derived::operator()) :
		con(makeConnection(obj)),
		func([&obj, memberFunc](Args... args) {
			return (obj.*memberFunc)(std::forward<Args>(args)...);
		}) {
		static_assert(std::is_base_of<TrackableBase, Object>::value,
				"Object type has to be Derived from TrackableBase "
				"(You may want to consider inheriting from libv::Trackable).");
		static_assert(std::is_base_of<Object, Derived>::value,
				"Member function has to be the derived member's function as Object");
	}
	template <typename T, typename Func, typename = is_acceptable_func<Func>>
	SignalOutput(std::shared_ptr<T>& tracker, Func&& func) :
		con(makeConnection(std::weak_ptr<void>(tracker))),
		func(std::forward<Func>(func)) { }

	template <typename Derived, typename Object = Derived>
	explicit SignalOutput(std::shared_ptr<Derived>& obj, RType(Object::*memberFunc)(Args...) = &Derived::operator()) :
		con(makeConnection(obj)),
		func([raw_ptr = obj.get(), memberFunc](Args... args){
				return (raw_ptr->*memberFunc)(std::forward<Args>(args)...);
		}) { }

	void swap(SignalOutput& other) {
		std::swap(con, other.con);
		std::swap(func, other.func);
	}
};

// =================================================================================================

//template <typename...>
//class SignalBaseImpl;
//
//template <typename R, typename... Args>
//struct module_filter<SignalBaseImpl, R(Args...)> {
//	using default_modules = list<
//			AccumulatorSum<R>,
//			SingleThread>;
//	using parameter_tags = list<
//			tag::accumulator,
//			tag::thread_policy>;
//};

//template <typename Signature, typename Accumulator, typename ThreadPolicy>
//struct Signal;
//
//template <typename R, typename Args..., typename AccumulatorT = Accumulator<R>, typename ThreadPolicy = SingleThread>
//struct Signal<R(Args...), AccumulatorT, ThreadPolicy> : private ThreadPolicy {

// -------------------------------------------------------------------------------------------------

template <typename...>
class SignalBaseImpl;

template <typename RType, typename... Args, typename Accumulator, typename ThreadPolicy>
class SignalBaseImpl<RType(Args...), Accumulator, ThreadPolicy>
		: public TrackableThread<ThreadPolicy> {
public:
	using this_type = SignalBaseImpl<RType(Args...), Accumulator, ThreadPolicy>;

	using accumulator = accumulator_traits<Accumulator>;
	using thread_policy = ThreadPolicy;
	using output_type = SignalOutput<RType(Args...)>;

	using result_type = typename accumulator::result_type;

	using module = tag_type<tag::signal>;

protected:
	template <typename... OArgs>
	using is_acceptable_output = decltype(output_type(std::declval<OArgs>()...));

	mutable thread_policy mutex;
	// ?This thread policy is for the signal itself, not for connections

	std::vector<output_type> outputs;

	// ---------------------------------------------------------------------------------------------
public:
	SignalBaseImpl() = default;
	SignalBaseImpl(const this_type& other) = delete;

	// ---------------------------------------------------------------------------------------------
protected:
	result_type fireImpl(Args... args) {
		size_t i = 0;
		auto size = outputs.size();
		auto acc = accumulator::create();
		while (i < size) {
			auto& output = outputs[i];
			auto connectionAlive = output.con->readLock();
			auto connectionKeepAliveLock = make_read_lock_guard(*output.con, adopt_lock);

			if (connectionAlive) {
				if (accumulator::add(acc, output.func, std::forward<Args>(args)...))
					break;
				i++;
			} else {
				--size;
				outputs[i].swap(outputs[size]);
			}
		}
		outputs.erase(outputs.begin() + size, outputs.end());
		return accumulator::result(acc);
	}

	// ---------------------------------------------------------------------------------------------
public:
	inline size_t outputSize() const {
		auto lock = make_read_lock_guard(this->mutex);
		return outputs.size();
	}

	// ---------------------------------------------------------------------------------------------

	template <typename... OArgs, typename = is_acceptable_output<OArgs...>>
	inline void output(OArgs&&... args){
		auto lock = make_write_lock_guard(this->mutex);
		outputs.emplace_back(std::forward<OArgs>(args)...);
	}
};

// === Signal ======================================================================================
// === CapacitiveSignal ============================================================================

template <typename...>
class CapacitiveSignalImpl;

template <typename R, typename... Args>
struct module_filter<CapacitiveSignalImpl, R(Args...)> {
	using base_type = module_filter<SignalBaseImpl, R(Args...)>;
};

template <typename RType, typename... Args, typename... Modules>
class CapacitiveSignalImpl<RType(Args...), Modules...>
		: public SignalBaseImpl<RType(Args...), Modules...> {
public:
	using base_type = SignalBaseImpl<RType(Args...), Modules...>;
	using this_type = CapacitiveSignalImpl<RType(Args...), Modules...>;

private:
	std::vector<std::tuple<std::remove_reference_t<Args>...>> argQue;
private:
	template <std::size_t... Is>
	inline void flushHelper(std::index_sequence<Is...>) {
		for (auto& item : argQue) {
			this->fireImpl(std::get<Is>(item)...);
		}
		argQue.clear();
	}
public:
	inline void fire(Args... args) {
		auto lock = make_write_lock_guard(this->mutex);
		argQue.emplace_back(args...);
	}
	inline void operator()(Args... args) {
		fire(std::forward<Args>(args)...);
	}
	inline void flush() {
		auto lock = make_write_lock_guard(this->mutex);
		flushHelper(std::index_sequence_for < Args...>{});
	}
public:
	virtual ~CapacitiveSignalImpl() {
		this->disconnect();
	}
};

// === HistorySignal ===============================================================================

template <typename...>
class HistorySignalImpl;

template <typename R, typename... Args>
struct module_filter<HistorySignalImpl, R(Args...)> {
	using base_type = module_filter<SignalBaseImpl, R(Args...)>;

	using default_modules = list<history_size<0>>;
	using parameter_tags = list<tag::history_size>;
};

template <typename RType, typename... Args, typename HistorySize, typename... Modules>
class HistorySignalImpl<RType(Args...), HistorySize, Modules...>
		: public SignalBaseImpl<RType(Args...), Modules...> {
public:
	using base_type = SignalBaseImpl<RType(Args...), Modules...>;
	using this_type = HistorySignalImpl<RType(Args...), HistorySize, Modules...>;

	using history_size = HistorySize;
	static constexpr size_t historySizeMax = history_size::value;
private:
	std::vector<std::tuple<typename std::remove_reference<Args>::type...>> history;
private:
	template <typename F, std::size_t... Is>
	inline void flushHelper(F&& func, std::index_sequence<Is...>) {
		for (auto& item : history) {
			func(std::get<Is>(item)...);
		}
	}
public:
	template <typename... OArgs, typename = typename base_type::template is_acceptable_output<OArgs...>>
	inline void output(OArgs&&... args) {
		typename base_type::output_type out(std::forward<OArgs>(args)...);
		{
			auto con_lock = make_read_lock_guard(*out.con);
			flushHelper(out.func, std::index_sequence_for<Args...>{});
		}
		base_type::output(std::move(out));
	}
public:
	inline size_t historySize() const {
		auto lock = make_read_lock_guard(this->mutex);
		return history.size();
	}
	inline size_t historyMax() const {
		return historySizeMax;
	}
	inline void fire(Args... args) {
		auto lock = make_read_lock_guard(this->mutex);
		history.emplace_back(args...);
		this->fireImpl(args...);
	}
	inline void operator()(Args... args) {
		fire(std::forward<Args>(args)...);
	}
	inline void clearHistory() {
		auto lock = make_write_lock_guard(this->mutex);
		history.clear();
	}
public:
	virtual ~HistorySignalImpl() {
		this->disconnect();
	}
};

// === Aliases =====================================================================================

//template <typename... Args>
//using Signal = signal_alias_t<SignalImpl, Args...>;
//
//template <typename... Args>
//using CapacitiveSignal = signal_alias_t<CapacitiveSignalImpl, Args...>;
//
//template <typename... Args>
//using HistorySignal = signal_alias_t<HistorySignalImpl, Args...>;

// -------------------------------------------------------------------------------------------------

} // namespace sig
} //namespace libv

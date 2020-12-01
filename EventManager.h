#pragma once

#include <cstdint>
#include <vector>
#include <functional>


typedef std::size_t EventID;


struct CallbackContainerBase {};
	
template<typename T>
struct CallbackContainer : CallbackContainerBase
{
	std::vector<std::function<void(T*)>> callbacks;
};


class EventManager
{
public:
	template<typename T, typename T_Function>
	static void listen(T_Function callback);

	template<typename T, typename T_Instance, typename T_Function>
	static void listen(T_Instance& instance, T_Function callback);

	template<typename T, typename T_Instance, typename T_Function>
	static void listen(T_Instance* instance, T_Function callback);

	template<typename T, typename... T_Args>
	static void fire(T_Args...args);

private:
	template<typename T>
	static auto get_event_id()->EventID;

	template<typename T>
	static auto register_event()->EventID;

	static auto get_next_event(EventID id)->std::uint32_t;


private:
	static inline const std::uint32_t s_event_pool_size = 2048;

	static inline std::vector<CallbackContainerBase*>	s_callbacks;
	static inline std::vector<void*>					s_event_pools;
	static inline std::vector<std::uint32_t>			s_pool_indexes;

	static inline EventID s_next_event_id = 0u;
};


template<typename T, typename T_Function>
inline void EventManager::listen(T_Function callback)
{
	static_cast<CallbackContainer<T>*>(s_callbacks[get_event_id<T>()])->callbacks.emplace_back(callback);
}


template<typename T, typename T_Instance, typename T_Function>
inline void EventManager::listen(T_Instance& instance, T_Function callback)
{
	static_cast<CallbackContainer<T>*>(s_callbacks[get_event_id<T>()])->callbacks.emplace_back([&instance, callback](T* event) { (instance.*callback)(event); });
}

template<typename T, typename T_Instance, typename T_Function>
inline void EventManager::listen(T_Instance* instance, T_Function callback)
{
	static_cast<CallbackContainer<T>*>(s_callbacks[get_event_id<T>()])->callbacks.emplace_back([instance, callback](T* event) { (instance->*callback)(event); });
}


template<typename T, typename ...T_Args>
inline void EventManager::fire(T_Args ...args)
{
	EventID id = get_event_id<T>();

	T* event = &reinterpret_cast<T*>(s_event_pools[id])[get_next_event(id)];
	*event = T{ args... };

	std::vector<std::function<void(T*)>>& callbacks = reinterpret_cast<CallbackContainer<T>*>(s_callbacks[id])->callbacks;
	for (auto& callback : callbacks)
		callback(event);
}


template<typename T>
inline EventID EventManager::get_event_id()
{
	static EventID event_id = register_event<T>();
	return event_id;
}


template<typename T>
inline EventID EventManager::register_event()
{
	s_event_pools.emplace_back(static_cast<void*>(new T[s_event_pool_size]));
	s_pool_indexes.emplace_back(0u);
	s_callbacks.emplace_back(new CallbackContainer<T*>());
	
	return s_next_event_id++;
}


inline std::uint32_t EventManager::get_next_event(EventID id)
{
	return s_pool_indexes[id]++ & (s_event_pool_size - 1u);
}
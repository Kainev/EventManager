#pragma once

#include <cstdint>
#include <vector>

typedef std::size_t EventID;

class EventManager
{
public:
	template<typename T>
	static void listen(void(*callback)(T*));

	template<typename T, typename... T_Args>
	static void fire(T_Args...args);

	//template<typename T>
	//static void connect();

private:
	template<typename T>
	static EventID get_event_id();

	template<typename T>
	static EventID register_event();

	static std::uint32_t get_next_event(EventID id);


private:
	static inline const std::uint32_t s_event_pool_size = 2048;

	static inline std::vector<std::vector<void(*)(void*)>>	s_callbacks;
	static inline std::vector<void*>						s_event_pools;
	static inline std::vector<std::uint32_t>				s_pool_indexes;

	static inline EventID s_next_event_id = 0u;
};


template<typename T>
inline void EventManager::listen(void(*callback)(T*))
{
	s_callbacks[get_event_id<T>()].emplace_back(reinterpret_cast<void(*)(void*)>(callback));
}


template<typename T, typename ...T_Args>
inline void EventManager::fire(T_Args ...args)
{
	EventID id = get_event_id<T>();

	T* event = &reinterpret_cast<T*>(s_event_pools[id])[get_next_event(id)];
	*event = T{ args... };
		
	for (auto& callback : s_callbacks[id])
		reinterpret_cast<void(*)(T*)>(callback)(event);
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
	s_callbacks.emplace_back();
	
	return s_next_event_id++;
}


inline std::uint32_t EventManager::get_next_event(EventID id)
{
	return s_pool_indexes[id]++ & (s_event_pool_size - 1u);
}
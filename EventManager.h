#pragma once

#include <cstdint>
#include <vector>
#include <functional>
#include <memory>


typedef std::size_t EventID;


struct CallbackContainerBase {};
	
template<typename T>
struct CallbackContainer : CallbackContainerBase
{
	CallbackContainer()
	{
		event = new T();
	}

	~CallbackContainer()
	{
		delete event;
	}

	std::vector<std::function<void(T*)>> callbacks;
	T* event;
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



private:
	static inline std::vector<CallbackContainerBase*>	s_callbacks;
	static inline std::vector<void*>					s_events;

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

	CallbackContainer<T>* callback_container = reinterpret_cast<CallbackContainer<T>*>(s_callbacks[id]);
	*callback_container->event = T{ args... };
	for (auto& callback : callback_container->callbacks)
		callback(callback_container->event);
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
	s_callbacks.emplace_back(new CallbackContainer<T*>());
	
	return s_next_event_id++;
}
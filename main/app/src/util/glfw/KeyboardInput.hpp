#pragma once

#include "glfw.hpp"

#include <vector>
#include <glm.hpp>

namespace KeyboardInput {
	template<typename C, typename A>
	struct Event {
		glfw::Key* key;
		void(C::*operation)(A);
		A arg;
		Event(glfw::Key* k, void(C::*o)(A), A a) { key = k; operation = o; arg = a; }
		Event() { key = nullptr; operation = nullptr; arg = A(); }
	};

	template<typename C, typename A>
	class EventHandler {
	private:
		std::vector<Event<C, A>> events;

	public:
		EventHandler() {}
		inline void addEvent(Event<C, A> event) { events.push_back(event); }
		inline const std::vector<Event<C, A>>& getEvents() const { return events; }
		void removeAllEvents();
		bool handleEvents(GLFWwindow* window, C* c);
		~EventHandler();
	};

	template<typename C, typename A>
	void EventHandler<C, A>::removeAllEvents() {
		for (const auto& e : events)
			delete e.key;

		events.clear();
	}

	template<typename C,typename A>
	bool EventHandler<C, A>::handleEvents(GLFWwindow* window, C* c) {
		bool event_occured = false;
		for (const auto& e : events) {
			if (e.key->isPressed(window)) {
				(c->*e.operation)(e.arg);
				event_occured = true;
			}
		}
		return event_occured;
	}

	template<typename C, typename A>
	EventHandler<C, A>::~EventHandler() {
		for (const auto& e : events)
			delete e.key;
	}
};
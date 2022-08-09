#pragma once

#include <iostream>
#include <vector>

namespace SC
{
	struct EventArgs {};

	/**
	 * @brief A class for Event handling inspired by C# event system
	 * 
	 * @tparam T Type to be used as data
	*/
	template<typename T = EventArgs>
	class EventHandler
	{
		private:
			std::vector<std::function<void(T)>> funcs;

		public:
			EventHandler() 
			{
				funcs = std::vector<std::function<void(T)>>(); 
			}

			/**
			 * @brief Adds a listener and the func is call when Call() method is called
			 * 
			 * @param func the callback function
			 */
			void AddListener(std::function<void(T)> func)
			{
				funcs.push_back(func);
			}

			/**
			 * @brief Calls all the fuctions in the list/ Triggers the event
			 * 
			 * @param Args 
			 */
			void Call(T Args = {})
			{
				for (auto &&func : funcs)
				{
					func(Args);
				}
			}

			void operator()(T args = {})
			{
				Call(args);
			}

			void operator+=(std::function<void(T)> func)
			{
				AddListener(func);
			}
	};
}
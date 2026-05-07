#pragma once
#include <functional>
#include <vector>

namespace TextRPG
{
	template<typename... Args>
	class Delegate
	{
	private:
		std::vector<std::function<void(Args...)>> listeners;
	public:
		void AddListener(std::function<void(Args...)> listener)
		{
			listeners.push_back(listener);
		}

		void Broadcast(Args... args)
		{
			for (auto& listener : listeners)
				listener(args...);
		}
	};
}
#pragma once

#include <unordered_map>
#include <typeindex>
#include <any>
#include <functional>
#include <memory>
#include <stdexcept>
#include <format>

namespace gem::ioc
{
	class Singletons
	{
	public:
		template<class T>
		using Generator = std::function<std::shared_ptr<T>()>;

		template<class T>
		void Register(Generator<T> gen)
		{
			serviceMap_[typeid(T)] = gen;
		}
		template<class T>
		std::shared_ptr<T> Resolve()
		{
			if (const auto i = serviceMap_.find(typeid(T)); i != serviceMap_.end())
			{
				auto& entry = i->second;
				try {
					// first check if we have an existing instance, return if so
					if (auto ppInstance = std::any_cast<std::shared_ptr<T>>(&entry)) {
						return *ppInstance;
					}
					// if not, generate instance, store, and return
					auto pInstance = std::any_cast<Generator<T>>(entry)();
					entry = pInstance;
					return pInstance;
				}
				catch (const std::bad_any_cast&) {
					throw std::logic_error{ std::format(
						"Could not resolve Singleton mapped type\nfrom: [{}]\n  to: [{}]\n",
						entry.type().name(), typeid(Generator<T>).name()
					) };
				}
			}
			else
			{
				throw std::runtime_error{ std::format("Could not find entry for type [{}] in singleton container", typeid(T).name()) };
			}
		}

		Singletons& Get();
	private:
		std::unordered_map<std::type_index, std::any> serviceMap_;
	};
}
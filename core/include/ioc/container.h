#pragma once
#include <unordered_map>
#include <typeindex>
#include <any>
#include <functional>
#include <memory>
#include <format>

namespace gem::ioc
{
	template<class T>
	concept Parameterized = requires() {
		{ typename T::IoCParams{} };
	};

	template<class T>
	concept NotParameterized = !Parameterized<T>;

	class Container
	{
	public:
		template<class T>
		using Generator = std::function<std::shared_ptr<T>()>;
		template<class T>
		using ParameterizedGenerator = std::function<std::shared_ptr<T>(typename T::IoCParams params)>;

		template<NotParameterized T>
		void Register(Generator<T> gen)
		{
			serviceMap_[typeid(T)] = { State_::InstanceGenerator, std::move(gen) };
		}

		template<Parameterized T>
		void Register(ParameterizedGenerator<T> gen)
		{
			serviceMap_[typeid(T)] = { State_::InstanceGenerator, std::move(gen) };
		}

		template<NotParameterized T>
		std::shared_ptr<T> Resolve()
		{
			return Resolve_<T, Generator<T>>();
		}

		template<Parameterized T>
		std::shared_ptr<T> Resolve(typename T::IoCParams&& params = {})
		{
			return Resolve_<T, ParameterizedGenerator<T>>(std::forward<typename T::IoCParams>(params));
		}

		static Container& Get()
		{
			static Container c;
			return c;
		}

	private:
		enum class State_
		{
			InstanceGenerator,
			SingletonGenerator,
			SingletonInstance,
		};
		struct Entry_
		{
			State_ state;
			std::any content;
		};

		template<class T, class G, class...P>
		std::shared_ptr<T> Resolve_(P...arg)
		{
			if (const auto i = serviceMap_.find(typeid(T)); i != serviceMap_.end())
			{
				const auto& entry = i->second;
				try {
					return std::any_cast<G>(entry)(std::forward<P>(arg)...);
				}
				catch (const std::bad_any_cast&) {
					throw std::logic_error{ std::format(
						"Could not resolve IoC mapped type\nfrom: [{}]\n  to: [{}]\n",
						entry.type().name(), typeid(G).name()
					) };
				}
			}
			else
			{
				throw std::runtime_error{ std::format("Could not find generator for type [{}] in IoC container", typeid(T).name()) };
			}
		}

		std::unordered_map<std::type_index, Entry_> serviceMap_;
	};
}
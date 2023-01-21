#pragma once
#include <unordered_map>
#include <typeindex>
#include <any>
#include <functional>
#include <memory>
#include <format>

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
	void RegisterFactory(Generator<T> gen)
	{
		mFactoryMap[typeid(T)] = { State_::InstanceGenerator, std::move(gen) };
	}

	template<Parameterized T>
	void RegisterFactory(ParameterizedGenerator<T> gen)
	{
		mFactoryMap[typeid(T)] = { State_::InstanceGenerator, std::move(gen) };
	}
	
	template<class T>
	void RegisterSingleton(Generator<T> gen)
	{
		mFactoryMap[typeid(T)] = { State_::SingletonGenerator, std::move(gen) };
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

	//avaliable only for friend classes: installers
	static Container& Get()
	{
		static Container c;
		return c;
	}

protected:
	Container() = default;

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

	template<class T, class G, class...Ps>
	std::shared_ptr<T> Resolve_(Ps...arg)
	{
		if (const auto i = mFactoryMap.find(typeid(T)); i != mFactoryMap.end())
		{
			auto& entry = i->second;

			if (entry.state != State_::InstanceGenerator && sizeof...(Ps) > 0) {
				throw std::runtime_error { "Parameters passed in while resolving singleton"}
			}

			try {
				std::shared_ptr<T> ptr;

				switch (entry.state) {
				case State_::InstanceGenerator:
					ptr = std::any_cast<G>(entry.content)(std::forward<Ps>(arg)...);
					break;
				case State_::SingletonGenerator:
					ptr = std::any_cast<G>(entry.content)(std::forward<Ps>(arg)...);
					entry.content = ptr;
					entry.state = State_::SingletonInstance;
					break;
				case State_::SingletonInstance:
					ptr = std::any_cast<std::shared_ptr<T>>(entry.content);
					break;
				}
				return ptr;
			}
			catch (const std::bad_any_cast&) {
				throw std::runtime_error{ std::format(
					"Could not resolve Services mapped type\nfrom: [{}]\n to [{}]\n",
					entry.content.type().name(), typeid(G).name()
				) };
			}
		}
		else
		{
			throw std::runtime_error{ std::format("Could not find generator for type [{}] in IoC container", typeid(T).name()) };
		}
	}

	std::unordered_map<std::type_index, Entry_> mFactoryMap;
};


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
		mFactoryMap[typeid(T)] = gen;
	}

	template<Parameterized T>
	void RegisterFactory(ParameterizedGenerator<T> gen)
	{
		mFactoryMap[typeid(T)] = gen;
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

protected:
	Container() = default;

	//avaliable only for friend classes: installers
	static Container& Get()
	{
		static Container c;
		return c;
	}

private:
	template<class T, class G, class...Ps>
	std::shared_ptr<T> Resolve_(Ps...arg)
	{
		if (const auto i = mFactoryMap.find(typeid(T)); i != mFactoryMap.end())
		{
			try {
				return std::any_cast<G>(i->second)(std::forward<Ps>(arg)...);
			}
			catch (const std::bad_any_cast&) {
				throw std::runtime_error{ std::format(
					"Could not resolve Services mapped type\nfrom: [{}]\n to [{}]\n",
					i->second.type().name(), typeid(G).name()
				) };
			}
		}
		else
		{
			throw std::runtime_error{ std::format("Could not find generator for type [{}] in IoC container", typeid(T).name()) };
		}
	}

	std::unordered_map<std::type_index, std::any> mFactoryMap;

};


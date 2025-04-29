#ifndef ECS_H_
#define ECS_H_

#include <cassert>
#include <bitset>
#include <array>
#include <vector>
#include <unordered_map>
#include <set>
#include <string>
#include <typeindex>
#include <memory>

#include "../logger/logger.h"

constexpr int kMaxComponents = 32;
typedef std::bitset<kMaxComponents> Signature;

typedef std::size_t id_type;

/*
 * A component is a simple struct that holds data and it's attached to an entity.
 * It is uniquely identified by and ID.
 */
struct IComponent {
protected:
	static id_type next_id;
};

template <typename T>
struct Component : public IComponent {
	static id_type id()
	{
		static id_type id = next_id++;
		return id;
	}
};

/*
 * An entity is an object in the game world.
 * It is uniquely identified by and ID.
 */
class Entity {
public:
	Entity() = delete;
	Entity(id_type id) : id_(id) {}

	[[nodiscard]] id_type id() const { return id_; }

	template <typename TComponent, typename... TArgs>
	void AddComponent(TArgs&&... args);

	template <typename TComponent> void RemoveComponent();
	template <typename TComponent> [[nodiscard]] bool HasComponent() const;
	template <typename TComponent> [[nodiscard]] TComponent& GetComponent() const;

	bool operator==(const Entity& other) const { return id_ == other.id_; }
	auto operator<=>(const Entity& other) const { return id_ <=> other.id_; }

	class Registry* registry{nullptr};

private:
	id_type id_;
};

/*
 * A system is a class thet processes entities with a specific set of components.
 */
class System {
public:
	System() = default;
	~System() = default;

	void AddEntityToSystem(const Entity& entity);
	void RemoveEntityFromSystem(const Entity& entity);
	[[nodiscard]] const std::vector<Entity>& GetSystemEntities() const;
	[[nodiscard]] const Signature& GetComponentSignature() const;

	template <typename TComponent>
	void RequireComponent();

private:
	Signature component_signature_;
       	std::vector<Entity> entities_;
};

/*
 * A pool is a container for a specific component type.
 * It stores components of that type in a contigous memory block.
 */
class IPool {
public:
	virtual ~IPool() = default;

protected:
	IPool() = default;
};

template <typename T, typename Container = std::vector<T>>
class Pool : IPool {
public:
	typedef Container					container_type;
	typedef typename container_type::value_type		value_type;
	typedef typename container_type::size_type		size_type;
	typedef typename container_type::reference		reference;
	typedef typename container_type::const_reference	const_reference;

	Pool(size_type size = 100) { c.resize(size); }
	~Pool() override = default;

        [[nodiscard]] bool empty() const { return c.empty(); }
	[[nodiscard]] size_type size() const { return c.size(); }
        void resize(size_type size) { c.resize(size); }
        void clear() { c.clear(); }
        void add(const value_type& value) { c.push_back(value); }

        void set(size_type pos, const value_type& value) { c[pos] = value; }
	[[nodiscard]] reference get(size_type pos) { return c[pos]; }
        [[nodiscard]] const_reference get(size_type pos) const { return c[pos]; }

        [[nodiscard]] reference operator[](size_type pos) { return c[pos]; }
	[[nodiscard]] const_reference operator[](size_type pos) const { return c[pos]; }

private:
	container_type c;
};

/*
 * The registry is the main class that manages entities, components and systems.
 */
class Registry {
public:
	Registry() = default;

        void Update();
        void AddEntityToSystems(const Entity& entity);

        // Entity management
        Entity CreateEntity();
        // void DestroyEntity(Entity entity);

        // Component management
        template <typename TComponent, typename... TArgs>
        void AddComponent(const Entity& entity, TArgs&&... args);

        template <typename TComponent> void RemoveComponent(const Entity& entity);
        template <typename TComponent> [[nodiscard]] bool HasComponent(const Entity& entity) const;
        template <typename TComponent> [[nodiscard]] TComponent& GetComponent(const Entity& entity) const;

        // System management
        template <typename TSystem, typename... TArgs> void AddSystem(TArgs&&... args);
        template <typename TSystem> void RemoveSystem();
        template <typename TSystem> [[nodiscard]] bool HasSystem() const;
        template <typename TSystem> [[nodiscard]] TSystem& GetSystem() const;

private:
	id_type num_entities_{0};

        std::vector<std::shared_ptr<IPool>> component_pools_{};
        std::vector<Signature> component_signatures_{};
        std::unordered_map<std::type_index, std::shared_ptr<System>> systems_{};

        std::set<Entity> entities_to_add_{};
        std::set<Entity> entities_to_remove_{};
};

template <typename TComponent, typename... TArgs>
void Entity::AddComponent(TArgs&&... args)
{
	registry->AddComponent<TComponent>(*this, std::forward<TArgs>(args)...);
}

template <typename TComponent>
void Entity::RemoveComponent()
{
	registry->RemoveComponent<TComponent>(*this);
}

template <typename TComponent>
bool Entity::HasComponent() const
{
	return registry->HasComponent<TComponent>(*this);
}

template <typename TComponent>
TComponent& Entity::GetComponent() const
{
	return registry->GetComponent<TComponent>(*this);
}

template <typename TComponent>
void System::RequireComponent()
{
	const auto id = Component<TComponent>::id();
	component_signature_.set(id);
}

template <typename TComponent, typename... TArgs>
void Registry::AddComponent(const Entity& entity, TArgs&&... args)
{
	const auto cmp_id = Component<TComponent>::id();
	const auto ent_id = entity.id();

	if (cmp_id >= component_pools_.size())
		component_pools_.resize((cmp_id + 1) * 2, nullptr);
	if (component_pools_[cmp_id] == nullptr)
		component_pools_[cmp_id] = std::make_shared<Pool<TComponent>>();

	auto cmp_pool = std::static_pointer_cast<Pool<TComponent>>(component_pools_[cmp_id]);
	if (ent_id >= cmp_pool->size())
		cmp_pool->resize((ent_id + 1) * 2);

	TComponent new_cmp(std::forward<TArgs>(args)...);

	cmp_pool->set(ent_id, new_cmp);
	component_signatures_[ent_id].set(cmp_id);

	Logger::Info("Component ID: " + std::to_string(cmp_id) + " was added to entity ID: " + std::to_string(ent_id));
}

template <typename TComponent>
void Registry::RemoveComponent(const Entity& entity)
{
	const auto cmp_id = Component<TComponent>::id();
	const auto ent_id = entity.id();

	component_signatures_[ent_id].reset(cmp_id);

	Logger::Info("Component ID: " + std::to_string(cmp_id) + " was removed from entity ID: " + std::to_string(ent_id));
}

template <typename TComponent>
bool Registry::HasComponent(const Entity& entity) const
{
	const auto cmp_id = Component<TComponent>::id();
	const auto ent_id = entity.id();

	return component_signatures_[ent_id].test(cmp_id);
}

template <typename TComponent>
TComponent& Registry::GetComponent(const Entity& entity) const
{
	const auto cmp_id = Component<TComponent>::id();
	const auto ent_id = entity.id();

	auto pool = std::static_pointer_cast<Pool<TComponent>>(component_pools_[cmp_id]);
	return pool->get(ent_id);
}

template <typename TSystem, typename... TArgs>
void Registry::AddSystem(TArgs&&... args)
{
	auto new_system = std::make_shared<TSystem>(std::forward<TArgs>(args)...);
	systems_.insert({std::type_index(typeid(TSystem)), new_system});
}

template <typename TSystem>
void Registry::RemoveSystem()
{
	systems_.erase(std::type_index(typeid(TSystem)));
}

template <typename TSystem>
bool Registry::HasSystem() const
{
	return systems_.contains(std::type_index(typeid(TSystem)));
}

template <typename TSystem>
TSystem& Registry::GetSystem() const
{
	auto system =  systems_.find(std::type_index(typeid(TSystem)));
	return *std::static_pointer_cast<TSystem>(system->second);
}

#endif // ECS_H_

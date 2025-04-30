#include "ecs.h"

#include <algorithm>

#include "../logger/logger.h"

id_type IComponent::next_id = 0;

void System::AddEntityToSystem(const Entity& entity)
{
	entities_.push_back(entity);
}

void System::RemoveEntityFromSystem(const Entity& entity)
{
	std::erase_if(entities_, [&entity](const Entity& e) {
		return e == entity;
	});
}

const std::vector<Entity>& System::GetSystemEntities() const
{
	return entities_;
}

const Signature& System::GetComponentSignature() const
{
	return component_signature_;
}

Entity Registry::CreateEntity()
{
	auto id = num_entities_++;

        if (id >= component_signatures_.size())
	    component_signatures_.resize((id + 1) * 2);

        Entity entity{id};
       	entity.registry = this;
        entities_to_add_.insert(entity);

        Logger::Info("Entity created with ID: " + std::to_string(id));

        return entity;
}

void Registry::DestroyEntity(const Entity& entity)
{
	entities_to_remove_.insert(entity);
}

void Registry::Update()
{
	for (auto entity : entities_to_add_)
		AddEntityToSystems(entity);
        entities_to_add_.clear();

	for (auto entity : entities_to_remove_) {
		RemoveEntityFromSystems(entity);
		entity.registry = nullptr;
	}
	entities_to_remove_.clear();
}

void Registry::AddEntityToSystems(const Entity& entity)
{
	const auto ent_id = entity.id();
        const auto& cmp_signature = component_signatures_[ent_id];

        for (auto& system : systems_) {
        	const auto& system_signature = system.second->GetComponentSignature();
                if ((cmp_signature & system_signature) == system_signature)
			system.second->AddEntityToSystem(entity);
        }
}

void Registry::RemoveEntityFromSystems(const Entity& entity)
{
	const auto ent_id = entity.id();
	const auto& cmp_signature = component_signatures_[ent_id];

	for (auto& system : systems_) {
		const auto& system_signature = system.second->GetComponentSignature();
		if ((cmp_signature & system_signature) == system_signature)
			system.second->RemoveEntityFromSystem(entity);
	}
}

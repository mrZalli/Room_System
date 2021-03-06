#include "entity_manager.hh"
#include "helper_functions.hh"

#include <utility>
#include <algorithm>


using namespace Room_System;

Entity_manager::Entity_manager(): arch_manager{}, ents{}
{
}

Entity_manager::~Entity_manager()
{
	std::for_each(ents.begin(), ents.end(), delete_pointed_to<Entity>); // delete all pointers
}

Entity* Entity_manager::add_entity(std::vector<Comps::Component*> comps)
{
	Entity* ent_ptr = new Entity(comps);
	ents.push_back(ent_ptr);

	//room_in->add_entity(ent_ptr);
	return ent_ptr;
}

Entity* Entity_manager::add_entity_from_archetype(std::string name,
												  std::initializer_list<Comps::Component*> component_values)
{
	//Entity* new_entity = new Entity(room_in, arch_manager.get_comps_of_archt(name));
	Entity* new_entity = arch_manager.make_entity_from_archetype(name, component_values);
	
	ents.push_back(new_entity);

	//room_in->add_entity(new_entity);
	return new_entity;
}

Entity* Entity_manager::get_entity_by_id(unsigned id) const throw(std::out_of_range)
{
	if (id > Entity::prev_uid) throw std::out_of_range("id is too high");
	for (auto it = ents.begin(); it != ents.end(); ++it) {
		// because our function is const but we still want to return non-const pointers we have to const_cast it
		if ((*it)->uid == id) return const_cast<Entity*>(*it);
	}
	throw std::out_of_range("id not found");
}

entities_t Entity_manager::get_entities_by_room(Room* room) const
{
	entities_t to_r{};
	for (Entity* ptr : ents) {
		try {
			// in the same room
			if (dynamic_cast<Comps::Position&>(ptr->get_component_with_name("position")).get_room_ptr() == room)
			{
				to_r.push_back(ptr);
			}
		} catch (std::out_of_range) {
			// component not found
		}
	}
	return to_r;
}

#include "entity.hh"
#include "helper_functions.hh"

using namespace Room_System;

unsigned Entity::prev_uid = (unsigned)-1; // initialize uid

Entity::Entity(): comps{}
{}

Entity::Entity(std::initializer_list<Comps::Component*> components):
	comps{components}
{
}

Entity::Entity(const std::vector<Comps::Component*>& components):
	comps{components}
{
}

Entity::~Entity()
{
	std::for_each(comps.begin(), comps.end(), delete_pointed_to<Comps::Component>);
	/*for (Comps::Component* cmp_ptr : comps) {
		delete cmp_ptr;
	}*/
}

bool Entity::has_component_with_name(const std::string& comp_name) const
{
	for (Comps::Component* cmp : comps) {
		if (cmp->get_name() == comp_name) return true;
	}
	return false;
}

std::string Entity::str_value_of_component(const std::string& comp_name) const
{
	for (Comps::Component* cmp : comps) {
		if (cmp->get_name() == comp_name) return cmp->get_value_str();
	}
	return std::string{};
}

Comps::Component& Entity::get_component_with_name(const std::string& comp_name) const throw(std::out_of_range)
{
	for (Comps::Component* cmp : comps) {
		if (cmp->get_name() == comp_name) return *cmp;
	}
	throw std::out_of_range("component " + comp_name + " not found");
}

const std::vector<Comps::Component*>& Entity::get_components() const
{
	return comps;
}


bool ent_id_key::operator()(const Entity& e1, const Entity& e2) {
	return e1.uid < e2.uid;
}


bool ent_id_key::operator()(const Entity* e1, const Entity* e2) {
	return e1->uid < e2->uid;
}

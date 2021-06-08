#include "Entity.h"

// this include must be in .cpp file to avoid circular dependency
#include "EntityManager.h"

Entity::Entity(olc::PixelGameEngine* pge, EntityManager* entity_manager, olc::vf2d _pos, Entity::Type entity_type)
	: pge(pge),
	entity_manager(entity_manager),
	pos(_pos),
	entity_type(entity_type)
{
}
Entity::~Entity()
{
	if (HasBody())
		entity_manager->physics_engine->DestroyBody(body);
}
bool Entity::Update(float delta)
{
	if (HasAnimation())
		animation->Update(delta);

	if (HasBody())
		pos = entity_manager->physics_engine->GetPosition(body) - collider_offset;

	return true;
}
void Entity::SetAnimation(olc::Renderable* spritesheet, Animation::Premade* animation)
{
	this->animation = std::make_unique<Animation>(spritesheet, animation);
}
void Entity::PlayAnimation(Animation::Name name)
{
	animation->PlayAnimation(name);
}
void Entity::DrawEntity(olc::vf2d camera_pos)
{
	olc::vf2d entity_pos = pos - camera_pos;
	animation->DrawAnimation(pge, entity_pos);
}
void Entity::AddRectangleCollider(PhysicsEngine* physics_engine, b2BodyType body_type, float collider_size_x, float collider_size_y, float offset_x, float offset_y, uint16_t category, uint16_t mask)
{
	collider_offset.x = offset_x;
	collider_offset.y = offset_y;

	collider_size.x = collider_size_x;
	collider_size.y = collider_size_y;

	if (HasBody() == false)
		body = physics_engine->CreateBody(body_type, pos.x + offset_x, pos.y + offset_y);

	physics_engine->AddRectangleCollider(body, collider_size_x, collider_size_y, false, category, mask);
}
void Entity::AddCircleCollider(PhysicsEngine* physics_engine, b2BodyType body_type, float radius, float offset_x, float offset_y, uint16_t category, uint16_t mask)
{
	collider_offset.x = offset_x;
	collider_offset.y = offset_y;

	collider_size.x = radius * 2;
	collider_size.y = radius * 2;

	if (HasBody() == false)
		body = physics_engine->CreateBody(body_type, pos.x + offset_x, pos.y + offset_y);

	physics_engine->AddCircleCollider(body, radius, false, category, mask);
}
bool Entity::HasBody()
{
	return body != nullptr;
}
bool Entity::HasAnimation()
{
	return animation != nullptr;
}
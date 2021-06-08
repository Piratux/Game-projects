#include "PhysicsEngine.h"

b2Body* PhysicsEngine::CreateBody(b2BodyType body_type, float pos_x, float pos_y, bool fixed_rotation)
{
	b2BodyDef body_def;
	body_def.type = body_type;
	body_def.position.Set(pos_x * scale, pos_y * scale);
	body_def.fixedRotation = fixed_rotation;
	body_def.linearDamping = 0.8f;
	body_def.angularDamping = 0.8f;
	b2Body* body = world.CreateBody(&body_def);
	return body;
}
void PhysicsEngine::DestroyBody(b2Body* body)
{
	world.DestroyBody(body);
}
void PhysicsEngine::AddCollider(b2Body* body, b2Shape* shape, bool is_sensor, uint16_t category, uint16_t mask)
{
	b2FixtureDef fixture_def;

	switch (body->GetType())
	{
	case b2BodyType::b2_staticBody:
		fixture_def.density = 0.0f;
		break;

	case b2BodyType::b2_kinematicBody:
		fixture_def.density = 1.0f;
		break;
	
	case b2BodyType::b2_dynamicBody:
		fixture_def.density = 1.0f;
		break;
	}

	fixture_def.friction = 0.0f;
	fixture_def.shape = shape;
	fixture_def.filter.categoryBits = category;
	fixture_def.filter.maskBits = mask;
	body->CreateFixture(&fixture_def);
}
void PhysicsEngine::AddRectangleCollider(b2Body* body, float size_x, float size_y, bool is_sensor, uint16_t category, uint16_t mask)
{
	b2PolygonShape rect;
	rect.SetAsBox((size_x * scale) / 2.0f, (size_y * scale) / 2.0f);
	AddCollider(body, &rect, is_sensor, category, mask);
}
void PhysicsEngine::AddCircleCollider(b2Body* body, float radius, bool is_sensor, uint16_t category, uint16_t mask)
{
	b2CircleShape circle;
	circle.m_radius = radius * scale;
	AddCollider(body, &circle, is_sensor, category, mask);
}
void PhysicsEngine::Update()
{
	world.Step(time_step, velocity_iterations, position_iterations);
}
olc::vf2d PhysicsEngine::GetPosition(b2Body* body)
{
	b2Vec2 new_pos = body->GetPosition();
	olc::vf2d temp_pos = { new_pos.x, new_pos.y };
	temp_pos /= scale;
	return temp_pos;
}
void PhysicsEngine::SetTransform(b2Body* body, float pos_x, float pos_y, float angle)
{
	b2Vec2 new_pos = b2Vec2{ pos_x * scale, pos_y * scale };
	body->SetTransform(new_pos, angle);
}


// Debug draw definitions

DebugDraw::DebugDraw(olc::PixelGameEngine* pge, PhysicsEngine* physics_engine, olc::vf2d* camera_pos)
	: pge(pge),
	physics_engine(physics_engine),
	camera_pos(camera_pos)
{
	physics_engine->world.SetDebugDraw(this);
	SetFlags(b2Draw::e_shapeBit);
}
void DebugDraw::Update()
{
	physics_engine->world.DebugDraw();
}

void DebugDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
	olc::Pixel _color{
		static_cast<uint8_t>(color.r * 255.0f),
		static_cast<uint8_t>(color.g * 255.0f),
		static_cast<uint8_t>(color.b * 255.0f),
		0xFF
	};

	olc::vf2d pos1{ vertices[vertexCount - 1].x, vertices[vertexCount - 1].y };
	olc::vf2d pos2{ vertices[0].x, vertices[0].y };
	pos1 /= physics_engine->scale;
	pos2 /= physics_engine->scale;
	pos1 -= *camera_pos;
	pos2 -= *camera_pos;
	for (int32 i = 1; i <= vertexCount; ++i)
	{
		pge->DrawLine(pos1, pos2, _color);

		pos1 = pos2;
		pos2 = { vertices[i].x, vertices[i].y };
		pos2 /= physics_engine->scale;
		pos2 -= *camera_pos;
	}
}
void DebugDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
	DrawPolygon(vertices, vertexCount, color);
}
void DebugDraw::DrawCircle(const b2Vec2& center, float radius, const b2Color& color)
{
	olc::Pixel _color{
		static_cast<uint8_t>(color.r * 255.0f),
		static_cast<uint8_t>(color.g * 255.0f),
		static_cast<uint8_t>(color.b * 255.0f),
		0xFF
	};

	olc::vf2d pos{ center.x, center.y };
	pos /= physics_engine->scale;
	pos -= *camera_pos;
	radius /= physics_engine->scale;

	pge->DrawCircle(pos, (int32_t)radius, _color);
}
void DebugDraw::DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color)
{
	DrawCircle(center, radius, color);
}
void DebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
	olc::Pixel _color{
		static_cast<uint8_t>(color.r * 255.0f),
		static_cast<uint8_t>(color.g * 255.0f),
		static_cast<uint8_t>(color.b * 255.0f),
		0xFF
	};

	olc::vf2d pos1{ p1.x, p1.y };
	olc::vf2d pos2{ p2.x, p2.y };
	pos1 /= physics_engine->scale;
	pos2 /= physics_engine->scale;
	pos1 -= *camera_pos;
	pos2 -= *camera_pos;

	pge->DrawLine(pos1, pos2, _color);
}
void DebugDraw::DrawTransform(const b2Transform& xf)
{

}
void DebugDraw::DrawPoint(const b2Vec2& p, float size, const b2Color& color)
{
	olc::Pixel _color{
		static_cast<uint8_t>(color.r * 255.0f),
		static_cast<uint8_t>(color.g * 255.0f),
		static_cast<uint8_t>(color.b * 255.0f),
		0xFF
	};

	olc::vf2d pos{ p.x, p.y };
	pos /= physics_engine->scale;
	pos -= *camera_pos;

	pge->Draw(pos, _color);
}
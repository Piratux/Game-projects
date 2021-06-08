#pragma once

#include "box2d.h"
#include "olcPixelGameEngine.h"

class DebugDraw;

// physics engine (box2d wrapper)
class PhysicsEngine
{
private:
	float scale = 0.01f;
	b2Vec2 gravity{ 0.0f, 0.0f };
	b2World world{ gravity };

	float time_step = 1.0f / 60.0f;
	int32 velocity_iterations = 6;
	int32 position_iterations = 2;

	friend DebugDraw;

public:
	b2Body* CreateBody(b2BodyType body_type, float pos_x, float pos_y, bool fixed_rotation = true);
	void DestroyBody(b2Body* body);

	// category - usually single bit representing group
	// mask - group of bits telling which group it collides with (collides with everything by default)
	// is_sensor - if true, body collects contact information, but doesn't generate collision response
	void AddCollider(b2Body* body, b2Shape* shape, bool is_sensor = false, uint16_t category = 0x0001, uint16_t mask = 0xFFFF);
	void AddRectangleCollider(b2Body* body, float size_x, float size_y, bool is_sensor = false, uint16_t category = 0x0001, uint16_t mask = 0xFFFF);
	void AddCircleCollider(b2Body* body, float radius, bool is_sensor = false, uint16_t category = 0x0001, uint16_t mask = 0xFFFF);
	void Update();
	olc::vf2d GetPosition(b2Body* body);
	void SetTransform(b2Body* body, float pos_x, float pos_y, float angle = 0.0f);
};

// draws physics engine bodies
class DebugDraw : public b2Draw
{
private:
	olc::PixelGameEngine* pge;
	PhysicsEngine* physics_engine;
	olc::vf2d* camera_pos;
public:
	DebugDraw(olc::PixelGameEngine* pge, PhysicsEngine* physics_engine, olc::vf2d* camera_pos);
	void Update();

	void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override;
	void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override;
	void DrawCircle(const b2Vec2& center, float radius, const b2Color& color) override;
	void DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color) override;
	void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) override;
	void DrawTransform(const b2Transform& xf) override;
	void DrawPoint(const b2Vec2& p, float size, const b2Color& color) override;
};
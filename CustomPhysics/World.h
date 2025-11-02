#pragma once
#include <vector>
#include <memory>
#include <unordered_map>
class PhysObject;



class World
{
protected:
	float AccumulatedFixedTime;
	std::vector<std::shared_ptr<PhysObject>> PhysObjects;
public:
	float TargetFixedStep;
	bool bUseGravity;
	float GravityScale;
	World();
	void Init();
	void Tick();
	void FixedTick();
	void Draw() const;	
	void Exit();
	bool ShouldClose() const;
	bool ShouldFixedTick() const;
	void Instantiate(std::shared_ptr<PhysObject> Object);

protected:

	virtual void OnInit() {}
	virtual void OnTick() {}
	virtual void OnFixedTick() {}
	virtual void OnDraw() const {}
	virtual void OnExit() {}

};


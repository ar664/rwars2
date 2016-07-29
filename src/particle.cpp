#include <stdio.h>
#include <utility>
#include <assert.h>
#include <cmath>
#include "vectors.h"
#include "particle.h"


particle::particle(void)
{
}


particle::~particle(void)
{
}

void particle::ClearAccumulator()
{
	forceAccum.x = 0;
	forceAccum.y = 0;

}
void particle::AddForce(const Vec2D &force)
{
    forceAccum += force;
}
void particle::Integrate(float deltaTime)
{
	//Dont integrate things with infinite mass
	if(inverseMass <= 0.0f)return;
	
	assert(deltaTime > 0.0);
	
	//Update linear position
	position += velocity * deltaTime;

	//work out acceleration from the force
	// We will add this vector when we cp,e tp generate forces
	Vec2D resultingAcc = acceleration;

	//Update linear velocity from the acceleration
	velocity += resultingAcc * deltaTime;

	//impose drag
	velocity *= std::pow(damping,deltaTime);

	ClearAccumulator();

}

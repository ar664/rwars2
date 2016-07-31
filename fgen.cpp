#include "include\globals.h"
#include "fgen.h"

Gravity gravity = Gravity::Gravity(CreateVec2D(0,Gravity_constant));

ForceGenerator::ForceGenerator(void)
{
	
}


ForceGenerator::~ForceGenerator(void)
{
}

void ForceRegistry::updateForces(float delta)
{
    Registry::iterator i = registrations.begin();
    for (; i != registrations.end(); i++)
    {
        i->fg->updateForce(i->body, delta);
    }
}
void ForceRegistry::add(RigidBody *body, ForceGenerator *fg)
{
    ForceRegistry::ForceRegistration registration;
    registration.body = body;
    registration.fg = fg;
    registrations.push_back(registration);
}

Gravity::Gravity(void)
{
}


Gravity::~Gravity(void)
{
}
Gravity::Gravity(const Vec2D &gravity)
: gravity(gravity)
{

}

void Gravity::updateForce(RigidBody* body, float delta)
{
    // Check that we do not have infinite mass
	if (body->mass == 0) return;

    // Apply the mass-scaled force to the body
	body->AddForce(gravity);
}
#ifndef __FGEN_H
#define __FGEN_H
#include "include\entity.h"

class ForceGenerator
{
public:
	ForceGenerator(void);
	~ForceGenerator(void);

	virtual void updateForce(RigidBody* ent, float delta) = 0;
};
/**
* A force generator that applies a gravitational force. One instance
* can be used for multiple rigid bodies.
*/
class Gravity : public ForceGenerator
    {
        /** Holds the acceleration due to gravity. */
        Vec2D gravity;

public:
	Gravity(void);
	~Gravity(void);

        /** Creates the generator with the given acceleration. */
        Gravity(const Vec2D &gravity);

        /** Applies the gravitational force to the given rigid body. */
        virtual void updateForce(RigidBody *body, float delta);
};

/**
* Holds all the force generators and the bodies they apply to.
*/

class ForceRegistry
{
protected:
	/**
     * Keeps track of one force generator and the body it
     * applies to.
     */
	struct ForceRegistration
	{
		RigidBody *body;
		ForceGenerator *fg;
	};
	 /**
     * Holds the list of registrations.
     */
        typedef std::vector<ForceRegistration> Registry;
        Registry registrations;
public:
        /**
        * Registers the given force generator to apply to the
        * given body.
        */
        void add(RigidBody* body, ForceGenerator *fg);

        /**
        * Removes the given registered pair from the registry.
        * If the pair is not registered, this method will have
        * no effect.
        */
        void remove(RigidBody* body, ForceGenerator *fg);

        /**
        * Clears all registrations from the registry. This will
        * not delete the bodies or the force generators
        * themselves, just the records of their connection.
        */
        void clear();

        /**
        * Calls all the force generators to update the forces of
        * their corresponding bodies.
        */
        void updateForces(float delta);
    };

#endif

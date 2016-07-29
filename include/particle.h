#ifndef __PARTICLE_H_
#define __PARTICLE_H_
/**
* This is a work in progress, A particle engine that we can use in the future
*
*/


class particle
{
protected:
	Vec2D	velocity;
	Vec2D	acceleration;
	Vec2D	forceAccum;
	Vec2D	position;

	float	inverseMass;
	float	damping;		//Acts similar to real world drag, Values of 0 means velocity will be reduced to zero while value of 1 means no damping
public:
	particle(void);
	~particle(void);
	void SetMass(float mass);
	void SetInverseMass(float mass);
	void SetDamping(const float damping);
	void SetVelocity(const Vec2D &velocity);
	void SetAcceleration(Vec2D &acceleartion);

	float GetMass() const;
	float GetInverseMass() const;
	float GetDamping() const;
	Vec2D GetAcceleration()const;
	/**
	*Integrates the particle forward in time
	*/
	void Integrate(float deltaTime);
	void ClearAccumulator();
	void AddForce(const Vec2D &force);
};

#endif

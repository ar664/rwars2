#ifndef __PARTICLE_H_
#define __PARTICLE_H_

class particle
{
protected:
	Vec2D	position;
	Vec2D	velocity;
	Vec2D	acceleration;

	float	inverseMass;
	float	damping;		//Acts similar to real world drag, Values of 0 means velocity will be reduced to zero while value of 1 means no damping
public:
	particle(void);
	~particle(void);
	void SetMass(float mass);
	void SetInverseMass(float mass);

};

#endif

#ifndef __UI_H
#define __UI_H

#include "resourcemanager.h"

/* @name	Element
 *
 * @breif	These are the foundation for our UI system,
 * 		they will be used to represent buttons, labels, images,
 * 		etc.. Certain apects, such as state and action will need
 * 		to be defined in the child class.
 *
 * @var		mChildren	The elements inside this element
 * @var		mName		The name of the element
 * @var		mPosition	The position of the element relative to its
 * 					parent class.
 * @var		mGrapics	The sprite info, if any.
 *   
 *
 */
virtual class Element
{
public:
	Element*	mChildren;
	std::String	mName;
	float		mPosition[2];
	float		mSize[2];
	Sprite		mGraphics;
	virtual void	Update();
}


/* @name	Window
 * 
 * @breif	These hold elements, and determine their positions based 
 * 		on the area it defines.
 * 
 * @var		mChildren	Elements inside this window.
 * @var		mName		Name of the window.
 * @var		mPosition	Position of the window in relation to either
 *					screen or parent windows.
 *
 */
class Window
{
public:
	Window();
	Element*	mChildren;
	std::String	mName;
	float		mPosition[2];
	bool		mFocus;
	void		Update();
	void		Draw();
}

#endif


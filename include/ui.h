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
 * @var		mSize		The size of the element as a percentage of 
 * 					the parent window/element.
 * @var		mGrapics	The sprite info, if any.
 *   
 *
 */
virtual class Element
{
public:
	Element(Element* childs, std::String name, float pos[2], float size[2], std::String image)
	{
		mChildren = childs;
		mName = name;
		mPosition = pos;
		mSize = size;
		//Load image
	}
	Element*	mChildren;
	std::String	mName;
	float		mPosition[2];
	float		mSize[2];
	Sprite*		mGraphics;
	//Empty Declared Voids
	virtual void	Update(){};
	virtual void	Draw(){};
};


/* @name	Window
 * 
 * @breif	These hold elements, and determine their positions based 
 * 		on the area it defines. This is main holder for all elements
 * 		as it does the updating and drawing of all children.
 * 
 * @fn		Update		Iterates over children to update position
 * 				and states.
 * @fn		Draw		Iterates over children to draw windows
 * 				based on focus and position.
 *
 */
class Window : public Element
{
public:
	Window(Element* childs, std::String name, float pos[2], float size[2], std::String image);
	bool		mFocus;
	void		Update();
	void		Draw();
};

#endif


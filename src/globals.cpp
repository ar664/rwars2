#include <SFML/Graphics.hpp>
#include "globals.h"
sf::Event gEvent;


int MemoryCount(void *src, int size_type)
{
	int i, offset;
	int *source = (int*)src;
	if(source == NULL)
		return 0;
	i = 0; offset = size_type/sizeof(int);
	while( *(source) ) 
	{
		source += offset;
		i++;
	}
	return i;
}
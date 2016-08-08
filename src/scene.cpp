#include <stdio.h>
#include <string.h>
#include <utility>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <string.h>
#include "include\scene.h"


Scene::Scene(void)
{
	memset(Players,0,sizeof(PlayerComponent)*MAX_ENTITIES);
}

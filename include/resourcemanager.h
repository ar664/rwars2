#ifndef _RESOURCEMANAGER_H
#define _RESOURCEMANAGER_H
#include <SFML/Graphics.hpp>
#include <map>



class TextureCache
{
public:
	TextureCache();
	~TextureCache();
	sf::Texture* GetTexture(char* filepath);
	void		FreeTexture(char* filepath);
private:
	std::map<char*,sf::Texture*> mTextureMap;

};
class mfSpriteCache
{
public:
	mfSpriteCache();
	~mfSpriteCache();
	sf::Sprite* GetmfSprite(char* filepath);
	void	FreemfSprite(char* filepath);
	std::map<char*,sf::Sprite*>	*GetSpriteMap();
private:
	std::map<char*,sf::Sprite*> mmfSpriteMap;

};

class ResourceManager
{
public:
	static sf::Texture* GetTexture(char* texturePath);
	static sf::Sprite* GetmfSprite(char* spritePath);
	static TextureCache* GetTextureCache();
	static mfSpriteCache* GetmfSpriteCache();

	static void ResourceManager::FreeTexture(char* texturePath);
	static void ResourceManager::FreemfSprite(char* spritePath);
	static void ResourceManager::FreeCaches();
private:
	static TextureCache mTextureCache;
	static mfSpriteCache mmfSpriteCache;


};




#endif
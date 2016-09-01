#ifndef _RESOURCEMANAGER_H
#define _RESOURCEMANAGER_H
#include <SFML/Graphics.hpp>
#include <map>
#include "include\sprite.h"
#include "include\rapidjson\filereadstream.h"
#include "include\rapidjson\document.h"


/**
* This class is used to manage the textures
*/

class TextureCache
{
public:
	TextureCache();
	~TextureCache();
	sf::Texture* GetTexture(char* filepath);
	void		FreeTexture(char* filepath);
private:
	std::map<std::string,sf::Texture*> mTextureMap;

};
/**
* This class is used to manage the sf::sprites
*/

class mfSpriteCache
{
public:
	mfSpriteCache();
	~mfSpriteCache();
	sf::Sprite* GetmfSprite(char* filepath);
	void	FreemfSprite(char* filepath);
	std::map<std::string,sf::Sprite*>	*GetSpriteMap();
private:
	std::map<std::string,sf::Sprite*> mmfSpriteMap;

};
/**
* This class is used to manage the texture cache and sf::sprite cache
*/

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
	static bool ResourceManager::AddAsset(char* assetPath);
	static void	
		ResourceManager::LoadCharacterSpriteAssets(char* characterName,Sprite** spriteDoublePointer);


private:
	static TextureCache			mTextureCache;
	static mfSpriteCache		mmfSpriteCache;
	static rapidjson::Document	mAssetCache;


};




#endif
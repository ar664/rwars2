#include "include\resourcemanager.h"

TextureCache ResourceManager::mTextureCache;
mfSpriteCache ResourceManager::mmfSpriteCache;

TextureCache::TextureCache()
{

}
TextureCache::~TextureCache()
{

}
mfSpriteCache::mfSpriteCache()
{

}
mfSpriteCache::~mfSpriteCache()
{

}
sf::Texture* TextureCache::GetTexture(char* filepath)
{
	//Look for Texture
	auto it = mTextureMap.find(filepath);
	if(it == mTextureMap.end())
	{
		sf::Texture* texture = new sf::Texture();
		if(!texture->loadFromFile(filepath))
		{
		printf("Failed to Load Texture: %s",filepath);		
		}
		std::pair<char*,sf::Texture*> p (filepath,texture);

		mTextureMap.insert(p);
		
		return texture;
	}
	return it->second;
}

void TextureCache::FreeTexture(char* filepath)
{
	auto it = mTextureMap.find(filepath);
	if(it !=mTextureMap.end())
	{
		if(&it != nullptr)
		{
			delete it->second;
			mTextureMap.erase(it);
		}
	}

}

sf::Texture* ResourceManager::GetTexture(char* texturePath)
{
	return mTextureCache.GetTexture(texturePath);
}
void ResourceManager::FreeTexture(char* texturePath)
{
	mTextureCache.FreeTexture(texturePath);
}
void ResourceManager::FreemfSprite(char* spritePath)
{
	mmfSpriteCache.FreemfSprite(spritePath);
}
sf::Sprite* ResourceManager::GetmfSprite(char* spritePath)
{
	return mmfSpriteCache.GetmfSprite(spritePath);
}
mfSpriteCache* ResourceManager::GetmfSpriteCache()
{
	return &mmfSpriteCache;

}
void ResourceManager::FreeCaches()
{
	mfSpriteCache* spriteCache = GetmfSpriteCache();
	auto it = spriteCache->GetSpriteMap()->begin();
	 while(it!= spriteCache->GetSpriteMap()->end()) {
		 ResourceManager::FreemfSprite(it->first);
		 it = spriteCache->GetSpriteMap()->begin();
	 }
	 
}
sf::Sprite* mfSpriteCache::GetmfSprite(char* filepath)
{

	//Look for Texture
	auto it = mmfSpriteMap.find(filepath);
	if(it == mmfSpriteMap.end())
	{
		sf::Sprite* s = new sf::Sprite();
		std::pair<char*,sf::Sprite*> p (filepath,s);

		mmfSpriteMap.insert(p);
		
		return s;
	}
	return it->second;
}

void mfSpriteCache::FreemfSprite(char* filepath)
{
	auto it = mmfSpriteMap.find(filepath);
	if(it != mmfSpriteMap.end())
	{
		delete it->second;
		mmfSpriteMap.erase(it);
	}
	sf::Texture* texture = ResourceManager::GetTexture(filepath);
	if(texture != nullptr)
	{
		ResourceManager::FreeTexture(filepath);
	}
}
std::map<char*,sf::Sprite*>* mfSpriteCache::GetSpriteMap()
{
	return &mmfSpriteMap;

}

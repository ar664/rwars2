#include "include\resourcemanager.h"

TextureCache ResourceManager::mTextureCache;
mfSpriteCache ResourceManager::mmfSpriteCache;
rapidjson::Document ResourceManager::mAssetCache;
using namespace rapidjson;
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
	{
	mfSpriteCache* spriteCache = GetmfSpriteCache();
	auto it = spriteCache->GetSpriteMap()->begin();
	 while(it!= spriteCache->GetSpriteMap()->end()) {
		 ResourceManager::FreemfSprite(it->first);
		 it = spriteCache->GetSpriteMap()->begin();
	 }

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
bool ResourceManager::AddAsset(char* assetPath)
{
	char fileName[155];

	strcpy(fileName,assetPath);
	
	FILE *file = fopen(fileName,"rb");
	if(file == nullptr)
	{
		printf("Unable to open file: %s",assetPath);
		return false;
	}
	char buffer[65536];
	FileReadStream frs(file,buffer,sizeof(buffer));
	//Parse the FileReadStream and close file
	mAssetCache.ParseStream(frs);
	fclose(file);
	assert(mAssetCache.IsObject());
	

	return true;
}
void ResourceManager::LoadCharacterSpriteAssets(char* characterName,Sprite** spriteDoublePointer)
{
	int count = 0;
	int maxFrames = 0;
	assert(mAssetCache.HasMember("CharacterSprites"));
	for(Value::ConstMemberIterator aIt =mAssetCache.MemberBegin();
		aIt != mAssetCache.MemberEnd(); ++aIt)
	{
		if(strcmp(aIt->name.GetString(),"CharacterSprites") == 0)
		{
			assert(aIt->value.IsArray());
			const Value& charSpriteArray = aIt->value;
			for (SizeType j = 0; j < charSpriteArray.Size(); j++)
			{
				for(Value::ConstMemberIterator spriteIt = charSpriteArray[j].MemberBegin();
								spriteIt != charSpriteArray[j].MemberEnd(); ++spriteIt)
				{
					if(strcmp(spriteIt->value.GetString(),characterName)==0)
					{
						spriteIt++;
						assert(spriteIt->value.IsArray());
						const Value& spriteArray = spriteIt->value;
						for(SizeType k = 0; k < spriteArray.Size();k++)
						{
							for(Value::ConstMemberIterator sIt = spriteArray[k].MemberBegin();
								sIt != spriteArray[k].MemberEnd(); ++sIt)
							{
								char* na = new char[128];
								strcpy(na,sIt->value.GetString());
								sIt++;
								maxFrames = sIt->value.GetInt();
								sIt++;
								char filePath[128];
								strcpy(filePath,sIt->value.GetString());
								spriteDoublePointer[count] = LoadSprite(filePath);
								spriteDoublePointer[count]->mAnimation.maxFrames = maxFrames;
								count++;
						
							}	
						}

					}


				}

			}

		}

	}
}
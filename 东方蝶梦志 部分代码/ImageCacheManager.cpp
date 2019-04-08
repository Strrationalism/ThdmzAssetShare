#include "NuzPrecompiled.h"
#include "ImageCacheManager.h"
using namespace Basic::Video::LowerRenderer;

std::map<std::string, std::weak_ptr<GLTexture>> ImageCacheManager::m_imageCache;

std::shared_ptr<GLTexture> ImageCacheManager::LoadImage(const std::string & path)
{
	if (m_imageCache.count(path)) {
		if (!m_imageCache.at(path).expired())
			return m_imageCache.at(path).lock();
	}
	std::shared_ptr<GLTexture> ret = std::make_shared<GLTexture>();
	ret->LoadImage(path);
	m_imageCache[path] = std::weak_ptr<GLTexture>(ret);
	return ret;
}

std::shared_ptr<GLTexture> ImageCacheManager::RenderSurface(SDL_Surface * sur,bool needConvert)
{
	std::shared_ptr<GLTexture> ret = std::make_shared<GLTexture>();
	if(needConvert) ret->LoadSurface(sur);
	else ret->LoadSurfaceSDLRGBA8888(sur);
	return ret;
}

void Basic::Video::LowerRenderer::ImageCacheManager::EarlyDelete()
{
	for (auto& p : m_imageCache)
		if(!p.second.expired())
			p.second.lock()->EarlyDelete();

}

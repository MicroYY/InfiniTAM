#pragma once

#ifdef COMPILE_WITH_NETWORK



#endif // COMPLIE_WITH_NETWORK

#include "ImageSourceEngine.h"

namespace InputSource {

	class NetworkEngine :public BaseImageSourceEngine
	{
	private:
		//class PrivateData;
		//PrivateData* data;
		Vector2i imageSize_rgb, imageSize_d;
	public:
		NetworkEngine(const char* calibFilenamea);
		~NetworkEngine();

		bool hasMoreImages() const;
		void getImages(ORUChar4Image* rgbImage, ORShortImage* rawDepthImage);
		Vector2i getDepthImageSize() const;
		Vector2i getRGBImageSize() const;
	};
}
#pragma once

#ifdef COMPILE_WITH_NETWORK2

#endif // COMPILE_WITH_NETWORK2

#include "ImageSourceEngine.h"

namespace InputSource {
	class Network2Engine :public BaseImageSourceEngine
	{
	private:
		class PrivateData;
		PrivateData* data;
		Vector2i imageSize_rgb, imageSize_d;
	public:
		Network2Engine(const char* calibFilename);
		~Network2Engine();

		bool hasMoreImages() const;
		void getImages(ORUChar4Image* rgbImage, ORShortImage* rawDepthImage);
		Vector2i getDepthImageSize() const;
		Vector2i getRGBImageSize() const;
	};
}
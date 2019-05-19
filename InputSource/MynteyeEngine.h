#pragma once
//test
#ifdef COMPILE_WITH_MYNTEYE
#include <mynteyed/camera.h>
#include <mynteyed/utils.h>

#pragma comment(lib, "mynteye_depth.lib")
#endif // COMPILE_WITH_MYNTEYE

#include "ImageSourceEngine.h"

namespace InputSource{

	class MynteyeEngine :public BaseImageSourceEngine
	{
	private:
		class PrivateData;
		PrivateData *data;
		Vector2i imageSize_rgb, imageSize_d;
		bool colorAvailable, depthAvailable;
	public:
		MynteyeEngine(const char* calibFilename);

		~MynteyeEngine();

		bool hasMoreImages(void) const;
		void getImages(ORUChar4Image *rgbImage, ORShortImage *rawDepthImage);
		Vector2i getDepthImageSize(void) const;
		Vector2i getRGBImageSize(void) const;

};
}
#include "MynteyeEngine.h"

#include "../ORUtils/FileUtils.h"

#ifdef COMPILE_WITH_MYNTEYE

using namespace InputSource;


class MynteyeEngine::PrivateData
{
public:
	PrivateData() {}
	mynteyed::Camera cam;
	mynteyed::DeviceInfo dev_info;
};



MynteyeEngine::MynteyeEngine(const char * calibFilename)
	:BaseImageSourceEngine(calibFilename)
{
	data = new PrivateData();

	if (!mynteyed::util::select(data->cam, &data->dev_info)) {
		return;
	}
	mynteyed::util::print_stream_infos(data->cam, data->dev_info.index);

	std::cout << "Open device: " << data->dev_info.index << ", "
		<< data->dev_info.name << std::endl << std::endl;

	mynteyed::OpenParams params(data->dev_info.index);
	params.depth_mode = mynteyed::DepthMode::DEPTH_RAW;
	params.stream_mode = mynteyed::StreamMode::STREAM_1280x720;
	params.ir_intensity = 4;
	params.framerate = 30;

	data->cam.Open(params);

	std::cout << std::endl;
	if (!data->cam.IsOpened()) {
		std::cerr << "Error: Open camera failed" << std::endl;
		return;
	}
	std::cout << "Open device success" << std::endl << std::endl;

	this->calib.disparityCalib.SetStandard();
	this->imageSize_d = Vector2i(1280, 720);
	this->imageSize_rgb = Vector2i(1280, 720);

	this->calib.intrinsics_d = this->calib.intrinsics_rgb;
}

InputSource::MynteyeEngine::~MynteyeEngine()
{
	data->cam.Close();
	delete data;
}

bool MynteyeEngine::hasMoreImages(void) const
{
	return (data != NULL);
}



void InputSource::MynteyeEngine::getImages(ORUChar4Image * rgbImage, ORShortImage * rawDepthImage)
{
	short* rawDepth = rawDepthImage->GetData(MEMORYDEVICE_CPU);
	Vector4u* rgb = rgbImage->GetData(MEMORYDEVICE_CPU);

	Vector2i noDims = rawDepthImage->noDims;
	auto left_color = data->cam.GetStreamData(mynteyed::ImageType::IMAGE_LEFT_COLOR);
	if (left_color.img) {
		const uint8_t *color_frame = left_color.img->data();
		for (size_t i = 0; i < rgbImage->noDims.x * rgbImage->noDims.y * 2; i+=4)
		{
			unsigned char y, u, v, r, g, b;
			y = color_frame[i + 0];
			u = color_frame[i + 1];
			v = color_frame[i + 3];
			b = 1.164*(y - 16) + 1.159*(v - 128);
			g = 1.164*(y - 16) - 0.380*(u - 128) + 0.813*(v - 128);
			r = 1.164*(y - 16) + 2.018*(u - 128);
			rgb[i / 2].x = b;
			rgb[i / 2].y = g;
			rgb[i / 2].z = r;
			rgb[i / 2].w = 255;

			y = color_frame[i + 2];
			u = color_frame[i + 1];
			v = color_frame[i + 3];
			b = 1.164*(y - 16) + 1.159*(v - 128);
			g = 1.164*(y - 16) - 0.380*(u - 128) + 0.813*(v - 128);
			r = 1.164*(y - 16) + 2.018*(u - 128);
			rgb[i / 2 + 1].x = b;
			rgb[i / 2 + 1].y = g;
			rgb[i / 2 + 1].z = r;
			rgb[i / 2 + 1].w = 255;
		}
		
	}
	auto image_depth = data->cam.GetStreamData(mynteyed::ImageType::IMAGE_DEPTH);
	if (image_depth.img) {
		const uint16_t * depth_frame = (uint16_t*)image_depth.img->data();
		for (size_t y = 0; y < noDims.y; y++)
		{
			for (size_t x = 0; x < noDims.x; x++)
			{
				rawDepth[x + y * noDims.x] = *(depth_frame);
				depth_frame++;
			}
		}
	}
}

Vector2i MynteyeEngine::getDepthImageSize(void) const
{
	return (data != NULL) ? imageSize_d : Vector2i(0, 0);
}

Vector2i MynteyeEngine::getRGBImageSize(void) const
{
	return (data != NULL) ? imageSize_rgb : Vector2i(0, 0);
}


#else

#endif // COMPILE_WITH_MYNTEYE
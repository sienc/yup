// ========================================================================== //
//
//  FrameBuffer.h
//  ---
//  A generalized FrameBuffer class that automatically manages the storage
//
//  Created: 2016-08-24
//  Updated: 2016-09-13
//
//  (C) 2016 Yu-hsien Chang
//
// ========================================================================== //

#pragma once

#include <cstdint>
#include <cstring>

#ifdef YUP_INCLUDE_OPENCV
#include <opencv2/imgproc/imgproc.hpp>
#endif

#ifdef USE_RSSDK
#include <pxcsensemanager.h>
#endif

#include "yup.h"
#include "matutil.h"

BEGIN_NAMESPACE_YUP

// A simple RGBA frame buffer
class FrameBuffer
{
private:
	uint8_t * mData = nullptr;

	int mWidth = 0;
	int mHeight = 0;
	int mColorDepth = 4;

public:
	FrameBuffer(int width = 0, int height = 0, int colorDepth = 4)
	: mColorDepth(colorDepth)
	{
		resize(width, height);
	}

	~FrameBuffer() {
		if (mData) {
			delete[] mData;
			mData = nullptr;
		}
	}

	int width() const { return mWidth; }
	int height() const { return mHeight; }
	int colorDepth() const { return mColorDepth; }
	int pitch() const { return mWidth * mColorDepth; }
	int size() const { return mWidth * mHeight * mColorDepth; } // Return the size of mdata

	uint8_t * data() { return mData; }
	const uint8_t * cst_data() const { return mData; }

	operator const uint8_t *() { return mData; }

	void resize(int width, int height, int colorDepth = 0) {
		if (colorDepth == 0)
			colorDepth = mColorDepth;

		if (mWidth * mHeight * mColorDepth != width * height * colorDepth)
		{
			if (mData)
				delete[] mData;

			size_t dataSize = width * height * mColorDepth;
			mData = new uint8_t[dataSize];
			memset(mData, 0, dataSize);
		}
		mWidth = width;
		mHeight = height;
		mColorDepth = colorDepth;
	}

	void clear() {
		if(mData)
			memset(mData, 0, size());
	}

	// Set the alpha channel to 0
	void clearMask() {

	}

	void setData(const uint8_t *data) {
		
		// Blit the image
		memcpy_s(mData, size(), data, size());
	}


	void setData(const uint8_t *colorData, const uint8_t *depthData) {

		// Expand for more efficiency

		if (mColorDepth >= 4)
		{
			const int nData = mWidth * mHeight;

			for (int c = 0; c < nData; c++)
			{
				if (colorData)
					memcpy(mData + c * mColorDepth, colorData + c * 3, 3);

				if (depthData)
					mData[c * mColorDepth + 3] = depthData[c];
				else
					mData[c * mColorDepth + 3] = 0xFF;
			}
		}
		else if (mColorDepth == 3 && colorData)
		{
			// Copy only color
			setData(colorData);
		}
		else if (mColorDepth == 1 && depthData)
		{
			// Copy only depth
			setData(depthData);
		}
	}

	void operator=(const FrameBuffer & buffer)
	{
		resize(buffer.mWidth, buffer.mHeight, buffer.mColorDepth);
		memcpy(mData, buffer.mData, size());
	}

#ifdef YUP_INCLUDE_OPENCV

	void setData(const cv::Mat &colorMat, const cv::Mat &depthMat) {

		// accept only char type matrices
		CV_Assert(colorMat.depth() == CV_8U && colorMat.elemSize() == 3);
		CV_Assert(depthMat.depth() == CV_8U && depthMat.elemSize() == 1);
		CV_Assert(colorMat.rows == depthMat.rows && colorMat.cols == depthMat.cols);

		CV_Assert(colorMat.isContinuous() && depthMat.isContinuous());

		setData(colorMat.ptr<uint8_t>(0), depthMat.ptr<uint8_t>(0));
	}

	void fillMat(cv::Mat &mat) const {
		// accept only char type matrices
		CV_Assert(mat.rows == mHeight);
		CV_Assert(mat.cols == mWidth);
		
		FillCVMatU8(mat, mData);
	}

#endif // YUP_INCLUDE_OPENCV

};

END_NAMESPACE_YUP
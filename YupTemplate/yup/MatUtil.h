// ========================================================================== //
//
//  matutil.h
//  ---
//  Utility functions for matrix handling
//
//  Created: 2016-08-24
//  Updated: 2016-08-24
//
//  (C) 2016 Yu-hsien Chang
//
// ========================================================================== //

#pragma once

#include "yup.h"

#ifdef YUP_INCLUDE_OPENVR
#ifdef YUP_INCLUDE_OPENCV

#endif // YUP_INCLUDE_OPENVR
#endif // YUP_INCLUDE_OPENCV

#ifdef YUP_INCLUDE_OPENVR
#include <openvr.h>
using vr::HmdMatrix34_t;
using vr::HmdMatrix44_t;
using vr::HmdVector3_t;
#endif // YUP_INCLUDE_OPENVR

#ifdef YUP_INCLUDE_OPENCV
#include <opencv2/core.hpp>
using cv::Point3f;
using cv::Point2f;
#endif // YUP_INCLUDE_OPENCV



BEGIN_NAMESPACE_YUP

#ifdef YUP_INCLUDE_OPENCV
typedef cv::Mat_<float> Matf;
#endif // YUP_INCLUDE_OPENCV


// ========================================================================== //
//  OpenVR to OpenCV conversion
// ========================================================================== //

#ifdef YUP_INCLUDE_OPENVR
#ifdef YUP_INCLUDE_OPENCV

// -------------------------------------------------------------------------- //
// Convert VRMat34 to CVMat44
// -------------------------------------------------------------------------- //
static inline void VRMatToCVMat(const HmdMatrix34_t &matPose, Matf &outMat)
{
	if (outMat.rows != 4 && outMat.cols != 4)
		outMat.create(4, 4);

	for (int i = 0; i < 3; i++)
	{
		float * rowi = outMat.ptr<float>(i);

		rowi[0] = matPose.m[i][0];
		rowi[1] = matPose.m[i][1];
		rowi[2] = matPose.m[i][2];
		rowi[3] = matPose.m[i][3];
	}

	outMat(3, 0) = 0.0f;
	outMat(3, 1) = 0.0f;
	outMat(3, 2) = 0.0f;
	outMat(3, 3) = 1.0f;
}

// -------------------------------------------------------------------------- //
// Convert VRMat34 to CVMat44
// -------------------------------------------------------------------------- //
static inline Matf VRMatToCVMat(const HmdMatrix34_t &matPose)
{
	Matf outMat(4, 4);

	for (int i = 0; i < 3; i++)
	{
		float * rowi = outMat.ptr<float>(i);

		rowi[0] = matPose.m[i][0];
		rowi[1] = matPose.m[i][1];
		rowi[2] = matPose.m[i][2];
		rowi[3] = matPose.m[i][3];
	}

	outMat(3, 0) = 0.0f;
	outMat(3, 1) = 0.0f;
	outMat(3, 2) = 0.0f;
	outMat(3, 3) = 1.0f;

	return outMat;
}

// -------------------------------------------------------------------------- //
// Convert VRMat44 to CVMat44
// -------------------------------------------------------------------------- //
static inline Matf VRMatToCVMat(const HmdMatrix44_t &mat44)
{
	Matf outMat(4, 4);

	for (int i = 0; i < 4; i++)
	{
		float * rowi = outMat.ptr<float>(i);

		rowi[0] = mat44.m[i][0];
		rowi[1] = mat44.m[i][1];
		rowi[2] = mat44.m[i][2];
		rowi[3] = mat44.m[i][3];
	}

	return outMat;
}

// -------------------------------------------------------------------------- //
// Convert VRVec3 to CVPoint3
// -------------------------------------------------------------------------- //
static inline Point3f VRVec3ToCVPoint(const HmdVector3_t vec)
{
	return Point3f(vec.v[0], vec.v[1], vec.v[2]);
}

#endif // YUP_INCLUDE_OPENVR
#endif // YUP_INCLUDE_OPENCV


// ========================================================================== //
//  OpenCV internal conversion
// ========================================================================== //

#ifdef YUP_INCLUDE_OPENCV

// -------------------------------------------------------------------------- //
// Convert CVMat31 to CVPoint3
// -------------------------------------------------------------------------- //
static inline Point3f CVMatToCVPoint3(const Matf mat)
{
	return Point3f(mat(0), mat(1), mat(2));
}

// -------------------------------------------------------------------------- //
// Convert CVPoint3 to CVMat31
// -------------------------------------------------------------------------- //
static inline Matf CVPointToCVMat31(const Point3f &pt)
{
	Matf mat(3, 1);

	mat(0) = pt.x;
	mat(1) = pt.y;
	mat(2) = pt.z;

	return mat;
}

// -------------------------------------------------------------------------- //
// Convert CVPoint3 to CVMat41
// -------------------------------------------------------------------------- //
static inline Matf CVPointToCVMat41(const Point3f &pt)
{
	Matf mat(4, 1);

	mat(0) = pt.x;
	mat(1) = pt.y;
	mat(2) = pt.z;
	mat(3) = 1;

	return mat;
}

// -------------------------------------------------------------------------- //
// Fill a CVMatU8 with provided data
// -------------------------------------------------------------------------- //
static void FillCVMatU8(cv::Mat &mat, const uint8_t *data, int srcChannel = 0)
{
	// accept only char type matrices
	CV_Assert(mat.depth() == CV_8U);

	const int channels = mat.channels();
	CV_Assert(channels >= srcChannel);

	int nRows = mat.rows;
	int nCols = mat.cols;

	if (srcChannel == 0 || srcChannel == channels)
	{
		nCols *= channels;

		// Channel matched, do bulk copy
		if (mat.isContinuous())
		{
			nCols *= nRows;
			nRows = 1;
		}

		uint8_t* p;
		for (int r = 0; r < nRows; ++r)
		{
			p = mat.ptr<uint8_t>(r);
			memcpy(p, data, nCols);
			// TODO: use memcpy
			//for (int c = 0; c < nCols; ++c)
			//	p[c] = data[c];

			data += nCols;
		}
	}
	else
	{
		// Channel does not match, copy one by one
		uint8_t* p;
		for (int r = 0; r < nRows; ++r)
		{
			p = mat.ptr<uint8_t>(r);
			for (int c = 0; c < nCols; ++c)
			{
				for (int ch = 0; ch < channels; ch++)
					p[c * channels + ch] = data[c * srcChannel + ch];
			}
			data += nCols * srcChannel;
		}
	}
}

// -------------------------------------------------------------------------- //
// Fill a CVMatU16 with provided data
// -------------------------------------------------------------------------- //
static void FillCVMatU16(cv::Mat &mat, const uint16_t *data, int srcChannel = 0)
{
	// accept only char type matrices
	CV_Assert(mat.depth() == CV_16U);

	const int channels = mat.channels();
	int nRows = mat.rows;
	int nCols = mat.cols;

	if (srcChannel == 0 || srcChannel == channels)
	{
		nCols *= channels;

		// Channel matched, do bulk copy
		if (mat.isContinuous())
		{
			nCols *= nRows;
			nRows = 1;
		}

		uint16_t* p;
		for (int r = 0; r < nRows; ++r)
		{
			p = mat.ptr<uint16_t>(r);
			memcpy(p, data, nCols * 2);
			// TODO: use memcpy
			//for (int c = 0; c < nCols; ++c)
			//	p[c] = data[c];

			data += nCols;
		}
	}
	else
	{
		// Channel does not match, copy one by one
		uint16_t* p;
		for (int r = 0; r < nRows; ++r)
		{
			p = mat.ptr<uint16_t>(r);
			for (int c = 0; c < nCols; ++c)
			{
				for (int ch = 0; ch < channels; ch++)
					p[c * channels + ch] = data[c * srcChannel + ch];
			}
			data += nCols * srcChannel;
		}
	}
}

#endif // YUP_INCLUDE_OPENCV

END_NAMESPACE_YUP

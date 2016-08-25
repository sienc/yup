#pragma once

#include "yup/VRSdlApp.h"
#include "yup/MatUtil.h"

#define WINDOW_NAME "Viveclops"

using yup::VRSdlApp;
using namespace vr;

class $AppClassName$ : public VRSdlApp
{
public:
	$AppClassName$(int argc, char *argv[]);
	~$AppClassName$();

	// SdlApp override
	virtual bool init() override;
	virtual bool update() override;
	virtual void shutdown() override;

	// Renderable override
	virtual bool onInit() override;
	virtual void onRender(Matrix4 &vpMat, int width, int height) override;
	virtual void onRenderEye(vr::Hmd_Eye nEye, Matrix4 &vpMat) override;
	virtual void onUpdate(const Matrix4 &headPose) override;
	virtual void onShutdown() override;
};

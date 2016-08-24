#include "$AppClassName$.h"
#include "yup/ShaderCollection.h"
#include "yup/Log.h"

//#include <Log.h>

using namespace vr;
using namespace std;

using yup::gl::Shader;

$AppClassName$::$AppClassName$(int argc, char *argv[])
	: VRSdlApp(argc, argv)
{
}

$AppClassName$::~$AppClassName$()
{
}

bool $AppClassName$::init()
{
	if (!VRSdlApp::init())
		return false;

	Shader.Compile();

	return true;
}

bool $AppClassName$::update()
{
	return VRSdlApp::update();
}

void $AppClassName$::shutdown()
{
	VRSdlApp::shutdown();
}


bool $AppClassName$::onInit()
{
	return true;
}

void $AppClassName$::onRender(Matrix4 &vpMat, int width, int height)
{
}

void $AppClassName$::onRenderEye(vr::Hmd_Eye nEye, Matrix4 &vpMat)
{
}

void $AppClassName$::onUpdate(const Matrix4 & headPose)
{
}

void $AppClassName$::onShutdown()
{
}

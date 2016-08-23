#include "YupTemplateApp.h"

YupTemplateApp::YupTemplateApp(int argc, char *argv[])
	: yup::App(argc, argv)
{
}


YupTemplateApp::~YupTemplateApp()
{
}

bool YupTemplateApp::init()
{
	return true;
}

bool YupTemplateApp::update()
{
	return false;
}

void YupTemplateApp::shutdown()
{
}

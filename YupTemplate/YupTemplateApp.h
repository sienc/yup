#pragma once
#include "yup/App.h"

using yup::App;

class YupTemplateApp : public App
{
public:
	YupTemplateApp(int argc, char *argv[]);
	~YupTemplateApp();

	bool init() override;
	bool update() override;
	void shutdown() override;
};


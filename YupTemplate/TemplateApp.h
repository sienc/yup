#pragma once
#include "yup/App.h"

using yup::App;

class $AppClassName$ : public App
{
public:
	$AppClassName$(int argc, char *argv[]);
	~$AppClassName$();

	bool init() override;
	bool update() override;
	void shutdown() override;
};


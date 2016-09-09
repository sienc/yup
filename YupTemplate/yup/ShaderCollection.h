// ========================================================================== //
//
//  ShaderCollection.h
//  ---
//  GL shader wrappers
//
//	* In one of the header files declare:
//  
//    YUP_BEGIN_DECLARE_SHADER_COLLECTION(Shader)
//    YUP_DECLARE_SHADER_PROGRAM(yup::gl::SolidProgram, Solid)
//    ...
//    YUP_END_DECLARE_SHADER_COLLECTION()
//
//
//	* In one of the sources files define:
//  
//    YUP_BEGIN_DEFINE_SHADER_COLLECTION(Shader)
//    YUP_DEFINE_SHADER_PROGRAM(yup::gl::SolidProgram, Solid)
//    ...
//    YUP_END_DEFINE_SHADER_COLLECTION()
//
//  * Stock shader programs:
//
//    - SolidProgram
//    - ColorProgram
//    - PosProgram
//    - RgbProgram
//    - RgbScaleProgram
//    - StereoProgram
//
//  Created: 2016-08-24
//  Updated: 2016-09-01
//
//  (C) 2016 Yu-hsien Chang
//
// ========================================================================== //

#pragma once

#ifdef YUP_INCLUDE_GLEW

#include <vector>
#include <string>

#include "yup.h"
#include "inc_sdl.h"
#include "ShaderSource.h"
#include "glutil.h"
#include "Matrices.h"

#define TEXTURE_RGB			GL_TEXTURE0
#define TEXTURE_YUV_Y		GL_TEXTURE0
#define TEXTURE_YUV_U		GL_TEXTURE1
#define TEXTURE_YUV_V		GL_TEXTURE2
#define TEXTURE_DEPTH		GL_TEXTURE3

#define UVMODE_FULL					0
#define UVMODE_TOP_HALF				1
#define UVMODE_BOTTOM_HALF			2
#define UVMODE_LEFT_HALF			3
#define UVMODE_RIGHT_HALF			4
#define UVMODE_RGB_OVER_DEPTH		5

#define COLORMODE_RGB				0
#define COLORMODE_YUV				1

#define DEPTHMODE_NONE				0
#define DEPTHMODE_LINEAR			1
#define DEPTHMODE_REVERSED_LINEAR	2
#define DEPTHMODE_INVERSE			3

BEGIN_NAMESPACE_YUP_GL

//============================================================================//
//
// GL Uniform Wrappers
//
//============================================================================//

struct GlUniformLoc
{
	GLint loc = 0;
};

struct GlUniformInt : public GlUniformLoc
{
	void operator=(int val) {
		glUniform1i(loc, val);
	}
};

struct GlUniformFloat : public GlUniformLoc
{
	void operator=(float val) {
		glUniform1f(loc, val);
	}
};

struct GlUniformVec4 : public GlUniformLoc
{
	void operator=(const Vector4 &color) {
		glUniform4f(loc, color.x, color.y, color.z, color.w);
	}
};

struct GlUniformMat4 : public GlUniformLoc
{
	void operator=(const Matrix4 & mat) {
		glUniformMatrix4fv(loc, 1, GL_FALSE, mat.get());
	}
};


//============================================================================//
//
// Shader Programs
//
//============================================================================//

class ShaderProgram
{
public:
	GLuint Id = 0;

private:
	std::string mName;
	std::string mVertShader;
	std::string mFragShader;

public:
	ShaderProgram(const char *name, const char *vertShader, const char *fragShader) {
		mName = name;
		mVertShader = vertShader;
		mFragShader = fragShader;
	}

	bool compile() {
		Id = CompileGLShader(mName.c_str(), mVertShader.c_str(), mFragShader.c_str());
		mVertShader.clear();
		mFragShader.clear();
		
		if (Id == 0)
			return false;

		use();
		onCompile();
		unuse();

		return true;
	}

	void cleanup() {
		if (Id)
		{
			onCleanup();
			glDeleteProgram(Id);
			Id = 0;
		}
	}

	GLint getLoc(const char *uniformName) {
		return glGetUniformLocation(Id, uniformName);
	}

	void use() {
		glUseProgram(Id);
	}

	void unuse() {
		glUseProgram(0);
	}

	virtual void onCompile() {};
	virtual void onCleanup() {};

};

//----------------------------------------------------------------------------//
// One solid color for all vertices
//
// - vec4 VertexArray(loc 0) : vertex position
// - Mat4 MvpMatrix : MVP matrix
// - Vec4 Color : color for all vertices
//----------------------------------------------------------------------------//
class SolidProgram : public ShaderProgram
{
public:
	GlUniformMat4 MvpMatrix;
	GlUniformVec4 Color;

public:
	SolidProgram() : ShaderProgram("Solid", SIMPLE_VERTEX_SHADER, SOLID_FRAGMENT_SHADER) {}

	virtual void onCompile() override {
		MvpMatrix.loc = getLoc("matrix");
		Color.loc = getLoc("color");
	}
};


//----------------------------------------------------------------------------//
// Individual color for every vertices
//
// - vec4 VertexArray(loc 0) : vertex position
// - vec4 VertexArray(loc 1) : vertex color
// - Mat4 MvpMatrix : MVP matrix
//----------------------------------------------------------------------------//
class ColorProgram : public ShaderProgram
{
public:
	GlUniformMat4 MvpMatrix;

public:
	ColorProgram() : ShaderProgram("Color", COLOR_VERTEX_SHADER, COLOR_FRAGMENT_SHADER) {}

	virtual void onCompile() override {
		MvpMatrix.loc = getLoc("matrix");
	}
};


//----------------------------------------------------------------------------//
// Color based on world position
//
// - vec4 VertexArray(loc 0) : vertex position
// - Mat4 MvpMatrix : MVP matrix
//----------------------------------------------------------------------------//
class PosProgram : public ShaderProgram
{
public:
	GlUniformMat4 MvpMatrix;

public:
	PosProgram() : ShaderProgram("Pos", POS_VERTEX_SHADER, POS_FRAGMENT_SHADER) {}

	virtual void onCompile() override {
		MvpMatrix.loc = getLoc("matrix");
	}
};

//----------------------------------------------------------------------------//
// Color based on z-depth
//
// - vec4 VertexArray(loc 0) : vertex position
// - Mat4 MvpMatrix : MVP matrix
//----------------------------------------------------------------------------//
class DepthProgram : public ShaderProgram
{
public:
	GlUniformMat4 MvpMatrix;

public:
	DepthProgram() : ShaderProgram("Depth", SIMPLE_VERTEX_SHADER, DEPTH_FRAGMENT_SHADER) {}

	virtual void onCompile() override {
		MvpMatrix.loc = getLoc("matrix");
	}
};

//----------------------------------------------------------------------------//
// Maps a mono RGB texture
//
// - vec4 VertexArray(loc 0) : vertex position
// - vec2 VertexArray(loc 1) : vertex UV
// - Mat4 MvpMatrix : MVP matrix
// - TEXTURE_RGB : RGB texture
//----------------------------------------------------------------------------//
class RgbProgram : public ShaderProgram
{
public:
	GlUniformMat4 MvpMatrix;

public:
	RgbProgram() : ShaderProgram("Rgb", UV_VERTEX_SHADER, RGB_FRAGMENT_SHADER) {}

	virtual void onCompile() override {
		MvpMatrix.loc = getLoc("matrix");

		glUniform1i(getLoc("tex0"), 0);
	}
};

//----------------------------------------------------------------------------//
// Maps a scaled mono RGB texture
//
// - vec4 VertexArray(loc 0) : vertex position
// - vec2 VertexArray(loc 1) : vertex UV
// - Mat4 MvpMatrix : MVP matrix
// - Float UScale : U scale factor
// - Float VScale : V scale factor
// - TEXTURE_RGB : RGB texture
//----------------------------------------------------------------------------//
class RgbScaleProgram : public ShaderProgram
{
public:
	GlUniformMat4 MvpMatrix;
	GlUniformFloat UScale;
	GlUniformFloat VScale;

public:
	RgbScaleProgram() : ShaderProgram("RgbScale", UV_SCALE_VERTEX_SHADER, RGB_FRAGMENT_SHADER) {}

	virtual void onCompile() override {
		MvpMatrix.loc = getLoc("matrix");
		UScale.loc = getLoc("u_scale");
		VScale.loc = getLoc("v_scale");
	}
};

//----------------------------------------------------------------------------//
// Maps a mono or stereo texture
// Texture can be RGB or HSV
//
// - vec4 VertexArray(loc 0) : vertex position
// - vec2 VertexArray(loc 1) : vertex UV
// - Mat4 MvpMatrix : MVP matrix
// - Int UVMode : MVP matrix
// - Int ColorMode : COLORMODE_RGB or COLORMODE_HSV
// - Float Opacity : Opacity of the texture
// - Int ClipOutOfBound : if not 0 then out of bound pixels will be discarded
// - TEXTURE_RGB : RGB texture
// - TEXTURE_YUV_Y : Y comopnent of YUV texture
// - TEXTURE_YUV_U : U comopnent of YUV texture
// - TEXTURE_YUV_V : V comopnent of YUV texture
//----------------------------------------------------------------------------//
class StereoProgram : public ShaderProgram
{
public:
	GlUniformMat4 MvpMatrix;
	GlUniformInt UVMode;
	GlUniformInt ColorMode;
	GlUniformFloat Opacity;
	GlUniformInt ClipOutOfBound;

public:
	StereoProgram() : ShaderProgram("Stereo", STEREO_VERTEX_SHADER, STEREO_RGBHSV_FRAGMENT_SHADER) {}

	virtual void onCompile() override {

		glUniform1i(getLoc("tex0"), 0);
		glUniform1i(getLoc("tex1"), 1);
		glUniform1i(getLoc("tex2"), 2);
		
		MvpMatrix.loc = getLoc("matrix");
		UVMode.loc = getLoc("uv_mode");
		ColorMode.loc = getLoc("color_mode");
		Opacity.loc = getLoc("opacity");
		ClipOutOfBound.loc = getLoc("clip_out_of_bound");
	}
};

END_NAMESPACE_YUP_GL


//============================================================================//
//
// ShaderCollection
//
//============================================================================//

//----------------------------------------------------------------------------//
// Declare ShaderCollection class
//----------------------------------------------------------------------------//

#define YUP_BEGIN_DECLARE_SHADER_COLLECTION(name) \
extern class ShaderCollection name; \
class ShaderCollection \
{ \
public: \
	ShaderCollection(); \
	bool Compile(); \
	void Cleanup(); \
private: \
	std::vector<yup::gl::ShaderProgram *> mPrograms;


#define YUP_DECLARE_SHADER_PROGRAM(program, name) \
public: program name;


#define YUP_END_DECLARE_SHADER_COLLECTION() };

//----------------------------------------------------------------------------//
// Define ShaderCollection class
//----------------------------------------------------------------------------//

#define YUP_BEGIN_DEFINE_SHADER_COLLECTION(name) \
ShaderCollection name; \
bool ShaderCollection::Compile() { \
	for (yup::gl::ShaderProgram *program : mPrograms) \
	{ \
		if (!program->compile()) \
			return false; \
	} \
	return true; \
} \
void ShaderCollection::Cleanup() { \
	for (yup::gl::ShaderProgram *program : mPrograms) \
		program->cleanup(); \
}\
ShaderCollection::ShaderCollection() {

#define YUP_DEFINE_SHADER_PROGRAM(program, name) \
mPrograms.push_back(static_cast<yup::gl::ShaderProgram *>(&name));

#define YUP_END_DEFINE_SHADER_COLLECTION() }


#endif // YUP_INCLUDE_GLEW
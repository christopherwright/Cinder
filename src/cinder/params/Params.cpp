/*
 Copyright (c) 2010, The Barbarian Group
 All rights reserved.

 Redistribution and use in source and binary forms, with or without modification, are permitted provided that
 the following conditions are met:

    * Redistributions of source code must retain the above copyright notice, this list of conditions and
	the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and
	the following disclaimer in the documentation and/or other materials provided with the distribution.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
 WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
*/

#include "cinder/app/App.h"
#include "cinder/params/Params.h"

#include "AntTweakBar.h"

using namespace std;

namespace cinder { namespace params {

class TweakBarListener : public app::App::Listener {
 public:
	TweakBarListener( app::App *app )
	{
		app->addListener( this );
	}
	
	bool mouseDown( app::MouseEvent event )
	{
		TwMouseButtonID button;
		if( event.isLeft() )
			button = TW_MOUSE_LEFT;
		else if( event.isRight() )
			button = TW_MOUSE_RIGHT;
		else
			button = TW_MOUSE_MIDDLE;
		TwMouseButton( TW_MOUSE_PRESSED, button );
		return false;
	}
	
	bool mouseUp( app::MouseEvent event )
	{
		TwMouseButtonID button;
		if( event.isLeft() )
			button = TW_MOUSE_LEFT;
		else if( event.isRight() )
			button = TW_MOUSE_RIGHT;
		else
			button = TW_MOUSE_MIDDLE;
		TwMouseButton( TW_MOUSE_RELEASED, button );
		return false;
	}
	
	bool mouseMove( app::MouseEvent event )
	{
		TwMouseMotion( event.getX(), event.getY() );
		return false;
	}
	
	bool mouseDrag( app::MouseEvent event )
	{
		TwMouseMotion( event.getX(), event.getY() );
		return false;
	}
	
	bool keyDown( app::KeyEvent event )
	{
		int kmod = 0;
		if( event.isShiftDown() )
			kmod |= TW_KMOD_SHIFT;
		if( event.isControlDown() )
			kmod |= TW_KMOD_CTRL;
		if( event.isAltDown() )
			kmod |= TW_KMOD_ALT;
		TwKeyPressed( event.getChar(), kmod );
		return false;
	}
	
	bool resize( int width, int height )
	{
		TwWindowSize(width, height);
		return false;
	}
};

namespace {

class AntMgr {
  public:
	AntMgr() {
		if( ! TwInit( TW_OPENGL, NULL ) ) {
			throw Exception();
		}
		
		new TweakBarListener( app::App::get() );	
	}
	
	~AntMgr() {
		TwTerminate();
	}
};

} // anonymous namespace

void initAntGl()
{
	static shared_ptr<AntMgr> mgr;
	if( ! mgr )
		mgr = shared_ptr<AntMgr>( new AntMgr );
}


InterfaceGl::InterfaceGl( const std::string &title, const Vec2i &size, const ColorA color )
{
	initAntGl();
	mBar = shared_ptr<TwBar>( TwNewBar( title.c_str() ), TwDeleteBar );
	char optionsStr[1024];
	sprintf( optionsStr, "%s size='%d %d' color='%d %d %d' alpha=%d", title.c_str(), size.x, size.y, (int)(color.r * 255), (int)(color.g * 255), (int)(color.b * 255), (int)(color.a * 255) );
	TwDefine( optionsStr );
}

void InterfaceGl::draw()
{
	TwDraw();
}

void InterfaceGl::implAddParam( const std::string &name, void *param, int type, const std::string &optionsStr, bool readOnly )
{
	if( readOnly )
		TwAddVarRO( mBar.get(), name.c_str(), (TwType)type, param, optionsStr.c_str() );
	else
		TwAddVarRW( mBar.get(), name.c_str(), (TwType)type, param, optionsStr.c_str() );
}

void InterfaceGl::addParam( const std::string &name, bool *param, const std::string &optionsStr, bool readOnly )
{
	implAddParam( name, param, TW_TYPE_BOOLCPP, optionsStr, readOnly );
} 

void InterfaceGl::addParam( const std::string &name, float *param, const std::string &optionsStr, bool readOnly )
{
	implAddParam( name, param, TW_TYPE_FLOAT, optionsStr, readOnly );
} 

void InterfaceGl::addParam( const std::string &name, int32_t *param, const std::string &optionsStr, bool readOnly )
{
	implAddParam( name, param, TW_TYPE_INT32, optionsStr, readOnly );
} 

void InterfaceGl::addParam( const std::string &name, Vec3f *param, const std::string &optionsStr, bool readOnly )
{
	implAddParam( name, param, TW_TYPE_DIR3F, optionsStr, readOnly );
} 

void InterfaceGl::addParam( const std::string &name, Quatf *param, const std::string &optionsStr, bool readOnly )
{
	implAddParam( name, param, TW_TYPE_QUAT4F, optionsStr, readOnly );
} 

void InterfaceGl::addParam( const std::string &name, Color *param, const std::string &optionsStr, bool readOnly )
{
	implAddParam( name, param, TW_TYPE_COLOR3F, optionsStr, readOnly );
} 

void InterfaceGl::addParam( const std::string &name, ColorA *param, const std::string &optionsStr, bool readOnly )
{
	implAddParam( name, param, TW_TYPE_COLOR4F, optionsStr, readOnly );
} 

void InterfaceGl::addParam( const std::string &name, std::string *param, const std::string &optionsStr, bool readOnly )
{
	implAddParam( name, param, TW_TYPE_STDSTRING, optionsStr, readOnly );
} 

void InterfaceGl::addSeparator( const std::string &name, const std::string &optionsStr )
{
	TwAddSeparator( mBar.get(), name.c_str(), optionsStr.c_str() );
}

} } // namespace cinder::params

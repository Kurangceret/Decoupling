#ifdef SFML_STATIC
#pragma comment(lib, "glew.lib")
#pragma comment(lib, "freetype.lib")
#pragma comment(lib, "jpeg.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "gdi32.lib")  
#endif // SFML_STATIC

#ifdef _CHECK_MEMORY_LEAKS_
#define _CRTDBG_MAP_ALLOC
#define _CRTDBG_MAP_ALLOC_NEW
#include <stdlib.h>
#include <crtdbg.h>
#endif   //_CHECK_MEMORY_LEAKS_

#include "Application.h"
#include <iostream>
#include <string>
#include <conio.h>

//#include <vld.h> 

extern "C" {
# include "lua.h"
# include "lauxlib.h"
# include "lualib.h"
};
#include <LuaBridge.h>
#include <LTBL\Light\Light_Point.h>
#include <LTBL\Light\LightSystem.h>
#include <LTBL\Light\ConvexHull.h>
#include <LTBL\Utils.h>
//using namespace luabridge;
int main()
{
	//lua_State* L = luaL_newstate();
	//luabridge::getGlobalNamespace(L).
	
	/*lua_State* L = luaL_newstate();
	luaL_dofile(L, "Scripts/script.lua");
	luaL_openlibs(L);
	lua_pcall(L, 0, 0, 0);
	LuaRef s = getGlobal(L, "testString");
	LuaRef n = getGlobal(L, "number");
	std::string luaString = s.cast<std::string>();
	int answer = n.cast<int>();
	std::cout << luaString << std::endl;
	std::cout << "And here's our number:" << answer << std::endl;
	_getch();*/
	Application game;
	try{
		game.run();
	}
	catch (std::exception e){
		std::cout << e.what() << std::endl;
	}
	//_CrtDumpMemoryLeaks();
	/*sf::VideoMode vidMode;
	vidMode.width = 800;
	vidMode.height = 600;
	vidMode.bitsPerPixel = 32;
	assert(vidMode.isValid());

	sf::RenderWindow win;
	win.create(vidMode, "Let there be Light - Demo");

	sf::View view;
	sf::Vector2u windowSize(win.getSize());
	view.setSize(sf::Vector2f(static_cast<float>(windowSize.x), static_cast<float>(windowSize.y)));
	view.setCenter(view.getSize() / 2.0f);

	// ---------------------- Background Image ---------------------
	
	sf::Texture backgroundImage;

	if (!backgroundImage.loadFromFile("Media/Textures/ShaderTexturesV1/background.png"))
		std::abort();

	// Tiling background
	backgroundImage.setRepeated(true);

	sf::Sprite backgroundSprite(backgroundImage);
	backgroundSprite.setTextureRect(sf::IntRect(0, 0, vidMode.width * 2, vidMode.height * 2));
	backgroundSprite.setPosition(-400.0f, -400.0f);

	// --------------------- Light System Setup ---------------------

	ltbl::LightSystem ls(AABB(Vec2f(0.0f, 0.0f), Vec2f(static_cast<float>(vidMode.width), 
		static_cast<float>(vidMode.height))), &win, 
		"Media/Textures/ShaderTexturesV1/lightFin.png", 
		"Media/Textures/ShaderTexturesV1/lightAttenuationShader.frag");

	// Create a light
	ltbl::Light_Point* testLight = new ltbl::Light_Point();
	testLight->m_intensity = 1.0f;
	testLight->m_center = Vec2f(200.0f, 200.0f);
	testLight->m_radius = 600.0f;
	testLight->m_size = 1.0f;
	testLight->m_spreadAngle = ltbl::pifTimes2;
	testLight->m_softSpreadAngle = 0.0f;
	testLight->CalculateAABB();

	testLight->m_bleed = 1.0f;
	testLight->m_linearizeFactor = 0.2f;

	ls.AddLight(testLight);

	testLight->SetAlwaysUpdate(true);*/

	// Create a light
	/*ltbl::Light_Point* testLight2 = new ltbl::Light_Point();
	testLight2->m_center = Vec2f(200.0f, 200.0f);
	testLight2->m_radius = 500.0f;
	testLight2->m_size = 0.0f;
	testLight2->m_color.r = 0.5f;
	testLight2->m_intensity = 0.0f;
	testLight2->m_spreadAngle = ltbl::pifTimes2;
	testLight2->m_softSpreadAngle = 30.f;
	testLight2->CalculateAABB();

	ls.AddLight(testLight2);

	testLight2->SetAlwaysUpdate(false);

	// Create an emissive light
	ltbl::EmissiveLight* emissiveLight = new ltbl::EmissiveLight();

	sf::Texture text;

	if (!text.loadFromFile("Media/Textures/ShaderTexturesV1/emissive.png"))
		std::abort();

	emissiveLight->SetTexture(&text);

	emissiveLight->SetRotation(45.0f);

	emissiveLight->m_intensity = 1.3f;

	ls.AddEmissiveLight(emissiveLight);

	emissiveLight->SetCenter(Vec2f(500.0f, 500.0f));

	// Create a hull by loading it from a file
	ltbl::ConvexHull* testHull = new ltbl::ConvexHull();

	if (!testHull->LoadShape("Media/Textures/ShaderTexturesV1/testShape.txt"))
		std::abort();

	// Pre-calculate certain aspects
	testHull->CalculateNormals();
	testHull->CalculateAABB();

	testHull->SetWorldCenter(Vec2f(300.0f, 300.0f));

	testHull->m_renderLightOverHull = true;

	ls.AddConvexHull(testHull);

	// ------------------------- Game Loop --------------------------

	sf::Event eventStructure;

	bool quit = false;

	ls.m_useBloom = true;

	while (!quit)
	{
		while (win.pollEvent(eventStructure))
		if (eventStructure.type == sf::Event::Closed)
		{
			quit = true;
			break;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			view.move(sf::Vector2f(-1.0f, 0.0f));
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			view.move(sf::Vector2f(1.0f, 0.0f));

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			view.move(sf::Vector2f(0.0f, -1.0f));
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			view.move(sf::Vector2f(0.0f, 1.0f));

		sf::Vector2f mousePos = win.mapPixelToCoords(sf::Mouse::getPosition(win));
		//testLight2->IncCenter(ltbl::Vec2f(0.1f, 0.0f));
		// Update light
		//testLight->SetCenter(Vec2f(mousePos.x, static_cast<float>(vidMode.height) - mousePos.y));

		win.clear();

		win.setView(view);
		ls.SetView(view);

		// Draw the background
		win.draw(backgroundSprite);

		// Calculate the lights
		ls.RenderLights();

		// Draw the lights
		ls.RenderLightTexture();

		//ls.DebugRender();

		win.display();
	}

	win.close();*/

	return 0;
}
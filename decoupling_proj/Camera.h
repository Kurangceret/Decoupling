#pragma once
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>



class Camera{
public:
	Camera(sf::RenderTarget& window ,float width, float height, 
		float speed = 1.f, sf::Vector2f mapSize = sf::Vector2f(0.f, 0.f));
	~Camera();

	//Moves camera immediately to coordinates
	void move(float x, float y);
	void moveCenter(float x, float y);
	//Sets camera target
	void goTo(float x, float y);
	void goToCenter(float x, float y);
	//get camera bounds in sf::FloatRect format
	sf::FloatRect getBoundingRect();

	void update(sf::Time dt);

	sf::View& getView(){ return mView; }

private:
	//Camera current position
	sf::Vector2f mPosition;
	//target's position, camera will move to the position in a set of speed
	sf::Vector2f mTarget;
	//size of the camera
	sf::Vector2f mSize; 
	//speed of the camera
	float mSpeed;

	sf::RenderTarget& mWindow;

	sf::View mView;

	//world map size, in real pixel
	sf::Vector2f mMapSize;
};


#include "LoadingState.h"
#include "Utility.h"

LoadingState::LoadingState(StateStack& stack, Context context)
:State(stack, context),
mLoadingTask(context.generalData)
{
	sf::RenderWindow& window = *getContext().window;
	sf::Font& font = *context.font;
	sf::Vector2f viewSize = window.getView().getSize();

	mLoadingText.setFont(font);
	mLoadingText.setCharacterSize(18);
	mLoadingText.setString("Loading Resources");
	Utility::centerOrigin(mLoadingText);
	mLoadingText.setPosition(viewSize.x / 2.f - 100.f, viewSize.y / 2.f + 50.f);

	mProgressBarBackground.setFillColor(sf::Color::White);
	mProgressBarBackground.setSize(sf::Vector2f(viewSize.x - 20, 10));
	mProgressBarBackground.setPosition(10, mLoadingText.getPosition().y + 40);

	mProgressBar.setFillColor(sf::Color(100, 100, 100));
	mProgressBar.setSize(sf::Vector2f(200, 10));
	mProgressBar.setPosition(10, mLoadingText.getPosition().y + 40);

	setCompletion(0.f, "Loading Resources");

	mLoadingTask.execute();


	ltbl::LightSystem* lightSystem = new ltbl::LightSystem();

	lightSystem->Create(AABB(Vec2f(-2000.f, -2000.f), Vec2f(800.f + 2000.f, 600.f + 2000.f))
		, context.window,
		"Media/Textures/ShaderTexturesV1/lightFin.png",
		"Media/Textures/ShaderTexturesV1/lightAttenuationShader.frag");

	context.generalData->setLightSystem(lightSystem);

}


LoadingState::~LoadingState()
{
}


void LoadingState::draw()
{
	sf::RenderWindow& window = *getContext().window;
	window.setView(window.getDefaultView());
	window.draw(mLoadingText);
	window.draw(mProgressBarBackground);
	window.draw(mProgressBar);
}
bool LoadingState::update(sf::Time)
{
	// Update the progress bar from the remote task or finish it
	if (mLoadingTask.isFinished()){
		requestStackPop();
		requestStackPush(States::Game);
		requestStackPush(States::GuiGame);
	}
	else if (mLoadingTask.isThreadTerminated()){
		requestStackPop();
		requestStackPush(States::Menu);
	}
	else{
		setCompletion(mLoadingTask.getCompletion(), mLoadingTask.getCurrentText());
	}
	return true;
}
bool LoadingState::handleEvent(const sf::Event& event)
{
	return true;
}
void LoadingState::setCompletion(float percent, std::string currentProcess)
{
	if (percent > 1.f) // clamp
		percent = 1.f;

	mProgressBar.setSize(sf::Vector2f(mProgressBarBackground.getSize().x * percent, mProgressBar.getSize().y));
	mLoadingText.setString(currentProcess);
}
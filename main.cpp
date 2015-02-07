#include <SFML/Graphics.hpp>

using namespace sf;

void draw(RenderWindow* window);
void update(sf::Time timeElapsed);
void checkEvents(RenderWindow* window);
void getInput();

// Constants:
const Vector2i screenSize(1920, 1080);
const double PI = 3.14159265359;

class Player : public sf::Drawable, public sf::Transformable
{
public:
	Player(int size, int noseSize, sf::Color mainColor, sf::Color noseColor)
	{
		mRotation = 0.0f;
		moveSpeed = Vector2f(0,0);
		mySize = size;
		myNoseSize = noseSize;
		vertices.setPrimitiveType(Triangles);
		vertices.resize(6);

		vertices[0].position = Vector2f(0,0);
		vertices[1].position = Vector2f(size, (size / 2));
		vertices[2].position = Vector2f(0, size);
		for (int i = 0; i < 3; i++) {
			vertices[i].color = mainColor;
		}

		vertices[3].position = Vector2f((size - noseSize)
										, ((size / 2) - (noseSize / 2)));
		vertices[4].position = Vector2f(size, (size / 2));
		vertices[5].position = Vector2f((size - noseSize)
										, ((size / 2) + (noseSize / 2)));
		for (int i = 3; i < 6; i++) {
			vertices[i].color = noseColor;
		}
	}

	void update(sf::Time timeElapsed)
	{
		moveSpeed *= friction;
		move(moveSpeed.x, moveSpeed.y);
	}

	void addSpeed(int direction)
	{
		float extraSpeed = direction * speed;
		moveSpeed += Vector2f(std::cos((mRotation / 180) * PI) * extraSpeed
							  , std::sin((mRotation / 180) * PI) * extraSpeed);
	}

	void rotateP(int direction)
	{
		float rotation = rotateSpeed * direction;
		if (mRotation + rotation < 0) {
			mRotation = 360 + (mRotation + rotation);
		} else if (mRotation + rotation > 360) {
			mRotation = (mRotation + rotation) - 360;
		} else {
			mRotation += rotation;
		}
		setRotation(mRotation);
	}

	float getRotation() {
		return mRotation;
	}

	int getSize() {
		return mySize;
	}

private:
	virtual void draw(RenderTarget& target, RenderStates states) const
	{
		states.transform *= getTransform();
		target.draw(vertices, states);
	}

	const int rotateSpeed = 5;
	const float friction = 0.97, speed = 0.6f;
	
	sf::VertexArray vertices;
	int mySize, myNoseSize;
	float mRotation;
	Vector2f moveSpeed;
};

Player mainPlayer(50, 10, sf::Color::Red, sf::Color(255, 102, 0));

int main()
{
	ContextSettings settings;
	settings.antialiasingLevel = 8;
	RenderWindow window(VideoMode(screenSize.x, screenSize.y)
						, "Simple Shooter", Style::Default, settings);
	window.setVerticalSyncEnabled(true);
	mainPlayer.setOrigin(mainPlayer.getSize() / 3, mainPlayer.getSize() / 2);
	mainPlayer.setPosition(screenSize.x / 2, screenSize.y / 2);
	Clock clock;
	while (window.isOpen()) {
		checkEvents(&window);
		update(clock.restart());
		draw(&window);
	}
}

void draw(RenderWindow* window)
{
	window->clear();
	window->draw(mainPlayer);
	window->display();
}

void checkEvents(RenderWindow* window)
{
	Event event;
	while (window->pollEvent(event)) {
		// Check and react to events
		if (event.type == Event::Closed) {
			window->close();
		}
	}
}

void update(sf::Time timeElapsed)
{
	getInput();
	mainPlayer.update(timeElapsed);
}

void getInput()
{
	// Turn left
	if ((Keyboard::isKeyPressed(Keyboard::Left)
		|| Keyboard::isKeyPressed(Keyboard::A))
		&& !(Keyboard::isKeyPressed(Keyboard::Right)
			 || Keyboard::isKeyPressed(Keyboard::D))) {
		mainPlayer.rotateP(-1);		
	}
	// Turn right
	if ((Keyboard::isKeyPressed(Keyboard::Right)
		|| Keyboard::isKeyPressed(Keyboard::D))
		&& !(Keyboard::isKeyPressed(Keyboard::Left)
			 || Keyboard::isKeyPressed(Keyboard::A))) {
		mainPlayer.rotateP(1);		
	}
	// Go forward
	if ((Keyboard::isKeyPressed(Keyboard::Up)
		 || Keyboard::isKeyPressed(Keyboard::W))
		&& !(Keyboard::isKeyPressed(Keyboard::Down)
			 || Keyboard::isKeyPressed(Keyboard::S))) {
		mainPlayer.addSpeed(1);
	}
	// Go backward
	if ((Keyboard::isKeyPressed(Keyboard::Down)
		 || Keyboard::isKeyPressed(Keyboard::S))
		&& !(Keyboard::isKeyPressed(Keyboard::Up)
			 || Keyboard::isKeyPressed(Keyboard::W))) {
		mainPlayer.addSpeed(-1);
	}
}

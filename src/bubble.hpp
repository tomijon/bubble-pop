#pragma once

#include <SFML/Graphics.hpp>

#include "menu.hpp"

/**
 * @class InputCharacter.
 * @brief A class for translating a key into a character.
 * 
 * Stores a character and a key assigned to that character.
 * Character used to display which key to press on the bubble
 * to pop it.
 */
class InputCharacter {
public:
	/**
	 * InputCharacter constructor.
	 * 
	 * Creates a relationship between a character and an sf::Keyboard::Key
	 * 
	 * @param character	The character that will be used to represent the key.
	 * @param key The key that is represented by the character.
	 */
	constexpr InputCharacter(char character, sf::Keyboard::Key key) : character(character), key(key) {}

	/**
	 * @brief Get the key from this class.
	 * 
	 * @return sf::Keyboard::Key stored in this class.
	 */
	inline sf::Keyboard::Key GetKey() const { return key; }

	/**
	 * @brief Get the character from this class.
	 * 
	 * @return char stored in this class.
	 */
	inline char GetChar() const { return character; }

private:
	const char character;
	const sf::Keyboard::Key key;
};


struct Coordinate {
	Coordinate() : x(0), y(0) {}
	Coordinate(float x, float y) : x(x), y(y) {}

	float x = 0;
	float y = 0;

	Coordinate operator+(const Coordinate& other) const {
		Coordinate sum;
		sum.x = x + other.x;
		sum.y = y + other.y;
		return sum;
	}

	Coordinate operator-(const Coordinate& other) const {
		Coordinate sum;
		sum.x = x - other.x;
		sum.y = y - other.y;
		return sum;
	}

	Coordinate operator*(const float& f) const {
		Coordinate copy(x * f, y * f);
		return copy;
	}
};


constexpr std::array<InputCharacter, 26> AvailableCharacters{
	InputCharacter('A', sf::Keyboard::Key::A),
	InputCharacter('B', sf::Keyboard::Key::B),
	InputCharacter('C', sf::Keyboard::Key::C),
	InputCharacter('D', sf::Keyboard::Key::D),
	InputCharacter('E', sf::Keyboard::Key::E),
	InputCharacter('F', sf::Keyboard::Key::F),
	InputCharacter('G', sf::Keyboard::Key::G),
	InputCharacter('H', sf::Keyboard::Key::H),
	InputCharacter('I', sf::Keyboard::Key::I),
	InputCharacter('J', sf::Keyboard::Key::J),
	InputCharacter('K', sf::Keyboard::Key::K),
	InputCharacter('L', sf::Keyboard::Key::L),
	InputCharacter('M', sf::Keyboard::Key::M),
	InputCharacter('N', sf::Keyboard::Key::N),
	InputCharacter('O', sf::Keyboard::Key::O),
	InputCharacter('P', sf::Keyboard::Key::P),
	InputCharacter('Q', sf::Keyboard::Key::Q),
	InputCharacter('R', sf::Keyboard::Key::R),
	InputCharacter('S', sf::Keyboard::Key::S),
	InputCharacter('T', sf::Keyboard::Key::T),
	InputCharacter('U', sf::Keyboard::Key::U),
	InputCharacter('V', sf::Keyboard::Key::V),
	InputCharacter('W', sf::Keyboard::Key::W),
	InputCharacter('X', sf::Keyboard::Key::X),
	InputCharacter('Y', sf::Keyboard::Key::Y),
	InputCharacter('Z', sf::Keyboard::Key::Z),
};


/**
 * @class Bubble
 * @brief A class to represnt a poppable bubble.
 * 
 * The bubble class stores information about the position,
 * size and the character needed to pop the bubble.
 */
class Bubble {
public:
	/**
	 * Bubble constructor. A bubble must have these things.
	 * 
	 * @param character	The InputCharacter that will pop the bubble.
	 * @param radius The size of the bubble.
	 */
	Bubble(InputCharacter character, float radius, Coordinate position);

	/**
	 * Bubble Deconstructor.
	 * 
	 * Frees the circle and text objects.
	 */
	~Bubble() { }

	/**
	 * @brief Static method for creating a new random bubble.
	 * 
	 * Radius is generated between bubble min and max radius.
	 * InputCharacter is chosen at random from AvailableCharacters.
	 * 
	 * @return The generated bubble.
	 */
	static Bubble GenerateRandomBubble(sf::Vector2u dimensions);

	/**
	 * @brief Determines if the given key will pop the bubble.
	 * 
	 * @param key The key attempting to pop the bubble.
	 * @return True if the key matches the popping key.
	 */
	inline bool Match(sf::Keyboard::Key key) { return character.GetKey() == key; }

	/**
	 * @brief Assign a minimum value to the bubble's radius. 
	 * @param f Value for the minimum radius.
	 * 
	 * Use without parameters to reset to default.
	 */
	inline static void SetMinRadius(float f = 10.0f) { MIN_RADIUS = f; }

	/**
	 * @brief Assign a maximum value to the bubble's radius.
	 * @param f Value for the maximum radius.
	 * 
	 * Use without parameters to reset to default.
	 */
	inline static void SetMaxRadius(float f = 16.0f) { MAX_RADIUS = f; }

	/**
	 * @brief Update the state of the bubble.
	 * @param state The new bubble state.
	 */
	inline void SetActive(bool state) { active = state; }

	/**
	 * @brief Check if the bubble is active or not.
	 * @return True if the bubble is active.
	 */
	inline bool IsActive() const { return active; }

	/**
	 * @brief Draws a circle representing the bubble.
	 * @param render The window to draw to.
	 */
	void Draw(sf::RenderWindow& render);

private:
	InputCharacter character;
	float radius;
	Coordinate position;
	bool active;

	sf::Text* text;
	sf::CircleShape circle;

	inline static float MIN_RADIUS = 10.0f;
	inline static float MAX_RADIUS = 16.0f;
};


/**
 * @brief A struct to store bubble game settings.
 * 
 */
struct BubbleGameSettings {
	const float bubbleSpawnRate;
	const float scoreResetTime;
	const uint32_t scorePerBubble;
};


/**
 * @brief Stores possible states the game can be in.
 */
enum class BubbleGameState {
	RUNNING,
	GAMEOVER
};


/** @brief A class to handle game logic. */
class BubbleGame {
public:
	/**
	 * @brief Initialises the game.
	 * @param render sf::RenderWindow to draw to.
	 * @param settings BubbleGameSettings to load into the game.
	 */
	BubbleGame(sf::RenderWindow& render, const BubbleGameSettings& settings);

	/**
	 * @brief Deconstructs the object.
	 * 
	 * Deletes all the bubbles stored in the bubble array.
	 */
	~BubbleGame();

	/**
	 * @brief Determine if the key will pop the current active bubble.
	 * @param key sf::Keyboard::key to compare to the target bubble.
	 * 
	 * Compares the passed key to the one stored in the Bubble object's
	 * InputCharacter. Handles the result.
	 * 
	 * @see OnPopAttemptSuccess
	 * @see OnPopAttemptFailure
	 */
	void AttemptBubblePop(sf::Keyboard::Key key);

	/**
	 * @brief Updates the game state.
	 * @param dt float of time since last update in seconds.
	 * 
	 * Determines whether to spawn a bubble or to reset the current multiplier.
	 */
	void Update(float dt);

	/**
	 * @brief Draws the game.
	 * 
	 * Calls the draw function of every available bubble in reverse order.
	 * Also draws the score and score multiplier.
	 */
	void Draw();

	/**
	 * @brief Fetch the state of the game.
	 */
	inline BubbleGameState GetGameState() const { return state; }

	/**
	 * @brief Return the score of the game.
	 */
	uint32_t GetScore() { return score; }

private:
	void SpawnRandomBubble();
	void UpdateScoreMultiplier();
	void OnPopAttemptSuccess();
	void OnPopAttemptFailure();

	void UpdateScoreText();
	void UpdateMultiplierText();

	sf::RenderWindow& render;
	sf::Text* scoreText;
	sf::Text* multiplierText;

	BubbleGameState state;

	std::array<Bubble*, 32> bubbles;
	int frontBubble;	// Stores the position of the longest lived bubble in the array.
	int emptySpace;		// Stores the position of the next empty space in the array.
	int numBubbles;

	float bubbleSpawnRate;
	float lastSpawnedTime;
	float lastPoppedTime;
	float totalTime;

	uint32_t score;
	uint32_t scorePerBubble;
	float scoreMult;
	float scoreMultResetTime;
};

#include "bubble.hpp"


Bubble::Bubble(InputCharacter character, float radius, Coordinate position) : 
	character(character), 
	radius(radius),
	position(position),
	active(false),
	text(new sf::Text(font, "" + character.GetChar()))
{
	circle = sf::CircleShape();
	circle.setFillColor(sf::Color(180, 204, 207, 255));
	circle.setOutlineThickness(5.0f);
	circle.setOutlineColor(sf::Color(55, 126, 184));
	circle.setRadius(radius);
	circle.setPointCount(50);
	circle.setOrigin({ radius, radius });

	text->setFillColor(sf::Color(0, 0, 255, 255));
}


Bubble Bubble::GenerateRandomBubble(sf::Vector2u dimensions) {
	InputCharacter chosen = AvailableCharacters[rand() % AvailableCharacters.size()];

	float x = dimensions.x * (static_cast<float>(rand()) / static_cast<float>(RAND_MAX));
	float y = dimensions.y * (static_cast<float>(rand()) / static_cast<float>(RAND_MAX));
	Coordinate position(x, y);

	float radius = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	radius = (radius * (Bubble::MAX_RADIUS - Bubble::MIN_RADIUS)) + Bubble::MIN_RADIUS;

	Bubble bubble(chosen, radius, position);
	return bubble;
}


void Bubble::Draw(sf::RenderWindow& render) {
	circle.setPosition(sf::Vector2f(position.x, position.y));
	text->setPosition(sf::Vector2f(position.x + 10 - (radius / 2.0f), position.y - (radius / 2.0f)));
	text->setCharacterSize(radius);
	text->setString(sf::String(character.GetChar()));

	if (active) circle.setFillColor({ 55, 193, 212 });

	render.draw(circle);
	if (active) {
		render.draw(*text);
	}
}


BubbleGame::BubbleGame(sf::RenderWindow& render, const BubbleGameSettings& settings) :
	render(render), bubbleSpawnRate(settings.bubbleSpawnRate),
	scoreMultResetTime(settings.scoreResetTime),
	scorePerBubble(settings.scorePerBubble),
	frontBubble(0), emptySpace(0), numBubbles(0),
	score(0), scoreMult(1.0f),
	lastSpawnedTime(0), totalTime(0)
{
	// Init all to nullptr.
	for (Bubble*& bubble : bubbles) {
		bubble = nullptr;
	}

    scoreText = new sf::Text(font, "Score: 0");
    scoreText->setCharacterSize(40);
    scoreText->setPosition(sf::Vector2f(0, render.getSize().y - 50));

    multiplierText = new sf::Text (font, "Multiplier: 1.0x");
    multiplierText->setCharacterSize(40);
    multiplierText->setPosition(sf::Vector2f(render.getSize().x - multiplierText->getLocalBounds().size.x - 20, render.getSize().y - 50));

	SpawnRandomBubble();
	bubbles[0]->SetActive(true);
	state = BubbleGameState::RUNNING;
}


BubbleGame::~BubbleGame() {
	for (Bubble* bubble : bubbles) {
		delete bubble;
	}
}


void BubbleGame::AttemptBubblePop(sf::Keyboard::Key key) {
	if (bubbles.at(frontBubble) == nullptr) return;
	if (bubbles.at(frontBubble)->Match(key)) OnPopAttemptSuccess();
	else OnPopAttemptFailure();
}


void BubbleGame::Update(float dt) {
	totalTime += dt;

	if (totalTime - lastSpawnedTime >= bubbleSpawnRate) {
		SpawnRandomBubble();
		lastSpawnedTime = totalTime;
	}

	UpdateScoreMultiplier();
}


void BubbleGame::Draw() {
	render.draw(*scoreText);
	render.draw(*multiplierText);

	// Draw in reverse order so that main target bubble is always on top.
	for (int i = 0; i < numBubbles; i++) {
		int index = (emptySpace - 1 - i) % bubbles.size();
		bubbles[index]->Draw(render);
	}

}


void BubbleGame::SpawnRandomBubble() {
	if (numBubbles >= bubbles.size()) {
		state = BubbleGameState::GAMEOVER;
		return;
	}

	Bubble* random = new Bubble(Bubble::GenerateRandomBubble(render.getSize()));
	bubbles[emptySpace] = random;
	emptySpace = (emptySpace + 1) % bubbles.size();

	if (numBubbles == 0) random->SetActive(true);
	numBubbles++;

	bubbleSpawnRate *= 0.99;
}


void BubbleGame::UpdateScoreMultiplier() {
	if (totalTime - lastPoppedTime > scoreMultResetTime) {
		scoreMult = 1.0f;
		UpdateMultiplierText();
	}
}


void BubbleGame::OnPopAttemptSuccess() {
	score += static_cast<uint32_t>(scorePerBubble * scoreMult);

	delete bubbles.at(frontBubble);
	bubbles.at(frontBubble) = nullptr;
	frontBubble = (frontBubble + 1) % bubbles.size();
	scoreMult += 0.1f;
	numBubbles--;

	if (numBubbles > 0) bubbles[frontBubble]->SetActive(true);

	lastPoppedTime = totalTime;

	UpdateMultiplierText();
	UpdateScoreText();
}


void BubbleGame::OnPopAttemptFailure() {
	scoreMult = 1.0f;

	if (score < scorePerBubble) score = 0;
	else score -= scorePerBubble;
}


void BubbleGame::UpdateScoreText() {
	scoreText->setString("Score: " + std::to_string(score));
}


void BubbleGame::UpdateMultiplierText() {
	std::string value = std::to_string(scoreMult);
	std::string trimmed;

	// Removing trailing zeros the long way.
	for (int i = value.length() - 1; i >= 0; i--) {
	    if (value[i] == '0') continue;
	    else trimmed = value[i] + trimmed;
	}

	if (trimmed.back() == '.') trimmed += '0';
	
	multiplierText->setString("Multiplier: " + trimmed + "x");
}

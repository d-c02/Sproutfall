#pragma once
#include "SFML_Lib.h"
#include "Entity.h"
#include"Enemy.h"
#include "Shotgun.h"
#include "BulletManager.h"
#include "AnimationManager.h"
#include "ShellManager.h"

class Player : public sf::Drawable, public Entity
{
public:
	Player(sf::RenderWindow* window);
	~Player();
	void Shoot();
	void Update(float tf);
	void handleInput(sf::Event* event);
	sf::FloatRect getGlobalBounds();
	void configureAnimations();
	void setDirection();
	void CheckCollisions(Enemy* enemy);
	void Die();
	bool getStatus();
	sf::Vector2f getPosition();
	void SetShellColor(sf::Color color);
	void SetShellGravity(float gravity);
	bool IsScreenShaking();
	void SetFallingParams(float gravity, float terminalVelocity);
	void SetOutlineColor(sf::Glsl::Vec4 color);
	void SetWindow(sf::RenderWindow* window);
protected:

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	float m_accelerationX = 0;
	float m_accelerationY = 0;
	float m_terminalVelocity = 100;
	const float m_airResistance = -500;
	const float m_airResistanceX = -250;
	const float m_recoil = -600;
	float m_gravity = 500;
	float m_reloadProgress = 0;
	const int m_bulletsMax = 4;
	int m_bullets = m_bulletsMax;
	const float m_reloadDelay = 0.75;
	std::unique_ptr<Shotgun> m_Shotgun;
	float m_initialPositionX = 640;
	float m_initialPositionY = 200;
	std::unique_ptr<BulletManager> m_bulletManager;
	std::unique_ptr<AnimationManager> m_AnimationManager;
	//sf::IntRect* m_Hitbox;
	enum States {neutral, falling, falling_fast, hurt, dead};
	int m_CurrentState = neutral;
	vector<std::unique_ptr<sf::Sound>> m_ShotgunShootSounds;
	vector<std::unique_ptr<sf::SoundBuffer>>m_ShotgunShootBuffers;
	float neutralThreshold = 90;
	float fallingThreshold = 600;
	bool m_SpriteFlipped = false;
	std::unique_ptr<sf::CircleShape> m_Hitbox;
	const float m_CollisionSlowdown = 0.2;
	int m_Health = 3;
	sf::RenderWindow* m_RenderWindow;
	bool m_Alive = true;
	std::unique_ptr<ShellManager> m_ShellManager;
	bool m_ScreenShake = false;

	const std::string m_outlineShaderCode = "uniform sampler2D texture; " \
		"uniform vec4 color;									" \
		"                                                       " \
		"void main() {                                          " \
		"    vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);" \
		"	if (pixel.rgb == vec3(0.0) && pixel.a == 1.0f)		" \
		"	{                                                   " \
		"    pixel.r = color.r;									" \
		"    pixel.g = color.g;									" \
		"    pixel.b = color.b;									" \
		"	pixel.a = color.a;									" \
		"   }                                                   " \
		"    gl_FragColor = pixel;                              " \
		"}";
	std::unique_ptr<sf::Shader> m_outlineShader;
};
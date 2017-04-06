/*
Projectile class for shooting bullets
*/

#pragma once

#include "entity/entity.h"

#include "gfx/particle.h"

class Projectile : public Entity
{
public:
    Projectile();
    ~Projectile();

	//To refresh an entity's state
	void reload(Vector2f position, Vector2f velocity, float alpha, float speed);

	void render() const;
	void update(float deltaTime);

	inline void setActive(bool a) { m_active = a; }
	inline bool isActive() const { return m_active; }

	void loadTexture(char* filename) { m_texture.loadFromFile(filename, 64, 64, false); }

	void setScale(GLfloat xscale, GLfloat yscale);
	inline GLfloat getXScale() { return m_xScale; }
	inline GLfloat getYScale() { return m_yScale; }

    void setCollisionBox(GLuint width, GLuint height);
	inline void setCenteredBox(bool centered) { m_centeredBox = centered;  }

	inline void setColor(SDL_Color color) { m_color = color; }

	void setParticleEmitter(ParticleEmitter* particleEmitter);
private:
    int m_projectileSpeed;

    //Whether projectile is on screen or not
    bool m_active;

	//Whether the collisionbox comes from the center or starts from the top left
    bool m_centeredBox;

    SDL_Color m_color;
	ParticleEmitter* m_particleEmitter;

    GLuint m_baseWidth;
    GLuint m_baseHeight;
    GLfloat m_xScale;
    GLfloat m_yScale;
};

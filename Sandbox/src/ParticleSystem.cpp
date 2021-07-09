#include "ParticleSystem.h"

#include <glm/gtc/constants.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/compatibility.hpp>

#include <random>

class Random
{
public:
	static void Init()
	{
		s_RandomEngine.seed(std::random_device()());
	}

	static float Float()
	{
		return (float)s_Distribution(s_RandomEngine) / (float)std::numeric_limits<uint32_t>::max();
	}

private:
	static std::mt19937 s_RandomEngine;
	static std::uniform_int_distribution<std::mt19937::result_type> s_Distribution;
};

std::mt19937 Random::s_RandomEngine;
std::uniform_int_distribution<std::mt19937::result_type> Random::s_Distribution;

ParticleSystem::ParticleSystem(uint32_t maxParticle)
	:m_PoolIndex(maxParticle - 1)
{
	m_ParticlePool.resize(maxParticle);
}

void ParticleSystem::OnUpdate(ModernEngine::DeltaTime dt)
{
	for (auto& particle : m_ParticlePool)
	{
		if (!particle.Active)
			continue;

		if (particle.LifeRemaining <= 0.0f)
		{
			particle.Active = false;
			continue;
		}

		particle.LifeRemaining -= dt;
		particle.Position += particle.Velocity * (float)dt;
		particle.Rotation += 0.01f * dt;
	}
}

void ParticleSystem::OnRender(const ModernEngine::OrthographicCamera& camera)
{
	ModernEngine::Renderer2D::BeginScene(camera);
	for (auto& particle : m_ParticlePool)
	{
		if (!particle.Active)
			continue;

		float life = particle.LifeRemaining / particle.LifeTime;
		glm::vec4 color = glm::lerp(particle.ColorEnd, particle.ColorBegin, life);
		float size = glm::lerp(particle.SizeEnd, particle.SizeBegin, life);

		glm::vec3 position = { particle.Position.x, particle.Position.y, 0.2f };
		ModernEngine::Renderer2D::DrawRotateQuad(position, { size, size }, particle.Rotation, color);
	}
	ModernEngine::Renderer2D::EndScene();
}

void ParticleSystem::Emit(const ParticleProps& props)
{
	Particle& particle = m_ParticlePool[m_PoolIndex];
	particle.Active = true;
	particle.Position = props.Position;
	particle.Rotation = Random::Float() * 2.0f * glm::pi<float>();

	particle.Velocity = props.Velocity;
	particle.Velocity.x += props.VelocityVariation.x * (Random::Float() - 0.5f);
	particle.Velocity.y += props.VelocityVariation.y * (Random::Float() - 0.5f);

	particle.ColorBegin = props.ColorBegin;
	particle.ColorEnd = props.ColorEnd;

	particle.LifeTime = props.LifeTime;
	particle.LifeRemaining = props.LifeTime;
	particle.SizeBegin = props.SizeBegin + props.SizeVariation * (Random::Float() - 0.5f);
	particle.SizeEnd = props.SizeEnd;

	m_PoolIndex = --m_PoolIndex % m_ParticlePool.size();
}

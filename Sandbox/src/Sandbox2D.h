#pragma once

#include "ModernEngine.h"
#include "ParticleSystem.h"

class Sanbdox2D : public ModernEngine::Layer
{
public:
	Sanbdox2D();
	virtual ~Sanbdox2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnUpdate(ModernEngine::DeltaTime dt) override;
	virtual void OnEvent(ModernEngine::Event& e) override;
	virtual void OnImGuiRender() override;

private:
	ModernEngine::OrthographicCameraController m_CameraController;
	glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.0f, 1.0f };

	ModernEngine::Ref<ModernEngine::Texture2D> m_Checkerboard;

	ParticleSystem m_ParticleSystem;
	ParticleProps m_ParticleProps;
};

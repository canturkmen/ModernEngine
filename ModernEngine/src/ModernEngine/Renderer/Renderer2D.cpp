#include "mnpch.h"
#include "Renderer2D.h"

#include "VertexArray.h"
#include "Shader.h"

#include "RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>

namespace ModernEngine {

	// 36 bytes
	struct QuadVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		float TexIndex;
		float TilingFactor;

		// For editor
		int EntityID;
	};


	struct CircleQuadVertex
	{
		glm::vec3 WorldPosition;
		glm::vec3 LocalPosition;
		glm::vec4 Color;
		float Thickness; 
		float Fade;

		// For editor
		int EntityID;
	};

	struct LineVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
	
		// For editor
		int EntityID;
	};

	struct Renderer2DData
	{
		static const uint32_t MaxQuads = 20000;
		static const uint32_t MaxVertices = MaxQuads * 4;
		static const uint32_t MaxIndices = MaxQuads * 6;
		static const uint32_t MaxTextureSlots = 32;

		Ref<VertexArray> QuadVA;
		Ref<VertexBuffer> QuadVB;

		uint32_t QuadIndexCount = 0;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;

		Ref<VertexArray> CircleQuadVA;
		Ref<VertexBuffer> CircleQuadVB;

		uint32_t CircleQuadIndexCount = 0;
		CircleQuadVertex* CircleQuadVertexBufferBase = nullptr;
		CircleQuadVertex* CircleQuadVertexBufferPtr = nullptr;

		Ref<VertexArray> LineVA;
		Ref<VertexBuffer> LineVB;

		uint32_t LineVertexCount = 0;
		LineVertex* LineVertexBufferBase = nullptr;
		LineVertex* LineVertexBufferPtr = nullptr;

		float LineWidth = 2.0f;

		Ref<Shader> QuadShader;
		Ref<Shader> CircleShader;
		Ref<Shader> LineShader;
		Ref<Texture2D> WhiteTexture;

		std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotIndex = 1; // 0 is for white texture.

		glm::vec4 VertexPositions[4];

		Renderer2D::Statistics Stats;
	};

	static Renderer2DData s_Data;

	void Renderer2D::Init()
	{
		MN_PROFILE_FUNCTION();

		// Rendering Quads
		s_Data.QuadVA = VertexArray::Create();
		s_Data.QuadVB = VertexBuffer::Create(s_Data.MaxQuads * sizeof(QuadVertex));

		BufferLayout SquareLayout =
		{
			{ShaderDataType::Float3, "a_Position"},
			{ShaderDataType::Float4, "a_Color"},
			{ShaderDataType::Float2, "a_TextureCoord"},
			{ShaderDataType::Float,	"a_TexIndex"},
			{ShaderDataType::Float,	"a_TilingFactor"},
			{ShaderDataType::Int, "a_EntityID"}
		};

		s_Data.QuadVB->SetBufferLayout(SquareLayout);
		s_Data.QuadVA->AddVertexBuffer(s_Data.QuadVB);

		s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxVertices];
		uint32_t* QuadIndices = new uint32_t[s_Data.MaxIndices];

		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_Data.MaxIndices; i += 6)
		{
			QuadIndices[i + 0] = offset + 0;
			QuadIndices[i + 1] = offset + 1;
			QuadIndices[i + 2] = offset + 2;

			QuadIndices[i + 3] = offset + 2;
			QuadIndices[i + 4] = offset + 3;
			QuadIndices[i + 5] = offset + 0;

			offset += 4;
		}

		Ref<IndexBuffer> QuadIB = IndexBuffer::Create(QuadIndices, s_Data.MaxIndices);
		s_Data.QuadVA->SetIndexBuffer(QuadIB);
		delete[] QuadIndices;

		// Rendering Circles
		s_Data.CircleQuadVA = VertexArray::Create();
		s_Data.CircleQuadVB = VertexBuffer::Create(s_Data.MaxQuads * sizeof(CircleQuadVertex));

		BufferLayout CircleLayout =
		{
			{ShaderDataType::Float3, "a_World_Position"},
			{ShaderDataType::Float3, "a_LocalPosition"},
			{ShaderDataType::Float4, "a_Color"},
			{ShaderDataType::Float,	 "a_Thickness"},
			{ShaderDataType::Float,	 "a_Fade"},
			{ShaderDataType::Int,    "a_EntityID"}
		};

		s_Data.CircleQuadVB->SetBufferLayout(CircleLayout);
		s_Data.CircleQuadVA->AddVertexBuffer(s_Data.CircleQuadVB);
		s_Data.CircleQuadVA->SetIndexBuffer(QuadIB);

		s_Data.CircleQuadVertexBufferBase = new CircleQuadVertex[s_Data.MaxVertices];

		// Rendering lines
		s_Data.LineVA = VertexArray::Create();
		s_Data.LineVB = VertexBuffer::Create(s_Data.MaxQuads * sizeof(LineVertex));

		BufferLayout LineLayout =
		{
			{ShaderDataType::Float3, "a_Position"},
			{ShaderDataType::Float4, "a_Color"},
			{ShaderDataType::Int,    "a_EntityID"}
		};

		s_Data.LineVB->SetBufferLayout(LineLayout);
		s_Data.LineVA->AddVertexBuffer(s_Data.LineVB);

		s_Data.LineVertexBufferBase = new LineVertex[s_Data.MaxVertices];

		s_Data.WhiteTexture = Texture2D::Create(1, 1);
		uint32_t data = 0xffffffff;
		s_Data.WhiteTexture->SetData(&data, sizeof(uint32_t));

		int32_t samplers[s_Data.MaxTextureSlots];
		for (uint32_t i = 0; i < s_Data.MaxTextureSlots; i++)
			samplers[i] = i;


		s_Data.QuadShader = Shader::Create("assets/shaders/Renderer2D_Quad.glsl");
		s_Data.CircleShader = Shader::Create("assets/shaders/Renderer2D_Circle.glsl");
		s_Data.LineShader = Shader::Create("assets/shaders/Renderer2D_Line.glsl");

		s_Data.QuadShader->Bind();
		s_Data.QuadShader->SetIntArray("u_Texture", samplers, s_Data.MaxTextureSlots);

		s_Data.TextureSlots[0] = s_Data.WhiteTexture;

		s_Data.VertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.VertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.VertexPositions[2] = { 0.5f,  0.5f, 0.0f, 1.0f };
		s_Data.VertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };
	}

	void Renderer2D::ShutDown()
	{
		MN_PROFILE_FUNCTION();

		delete[] s_Data.QuadVertexBufferBase;
		delete[] s_Data.CircleQuadVertexBufferBase;
		delete[] s_Data.LineVertexBufferBase;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		MN_PROFILE_FUNCTION();

		s_Data.QuadShader->Bind();
		s_Data.QuadShader->SetMat4("u_ViewProjectionMatrix", camera.GetViewProjectionMatrix());

		s_Data.CircleShader->Bind();
		s_Data.CircleShader->SetMat4("u_ViewProjectionMatrix", camera.GetViewProjectionMatrix());

		s_Data.LineShader->Bind();
		s_Data.LineShader->SetMat4("u_ViewProjectionMatrix", camera.GetViewProjectionMatrix());

		StartBatch();
	}

	void Renderer2D::BeginScene(const Camera& camera, const glm::mat4& transform)
	{
		MN_PROFILE_FUNCTION();

		glm::mat4 viewProjectionMatrix = camera.GetProjectionMatrix() * glm::inverse(transform);

		s_Data.QuadShader->Bind();
		s_Data.QuadShader->SetMat4("u_ViewProjectionMatrix", viewProjectionMatrix);

		s_Data.CircleShader->Bind();
		s_Data.CircleShader->SetMat4("u_ViewProjectionMatrix", viewProjectionMatrix);

		s_Data.LineShader->Bind();
		s_Data.LineShader->SetMat4("u_ViewProjectionMatrix", viewProjectionMatrix);

		StartBatch();
	}

	void Renderer2D::BeginScene(EditorCamera& camera)
	{
		MN_PROFILE_FUNCTION();

		glm::mat4 viewProjectionMatrix = camera.GetViewProjectionMatrix();

		s_Data.QuadShader->Bind();
		s_Data.QuadShader->SetMat4("u_ViewProjectionMatrix", viewProjectionMatrix);

		s_Data.CircleShader->Bind();
		s_Data.CircleShader->SetMat4("u_ViewProjectionMatrix", viewProjectionMatrix);

		s_Data.LineShader->Bind();
		s_Data.LineShader->SetMat4("u_ViewProjectionMatrix", viewProjectionMatrix);

		StartBatch();
	}

	void Renderer2D::StartBatch()
	{
		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

		s_Data.CircleQuadIndexCount = 0; 
		s_Data.CircleQuadVertexBufferPtr = s_Data.CircleQuadVertexBufferBase;

		s_Data.LineVertexCount = 0;
		s_Data.LineVertexBufferPtr = s_Data.LineVertexBufferBase;

		s_Data.TextureSlotIndex = 1;
	}

	void Renderer2D::NextBatch()
	{
		Flush();
		StartBatch();
	}

	void Renderer2D::Flush()
	{
		if (s_Data.QuadIndexCount)
		{
			uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase);
			s_Data.QuadVB->SetData(s_Data.QuadVertexBufferBase, dataSize);

			for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
				s_Data.TextureSlots[i]->Bind(i);

			s_Data.QuadShader->Bind();
			RenderCommand::DrawIndexed(s_Data.QuadVA, s_Data.QuadIndexCount);
			s_Data.Stats.DrawCalls++;
		}
		
		if (s_Data.CircleQuadIndexCount)
		{
			uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.CircleQuadVertexBufferPtr - (uint8_t*)s_Data.CircleQuadVertexBufferBase);
			s_Data.CircleQuadVB->SetData(s_Data.CircleQuadVertexBufferBase, dataSize);

			s_Data.CircleShader->Bind();
			RenderCommand::DrawIndexed(s_Data.CircleQuadVA, s_Data.CircleQuadIndexCount);
			s_Data.Stats.DrawCalls++;
		}

		if (s_Data.LineVertexCount)
		{
			uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.LineVertexBufferPtr - (uint8_t*)s_Data.LineVertexBufferBase);
			s_Data.LineVB->SetData(s_Data.LineVertexBufferBase, dataSize);

			s_Data.LineShader->Bind();
			RenderCommand::SetLineWidth(s_Data.LineWidth);
			RenderCommand::DrawLines(s_Data.LineVA, s_Data.LineVertexCount);
			s_Data.Stats.DrawCalls++;
		}
	}

	void Renderer2D::EndScene()
	{
		MN_PROFILE_FUNCTION();

		Flush();
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		MN_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f));
		
		DrawQuad(transform, color);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float TilingFactor, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, texture, TilingFactor, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float TilingFactor, const glm::vec4& color)
	{
		MN_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f));

		DrawQuad(transform, texture, TilingFactor);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture2D>& subtexture, float TilingFactor /*= 1.0f*/, const glm::vec4& color /*= glm::vec4(1.0f)*/)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, subtexture, TilingFactor, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D>& subtexture, float TilingFactor /*= 1.0f*/, const glm::vec4& color /*= glm::vec4(1.0f)*/)
	{
		MN_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f));

		DrawQuad(transform, subtexture, TilingFactor);
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color, int entityID)
	{
		MN_PROFILE_FUNCTION();

		constexpr size_t QuadVertexCount = 4;
		constexpr glm::vec2 textureCoords[] = { {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f} };
		const float textureIndex = 0.0f; // White Texture
		const float tilingFactor = 1.0f;

		if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
			NextBatch();

		for (int i = 0; i < QuadVertexCount; i++)
		{
			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.VertexPositions[i];
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.QuadVertexBufferPtr->EntityID = entityID;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;
		s_Data.Stats.QuadCount++;
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& color /*= glm::vec4(1.0f)*/, int entityID)
	{
		MN_PROFILE_FUNCTION();

		constexpr size_t QuadVertexCount = 4;
		constexpr glm::vec2 textureCoords[] = { {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f} };

		if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
			NextBatch();

		float textureIndex = 0.0f;

		for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
		{
			if (s_Data.TextureSlots[i] == texture)
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			if (s_Data.TextureSlotIndex >= Renderer2DData::MaxTextureSlots)
				NextBatch();

			textureIndex = (float)s_Data.TextureSlotIndex;
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;
		}

		for (int i = 0; i < QuadVertexCount; i++)
		{
			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.VertexPositions[i];
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.QuadVertexBufferPtr->EntityID = entityID;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;
		s_Data.Stats.QuadCount++;
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const Ref<SubTexture2D>& subtexture, float tilingFactor, const glm::vec4& color /*= glm::vec4(1.0f)*/, int entityID)
	{
		constexpr size_t QuadVertexCount = 4;
		const glm::vec2* textureCoords = subtexture->GetTexCoords();
		const Ref<Texture2D> texture = subtexture->GetTexture();

		if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
			NextBatch();

		float textureIndex = 0.0f;

		for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
		{
			if (s_Data.TextureSlots[i] == texture)
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			if (s_Data.TextureSlotIndex >= Renderer2DData::MaxTextureSlots)
				NextBatch();

			textureIndex = (float)s_Data.TextureSlotIndex;
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;
		}

		for (int i = 0; i < QuadVertexCount; i++)
		{
			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.VertexPositions[i];
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.QuadVertexBufferPtr->EntityID = entityID;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;
		s_Data.Stats.QuadCount++;
	}

	void Renderer2D::DrawRotateQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		DrawRotateQuad({ position.x, position.y, 0.0f }, size, rotation, color);
	}

	void Renderer2D::DrawRotateQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		MN_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f)) *
			glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f));

		DrawQuad(transform, color);
	}

	void Renderer2D::DrawRotateQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float TilingFactor, const glm::vec4& color)
	{
		DrawRotateQuad({ position.x, position.y, 0.0f }, size, rotation, texture, TilingFactor, color);
	}

	void Renderer2D::DrawRotateQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float TilingFactor, const glm::vec4& color)
	{
		MN_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f)) *
			glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f));

		DrawQuad(transform, texture, TilingFactor, color);
	}

	void Renderer2D::DrawRotateQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<SubTexture2D>& subtexture, float TilingFactor /*= 1.0f*/, const glm::vec4& color /*= glm::vec4(1.0f)*/)
	{
		DrawRotateQuad({ position.x, position.y, 0.0f }, size, rotation, subtexture, TilingFactor, color);
	}

	void Renderer2D::DrawRotateQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<SubTexture2D>& subtexture, float TilingFactor /*= 1.0f*/, const glm::vec4& color /*= glm::vec4(1.0f)*/)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f));

		DrawQuad(transform, subtexture, TilingFactor, color);
	}

	void Renderer2D::DrawLine(const glm::vec3& p0,  glm::vec3& p1, const glm::vec4& color, int entityID)
	{
		s_Data.LineVertexBufferPtr->Position = p0;
		s_Data.LineVertexBufferPtr->Color = color;
		s_Data.LineVertexBufferPtr->EntityID = entityID;
		s_Data.LineVertexBufferPtr++;

		s_Data.LineVertexBufferPtr->Position = p1;
		s_Data.LineVertexBufferPtr->Color = color;
		s_Data.LineVertexBufferPtr->EntityID = entityID;
		s_Data.LineVertexBufferPtr++;

		s_Data.LineVertexCount += 2;
	}

	void Renderer2D::SetLineWidth(float width)
	{
		s_Data.LineWidth = width;
	}

	float Renderer2D::GetLineWidth()
	{
		return s_Data.LineWidth;
	}

	void Renderer2D::DrawSprite(const glm::mat4& transform, SpriteRendererComponent& comp, int entityID)
	{
		if (comp.Texture)
			DrawQuad(transform, comp.Texture, comp.TilingFactor, comp.Color, entityID);
		else
			DrawQuad(transform, comp.Color, entityID);
	}


	void Renderer2D::DrawRect(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, int entityID /*= -1*/)
	{
		glm::vec3 p0 = glm::vec3(position.x - size.x, position.y - size.y, 0.0f);
		glm::vec3 p1 = glm::vec3(position.x + size.x, position.y - size.y, 0.0f);
		glm::vec3 p2 = glm::vec3(position.x + size.x, position.y + size.y, 0.0f);
		glm::vec3 p3 = glm::vec3(position.x - size.x, position.y + size.y, 0.0f);

		DrawLine(p0, p1, color);
		DrawLine(p1, p2, color);
		DrawLine(p2, p3, color);
		DrawLine(p3, p0, color);
	}

	void Renderer2D::DrawRect(const glm::mat4& transform, const glm::vec4& color, int entityID /*= -1*/)
	{
		glm::vec3 LineVertices[4];

		for (size_t i = 0; i < 4; i++)
			LineVertices[i] = transform * s_Data.VertexPositions[i];

		DrawLine(LineVertices[0], LineVertices[1], color);
		DrawLine(LineVertices[1], LineVertices[2], color);
		DrawLine(LineVertices[2], LineVertices[3], color);
		DrawLine(LineVertices[3], LineVertices[0], color);
	}

	void Renderer2D::DrawCircle(const glm::mat4& transform, const glm::vec4& color, float thickness, float fade, int entityID)
	{
		MN_PROFILE_FUNCTION();
		constexpr size_t CircleQuadVertexCount = 4;

		for (int i = 0; i < CircleQuadVertexCount; i++)
		{
			s_Data.CircleQuadVertexBufferPtr->WorldPosition = transform * s_Data.VertexPositions[i];
			s_Data.CircleQuadVertexBufferPtr->LocalPosition = s_Data.VertexPositions[i] * 2.0f;
			s_Data.CircleQuadVertexBufferPtr->Color = color;
			s_Data.CircleQuadVertexBufferPtr->Thickness = thickness;
			s_Data.CircleQuadVertexBufferPtr->Fade = fade;
			s_Data.CircleQuadVertexBufferPtr->EntityID = entityID;
			s_Data.CircleQuadVertexBufferPtr++;
		}

		s_Data.CircleQuadIndexCount += 6;
		s_Data.Stats.QuadCount++;
	}

	void Renderer2D::ResetStats()
	{
		memset(&s_Data.Stats, 0, sizeof(Statistics));
	}

	Renderer2D::Statistics Renderer2D::GetStats()
	{
		return s_Data.Stats;
	}
}
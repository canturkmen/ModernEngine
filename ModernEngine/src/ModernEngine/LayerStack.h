#pragma once

#include "Core.h"
#include "Layer.h"
#include <vector>

namespace ModernEngine {

	class MN_API LayerStack
	{
	public:
		LayerStack();
		~LayerStack();

		void PushOverlay(Layer* overlay);
		void PushLayer(Layer* layer);
		void PopLayer(Layer* layer);
		void PopOverlay(Layer* overlay);

		std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
		std::vector<Layer*>::iterator end() { return m_Layers.end(); }

	private:
		std::vector<Layer*> m_Layers;
		std::vector<Layer*>::iterator m_LayerInsert;
	};
}


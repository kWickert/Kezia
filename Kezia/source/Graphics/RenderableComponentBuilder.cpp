#include "RenderableComponentBuilder.h"

#include "Core/AttachmentTarget.h"
#include "Core/XmlObjectBuilder.h"

#include "Graphics/StaticMeshComponent.h"
#include "Graphics/IRenderer.h"

namespace Kezia
{
	template<typename Component_T>
	static void * BuildRenderableObject(const XmlNode & node)
	{
		Component_T * component = new Component_T(node);

		component->Attach(GetAttachmentTarget());

		//we're making an archetype don't actually manage it
		g_Renderer->RemoveRenderableComponent(component);

		return component;
	}

	static XmlObjectBuilder * CreateRenderableComponentBuilder()
	{
		XmlObjectBuilder * renderableComponentBuilder = new XmlObjectBuilder();

		renderableComponentBuilder->RegisterObjectBuilder("StaticMesh", BuildRenderableObject<StaticMeshComponent>);

		return renderableComponentBuilder;
	}

	XmlObjectBuilder * CreateOrGetRenderableComponentBuilder()
	{
		static XmlObjectBuilder * renderableComponentBuilder = CreateRenderableComponentBuilder();

		return renderableComponentBuilder;
	}
}
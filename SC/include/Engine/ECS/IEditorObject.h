#pragma once

#include "IScript.h"

#ifdef SC_EDITOR_IMPL

namespace SC {
	class IEditorObject {
	  public:
		IEditorObject() = default;
		virtual ~IEditorObject() = default;
		
		virtual void OnIGUI(Editor::EditorDrawData& dcmd) = 0;
		virtual void PostIGUI(Editor::EditorDrawData& dcmd) = 0;

	};
}
#endif
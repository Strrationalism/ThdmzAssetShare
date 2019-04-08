#pragma once
#include "Dmsl.h"
#include <optional>

namespace Dmsl {
	struct GameObjectLibraryLinkTable {
		//Attribute×é
		int angle, speed;

		//Interface
		std::optional<uint32_t> interfaceDied;
	};

	void LoadMathLibrary(DmslVirtualMachine&);
	void LoadGameObjectLibrary(DmslVirtualMachine&, GameObjectLibraryLinkTable&);
	void LoadSePlayer(DmslVirtualMachine&);
}

#pragma once

namespace GAME {
	const auto WINDOW_WIDTH = 512;
	const auto WINDOW_HEIGHT = 512;

	const auto TILE_SIZE = 32;
	const auto SOBJECT_SIZE = 16;
	/*Tile Filenames*/
	/*RGB*/
	
	const auto NORMAL_FILE = "Art/Tiles/Indestructible_Tile_32.png"; // 0 0 0
	const auto NORMAL_RGB = sf::Vector3i(0, 0, 0);

	const auto LAVA_FILE = "Art/Tiles/Lava_Tile_32.png"; // 75 75 0
	const auto LAVA_RGB = sf::Vector3i(75, 75, 0);
	const auto LAVA_TIMER = 2.5f;

	const auto TRAMPOLINE_FILE = "Art/Tiles/Trampoline_Tile_32.png"; //0 75 75
	const auto TRAMPOLINE_RGB = sf::Vector3i(0, 75, 75);

	const auto TAR_FILE = "Art/Tiles/Tar_Tile_32.png"; // 0 150 150
	const auto TAR_RGB = sf::Vector3i(0,150,150);

	const auto TELEPORTER_FILE = "Art/Tiles/Teleporter_Tile_32.png"; //75 0 75
	const auto TELEPORTER_RGB = sf::Vector3i(75, 75, 0);

	/*SObject Filenames*/
	const auto SOBJECT_RGB = sf::Vector3i(0, 225, 75);
	const auto EXPLODEB_FILE = "Art/SObjects/ExplodingBarrel.png";
	const auto HEALB_FILE = "Art/SObjects/HealBarrel.png";

	/*Character Filenames*/
	const auto WAR_FILE = "Art/Characters/2.png";

	/*Weapon Filenames*/
	const auto WAR_SWORD_FILE = "Art/Weapons/WarSword.png";
}

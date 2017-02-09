#ifndef CATEGORY_HPP
#define CATEGORY_HPP

namespace Category{
	enum Type{
		None = 0,
		SceneAirLayer = 1 << 0,
		PlayerHuman = 1 << 1,
		EnemyProjectile = 1 << 4,
		AlliedProjectile = 1 << 5,
		Pickup = 1 << 6,
		ParticleSystem = 1 << 7,
		SoundEffect = 1 << 8,
		Solid = 1 << 9,
		Zombie = 1 << 10,
		Spawner = 1 << 11,
		Fence = 1 << 12,
		HUD = 1 << 13,

		Projectile = AlliedProjectile | EnemyProjectile,
		Obstacle = Solid | Fence
	};
}

#endif
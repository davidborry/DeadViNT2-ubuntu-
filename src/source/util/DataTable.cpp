#include "../../headers/util/DataTable.hpp"
#include "../../headers/entities/Projectile.hpp"
#include "../../headers/entities/Pickup.hpp"
#include "../../headers/graphics/Particle.hpp"
#include "../../headers/entities/weapons/Gun.hpp"

Direction::Direction(float angle, float distance) : angle(angle), distance(distance)
{

}



std::vector<ProjectileData> initializeProjectileData(){
	std::vector<ProjectileData> data(Projectile::TypeCount);

	data[Projectile::AlliedBullet].damage = 10;
	data[Projectile::AlliedBullet].speed = 300.f;
	data[Projectile::AlliedBullet].texture = Resources::Textures::Entities;
	data[Projectile::AlliedBullet].textureRect = sf::IntRect(175, 64, 3, 14);

	data[Projectile::EnemyBullet].damage = 10;
	data[Projectile::EnemyBullet].speed = 300.f;
	data[Projectile::EnemyBullet].texture = Resources::Textures::Entities;
	data[Projectile::EnemyBullet].textureRect = sf::IntRect(175, 64, 3, 14);


	data[Projectile::Missile].damage = 200;
	data[Projectile::Missile].speed = 150.f;
	data[Projectile::Missile].texture = Resources::Textures::Entities;
	data[Projectile::Missile].textureRect = sf::IntRect(160, 64, 15, 32);

	return data;
}

std::vector<PickupData> initializePickUpData(){
	std::vector<PickupData> data(Pickup::TypeCount);

	

	return data;
}

std::vector<ParticleData> initializeParticleData(){
	std::vector<ParticleData> data(Particle::TypeCount);

	data[Particle::Propellant].color = sf::Color(255, 255, 50);
	data[Particle::Propellant].timelife = sf::seconds(0.6f);

	data[Particle::Smoke].color = sf::Color(50, 50, 50);
	data[Particle::Smoke].timelife = sf::seconds(4.f);

	return data;
}

std::vector<GunData> initializeGunData(){
	std::vector<GunData> data(Gun::TypeCount);

	data[Gun::Pistol].damage = 10;
	data[Gun::Pistol].fireRate = 1;
	data[Gun::Pistol].range = 500;
	data[Gun::Pistol].texture = Resources::Textures::Weapons;

	return data;
}
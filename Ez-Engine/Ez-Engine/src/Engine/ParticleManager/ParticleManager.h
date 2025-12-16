#pragma once

#include<cmath>

#include"Engine/Core/GameTime.h"
#include"Engine/Core/Random.h"
#include"Engine/ECS/Registry.h"

namespace ENGINE {

	class ParticleManager {
	public:
		ParticleManager() = default;

		void Update(ECS::Registry* registry) {
			emitTime += GameTime::deltaTime;

			// Emitter
			/*
			if (emitTime >= 2.0f) {
				for (short i = 0; i < 32; i++) {
					Entity e = registry->CreateEntity();

					double rng = CORE::Random::Range(0, 360);
					float3 randomDir = float3(sin(rng * Deg2Rad), 1, cos(rng * Deg2Rad)).normalize();

					registry->AddComponent(e, ECS::ParticleComponent(float3(0, 2, 0), randomDir, 0.1f, 1, 3));
				}

				emitTime = 0;
			}*/

			// Particle
			for (uint64_t e : registry->view().Has<ECS::ParticleComponent>().List()) {
				ECS::ParticleComponent& particle = registry->GetComponent<ECS::ParticleComponent>(e);

				particle.position += particle.veloctiy * particle.speed * GameTime::deltaTime;
				particle.currentLifeTime += GameTime::deltaTime;

				float t = particle.currentLifeTime / particle.lifeTime;
				particle.size = (1 - (particle.currentLifeTime / particle.lifeTime)) * particle.startSize;

				if (particle.lifeTime <= particle.currentLifeTime) registry->DestroyEntity(e);
			}
		}

	private:
		float emitTime;
	};

}
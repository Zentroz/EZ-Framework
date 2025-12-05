#ifndef ECSCOMPONENT_CLASS_H
#define ECSCOMPONENT_CLASS_H

#include"Engine/Core/Math.h"
#include"Engine/Scripting/ScriptObject.h"

#include"Engine/ECS/ECSData.h"	
#include"Engine/Assets/Assets.h"/

using namespace MATH;

#pragma region BulletForwardDeclaration

class btBoxShape;
class btRigidBody;
class btCollisionShape;
class btStaticPlaneShape;

#pragma endregion

namespace ENGINE {
	namespace ECS {

		struct IComponent {
			Entity entity;
		};

		struct TransformComponent : public IComponent {
			float3 position;
			float3 scale;
			quaternion rotation;

			TransformComponent();
			TransformComponent(float3 position, float3 scale, quaternion rotation);

			void AddPosition(float3 delta);
		};

		struct MeshComponent : public IComponent {
			uint16_t assetId;
			std::shared_ptr<ASSET::Asset> meshAsset = nullptr;

			MeshComponent();
			MeshComponent(uint16_t assetId);
		};

		struct MaterialComponent : public IComponent {
			uint16_t shaderAssetId;
			uint16_t textureAssetId;
			float4 baseColor;

			MaterialComponent();
			MaterialComponent(uint16_t shaderAssetId, uint16_t textureAssetId, float4 baseColor = float4::one());

			void SetFloat(std::string name, float value);

		private:
			char shaderPathInput[256] = "";
			char texturePathInput[256] = "";
			float baseColorInput[4];
		};

		enum ColliderType { Box, Plane, Cylinder, Capsule, Sphere };

		struct ColliderData {
			float3 offset;

			virtual ~ColliderData() = default;
			virtual ColliderType GetType() = 0;

			btCollisionShape* GetBulletCollisionShape();
			bool IsStatic();

		protected:
			ColliderData(float3 offset) : offset(offset) {}
			btCollisionShape* bulletCollider = nullptr;
			bool isStatic = false;
		};

		struct StaticPlaneColliderData : public ColliderData {
			StaticPlaneColliderData();

			ColliderType GetType() override;
		};

		struct BoxColliderData : public ColliderData {
			float3 halfExtents;

			BoxColliderData(float3 offset, float3 halfExtents);

			ColliderType GetType() override;
		};

		struct CapsuleColliderData : public ColliderData {
			float radius;

			CapsuleColliderData(float3 offset, float radius, float height);
			ColliderType GetType() override;
		};

		struct CylinderColliderData : public ColliderData {
			float3 halfExtents;

			CylinderColliderData(float3 offset, float3 halfExtents);
			ColliderType GetType() override;
		};

		struct ColliderComponent : public IComponent {
			ColliderType colliderType;
			ColliderData* data;

			ColliderComponent(ColliderData* data);
		};

		struct RigidBodyComponent : public IComponent {
			btRigidBody* bulletRb = nullptr;
			float mass;

			RigidBodyComponent(float mass = 1.0f);

			void SetCollisionShape(ColliderComponent& collider);

			void SetPosition(float3 position);

			void SetRotation(quaternion rotation);
		};

		struct KinematicBody : public IComponent {
			btRigidBody* bulletRb = nullptr;

			KinematicBody();

			void SetCollisionShape(ColliderComponent& collider);
		};

		struct ParticleComponent : public IComponent {
		public:
			float3 startPosition;
			float3 position;
			float3 veloctiy;
			float speed;
			float startSpeed;
			float size;
			float startSize;
			float lifeTime;
			float currentLifeTime;
			bool applyGravity;

			ParticleComponent(float3 startPosition, float3 veloctiy, float size, float lifeTime, float speed, bool applyGravity = false);
		};

		struct ScriptComponent : public IComponent {
		public:
			std::string className;
			std::string nameSpace;
			SCRIPT::ScriptObject* scriptObject = nullptr;

			ScriptComponent(std::string nameSpace, std::string className);
		};

	}
}

#endif
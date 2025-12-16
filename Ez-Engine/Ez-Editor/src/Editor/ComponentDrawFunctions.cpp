#include"ComponentDrawFunctions.h"

namespace EDITOR {
	void DrawTransform(ComponentUIDrawInput& input) {
		TransformComponent& t = input.registry.GetComponent<TransformComponent>(input.entity);

		if (ImGui::CollapsingHeader("Transform Component")) {
			// Position
			input.float3Buffer[0] = t.position.x;
			input.float3Buffer[1] = t.position.y;
			input.float3Buffer[2] = t.position.z;
			ImGui::Text("Position: ");
			ImGui::SameLine();
			if (ImGui::InputFloat3("##PositionInput", input.float3Buffer)) {
				t.position = float3(input.float3Buffer[0], input.float3Buffer[1], input.float3Buffer[2]);
			}

			// Rotation
			input.float4Buffer[0] = t.rotation.x;
			input.float4Buffer[1] = t.rotation.y;
			input.float4Buffer[2] = t.rotation.z;
			input.float4Buffer[3] = t.rotation.w;
			ImGui::Text("Rotation: ");
			ImGui::SameLine();
			if (ImGui::InputFloat4("##RotationInput", input.float4Buffer)) {
				t.rotation = quaternion(input.float4Buffer[0], input.float4Buffer[1], input.float4Buffer[2], input.float4Buffer[3]);
			}

			// Scale
			input.float3Buffer[0] = t.scale.x;
			input.float3Buffer[1] = t.scale.y;
			input.float3Buffer[2] = t.scale.z;
			ImGui::Text("Scale:    ");
			ImGui::SameLine();
			if (ImGui::InputFloat3("##ScaleInput", input.float3Buffer)) {
				t.scale = float3(input.float3Buffer[0], input.float3Buffer[1], input.float3Buffer[2]);
			}
		}
	}

	void DrawMeshComponent(ComponentUIDrawInput& input) {
		if (ImGui::CollapsingHeader("Mesh Component")) {
			MeshComponent& m = input.registry.GetComponent<MeshComponent>(input.entity);

			ImGui::Text("Mesh: ");
			ImGui::SameLine();

			//input.assetManager->GetAsset(assetmana);

			if (!m.assetId.isNull()) {

				ImGui::Button("Assigned##MeshAssetIdBtn");
			}
			else ImGui::Button("Unassigned##MeshAssetIdBtn");

			if (ImGui::BeginDragDropTarget()) {
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET_ID_PAYLOAD")) {
					std::string droppedID = (const char*)payload->Data;
					if (input.assetManager.GetAssetType(droppedID) == AssetType::MESH) {
						m.assetId = EUID(droppedID);
					}
				}
				ImGui::EndDragDropTarget();
			}
		}
	}

	void DrawMaterialComponent(ComponentUIDrawInput& input) {
		if (ImGui::CollapsingHeader("Material Component")) {
			MaterialComponent& m = input.registry.GetComponent<MaterialComponent>(input.entity);

			ImGui::Text("Shader: ");
			ImGui::SameLine();

			if (!m.shaderAssetId.isNull()) {
				ImGui::Button("Assgined##ShaderAssetIdBtn");
			}
			else ImGui::Button("Unassigned##ShaderAssetIdBtn");

			if (ImGui::BeginDragDropTarget()) {
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET_ID_PAYLOAD")) {
					std::string droppedID = (const char*)payload->Data;
					if (input.assetManager.GetAssetType(droppedID) == AssetType::SHADER) {
						m.shaderAssetId = EUID(droppedID);
					}
				}
				ImGui::EndDragDropTarget();
			}

			ImGui::Text("Texture: ");
			ImGui::SameLine();

			if (!m.textureAssetId.isNull()) {

				ImGui::Button("Assigned##TextureAssetIdBtn");
			}
			else ImGui::Button("Unassigned##TextureAssetIdBtn");

			if (ImGui::BeginDragDropTarget()) {
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET_ID_PAYLOAD")) {
					std::string droppedID = (const char*)payload->Data;
					AssetType assetType = input.assetManager.GetAssetType(droppedID);
					if (assetType == AssetType::TEXTURE) {
						m.textureAssetId = EUID(droppedID);
					}
				}
				ImGui::EndDragDropTarget();
			}

			ImGui::Text("BaseColor: ");
			input.float4Buffer[0] = m.baseColor.x;
			input.float4Buffer[1] = m.baseColor.y;
			input.float4Buffer[2] = m.baseColor.z;
			input.float4Buffer[3] = m.baseColor.w;
			if (ImGui::ColorEdit4("##BaseColorPicker", input.float4Buffer)) {
				m.baseColor = float4(input.float4Buffer[0], input.float4Buffer[1], input.float4Buffer[2], input.float4Buffer[3]);
			}
		}
	}
}
#include "Renderer/Mesh.h"

#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>

#define MESH_DEBUG_LOG 1
#if MESH_DEBUG_LOG
#define SP_MESH_LOG(...) SP_CORE_TRACE_TAG("Mesh", __VA_ARGS__)
#define SP_MESH_ERROR(...) SP_CORE_ERROR_TAG("Mesh", __VA_ARGS__)
#define SP_MESH_WARN(...) SP_CORE_WARN_TAG("Mesh", __VA_ARGS__)
#else
#define SP_MESH_LOG(...)
#define SP_MESH_ERROR(...)
#define SP_MESH_WARN(...)
#endif



namespace Spark
{

	namespace Utils
	{
		glm::mat4 Mat4FromAIMatrix4x4(const aiMatrix4x4& matrix)
		{
			glm::mat4 result;
			//the a,b,c,d in assimp is the row ; the 1,2,3,4 is the column
			result[0][0] = matrix.a1; result[1][0] = matrix.a2; result[2][0] = matrix.a3; result[3][0] = matrix.a4;
			result[0][1] = matrix.b1; result[1][1] = matrix.b2; result[2][1] = matrix.b3; result[3][1] = matrix.b4;
			result[0][2] = matrix.c1; result[1][2] = matrix.c2; result[2][2] = matrix.c3; result[3][2] = matrix.c4;
			result[0][3] = matrix.d1; result[1][3] = matrix.d2; result[2][3] = matrix.d3; result[3][3] = matrix.d4;
			return result;
		}
	}




	static constexpr uint32_t s_MeshImportFlags =
		aiProcess_CalcTangentSpace |        // Create binormals/tangents just in case
		aiProcess_Triangulate |             // Make sure we're triangles
		aiProcess_SortByPType |             // Split meshes by primitive type
		aiProcess_GenNormals |              // Make sure we have legit normals
		aiProcess_GenUVCoords |             // Convert UVs if required 
		aiProcess_OptimizeMeshes |          // Batch draws where possible
		aiProcess_JoinIdenticalVertices |
		aiProcess_LimitBoneWeights |        // If more than N (=4) bone weights, discard least influencing bones and renormalise sum to 1
		aiProcess_ValidateDataStructure |   // Validation
		aiProcess_GlobalScale;              // e.g. convert cm to m for fbx import (and other formats where cm is native)



	class BoneHierarchy
	{
	public:
		BoneHierarchy(const aiScene* scene) :m_Scene(scene)
		{
		}
		Skeleton* CreateSkeleton();

	private:
		void ExtractBones();
		void TraverseNode(const aiNode* node, Skeleton* skeleton);
		static void TraverseBone(const aiNode* node, Skeleton* skeleton, uint32_t parentIndex);

	private:
		const aiScene* m_Scene;
		std::set<std::string_view> m_Bones;
	};

	Skeleton* BoneHierarchy::CreateSkeleton()
	{
		if (m_Scene == nullptr) return nullptr;

		ExtractBones();
		if (m_Bones.empty()) return nullptr;
		Skeleton* skeleton = new Skeleton(static_cast<uint32_t>(m_Bones.size()));
		TraverseNode(m_Scene->mRootNode, skeleton);
		return skeleton;
	}

	void BoneHierarchy::ExtractBones()
	{
		for (uint32_t meshIndex = 0; meshIndex < m_Scene->mNumMeshes; ++meshIndex)
		{
			const aiMesh* mesh = m_Scene->mMeshes[meshIndex];
			for (uint32_t boneIndex = 0; boneIndex < mesh->mNumBones; ++boneIndex)
			{
				m_Bones.emplace(mesh->mBones[boneIndex]->mName.C_Str());
			}
		}
	}

	void BoneHierarchy::TraverseNode(const aiNode* node, Skeleton* skeleton)
	{
		SP_TRACE(node->mName.C_Str());
		if (m_Bones.find(node->mName.C_Str()) != m_Bones.end())
		{
			TraverseBone(node, skeleton, Skeleton::NullIndex);
		}
		else
		{
			for (uint32_t nodeIndex = 0; nodeIndex < node->mNumChildren; ++nodeIndex)
			{
				TraverseNode(node->mChildren[nodeIndex], skeleton);
			}
		}
	}

	void BoneHierarchy::TraverseBone(const aiNode* node, Skeleton* skeleton, uint32_t parentIndex)
	{
		const uint32_t boneIndex = skeleton->AddBone(node->mName.C_Str(), parentIndex, Utils::Mat4FromAIMatrix4x4(node->mTransformation));
		for (uint32_t nodeIndex = 0; nodeIndex < node->mNumChildren; ++nodeIndex)
		{
			TraverseBone(node->mChildren[nodeIndex], skeleton, boneIndex);
		}
	}


	Skeleton* ImportSkeleton(const aiScene* scene)
	{
		BoneHierarchy boneHierarchy(scene);
		return boneHierarchy.CreateSkeleton();
	}

	std::vector<std::string> GetAnimationNames(const aiScene* scene)
	{
		std::vector<std::string> animationNames;
		animationNames.reserve(scene->mNumAnimations);
		for (size_t i = 0; i < scene->mNumAnimations; ++i)
		{
			const auto animation = scene->mAnimations[i];
			if (animation->mDuration <= 0.0f)
			{
				SP_MESH_WARN("Animation '{0}' duration is zero or negative. This animation was ignored!", animation->mName.C_Str());
				continue;
			}
			animationNames.emplace_back(animation->mName.C_Str());
		}
		return animationNames;
	}


	template<typename T>
	struct KeyFrame
	{
		float FrameTime;
		T Value;
		KeyFrame(float frameTime, const T& value) : FrameTime(frameTime), Value(value) {}
	};

	struct Channel
	{
		std::vector<KeyFrame<glm::vec3>> Translations;
		std::vector<KeyFrame<glm::quat>> Rotations;
		std::vector<KeyFrame<glm::vec3>> Scales;
		uint32_t Index;
	};


	static auto ImportChannels(const aiAnimation* anim, const Skeleton* skeleton)
	{
		std::vector<Channel> channels;

		std::unordered_map<std::string, uint32_t> boneIndices;
		for (uint32_t i = 0; i < skeleton->GetNumBones(); ++i)
		{
			boneIndices.emplace(skeleton->GetBoneName(i), i);
		}

		std::map<uint32_t, aiNodeAnim*> validChannels;
		for (uint32_t channelIndex = 0; channelIndex < anim->mNumChannels; ++channelIndex)
		{
			aiNodeAnim* nodeAnim = anim->mChannels[channelIndex];
			if (auto it = boneIndices.find(nodeAnim->mNodeName.C_Str()); it != boneIndices.end())
			{
				validChannels.emplace(it->second, nodeAnim);
			}
		}
		channels.resize(skeleton->GetNumBones());
		for (uint32_t boneIndex = 0; boneIndex < skeleton->GetNumBones(); ++boneIndex)
		{
			Channel& channel = channels[boneIndex];
			channel.Index = boneIndex;
			auto validChannel = validChannels.find(boneIndex);
			if (validChannel == validChannels.end())
			{
				SP_MESH_WARN("No animations tracks found for bone '{}'", skeleton->GetBoneName(boneIndex));
				channel.Translations = { {0.0f, glm::vec3{0.0f}}, {1.0f, glm::vec3{0.0f}} };
				channel.Rotations = { {0.0f, glm::quat{1.0f, 0.0f, 0.0f, 0.0f}}, {1.0f, glm::quat{1.0f, 0.0f, 0.0f, 0.0f}} };
				channel.Scales = { {0.0f, glm::vec3{1.0f}}, {1.0f, glm::vec3{1.0f}} };
				continue;
			}

			const auto nodeAnim = validChannel->second;
			channel.Translations.reserve(nodeAnim->mNumPositionKeys + 2);
			channel.Scales.reserve(nodeAnim->mNumScalingKeys + 2);
			channel.Rotations.reserve(nodeAnim->mNumRotationKeys + 2);

			// Translation Keys
			{
				for (uint32_t keyIndex = 0; keyIndex < nodeAnim->mNumPositionKeys; ++keyIndex)
				{
					const auto& key = nodeAnim->mPositionKeys[keyIndex];
					const float frameTime = std::clamp(static_cast<float>(key.mTime / anim->mDuration), 0.0f, 1.0f);
					if (keyIndex == 0 && frameTime > 0.0f)
					{
						channel.Translations.emplace_back(0.0f, Vector3{ key.mValue.x,key.mValue.y,key.mValue.z });
					}
					channel.Translations.emplace_back(frameTime, Vector3{ key.mValue.x,key.mValue.y,key.mValue.z });
				}
				if (channel.Translations.empty())
				{
					SP_MESH_WARN("No translation tracks found for bone '{}'", skeleton->GetBoneName(boneIndex));
					channel.Translations = { {0.0f, Vector3{0.0f}}, {1.0f, Vector3{0.0f}} };
				}
				else if (channel.Translations.back().FrameTime < 1.0f)
				{
					channel.Translations.emplace_back(1.0f, channel.Translations.back().Value);
				}
			}

			// Rotation Keys
			{
				for (uint32_t keyIndex = 0; keyIndex < nodeAnim->mNumRotationKeys; ++keyIndex)
				{
					const auto& key = nodeAnim->mRotationKeys[keyIndex];
					const float frameTime = std::clamp(static_cast<float>(key.mTime / anim->mDuration), 0.0f, 1.0f);
					if (keyIndex == 0 && frameTime > 0.0f)
					{
						channel.Rotations.emplace_back(0.0f, Quat{ key.mValue.w,key.mValue.x,key.mValue.y,key.mValue.z });
					}
					channel.Rotations.emplace_back(frameTime, Quat{ key.mValue.w,key.mValue.x,key.mValue.y,key.mValue.z });
				}
				if (channel.Rotations.empty())
				{
					SP_MESH_WARN("No rotations tracks found for bone '{}'", skeleton->GetBoneName(boneIndex));
					channel.Rotations = { {0.0f, Quat{1.0f, 0.0f, 0.0f, 0.0f}}, {1.0f, Quat{1.0f, 0.0f, 0.0f, 0.0f}} };
				}
				else if (channel.Rotations.back().FrameTime < 1.0f)
				{
					channel.Rotations.emplace_back(1.0f, channel.Rotations.back().Value);
				}
			}

			// Scale Keys
			{
				for (uint32_t keyIndex = 0; keyIndex < nodeAnim->mNumScalingKeys; ++keyIndex)
				{
					const auto& key = nodeAnim->mScalingKeys[keyIndex];
					const float frameTime = std::clamp(static_cast<float>(key.mTime / anim->mDuration), 0.0f, 1.0f);
					if (keyIndex == 0 && frameTime > 0.0f)
					{
						channel.Scales.emplace_back(0.0f, Vector3{ key.mValue.x,key.mValue.y,key.mValue.z });
					}
					channel.Scales.emplace_back(frameTime, Vector3{ key.mValue.x,key.mValue.y,key.mValue.z });
				}
				if (channel.Scales.empty())
				{
					SP_MESH_WARN("No scale tracks found for bone '{}'", skeleton->GetBoneName(boneIndex));
					channel.Scales = { {0.0f, Vector3{1.0f}},  {1.0f, Vector3{1.0f}} };
				}
				else if (channel.Scales.back().FrameTime < 1.0f)
				{
					channel.Scales.emplace_back(1.0f, channel.Scales.back().Value);
				}
			}
		}

		return channels;
	}

	static auto ConcatenateChannelsAndSort(const std::vector<Channel>& channels)
	{
		uint32_t numTranslations = 0;
		uint32_t numRotations = 0;
		uint32_t numScales = 0;

		for (const auto& channel : channels)
		{
			numTranslations += channel.Translations.size();
			numRotations += channel.Rotations.size();
			numScales += channel.Scales.size();
		}

		std::vector<std::pair<float, TranslationKey>> translationKeysTemp;
		std::vector<std::pair<float, RotationKey>> rotationKeysTemp;
		std::vector<std::pair<float, ScaleKey>> scaleKeysTemp;

		for (const auto& channel : channels)
		{
			float prevFrameTime = -1.0f;
			for (const auto& translation : channel.Translations)
			{
				translationKeysTemp.emplace_back(prevFrameTime, TranslationKey{ translation.FrameTime,channel.Index,translation.Value });
				prevFrameTime = translation.FrameTime;
			}

			prevFrameTime = -1.0f;
			for (const auto& rotation : channel.Rotations)
			{
				rotationKeysTemp.emplace_back(prevFrameTime, RotationKey{ rotation.FrameTime,channel.Index,rotation.Value });
				prevFrameTime = rotation.FrameTime;
			}

			prevFrameTime = -1.0f;
			for (const auto& scale : channel.Scales)
			{
				scaleKeysTemp.emplace_back(prevFrameTime, ScaleKey{ scale.FrameTime,channel.Index,scale.Value });
				prevFrameTime = scale.FrameTime;
			}
		}

		std::sort(translationKeysTemp.begin(), translationKeysTemp.end(), [](const auto& a, const auto& b)
			{
				return (a.first < b.first) || ((a.first == b.first) && a.second.Track < b.second.Track);
			});

		std::sort(rotationKeysTemp.begin(), rotationKeysTemp.end(), [](const auto& a, const auto& b)
			{
				return (a.first < b.first) || ((a.first == b.first) && a.second.Track < b.second.Track);
			});

		std::sort(rotationKeysTemp.begin(), rotationKeysTemp.end(), [](const auto& a, const auto& b)
			{
				return (a.first < b.first) || ((a.first == b.first) && a.second.Track < b.second.Track);
			});

		return std::tuple{ translationKeysTemp,rotationKeysTemp,scaleKeysTemp };
	}

	static auto ExtractKeys(const std::vector<std::pair<float, TranslationKey>>& translationKeysTemp,
		const std::vector<std::pair<float, RotationKey>>& rotationKeysTemp,
		const std::vector<std::pair<float, ScaleKey>>& scaleKeysTemp)
	{
		std::vector<TranslationKey> translationKeys;
		std::vector<RotationKey> rotationKeys;
		std::vector<ScaleKey> scaleKeys;

		translationKeys.reserve(translationKeysTemp.size());
		rotationKeys.reserve(rotationKeysTemp.size());
		scaleKeys.reserve(scaleKeysTemp.size());

		for (const auto& translation : translationKeysTemp)
		{
			translationKeys.emplace_back(translation.second);
		}
		for (const auto& rotation : rotationKeysTemp)
		{
			rotationKeys.emplace_back(rotation.second);
		}
		for (const auto& scale : scaleKeysTemp)
		{
			scaleKeys.emplace_back(scale.second);
		}
		return std::tuple{ translationKeys,rotationKeys,scaleKeys };
	}


	Animation* ImportAnimation(const aiScene* scene, const std::string& animationName, const Skeleton* skeleton)
	{
		Animation* animation = nullptr;
		for (uint32_t animIndex = 0; animIndex < scene->mNumAnimations; ++animIndex)
		{
			const auto anim = scene->mAnimations[animIndex];
			if (animationName != anim->mName.C_Str())
			{
				continue;
			}

			const auto channels = ImportChannels(anim, skeleton);
			auto [translationKeysTemp, rotationKeysTemp, scaleKeysTemp] = ConcatenateChannelsAndSort(channels);
			auto [translationKeys, rotationKeys, scaleKeys] = ExtractKeys(translationKeysTemp, rotationKeysTemp, scaleKeysTemp);

			double samplingRate = anim->mTicksPerSecond;
			if (samplingRate < 0.00001)
			{
				samplingRate = 1.0f;
			}
			animation = new Animation(animationName, anim->mDuration / samplingRate);
			animation->SetKeyFrames(std::move(translationKeys), std::move(rotationKeys), std::move(scaleKeys));
			break;
		}
		return animation;
	}


	void BoneInfluence::AddBoneData(uint32_t boneInfoIndex, float weight)
	{
		if (weight < 0.0f || weight > 1.0f)
		{
			SP_MESH_WARN("Vertex bone weight is out of range. we will clamp it to [0,1] (BoneID={0},Weight={1})", boneInfoIndex, weight);
			weight = std::clamp(weight, 0.0f, 1.0f);
		}
		if (weight > 0.0f)
		{
			for (int i = 0; i < 4; ++i)
			{
				if (Weights[i] == 0.0f)
				{
					BoneInfoIndices[i] = boneInfoIndex;
					Weights[i] = weight;
					return;
				}
			}
			SP_MESH_WARN("Vertex has more than four bones affecting it,extra bone infuences will be discarded (BoneId={0}, Weight={1})", boneInfoIndex, weight);
		}
	}

	void BoneInfluence::NormalizeWeights()
	{
		float sumWeights = 0.0f;
		for (int i = 0; i < 4; ++i)
		{
			sumWeights += Weights[i];
		}
		if (sumWeights > 0.0f)
		{
			for (int i = 0; i < 4; i++)
			{
				Weights[i] /= sumWeights;
			}
		}
	}

	Mesh::Mesh(const std::filesystem::path& filepath)
	{
		Assimp::Importer importer;
		importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, false);

		const aiScene* scene = importer.ReadFile(filepath.string(), s_MeshImportFlags);

		if (scene == nullptr)
		{
			SP_MESH_ERROR("Failed to load mesh file: {0}", filepath.string());
			return;
		}

		m_Skeleton = ImportSkeleton(scene);
		SP_MESH_LOG("Skeleton {0} found in mesh file {1}", HasSkeleton() ? "" : "not", filepath.string());
		if (HasSkeleton())
		{
			const auto& animationNames = GetAnimationNames(scene);
			m_Animations.reserve(animationNames.size());
			for (auto animationName : animationNames)
			{
				auto animation = ImportAnimation(scene, animationName, m_Skeleton);
				m_Animations.emplace_back(animation);
			}
		}
		// Meshes

		if (scene->HasMeshes())
		{
			uint32_t vertexCount = 0;
			uint32_t indexCount = 0;

			m_BoundingBox.Min = { FLT_MAX,FLT_MAX,FLT_MAX };
			m_BoundingBox.Max = { -FLT_MAX,-FLT_MAX ,-FLT_MAX };
			m_Submeshes.reserve(scene->mNumMeshes);

			for (unsigned m = 0; m < scene->mNumMeshes; m++)
			{
				aiMesh* mesh = scene->mMeshes[m];
				Submesh& submesh = m_Submeshes.emplace_back();
				submesh.BaseVertex = vertexCount;
				submesh.BaseIndex = indexCount;
				submesh.VertexCount = mesh->mNumVertices;
				submesh.IndexCount = mesh->mNumFaces * 3;
				submesh.MeshName = mesh->mName.C_Str();

				vertexCount += submesh.VertexCount;
				indexCount += submesh.IndexCount;

				SP_CORE_ASSERT(mesh->HasPositions(), "Mesh require positions.");
				SP_CORE_ASSERT(mesh->HasNormals(), "Mesh require normals.");

				// Vertices
				auto& aabb = submesh.BoundingBox;
				aabb.Min = { FLT_MAX,FLT_MAX,FLT_MAX };
				aabb.Max = { -FLT_MAX,-FLT_MAX ,-FLT_MAX };
				m_Vertices.reserve(mesh->mNumVertices);
				for (unsigned i = 0; i < mesh->mNumVertices; i++)
				{
					Vertex& vertex = m_Vertices.emplace_back();
					vertex.Position = { mesh->mVertices[i].x,mesh->mVertices[i].y,mesh->mVertices[i].z };
					vertex.Normal = { mesh->mNormals[i].x,mesh->mNormals[i].y,mesh->mNormals[i].z };
					aabb.Min.x = glm::min(vertex.Position.x, aabb.Min.x);
					aabb.Min.y = glm::min(vertex.Position.y, aabb.Min.y);
					aabb.Min.z = glm::min(vertex.Position.z, aabb.Min.z);
					aabb.Max.x = glm::max(vertex.Position.x, aabb.Max.x);
					aabb.Max.y = glm::max(vertex.Position.y, aabb.Max.y);
					aabb.Max.z = glm::max(vertex.Position.z, aabb.Max.z);

					if (mesh->HasTangentsAndBitangents())
					{
						vertex.Tangent = { mesh->mTangents[i].x,mesh->mTangents[i].y ,mesh->mTangents[i].z };
						vertex.Binormal = { mesh->mBitangents[i].x,mesh->mBitangents[i].y, mesh->mBitangents[i].z };
					}

					if (mesh->HasTextureCoords(0))
					{
						vertex.Texcoord = { mesh->mTextureCoords[0][i].x,mesh->mTextureCoords[0][i].y };
					}
				}

				// Indices
				m_Indices.reserve(mesh->mNumFaces);
				for (unsigned i = 0; i < mesh->mNumFaces; i++)
				{
					SP_CORE_ASSERT(mesh->mFaces[i].mNumIndices == 3, "Must have 3 indices.");
					Index& index = m_Indices.emplace_back();
					index.V1 = mesh->mFaces[i].mIndices[0];
					index.V2 = mesh->mFaces[i].mIndices[1];
					index.V3 = mesh->mFaces[i].mIndices[2];
				}
			}


			MeshNode& rootNode = m_Nodes.emplace_back();
			TraverseNodes(scene->mRootNode, 0);
			for (const auto& submesh : m_Submeshes)
			{
				AABB transformedSubmeshAABB = submesh.BoundingBox;
				Vector3 min = Vector3(submesh.Transform * Vector4(transformedSubmeshAABB.Min, 1.0f));
				Vector3 max = Vector3(submesh.Transform * Vector4(transformedSubmeshAABB.Max, 1.0f));
				m_BoundingBox.Min.x = glm::min(m_BoundingBox.Min.x, min.x);
				m_BoundingBox.Min.y = glm::min(m_BoundingBox.Min.y, min.y);
				m_BoundingBox.Min.z = glm::min(m_BoundingBox.Min.z, min.z);
				m_BoundingBox.Max.x = glm::max(m_BoundingBox.Max.x, max.x);
				m_BoundingBox.Max.y = glm::max(m_BoundingBox.Max.y, max.y);
				m_BoundingBox.Max.z = glm::max(m_BoundingBox.Max.z, max.z);
			}
		}

		// Bones

		if (HasSkeleton())
		{
			m_BoneInfluences.resize(m_Vertices.size());
			for (uint32_t m = 0; m < scene->mNumMeshes; m++)
			{
				const aiMesh* mesh = scene->mMeshes[m];
				Submesh& submesh = m_Submeshes[m];

				if (mesh->mNumBones == 0) continue;

				submesh.IsRigged = true;
				for (uint32_t i = 0; i < mesh->mNumBones; i++)
				{
					const aiBone* bone = mesh->mBones[i];
					bool hasNonZeroWeight = false;
					for (int j = 0; j < bone->mNumWeights; ++j)
					{
						if (bone->mWeights[j].mWeight > 0.000001f)
						{
							hasNonZeroWeight = true;
						}
					}
					if (!hasNonZeroWeight) continue;

					// Find bone in skeleton
					uint32_t boneIndex = m_Skeleton->GetBoneIndex(bone->mName.C_Str());
					if (boneIndex == Skeleton::NullIndex)
					{
						SP_CORE_ERROR_TAG("Animation", "Could not find mesh bone '{}' in skeleton!", bone->mName.C_Str());
						continue;
					}
					uint32_t boneInfoIndex = ~0;
					for (size_t j = 0; j < m_BoneInfo.size(); ++j)
					{
						const auto& boneInfo = m_BoneInfo[j];
						if (boneInfo.BoneIndex == boneIndex && boneInfo.SubMeshIndex == m)
						{
							boneInfoIndex = static_cast<uint32_t>(j);
							break;
						}
					}

					if (boneInfoIndex == ~0)
					{
						boneInfoIndex = static_cast<uint32_t>(m_BoneInfo.size());
						m_BoneInfo.emplace_back(glm::inverse(submesh.Transform), Utils::Mat4FromAIMatrix4x4(bone->mOffsetMatrix), m, boneIndex);
					}

					for (size_t j = 0; j < bone->mNumWeights; j++)
					{
						int vertexId = submesh.BaseVertex + bone->mWeights[j].mVertexId;
						float weight = bone->mWeights[j].mWeight;
						m_BoneInfluences[vertexId].AddBoneData(boneInfoIndex, weight);
					}

				}

			}

			for (auto& boneInfluence : m_BoneInfluences)
			{
				boneInfluence.NormalizeWeights();
			}
		}
	}

	Mesh::~Mesh()
	{
		if (m_Skeleton)
		{
			delete m_Skeleton;
			m_Skeleton = nullptr;
		}

		for (const Animation* animation : m_Animations)
		{
			delete animation;
		}
		m_Animations.clear();
	}


	void Mesh::TraverseNodes(void* assimpNode, const uint32_t nodeIndex, const Mat4& parentTransform, uint32_t level)
	{
		const aiNode* aNode = static_cast<aiNode*>(assimpNode);

		MeshNode& node = m_Nodes[nodeIndex];
		node.Name = aNode->mName.C_Str();
		node.LocalTransform = Utils::Mat4FromAIMatrix4x4(aNode->mTransformation);

		const Mat4 transform = parentTransform * node.LocalTransform;
		for (int i = 0; i < aNode->mNumMeshes; ++i)
		{
			uint32_t submeshIndex = aNode->mMeshes[i];
			auto& submesh = m_Submeshes[submeshIndex];
			submesh.NodeName = aNode->mName.C_Str();
			submesh.Transform = transform;
			submesh.LocalTransform = node.LocalTransform;

			node.Submeshes.push_back(submeshIndex);
		}

		const uint32_t parentNodeIndex = m_Nodes.size() - 1;
		node.Children.resize(aNode->mNumChildren);
		for (uint32_t i = 0; i < aNode->mNumChildren; i++)
		{
			MeshNode& child = m_Nodes.emplace_back();
			const uint32_t childIndex = m_Nodes.size() - 1;
			child.Parent = parentNodeIndex;
			m_Nodes[nodeIndex].Children[i] = childIndex;
			TraverseNodes(aNode->mChildren[i], childIndex, transform, level + 1);
		}
	}


	Ref<Mesh> Mesh::CreateQubeMesh()
	{
		Ref<Mesh> mesh = CreateRef<Mesh>();
		mesh->m_Vertices = {
			{{-0.5f, -0.5f, -0.5f},  {0.0f,  0.0f, -1.0f}},
			{{ 0.5f, -0.5f, -0.5f},  {0.0f,  0.0f, -1.0f}},
			{{ 0.5f,  0.5f, -0.5f},  {0.0f,  0.0f, -1.0f}},
			{{ 0.5f,  0.5f, -0.5f},  {0.0f,  0.0f, -1.0f}},
			{{-0.5f,  0.5f, -0.5f},  {0.0f,  0.0f, -1.0f}},
			{{-0.5f, -0.5f, -0.5f},  {0.0f,  0.0f, -1.0f}},
			{{-0.5f, -0.5f,  0.5f},  {0.0f,  0.0f, 1.0f,}},
			{{ 0.5f, -0.5f,  0.5f},  {0.0f,  0.0f, 1.0f,}},
			{{ 0.5f,  0.5f,  0.5f},  {0.0f,  0.0f, 1.0f,}},
			{{ 0.5f,  0.5f,  0.5f},  {0.0f,  0.0f, 1.0f,}},
			{{-0.5f,  0.5f,  0.5f},  {0.0f,  0.0f, 1.0f,}},
			{{-0.5f, -0.5f,  0.5f},  {0.0f,  0.0f, 1.0f,}},
			{{-0.5f,  0.5f,  0.5f}, {-1.0f,  0.0f,  0.0f}},
			{{-0.5f,  0.5f, -0.5f}, {-1.0f,  0.0f,  0.0f}},
			{{-0.5f, -0.5f, -0.5f}, {-1.0f,  0.0f,  0.0f}},
			{{-0.5f, -0.5f, -0.5f}, {-1.0f,  0.0f,  0.0f}},
			{{-0.5f, -0.5f,  0.5f}, {-1.0f,  0.0f,  0.0f}},
			{{-0.5f,  0.5f,  0.5f}, {-1.0f,  0.0f,  0.0f}},
			{{ 0.5f,  0.5f,  0.5f},  {1.0f,  0.0f,  0.0f}},
			{{ 0.5f,  0.5f, -0.5f},  {1.0f,  0.0f,  0.0f}},
			{{ 0.5f, -0.5f, -0.5f},  {1.0f,  0.0f,  0.0f}},
			{{ 0.5f, -0.5f, -0.5f},  {1.0f,  0.0f,  0.0f}},
			{{ 0.5f, -0.5f,  0.5f},  {1.0f,  0.0f,  0.0f}},
			{{ 0.5f,  0.5f,  0.5f},  {1.0f,  0.0f,  0.0f}},
			{{-0.5f, -0.5f, -0.5f},  {0.0f, -1.0f,  0.0f}},
			{{ 0.5f, -0.5f, -0.5f},  {0.0f, -1.0f,  0.0f}},
			{{ 0.5f, -0.5f,  0.5f},  {0.0f, -1.0f,  0.0f}},
			{{ 0.5f, -0.5f,  0.5f},  {0.0f, -1.0f,  0.0f}},
			{{-0.5f, -0.5f,  0.5f},  {0.0f, -1.0f,  0.0f}},
			{{-0.5f, -0.5f, -0.5f},  {0.0f, -1.0f,  0.0f}},
			{{-0.5f,  0.5f, -0.5f},  {0.0f,  1.0f,  0.0f}},
			{{ 0.5f,  0.5f, -0.5f},  {0.0f,  1.0f,  0.0f}},
			{{ 0.5f,  0.5f,  0.5f},  {0.0f,  1.0f,  0.0f}},
			{{ 0.5f,  0.5f,  0.5f},  {0.0f,  1.0f,  0.0f}},
			{{-0.5f,  0.5f,  0.5f},  {0.0f,  1.0f,  0.0f}},
			{{-0.5f,  0.5f, -0.5f},  {0.0f,  1.0f,  0.0f}}
		};

		mesh->m_Indices = {
			{0,1,2},
			{3,4,5},
			{6,7,8},
			{9,10,11},
			{12,13,14},
			{15,16,17},
			{18,19,20},
			{21,22,23},
			{24,25,26},
			{27,28,29},
			{30,31,32},
			{33,34,35},
		};

		Submesh submesh;
		submesh.Transform = Mat4(1.0f);
		submesh.LocalTransform = Mat4(1.0f);
		submesh.BaseIndex = 0;
		submesh.BaseVertex = 0;
		submesh.IndexCount = 36;
		submesh.MeshName = "Qube";
		submesh.NodeName = "Root";
		mesh->m_Submeshes.emplace_back(submesh);

		return mesh;
	}

}

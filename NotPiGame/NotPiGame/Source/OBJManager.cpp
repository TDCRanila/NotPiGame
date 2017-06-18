#include "../Headers/OBJManager.h"

#include "../Headers/Texture.h"

OBJManager::OBJManager() {
	
}

OBJManager::~OBJManager() {
	
}

std::string OBJManager::GetBaseDir(const std::string &filepath) {
	if (filepath.find_last_of("/\\") != std::string::npos)
		return filepath.substr(0, filepath.find_last_of("/\\"));
	return "";
}

bool OBJManager::FileExists(const std::string &abs_filename) {
	bool ret;
	FILE *fp = fopen(abs_filename.c_str(), "rb");
	if (fp) {
		ret = true;
		fclose(fp);
	}
	else {
		ret = false;
	}

	return ret;
}

void OBJManager::CheckErrors(std::string desc) {
	GLenum e = glGetError();
	if (e != GL_NO_ERROR) {
		fprintf(stderr, "OpenGL error in \"%s\": %d (%d)\n", desc.c_str(), e, e);
		exit(20);
	}
}

static void CalcNormal(float N[3], float v0[3], float v1[3], float v2[3]) {
	float v10[3];
	v10[0] = v1[0] - v0[0];
	v10[1] = v1[1] - v0[1];
	v10[2] = v1[2] - v0[2];

	float v20[3];
	v20[0] = v2[0] - v0[0];
	v20[1] = v2[1] - v0[1];
	v20[2] = v2[2] - v0[2];

	N[0] = v20[1] * v10[2] - v20[2] * v10[1];
	N[1] = v20[2] * v10[0] - v20[0] * v10[2];
	N[2] = v20[0] * v10[1] - v20[1] * v10[0];

	float len2 = N[0] * N[0] + N[1] * N[1] + N[2] * N[2];
	if (len2 > 0.0f) {
		float len = sqrt(len2);

		N[0] /= len;
		N[1] /= len;
	}
}

bool OBJManager::loadObject(float bmin[3], float bmax[3],
	std::vector<DrawObject> *drawObjects, std::vector<tinyobj::material_t>& materials,
	std::map<std::string, GLuint>& textures, const char* filename) {
	
	printf("###########################################\n");
	printf("#Loading OBJ File: %s#\n#", filename);
	
	std::string err;
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;

	std::string base_dir = GetBaseDir(filename);
#ifdef _WIN32
	base_dir += "\\";
#else
	base_dir += "/";
#endif
		
	/// Call to tinyobj to load our model. All the informations we need are stored
    /// in the containers we pass through. We then check for errors and return in that case.

	bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, filename, base_dir.c_str());
	
	if (!err.empty()) {
		fprintf(stderr, "%s\n", err.c_str());
	}

	if (!ret) {
		fprintf(stderr, "Failed to load %s \n", filename);
		return false;
	}

	printf("# of vertices  = %d\n", (int)(attrib.vertices.size()) / 3);
	printf("# of normals   = %d\n", (int)(attrib.normals.size()) / 3);
	printf("# of texcoords = %d\n", (int)(attrib.texcoords.size()) / 2);
	printf("# of materials = %d\n", (int)materials.size());
	printf("# of shapes    = %d\n", (int)shapes.size());

	  /// Append 'default' material
	materials.push_back(tinyobj::material_t());

	  /// Load diffuse textures
	{
		for (size_t m = 0; m < materials.size(); m++) {
			tinyobj::material_t* mp = &materials[m];
          
			if (mp->diffuse_texname.length() > 0) {
			    /// Only load the texture if it is not already loaded
				if (textures.find(mp->diffuse_texname) == textures.end()) {
					GLuint texture_id;
					int w, h;
					int comp;

					std::string texture_filename = mp->diffuse_texname;
					if (!FileExists(texture_filename)) {
					  /// Append base dir.
						texture_filename = base_dir + mp->diffuse_texname;
						if (!FileExists(texture_filename)) {
							fprintf(stderr, "Unable to find file: %s\n", mp->diffuse_texname.c_str());
							exit(1);
						}
					}
					
					{
						// Loading the texture from a file with stb
						Texture texture(texture_filename);
						textures.insert(std::make_pair(mp->diffuse_texname, texture.textureID));
					}
				}
			}
		}
	}

	bmin[0] = bmin[1] = bmin[2] = std::numeric_limits<float>::max();
	bmax[0] = bmax[1] = bmax[2] = -std::numeric_limits<float>::max();

	{
		/// Loading in the rest of the obj
		for (size_t s = 0; s < shapes.size(); s++) {
			/// Create a new drawobject for every shape we have
			DrawObject o;
			
			std::vector<float> vb;  // pos(3float), normal(3float), color(3float), texcoords(2float)
			
			for (size_t f = 0; f < shapes[s].mesh.indices.size() / 3; f++) {
				tinyobj::index_t idx0 = shapes[s].mesh.indices[3 * f + 0];
				tinyobj::index_t idx1 = shapes[s].mesh.indices[3 * f + 1];
				tinyobj::index_t idx2 = shapes[s].mesh.indices[3 * f + 2];
        
				int current_material_id = shapes[s].mesh.material_ids[f];

				if ((current_material_id < 0) || (current_material_id >= static_cast<int>(materials.size()))) {
				  // Invaid material ID. Use default material.
					current_material_id = materials.size() - 1; // Default material is added to the last item in `materials`.
				}
				
				if (current_material_id >= materials.size()) {
				fprintf(stderr, "Invalid material index %d\n", current_material_id);
				current_material_id = materials.size() - 1; /// Default material is added to the last item in `materials`.
				}
				
				float diffuse[3];
				for (size_t i = 0; i < 3; i++) {
					diffuse[i] = materials[current_material_id].diffuse[i];
				}
				
				//Calculate the texture coordinates
				float tc[3][2];
				if (attrib.texcoords.size() > 0) {
					assert(attrib.texcoords.size() > 2 * idx0.texcoord_index + 1);
					assert(attrib.texcoords.size() > 2 * idx1.texcoord_index + 1);
					assert(attrib.texcoords.size() > 2 * idx2.texcoord_index + 1);
					
				/// The division by materials[current_material_id].diffuse_texopt.scale[] solves the problem of textures
                /// Being stretched on the whole surface. If we want to repeat a texture we need to give the uv coordinates
                /// a value > 1. In example a value of 2 means the texture is going to be repeated twice.
                /// The scale value ranges from 0.0 to 1.0 so dividing by a number < 0 we're actually increasing the uv values
                /// and our textures will be repeated accordingly.

					tc[0][0] = attrib.texcoords[2 * idx0.texcoord_index] / materials[current_material_id].diffuse_texopt.scale[0];
					tc[0][1] = 1.0f - attrib.texcoords[2 * idx0.texcoord_index + 1] / materials[current_material_id].diffuse_texopt.scale[1];
					tc[1][0] = attrib.texcoords[2 * idx1.texcoord_index] / materials[current_material_id].diffuse_texopt.scale[0];
					tc[1][1] = 1.0f - attrib.texcoords[2 * idx1.texcoord_index + 1] / materials[current_material_id].diffuse_texopt.scale[1];
					tc[2][0] = attrib.texcoords[2 * idx2.texcoord_index] / materials[current_material_id].diffuse_texopt.scale[0];
					tc[2][1] = 1.0f - attrib.texcoords[2 * idx2.texcoord_index + 1] / materials[current_material_id].diffuse_texopt.scale[1];
				} else {
				/// If we have no texCoord just set everything to 0.
					tc[0][0] = 0.0f;
					tc[0][1] = 0.0f;
					tc[1][0] = 0.0f;
					tc[1][1] = 0.0f;
					tc[2][0] = 0.0f;
					tc[2][1] = 0.0f;
				}

				/// Calculate ther vertices
				float v[3][3];
				for (int k = 0; k < 3; k++) {
					int f0 = idx0.vertex_index;
					int f1 = idx1.vertex_index;
					int f2 = idx2.vertex_index;
					assert(f0 >= 0);
					assert(f1 >= 0);
					assert(f2 >= 0);

					v[0][k] = attrib.vertices[3 * f0 + k];
					v[1][k] = attrib.vertices[3 * f1 + k];
					v[2][k] = attrib.vertices[3 * f2 + k];
					bmin[k] = std::min(v[0][k], bmin[k]);
					bmin[k] = std::min(v[1][k], bmin[k]);
					bmin[k] = std::min(v[2][k], bmin[k]);
					bmax[k] = std::max(v[0][k], bmax[k]);
					bmax[k] = std::max(v[1][k], bmax[k]);
					bmax[k] = std::max(v[2][k], bmax[k]);
				}
				
				// Create Triangle Objects with the triangle and push it into the vector
				glm::vec3 vertex0(v[0][0], v[0][1], v[0][2]);
				glm::vec3 vertex1(v[1][0], v[1][1], v[1][2]);
				glm::vec3 vertex2(v[2][0], v[2][1], v[2][2]);
				Triangle triangle(vertex0, vertex1, vertex2, false);
				o.triangles.push_back(triangle);

				/// Calculate the normals
				float n[3][3];
				if (attrib.normals.size() > 0) {
					int f0 = idx0.normal_index;
					int f1 = idx1.normal_index;
					int f2 = idx2.normal_index;
					assert(f0 >= 0);
					assert(f1 >= 0);
					assert(f2 >= 0);
					for (int k = 0; k < 3; k++) {
						n[0][k] = attrib.normals[3 * f0 + k];
						n[1][k] = attrib.normals[3 * f1 + k];
						n[2][k] = attrib.normals[3 * f2 + k];
					}
				}
				else {
				  /// If there are no normals in the model, we create them
					CalcNormal(n[0], v[0], v[1], v[2]);
					n[1][0] = n[0][0];
					n[1][1] = n[0][1];
					n[1][2] = n[0][2];
					n[2][0] = n[0][0];
					n[2][1] = n[0][1];
					n[2][2] = n[0][2];
				}

				/// Push back the vertices into the drawobject for the shape
				for (int k = 0; k < 3; k++) {
					vb.push_back(v[k][0]);
					vb.push_back(v[k][1]);
					vb.push_back(v[k][2]);
					vb.push_back(n[k][0]);
					vb.push_back(n[k][1]);
					vb.push_back(n[k][2]);
										
					/// Combine normal and diffuse to get color.
					float normal_factor = 0.2;
					float diffuse_factor = 1 - normal_factor;
					
					///Calculate the colour
					float c[3] = {
						n[k][0] * normal_factor + diffuse[0] * diffuse_factor,
						n[k][1] * normal_factor + diffuse[1] * diffuse_factor,
						n[k][2] * normal_factor + diffuse[2] * diffuse_factor
					};
					float len2 = c[0] * c[0] + c[1] * c[1] + c[2] * c[2];
					if (len2 > 0.0f) {
						float len = sqrtf(len2);

						c[0] /= len;
						c[1] /= len;
						c[2] /= len;
					}
					
					/// Push back the colour 
					vb.push_back(c[0] * 0.5 + 0.5);
					vb.push_back(c[1] * 0.5 + 0.5);
					vb.push_back(c[2] * 0.5 + 0.5);
          
					/// Push back the UV coordinates
					vb.push_back(tc[k][0]);
					vb.push_back(tc[k][1]);
				}
			}

			o.vb = 0;
			o.numTriangles = 0;

			/// OpenGL viewer does not support texturing with per-face material.
			if (shapes[s].mesh.material_ids.size() > 0 && shapes[s].mesh.material_ids.size() > s) {
			    /// Base case
				o.material_id = shapes[s].mesh.material_ids[s];
			}
			else {
				o.material_id = materials.size() - 1; // = ID for default material.
			}
          
			/// Now that we have loaded in all the data, we ar going to create the buffer for this shape.
			if (vb.size() > 0) {
				glGenBuffers(1, &o.vb);
				glBindBuffer(GL_ARRAY_BUFFER, o.vb);
				glBufferData(GL_ARRAY_BUFFER, vb.size() * sizeof(float), &vb.at(0), GL_STATIC_DRAW);
				o.numTriangles = vb.size() / ((3 + 3 + 3 + 2) * 3);
				printf("shape[%d] # of triangles = %d\n", static_cast<int>(s), o.numTriangles);
			}
			drawObjects->push_back(o);
		}
	}

	printf("bmin = %f, %f, %f\n", bmin[0], bmin[1], bmin[2]);
	printf("bmax =  %f,  %f,  %f\n", bmax[0], bmax[1], bmax[2]);

	printf("#Succesfully loaded the OBJ File...       #\n");
	printf("###########################################\n");
	return true;
}
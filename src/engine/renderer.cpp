#include "renderer.hpp"

void renderer_static_init(RendererStatic* renderer) {
	shader_static_init(&renderer->shader, program_init("default"));
	renderer->fov = 60.0;
	renderer->plane_near = 0.1;
	renderer->plane_far = 200.0;
}

void solid_render(GLuint program_id, Transform* transform, Mesh* mesh) {
	glm::mat4 mat_model = transform_model_compute(transform);
	glUniformMatrix4fv(glGetUniformLocation(program_id, "matModel"), 1, GL_FALSE, glm::value_ptr(mat_model));

	glm::mat3 matNormal = glm::transpose(glm::inverse(mat_model));
	glUniformMatrix3fv(glGetUniformLocation(program_id, "matNormal"), 1, GL_FALSE, glm::value_ptr(matNormal));

	mesh_texture_draw(mesh);
}

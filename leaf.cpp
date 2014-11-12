#include "leaf.h"
#include "tree.h"
//float equation(float x)
//{
//    return std::sqrt(std::max(0.0f, 1.0f - 80.f * static_cast<float>(cos (x) * cos(x)) * (x * x) /20.0f));
//}

void leaf::init_matrix() {
    model_matrix = glm::mat4();
    model_matrix = glm::translate(model_matrix,glm::vec3(-1.0f/sqrt(2.0f)-0.5f,1.0f/sqrt(2.0f)+1.0f,0.0f));
    object::init_matrix();
    model_matrix = glm::scale(model_matrix,0.3f*glm::vec3(w * 1.0f, l * 1.0f,w * 1.0f));

}

void leaf::draw()
{
    leafVis *f = Tree::getLeafVis();
    Tree::gui_pre_frame(model_matrix, -1);
    f->visualize();
}

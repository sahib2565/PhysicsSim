#include <glm/glm.hpp>
#include <iostream>

int main() {
    glm::vec3 v(1.0f, 2.0f, 3.0f);
    std::cout << "Vector: (" << v.x << ", " << v.y << ", " << v.z << ")" << std::endl;
    return 0;
}
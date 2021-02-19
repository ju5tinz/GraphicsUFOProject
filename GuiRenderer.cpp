//  adapted from youtube tutorial
//  https://www.youtube.com/watch?v=vOmJ1lyiJ4A&list=PLRIWtICgwaX0u7Rf9zkZhLoLuZVfUksDP&index=25&t=0s
//
#include "GuiRenderer.hpp"

GuiRenderer::GuiRenderer(){
    std::vector<float> vertices = {
            //triangle 1
            -1.f,  1.f,
            -1.f, -1.f,
             1.f,  1.f,
            //triangle 2
             1.f,  1.f,
            -1.f, -1.f,
             1.f, -1.f
        };

        
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    // Bind to the VAO.
    glBindVertexArray(vao);

    // Bind to the first VBO. We will use it to store the vertices.
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
    
    // Unbind from the VBOs & VAO.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    shader = new GuiShader();
}

void GuiRenderer::render(std::vector<GuiTexture *> guis){
    shader->start();
    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);
    glEnable(GL_BLEND); //for transparency
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST);
    //render
    for(GuiTexture * gui: guis){
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, gui->getTexture());
        shader->loadtexture();
        glm::mat4 matrix = createTransformationMatrix(gui->getPosition(), gui->getScale());
        shader->loadTransformation(matrix);
        
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    glDisableVertexAttribArray(0);
    glBindVertexArray(0);
    shader->stop();
}

glm::mat4 GuiRenderer::createTransformationMatrix(glm::vec2 translation, glm::vec2 scale){
    glm::mat4 matrix = glm::mat4(1);
    matrix = glm::translate(glm::vec3(translation, 0)) * matrix;
    matrix = glm::scale(glm::vec3(scale.x, scale.y, 1.f)) * matrix;
    return matrix;
}

GuiRenderer::~GuiRenderer(){
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
}

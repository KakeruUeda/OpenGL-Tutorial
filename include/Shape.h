#pragma once
#include <memory>

#include "Object.h"

class Shape
{
    std::shared_ptr<const Object> object;

protected:
    const GLsizei vertexcount;

public:
    Shape(GLint size, GLsizei vertexcount, const Object::Vertex *vertex, GLsizei indexcount = 0,
          const GLuint *index = NULL)
        : object(new Object(size, vertexcount, vertex, indexcount, index)), vertexcount(vertexcount)
    {
    }
    
    virtual ~Shape()
    {
    }

    void draw() const
    {
        object->bind();
        excute();
    }

    virtual void excute() const
    {
        glDrawArrays(GL_LINE_LOOP, 0, vertexcount);
    }
};
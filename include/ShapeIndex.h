#pragma once

#include "Shape.h"

class ShapeIndex : public Shape
{
protected:
    const GLsizei indexcount;

public:
    ShapeIndex(GLint size, GLsizei vertexcount, const Object::Vertex *vertex, GLsizei indexcount, const GLuint *index)
        : Shape(size, vertexcount, vertex, indexcount, index), indexcount(indexcount)
    {
    }

    virtual void excute() const
    {
        glDrawElements(GL_LINES, indexcount, GL_UNSIGNED_INT, 0);
    }
};
#pragma

#include "ShapeIndex.h"

class SolidShapeIndex : public ShapeIndex
{
public:
    SolidShapeIndex(GLint size, GLsizei vertexcount, const Object::Vertex *vertex, GLsizei indexcount,
                    const GLuint *index)
        : ShapeIndex(size, vertexcount, vertex, indexcount, index)
    {
    }

    virtual void excute() const
    {
        glDrawElements(GL_TRIANGLES, indexcount, GL_UNSIGNED_INT, 0);
    }
};
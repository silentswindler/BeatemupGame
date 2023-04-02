#include "VAO.h"

VAO::VAO()
{
	glGenVertexArrays(1, &ID);
}

void VAO::LinkAttrib(VBO& VBO, GLuint layout, GLuint numComponenets, GLenum type, GLsizeiptr stride, void* offset)
{
	VBO.Bind();
	glVertexAttribPointer(layout, numComponenets, type, GL_FALSE, stride, offset);
	glEnableVertexAttribArray(layout);
	VBO.UnBind();
}

void VAO::Bind()
{
	glBindVertexArray(ID);

}

void VAO::UnBind()
{
	glBindVertexArray(0);

}

void VAO::Delete()
{
	glDeleteVertexArrays(1, &ID);

}
#include "Box.h"




void Box::initGeom()
{
    float vtx[8][4] = { {-1, -1, -1, 1},{1, -1, -1, 1},
                       {1, -1, 1, 1}, {-1, -1, 1, 1},
                       {-1, 1, -1, 1}, {1, 1, -1, 1},
                       {1, 1, 1, 1}, {-1, 1, 1, 1}
    };

    float col[8][4] = { {1, 0, 0, 1},{0, 1, 0, 1},
                        {0, 0, 1, 1}, {1, 1, 0, 1},
                        {1, 0, 1, 1}, {0, 1, 1, 1},
                        {0.5, 0.5, 0, 1}, {0.5, 0, 0.5, 1}
    };

    // using a for loop push the vertex information into the m_vertices structure 
    // by invoking the  m_vertices.push_back() function.
    // note that you will have to assemble the data for each vertex (i.e., pos and col)
    for (int i = 0; i < 8; i++) {
        Vertex v = Vertex(Vector4f(vtx[i][0], vtx[i][1], vtx[i][2], vtx[i][3]),
            Vector4f(col[i][0], col[i][1], col[i][2], col[i][3]));
        m_vertices.push_back(v);
    }

    int indices[] = {
        // front face
        0, 1, 4,
        1, 5, 4,
        // top face
        4, 5, 6,
        6, 7, 4,
        // back face
        2, 6, 7,
        3, 2, 7,
        // bottom face
        0, 2, 1,
        2, 0, 3,
        // left face
        0, 4, 7,
        0, 7, 3,
        // right face
        1, 2, 5,
        2, 6, 5
    };

    for (int i = 0; i < 36; i++) {
        m_indices.push_back(indices[i]);
    }
}


//trnMat = this->worldMat * modelMat;



/**************************************************************************************/

// Render the Cube

int Box::render()
{
    int i;
    int vtxIndex;
    Vector4f p;

    Matrix4f trnMat;	// world matrix


    trnMat = Matrix4f::scale(scale.x, scale.y, scale.z);
    //trnMat.printMatrix("trnMat is");

    glBegin(GL_TRIANGLES);
    //glBegin(GL_POINTS);
    for (i = 0; i < m_indices.size(); ++i) {
        // get the vertex index
        vtxIndex = m_indices[i];


        // set the vertex colour
        glColor3f(m_vertices[vtxIndex].col.r, m_vertices[vtxIndex].col.g,
            m_vertices[vtxIndex].col.b);

        // compute the vertex transformation
        p = trnMat * m_vertices[vtxIndex].pos;

        // add the vertext to the triangle list
        glVertex3f(p.x, p.y, p.z);
    }
    glEnd();




    return(0);
}




#include <string>
#include <GL/glew.h>
#include <GL/glut.h>
#include "trfm3D.h"
#include "texture.h"

class ProjectiveTexture {

public:

    // La funcion constructora ProjectiveTexture recibe el nombre de la textura a
    // proyectar y el nombre de la camara en perspectiva asociada
    ProjectiveTexture(const std::string & texName,const std::string & camName);

    // Devuelve la transformacion de la textura proyectiva
    Trfm3D &getMatrix();

    // Devuelve la textura asociada
    Texture *getTexture();

    // Transforma la posici´on de la textura proyectiva (y demás) mediante la matriz 
    // modelview de la escena 
    void PlaceScene();

private:

    void createProjectiveTrfm();

    std::string m_texName;     // nombre de la textura a proyectar
    std::string m_camName;  // nombre de la camara en perspectiva asociada
    Trfm3D    *m_projTrfm;  // transformacion de la textura proyectiva

};


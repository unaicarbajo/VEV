#include <string>
#include <GL/glew.h>
#include <GL/glut.h>
#include "trfm3D.h"
#include "texture.h"
#include "camera.h"
#include "renderState.h"
class ProjectiveTexture {

public:

    // La funcion constructora ProjectiveTexture recibe el nombre de la textura a
    // proyectar y el nombre de la camara en perspectiva asociada
    ProjectiveTexture(const std::string & texName,const std::string & camName);

    // Devuelve la transformacion de la textura proyectiva
    Trfm3D &getMatrix();

    // Devuelve la textura asociada
    Texture *getTexture();

    // Transforma la posicion de la textura proyectiva (y demás) mediante la matriz 
    // modelview de la escena 
    void placeScene();

private:

    // Se utiliza para crear la matriz de proyeccion
    void createProjectiveTrfm();

    // Guardo solo los nombres, no es necesario almacenar la textura ni la cámara
    // ya que solo se utilizan una única vez.
    Texture     *m_tex;         // textura a proyectar
    Camera      *m_cam;         // camara en perspectiva asociada
    Trfm3D      *m_projTrfm;    // transformacion de la textura proyectiva

};


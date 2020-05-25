#include <stdlib.h>
#include <stdio.h>
#include "projectiveTexture.h"
#include "textureManager.h"
#include "shader.h"
#include "vector3.h"
#include "cameraManager.h"
#include "camera.h"



using std::string;

ProjectiveTexture::ProjectiveTexture(const std::string & texName,const std::string & camName):
    m_camName(camName),
    m_texName(texName),
    m_projTrfm(new Trfm3D())
    {
        createProjectiveTrfm();
    }

Texture *ProjectiveTexture::getTexture(){
    TextureManager *texManager = TextureManager::instance();
    return texManager->find(this->m_texName);
}

Trfm3D &ProjectiveTexture::getMatrix(){
    return *m_projTrfm;
}

void ProjectiveTexture::createProjectiveTrfm(){
    // I*Mts*Mp*Mb
    //
    // I: Matriz identidad
    // Mts: Matriz utilizada para pasar las coordenadas del objeto de [-1,1]
    //      a [0,1]
    // Mp:  Matriz de proyección que tiene en cuenta el frustrum de la textura
    //      proyectiva
    // Mb:  Matriz de transformaci´on de vista de la textura proyectiva que coloca
    //      la textura en el origen, al igual que se posiciona la c´amara en el origen

    Camera *cam = CameraManager::instance()->find(m_camName);

    // Se ha inicializado la matriz como unidad

    // Mts= | 1/2 0   0   1/2 |
    //      | 0   1/2 0   1/2 |
    //      | 0   0   1/2 1/2 |
    //      | 0   0   0   1   |
    // Mts utilizado para realizar (P.p+1)/2    Siendo P.p cada elemento del punto

    m_projTrfm->setTrans(*new Vector3(0.5f,0.5f,0.5f));
    m_projTrfm->addScale(0.5f);

    // Se multiplica matriz Mp
    const Trfm3D *mp = cam->projectionTrfm();
    m_projTrfm->add(mp);

    // Se multiplica matriz Mb
    const Trfm3D *mb = cam->viewTrfm();
    m_projTrfm->add(mb);
}

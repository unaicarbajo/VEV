#include <stdlib.h>
#include <stdio.h>
#include "textureManager.h"
#include "shader.h"
#include "vector3.h"
#include "cameraManager.h"

using std::string;

ProjectiveTexture::ProjectiveTexture(const std::string & texName,const std::string & camName):
    m_cam(CameraManager::instance()->find(camName)),
    m_tex(TextureManager::instance()->find(texName)),
    m_upOrig(m_cam->getUp()),
    m_atOrig(m_cam->getAt()),
    m_posOrig(m_cam->getPosition())
    { createProjectiveTrfm();}

Texture *ProjectiveTexture::getTexture(){ return m_tex; }

Trfm3D &ProjectiveTexture::getMatrix(){
    if (m_cam != 0) createProjectiveTrfm();
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
    m_projTrfm = new Trfm3D();
    // Se ha inicializado la matriz como unidad

    // Mts= | 1/2 0   0   1/2 |
    //      | 0   1/2 0   1/2 |
    //      | 0   0   1/2 1/2 |
    //      | 0   0   0   1   |
    // Mts utilizado para realizar (P.p+1)/2    Siendo P.p cada elemento del punto
    // con el fin de mapear de coordenadas establecidas en [-1,1] a [0,1]

    m_projTrfm->setTrans(*new Vector3(0.5f,0.5f,0.5f));
    m_projTrfm->addScale(0.5f);

    // Se multiplica matriz Mp
    const Trfm3D *mp = m_cam->projectionTrfm();
    m_projTrfm->add(mp);

    // Se multiplica matriz Mb
    const Trfm3D *mb = m_cam->viewTrfm();
    m_projTrfm->add(mb);

}

void ProjectiveTexture::placeScene(){
    if (m_cam != 0){
        RenderState *rs = RenderState::instance();
	    Trfm3D &modelView = rs->top(RenderState::modelview);
        Vector3 E  = modelView.transformPoint(m_posOrig);
        Vector3 at = modelView.transformPoint(m_atOrig);
        // El vector up no se normaliza, ya que al hacer loopAt(..) se normaliza
        Vector3 up = modelView.transformVector(m_upOrig);
        m_cam->lookAt(E,at,up);
    }
}
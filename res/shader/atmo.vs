#version 330 core

layout(location = 0) in vec3 positionVertex;

uniform mat4 matrixMVP;
uniform mat4 matrixModel;
uniform vec3 positionPlanetCenterW;
uniform vec3 positionCameraW;

out vec3 positionFragmentW;
out vec3 positionEntryW;
out vec3 directionCamera;

void main() {
	gl_Position = matrixMVP * vec4(positionVertex, 1);
	positionFragmentW = (matrixModel * vec4(positionVertex, 1)).xyz;
	positionEntryW = positionPlanetCenterW - positionFragmentW;
	directionCamera = normalize(positionPlanetCenterW - positionFragmentW);
}


/*
// attr
varying vec3 positionVertex;

// uniform
varying mat4 matrixMVP;
varying mat4 matrixModel;
varying vec3 positionPlanetCenterW;
varying vec3 positionCameraW;

// out
varying vec3 positionFragmentW;
varying vec3 positionEntryM;
varying vec3 directionEntry;
varying vec3 directionCamera;


void main() {
    // default
    positionVertex = position;
    matrixMVP = projectionMatrix * modelViewMatrix;
    matrixModel = modelMatrix;
    positionPlanetCenterW = vec3(0,0,0);
    positionCameraW = cameraPosition;

    // main
	gl_Position = matrixMVP * vec4(positionVertex, 1);
	positionFragmentW = vec3(matrixModel * vec4(positionVertex, 1));
	positionEntryM = positionFragmentW - positionPlanetCenterW;
	directionEntry = normalize(positionEntryM);
	directionCamera = normalize(positionCameraW - positionFragmentW);
}
*/

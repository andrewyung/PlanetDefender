# OpenglTriangularization
Work in progress. So far uses glew, freeglut, glm and OBJ_Loader

<img src="/gifs/camera.gif?raw=true">
<img src="/gifs/instancing.gif?raw=true">
	1 mil+ instances of triangle<br /><br />

<img src="/gifs/lights.gif?raw=true">
	Diffuse shading with multiple lights<br /><br />
<img src="/gifs/earsplitting.gif?raw=true">
	Ear cutting algorithm implementation for polygons<br /><br />

### Implementations:

-Instancing

-Add additional models into existing VBOs if possible (for static models)

-Model transformations within a game loop

-Uses OBJ_Loader to load and render .obj models

-Camera translation and rotation

-Diffuse shader


### TODO:

-Implement model materials

-Determine and use library for delaunay triangularization

# path-tracer
Multi-threaded Monte Carlo Path-Tracer in C++.

# Current Features
- Objects: spheres, planes, boxes.
- Materials: Lambertian, Dielectrics and Metals.
- Matrix transforms can be applied to any object.
- Area lights. Every object can be a light by having an emissive material.
- Camera positioning and aperture/focal distance (depth of field) simulation.
- Image and procedural textures.
- Normal mapping.
- Bounding volume hierarchy (AABB) for optimized scene traversal.
- Randomized super-sampling antialiasing.
- Multi-threaded rendering.
- sRGB/Gamma correct rendering.
- Detailed render statistics feedback.

# License
Licensed under the MIT license, see [LICENSE](https://github.com/MadEqua/path-tracer/blob/master/LICENSE) for details.
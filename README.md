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
- Bloom post-processing effect to emphasize HDR rendering.
- Tone Mapping and sRGB/Gamma correction when outputing to a non-HDR format.
- Multi-threaded rendering.
- Detailed render statistics feedback.

# License
Licensed under the MIT license, see [LICENSE](https://github.com/MadEqua/path-tracer/blob/master/LICENSE) for details.
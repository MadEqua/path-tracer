# path-tracer
Multi-threaded Path-Tracer in C++.

# Current Features
- Objects: spheres, planes and boxes.
- Materials: Lambertians, Dielectrics and Metals.
- Matrix transforms can be applied to any object.
- Area lights. Every object can be a light by having an emissive material.
- Camera positioning and aperture/focal distance (depth of field) simulation.
- Image and procedural textures.
- Normal mapping.
- Bounding volume hierarchy (AABB) for optimized scene traversal.
- Randomized super-sampling antialiasing.
- Image based-lighting through HDR light probes.
- Bloom post-processing effect to emphasize HDR rendering.
- Tone Mapping and sRGB/Gamma correction when outputing to a non-HDR format.
- Multi-threaded rendering.
- Detailed render statistics feedback.

# Example Renders
![](http://bmlourenco.com/portfolio/path-tracer/images/gallery/1.jpg)
![](http://bmlourenco.com/portfolio/path-tracer/images/gallery/2.jpg)
![](http://bmlourenco.com/portfolio/path-tracer/images/gallery/3.jpg)
![](http://bmlourenco.com/portfolio/path-tracer/images/gallery/5.jpg)

# License
Licensed under the MIT license, see [LICENSE](https://github.com/MadEqua/path-tracer/blob/master/LICENSE) for details.

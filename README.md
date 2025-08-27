# 42-miniRT

<p align='center'>
   	<a href="https://www.linux.org/">
      	<img src="https://img.shields.io/badge/OS-Linux-blue?logo=linux&logoColor=white"
            alt="OS - Linux"></a>
   	<a href="https://wakatime.com/badge/user/7d36b55a-b5ff-46c4-b7fd-57604568d382/project/2be7688e-8980-4f81-b3b5-0d735d78b506">
		<img src="https://wakatime.com/badge/user/7d36b55a-b5ff-46c4-b7fd-57604568d382/project/2be7688e-8980-4f81-b3b5-0d735d78b506.svg"
			alt="wakatime"></a>
	<a href="https://wakatime.com/badge/user/2cf422fc-7a8c-481e-9a55-c4b5a1e914c2/project/e05ba099-dace-459e-9e7f-91951edb42a6">
      	<img src="https://wakatime.com/badge/user/2cf422fc-7a8c-481e-9a55-c4b5a1e914c2/project/e05ba099-dace-459e-9e7f-91951edb42a6.svg"
            alt="wakatime"></a>
	<a href="https://img.shields.io/badge/C-00599C?style=for-the-badge&logo=c&logoColor=white">
		<img src="https://img.shields.io/badge/C-00599C?logo=c&logoColor=white"
			alt="C"></a>
	<a href="https://github.com/Liammmmmmmm/42-miniRT/wiki">
		<img src="https://img.shields.io/badge/Wiki-Documentation-green?logo=github&logoColor=white"
			alt="Wiki"></a>
</p>

![Sponza](https://imgur.com/QvYqsTx.png)

## Introduction

It all started with a classic **miniRT** project from the 42 curriculum: coding a basic raytracer in C with a few spheres and a plane. But with [madelvin](https://github.com/delmath), we quickly thought: *"What if we pushed this a bit further?"*

Six months later, here we are with a complete **path tracer**, capable of generating better renders than we could have ever imagined at the start.

## What we achieved

In the end, we turned this small raytracer into something much more ambitious:

- **A true path tracer** that faithfully simulates the physics of light
- **An editing interface** to create and modify scenes in real-time (by integrating a whole component system with mlx)
- **3D model support**: goodbye spheres, hello dragons and cathedrals!

![dinner table](https://imgur.com/b63ZIF1.png)
> Looks real, doesn't it?

## Want to know more?

We've documented (almost) everything in our **[Wiki](https://github.com/Liammmmmmmm/42-miniRT/wiki)**! 

Whether you're curious about how path tracing works, want to implement your own BVH, or just want to see more pretty pictures, you'll find what you're looking for in the documentation.

**Some cool entry points:**
- **[Path Tracing & Materials](https://github.com/Liammmmmmmm/42-miniRT/wiki/Ray-tracing-&-Material)** - The core of the rendering engine
- **[BVH](https://github.com/Liammmmmmmm/42-miniRT/wiki/BVH)** - How we handle scenes with millions of triangles
- **[HDRI Skybox](https://github.com/Liammmmmmmm/42-miniRT/wiki/Skybox)** - For realistic lighting
- **[Caustics](https://github.com/Liammmmmmmm/42-miniRT/wiki/Caustics)** - Because reflections in water are beautiful

## Installation

```bash
sudo apt install libx11-dev libx11-xcb-dev libxext-dev libxrandr-dev libxfixes-dev libxrender-dev libxinerama-dev libbsd-dev libglew-dev libglfw3-dev

git clone https://github.com/Liammmmmmmm/42-miniRT.git
git submodule update --init
cd 42-miniRT

make

./miniRT scenes/example.rt
```

## Authors

**[Liammmmmmmm](https://github.com/Liammmmmmmm)** & **[delmath](https://github.com/delmath)**

*Project carried out as part of the 42 curriculum, but we admit we got a little carried away...*

---

📖 **[Check out our Wiki for the full documentation](https://github.com/Liammmmmmmm/42-miniRT/wiki)**

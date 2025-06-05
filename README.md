
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
</p>



# *mini***RT**

Courte description

## Content table
- [*mini***RT**](#minirt)
	- [Content table](#content-table)
	- [Features](#features)
		- [Path tracing \& material managment](#path-tracing--material-managment)
		- [Primitives objects](#primitives-objects)
		- [Custom objects](#custom-objects)
		- [Textures](#textures)
		- [Lights](#lights)
		- [Bounding Volume Hierarchie (BVH)](#bounding-volume-hierarchie-bvh)
		- [Control UI](#control-ui)
		- [Basics options + animate option](#basics-options--animate-option)
			- [`--max-samples=x`](#--max-samplesx)
			- [`--output-dir=/dir_example/`](#--output-dirdir_example)
			- [`--auto-export`](#--auto-export)
			- [`--no-display`](#--no-display)
			- [`--animate=""`](#--animate)
			- [`--standard-galactic-alphabet`](#--standard-galactic-alphabet)
	- [Installation](#installation)
	- [Resources](#resources)
	- [PARTIE RANDOM A VIRER UN JOUR](#partie-random-a-virer-un-jour)


## Features

### Path tracing & material managment

Illustrer d'une maniere ou d'une autre un peu tous les types de materiaux

<details>
	<summary><strong>See more about it</strong></summary>
	<br>


</details>

### Primitives objects

IMAGE POUR ILLUSTRER DANS LAQUELLE Y'A UNE LISTE DE TOUS LES OBJETS (J'aime bcp la maniere dont ils sont presentés sur le minirt de bwerner)

<details>
	<summary><strong>See more about it</strong></summary>
	<br>

Our minirt implements mulitples elementals objects :
- Sphere
- Planes
- Cylinder
- Cone
- Hyperboloid

</details>

### Custom objects

Giga image d'un modele 3d pour illustrer

<details>
	<summary><strong>See more about it</strong></summary>
	<br>

Parler vite fais de comment tu parse ca et tout

</details>

### Textures

Image d'illustration de differentes textures genre obsidian ou un truc dans le genre

<details>
	<summary><strong>See more about it</strong></summary>
	<br>

In order to add realism to your scenes you can add multiple layer of textures :
- Color texture
- Metallic map
- Roughness map
- Ambiant occlusion map
- Normal & bump map
- transmission map
- emission strength map
- emission color map

You can import your textures in bmp format (*maybe png later*)
- Local checkerboard texture (prends en parametre 2 couleurs, un "nombre de repetitions"/scale)
- Global checkerboard texture (same mais pour faire une solid texture)

*Maybe later to* :
- Text texture (ce serait un truc tu passe 2 color (fond et text), un string, et le path vers un ttf et ca te gen une texture avec ca)


</details>

### Lights

Image avec differentes light de couleurs points/directionnals/emissive/ambiant a presenter idealement

<details>
	<summary><strong>See more about it</strong></summary>
	<br>

Decrire les differents types de light 2 catégories avec d'un coté les emissives et l'ambiant qui sont des lights "realistes" la ou les autre moins

</details>

### Bounding Volume Hierarchie (BVH)

Image d'illustration

<details>
	<summary><strong>See more about it</strong></summary>
	<br>

More information
Mathis tu gere ce truc x)
Du coup peut etre parler de trucs comme le bvh itteratif avec la task pool, decrire le fonctionnement de creation et celui d'utilisation pour l'affichage

</details>

### Control UI

Image d'illustration

Doing UI in c is not an easy thing, from text displaying to custom reusable control components there is a lot to talk about

<details>
	<summary><strong>See more about it</strong></summary>
	<br>

Explication ttf
Description des différents composants et utilisation de l'ui
parler aussi un peu de micrort pour modifier les materiaux

</details>

### Basics options + animate option

GIF D'UNE ANIMATION

<details>
	<summary><strong>See more about it</strong></summary>
	<br>

Differents options are available in our miniRT, basics ones and more complex ones.

#### `--max-samples=x`

Sets the maximum number of samples for a render.

#### `--output-dir=/dir_example/`

Specifies the output directory where rendered images are saved, to avoid cluttering the home directory.

#### `--auto-export`

Automatically exports the current frame once the maximum number of samples is reached.

#### `--no-display`

Disables the image preview and replaces it with a minimal UI displaying useful information such as average time per sample and estimated time remaining.

#### `--animate=""`

Defines an animation for the scene and generates all frames accordingly.

#### `--standard-galactic-alphabet`

If you can fluently read the standard galactic alphabet, this option is for you

---

> How to Create an Animation

To create an animation, start by setting the `--animate` flag:

```bash
--animate=""
```

Then specify the parameters:

**1. Select the Object to Animate**

You must first indicate which object to animate:

* `C:` → animates the **camera**
* `sp:` → animates the **first sphere** in the scene
* `co.3:` → animates the **fourth cone** (index starts at 0)

**2. Define the Trajectory**

You must provide at least two points with frame indices to define the animation path.

Example:

```
C:x1,y1,z1,0 x2,y2,z2,60
```

In this example, the camera will move from `(x1,y1,z1)` at frame 0 to `(x2,y2,z2)` at frame 60. Intermediate frames are linearly interpolated (lerped) between the two positions.

You can also define more complex paths:

```
C:x1,y1,z1,0 x2,y2,z2,60 x3,y3,z3,120
```

Here, the camera moves from point 1 to point 2 in 60 frames, then from point 2 to point 3 in another 60 frames.

To create **Bezier curves**, insert control points (points **without** a frame index):

```
C:x1,y1,z1,0 x2,y2,z2 x3,y3,z3,120
```

In this example, `x2,y2,z2` is a control point, creating a Bezier curve between the start and end points.

You can define **as many points and segments** as you like.

**3. Define Orientation (Optional)**

You can also specify the orientation (rotation) of the object over time:

```
C:x1,y1,z1,0 x2,y2,z2 x3,y3,z3,120|1,0,0,0 0,0,1,120
```

After the `|` symbol, orientation keyframes are defined in the format `x,y,z,frame`. Here, the camera will rotate from `(1,0,0)` at frame 0 to `(0,0,1)` at frame 120, interpolating between them. x,y,z form a unit vector wich is the direction where the object is oriented to

Like position, you can define multiple rotation keyframes.

---

**Important Notes**

* The `--animate` option works for any object (not just the camera).
* You can animate multiple objects in the same scene. For example: `C:animation_properties;sp:animation_properties` will animate the camera and a sphere
* For a proper animation rendering workflow, you **should use** both `--max-samples` and `--auto-export`.
  * Without `--auto-export`, each frame will be computed but **not** saved.
  * Without `--max-samples`, each frame will take 100 000 samples to be saved

</details>

## Installation

git clone, make gnagnagna

## Resources

- [Ray Tracing in One Weekend](https://raytracing.github.io/)
- [Apple True Type reference manual](https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6.html)
- https://www.adobe.com/learn/substance-3d-designer/web/the-pbr-guide-part-2




## PARTIE RANDOM A VIRER UN JOUR

Random : textures
https://www.solarsystemscope.com/textures/
https://3dtextures.me/2019/11/21/brick-wall-017/

IOR
https://pixelandpoly.com/ior.html


Materials

https://www.cg.tuwien.ac.at/sites/default/files/course/4411/attachments/04_path_tracing_0.pdf


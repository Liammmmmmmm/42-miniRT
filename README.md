
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
		- [Objects](#objects)
		- [Textures](#textures)
		- [Lights](#lights)
		- [Bounding Volume Hierarchie (BVH)](#bounding-volume-hierarchie-bvh)
		- [Control UI](#control-ui)
	- [Installation](#installation)
	- [Resources](#resources)
	- [PARTIE RANDOM A VIRER UN JOUR](#partie-random-a-virer-un-jour)


## Features

### Objects

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

In addition of these objects you can add .obj objects to describe much more complex objects

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

*Maybe added later* :
- transmission map
- emission strength map
- emission color map

You can import your textures in bmp format (*maybe png later*)

*Maybe later to* :
- Local checkerboard texture (prends en parametre 2 couleurs, un "nombre de repetitions"/scale)
- Global checkerboard texture (same mais pour faire une solid texture)
- Text texture (ce serait un truc tu passe 2 color (fond et text), un string, et le path vers un ttf et ca te gen une texture avec ca)


</details>

### Lights

Image avec differentes light de couleurs, et idealement le jour ou l'emissive marche un truc avec jsp un text emissif en mode enseigne


### Bounding Volume Hierarchie (BVH)

Image d'illustration

<details>
	<summary><strong>See more about it</strong></summary>
	<br>

More information
Mathis tu gere ce truc x)

</details>

### Control UI

Image d'illustration

Doing UI in c is not an easy thing, from text displaying to custom reusable control components there is a lot to talk about

<details>
	<summary><strong>See more about it</strong></summary>
	<br>

Explication ttf
Description des différents composants et utilisation de l'ui


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


import bpy
import math
import re
import mathutils

def parse_rt_file(file_path):
    spheres = []
    cylinders = []
    planes = []
    cameras = []

    with open(file_path, 'r') as f:
        lines = f.readlines()
    def parse_value(value):
        return float(value.strip()) if value.strip() else 0.0
    for line in lines:
        if line.startswith("sp"):
            parts = line.replace(',', ' ').split()
            location = (parse_value(parts[1]), parse_value(parts[2]), parse_value(parts[3]))
            diameter = parse_value(parts[4])
            spheres.append((location, diameter))
    for line in lines:
        if line.startswith("cy"):
            parts = line.replace(',', ' ').split()
            location = (parse_value(parts[1]), parse_value(parts[2]), parse_value(parts[3]))
            orientation = (parse_value(parts[4]), parse_value(parts[5]), parse_value(parts[6]))
            diameter = parse_value(parts[7])
            height = parse_value(parts[8])
            cylinders.append((location, orientation, diameter, height))
    for line in lines:
        if line.startswith("pl"):
            parts = line.replace(',', ' ').split()
            location = (parse_value(parts[1]), parse_value(parts[2]), parse_value(parts[3]))
            orientation = (parse_value(parts[4]), parse_value(parts[5]), parse_value(parts[6]))
            planes.append((location, orientation))

    for line in lines:
        if line.startswith("C"):
            parts = line.replace(',', ' ').split()
            location = (parse_value(parts[1]), parse_value(parts[2]), parse_value(parts[3]))
            orientation = (parse_value(parts[4]), parse_value(parts[5]), parse_value(parts[6]))
            fov = parse_value(parts[7])
            cameras.append((location, orientation, fov))
    return spheres, cylinders, planes, cameras

def create_sphere(location, diameter):
    bpy.ops.mesh.primitive_uv_sphere_add(radius=diameter / 2, location=location)

def create_cylinder(location, orientation, diameter, height):
    bpy.ops.mesh.primitive_cylinder_add(radius=diameter / 2, depth=height, location=location)
    orientation_in_radians = tuple(map(math.radians, orientation))
    direction = mathutils.Vector(orientation_in_radians)
    quaternion = direction.to_track_quat('Z', 'Y')
    bpy.context.object.rotation_mode = 'QUATERNION'
    bpy.context.object.rotation_quaternion = quaternion

def create_plane(location, orientation):
    bpy.ops.mesh.primitive_plane_add(size=10000, location=location)
    orientation_in_radians = tuple(map(math.radians, orientation))
    direction = mathutils.Vector(orientation_in_radians)
    quaternion = direction.to_track_quat('Z', 'Y')
    bpy.context.object.rotation_mode = 'QUATERNION'
    bpy.context.object.rotation_quaternion = quaternion

def create_camera(location, orientation, fov):
    bpy.ops.object.camera_add(location=location)
    orientation_in_radians = tuple(map(math.radians, orientation))
    direction = mathutils.Vector(orientation_in_radians)
    direction = -direction
    quaternion = direction.to_track_quat('Z', 'Y')
    bpy.context.object.rotation_mode = 'QUATERNION'
    bpy.context.object.rotation_quaternion = quaternion
    bpy.context.object.data.angle = math.radians(fov)
    bpy.context.scene.camera = bpy.context.object

spheres, cylinders, planes, cameras = parse_rt_file("/home/madelvin/Documents/miniRT/scenes/compare/metalic/compare_metalic_000.rt")

for sphere in spheres:
    location, diameter = sphere
    create_sphere(location, diameter)

for cylinder in cylinders:
    location, orientation, diameter, height = cylinder
    create_cylinder(location, orientation, diameter, height)

for plane in planes:
    location, orientation = plane
    create_plane(location, orientation)
    
for camera in cameras:
    location, orientation, fov = camera
    create_camera(location, orientation, fov)

# 
# 
# 
# 
# 
# 
# 
# 
# 
# 
# 
# 
# 
# 
# 
# 
# 
# 
# 
# 
# 
# 
# 
# 
# 
# 
# 
# 
# 
# 
# 


import bpy
import os

# Fonction pour analyser et créer un matériau à partir de la ligne
def parse_material(line):
    # Nettoyer la ligne pour éviter les espaces et tabulations excessives
    parts = line.strip().split()

    # Si le nombre de parties est trop faible, ignorer cette ligne
    if len(parts) < 11:
        print(f"Erreur : Ligne mal formatée : {line}")
        return None

    try:
        name = parts[1]  # Le nom du matériau
        
        # Détecter si la couleur est une valeur RGB ou un nom de texture
        color = parts[2]
        
        if ',' in color:  # Si la couleur est sous forme de RGB
            color = tuple(map(int, color.split(',')))  # Couleur (r, g, b)
            color = (*[c / 255.0 for c in color], 1)  # Normaliser la couleur en [0, 1] pour Blender
        else:
            # Si la couleur n'est pas un RGB, c'est une chaîne de texte (nom de texture)
            print(f"Attention : {color} est une chaîne (probablement une texture), un nœud de texture vide sera créé.")
            color = None  # Nous n'allons pas l'utiliser directement, mais un nœud de texture vide sera créé

        # Traitement de metallic et roughness
        metallic = parts[3]
        if ',' in metallic or not metallic.replace('.', '', 1).isdigit():  # Vérifie si c'est une chaîne ou non numérique
            print(f"Attention : {metallic} est une chaîne (probablement une texture), un nœud de texture vide sera créé.")
            metallic = None  # Nous allons créer un nœud de texture vide pour metallic
        else:
            metallic = float(metallic)  # Valeur numérique pour metallic

        roughness = parts[4]
        if ',' in roughness or not roughness.replace('.', '', 1).isdigit():  # Vérifie si c'est une chaîne ou non numérique
            print(f"Attention : {roughness} est une chaîne (probablement une texture), un nœud de texture vide sera créé.")
            roughness = None  # Nous allons créer un nœud de texture vide pour roughness
        else:
            roughness = float(roughness)  # Valeur numérique pour roughness

        # Autres propriétés du matériau
        ior = float(parts[5])  # IOR
        transmission = float(parts[6])  # Transmission
        ao = float(parts[7])  # Ambient Occlusion
        emission_strength = float(parts[8])  # Émission
        emission_color = tuple(map(int, parts[9].split(',')))  # Couleur d'émission (r, g, b)
        scale = float(parts[10])  # Échelle

        # Créer et retourner le matériau dans Blender
        material = bpy.data.materials.new(name=name)
        material.use_nodes = True
        bsdf = material.node_tree.nodes["Principled BSDF"]

        # Appliquer les propriétés de base
        bsdf.inputs["Base Color"].default_value = color if color else (1, 1, 1, 1)  # Si pas de couleur, mettre blanc
        bsdf.inputs["Metallic"].default_value = metallic if metallic is not None else 0.0  # Si pas de texture, utiliser la valeur par défaut
        bsdf.inputs["Roughness"].default_value = roughness if roughness is not None else 0.5  # Si pas de texture, utiliser la valeur par défaut
        bsdf.inputs["IOR"].default_value = ior  # IOR
        bsdf.inputs["Transmission"].default_value = transmission  # Transmission
        bsdf.inputs["Ambient Occlusion"].default_value = ao  # Ambient Occlusion
        bsdf.inputs["Emission Strength"].default_value = emission_strength  # Émission
        bsdf.inputs["Emission"].default_value = (*[ec / 255.0 for ec in emission_color], 1)  # Couleur d'émission

        # Si la couleur n'est pas une image (chaine de texte), créer un nœud de texture vide
        if color is None:
            tex_node = material.node_tree.nodes.new('ShaderNodeTexImage')  # Nœud de texture vide
            tex_node.location = (200, 200)  # Placer le nœud à un endroit visible
            material.node_tree.links.new(tex_node.outputs["Color"], bsdf.inputs["Base Color"])

        # Si metallic est une texture (chaîne de texte), créer un nœud de texture vide
        if metallic is None:
            tex_node = material.node_tree.nodes.new('ShaderNodeTexImage')  # Nœud de texture vide pour metallic
            tex_node.location = (400, 200)  # Placer le nœud à un endroit visible
            material.node_tree.links.new(tex_node.outputs["Color"], bsdf.inputs["Metallic"])

        # Si roughness est une texture (chaîne de texte), créer un nœud de texture vide
        if roughness is None:
            tex_node = material.node_tree.nodes.new('ShaderNodeTexImage')  # Nœud de texture vide pour roughness
            tex_node.location = (600, 200)  # Placer le nœud à un endroit visible
            material.node_tree.links.new(tex_node.outputs["Color"], bsdf.inputs["Roughness"])

        # Retourner le matériau créé
        return material

    except IndexError as e:
        print(f"Erreur de parsing dans la ligne suivante : {line}")
        return None
    except ValueError as e:
        print(f"Erreur de conversion de valeur dans la ligne suivante : {line}. Erreur: {e}")
        return None

# Fonction pour analyser le fichier complet
def parse_rt_file(file_path):
    materials = []
    try:
        with open(file_path, 'r') as file:
            lines = file.readlines()

            for line in lines:
                if line.startswith('mat'):  # Vérifier si la ligne correspond à un matériau
                    material = parse_material(line)
                    if material:
                        materials.append(material)
        return materials
    except Exception as e:
        print(f"Erreur lors de l'ouverture du fichier : {file_path}. Erreur: {e}")
        return []

# Exemple d'appel de la fonction avec le chemin du fichier
file_path = "/home/madelvin/Downloads/test.txt"
materials = parse_rt_file(file_path)

# Affichage des matériaux créés dans Blender
for material in materials:
    print(f"Matériau créé : {material.name}")

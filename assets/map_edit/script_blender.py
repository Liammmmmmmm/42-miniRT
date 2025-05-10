import bpy
import math
import mathutils

output_file_path = bpy.path.abspath("/home/madelvin/Documents/miniRT/scenes/aaa.rt")

def get_material_info(material):
    # Variables par défaut
    color = (204, 204, 204)  # Valeur par défaut pour la couleur
    metallic = 0.0
    roughness = 0.5
    ior = 1.45
    transmission = 0.0
    ambient_occlusion = 1.0
    emission_strength = 0.0
    emission_color = (0, 0, 0)
    scale = 1.0  # Valeur de scale par défaut
    normal_map = None
    normal_intensity = None

    # Gestion des nodes
    if material.use_nodes and material.node_tree:
        for node in material.node_tree.nodes:
            # Si le noeud est un BSDF Principled
            if node.type == 'BSDF_PRINCIPLED' and mat.users > 0:
                # Color
                color_map = None
                if node.inputs['Base Color'].is_linked:
                    color_map = node.inputs['Base Color'].links[0].from_node.image.name
                else:
                    c = node.inputs['Base Color'].default_value
                    color = tuple(int(c[i] * 255) for i in range(3))

                # Metallic
                metallic_map = None
                if node.inputs['Metallic'].is_linked:
                    metallic_map = node.inputs['Metallic'].links[0].from_node.image.name
                else:
                    metallic = node.inputs['Metallic'].default_value

                # Roughness
                roughness_map = None
                if node.inputs['Roughness'].is_linked:
                    roughness_map = node.inputs['Roughness'].links[0].from_node.image.name
                else:
                    roughness = node.inputs['Roughness'].default_value

                # Transmission
                transmission_map = None
                if node.inputs['Transmission'].is_linked:
                    transmission_map = node.inputs['Transmission'].links[0].from_node.image.name
                else:
                    transmission = node.inputs['Transmission'].default_value

                # Ambient Occlusion
                ao_map = None
                if node.inputs.get('Ambient Occlusion', None):
                    if node.inputs['Ambient Occlusion'].is_linked:
                        ao_map = node.inputs['Ambient Occlusion'].links[0].from_node.image.name
                    else:
                        ambient_occlusion = node.inputs['Ambient Occlusion'].default_value

                # Emission
                emission_map = None
                if node.inputs['Emission'].is_linked:
                    emission_map = node.inputs['Emission'].links[0].from_node.image.name
                else:
                    c = node.inputs['Emission'].default_value
                    emission_color = tuple(int(c[i] * 255) for i in range(3))
                
                emission_strength = node.inputs['Emission Strength'].default_value

            # Recherche d'une Normal Map : ici, on cherche un nœud "Normal Map"
            if node.type == 'TEX_IMAGE':
                # Vérifier si cette image est connectée à un nœud "Normal Map"
                for link in node.outputs:
                    if link.is_linked:
                        target_node = link.links[0].to_node
                        if target_node.type == 'NORMAL_MAP':  # Vérifie si c'est un nœud de type "Normal Map"
                            normal_map = node.image.name if node.image else None
                            normal_intensity = target_node.inputs['Strength'].default_value  # Intensité normal map

            # MAPPING - gestion de scale
            if node.type == 'MAPPING' and node.inputs.get('Scale'):
                scale = node.inputs['Scale'].default_value[0]  # Une seule valeur pour le scale

    # Retourne les informations du matériau
    return {
        'name': material.name.replace(' ', '_'),
        'color': color_map if color_map else color,
        'metallic': metallic_map if metallic_map else metallic,
        'roughness': roughness_map if roughness_map else roughness,
        'ior': ior,
        'transmission': transmission_map if transmission_map else transmission,
        'ambient_occlusion': ao_map if ao_map else ambient_occlusion,
        'emission_strength': emission_strength,
        'emission_color': emission_map if emission_map else emission_color,
        'scale': scale,
        'normal_map': normal_map,
        'normal_intensity': normal_intensity,
    }


# Object filters
def is_sphere(obj): 
    return obj.type == 'MESH' and 'Sphere' in obj.name

def is_plane(obj): 
    return obj.type == 'MESH' and 'Plane' in obj.name

def is_cylinder(obj): 
    return obj.type == 'MESH' and 'Cylinder' in obj.name

def is_pointlight(obj): 
    return obj.type == 'LIGHT' and obj.data.type == 'POINT'

def is_camera(obj): 
    return obj.type == 'CAMERA'

def is_custom(obj): 
    return obj.type == 'MESH' and obj.name.lower().startswith("cu_")

def get_custom_info(obj):
    location = tuple(round(coord, 10) for coord in (obj.location.x, obj.location.z, -obj.location.y))

    # Orientation réelle dans le monde (transforme le vecteur avant)
    orientation_vec = obj.matrix_world.to_3x3() @ mathutils.Vector((0.0, 0.0, 1.0))
    orientation_vec = orientation_vec.normalized()
    orientation = tuple(round(coord, 10) for coord in (orientation_vec.x, orientation_vec.z, -orientation_vec.y))
    scale_x = obj.scale.x
    scale_y = obj.scale.y
    scale_z = obj.scale.z
    material = obj.active_material.name if obj.active_material else "None"
    name = obj.name.lower()
    return location, orientation, scale_x, scale_y, scale_z, material, name



def get_sphere_info(obj):
    location = tuple(round(coord, 10) for coord in (obj.location.x, obj.location.z, -obj.location.y))  # Inversé Y et Z
    diameter = max(obj.dimensions)
    material = obj.active_material.name if obj.active_material else "None"
    return location, diameter, material

def get_plane_info(obj):
    location = tuple(round(coord, 10) for coord in (obj.location.x, obj.location.z, -obj.location.y))  # Inversé Y et Z
    normal = obj.matrix_world.to_3x3() @ mathutils.Vector((0.0, 0.0, 1.0))
    normal = tuple(round(n, 10) for n in (normal.x, normal.z, -normal.y))  # Inversé Y et Z dans le vecteur normal
    material = obj.active_material.name if obj.active_material else "None"
    return location, normal, material

def get_cylinder_info(obj):
    location = tuple(round(coord, 10) for coord in (obj.location.x, obj.location.z, -obj.location.y))  # Inversé Y et Z
    orientation = obj.matrix_world.to_3x3() @ mathutils.Vector((0.0, 0.0, 1.0))
    orientation = tuple(round(n, 10) for n in (orientation.x, orientation.z, -orientation.y))  # Inversé Y et Z
    diameter = obj.dimensions.x
    height = obj.dimensions.z
    material = obj.active_material.name if obj.active_material else "None"
    return location, orientation, diameter, height, material

def get_pointlight_info(obj):
    location = tuple(round(coord, 10) for coord in (obj.location.x, obj.location.z, -obj.location.y))  # Inversé Y et Z
    power = min(obj.data.energy / 1000, 1)
    color = tuple(int(c * 255) for c in obj.data.color[:3])
    return location, power, color

def get_camera_info(obj):
    location = tuple(round(coord, 10) for coord in (obj.location.x, obj.location.z, -obj.location.y))  # Inversé Y et Z
    direction = obj.matrix_world.to_3x3() @ mathutils.Vector((0.0, 0.0, -1.0))
    direction = tuple(round(d, 10) for d in (direction.x, direction.z, -direction.y))  # Inversé Y et Z dans la direction
    fov = math.degrees(obj.data.angle)
    return location, fov, direction

def get_world_background_info():
    world = bpy.context.scene.world
    if not world or not world.node_tree:
        return (0, 0, 0), 0.0
    for node in world.node_tree.nodes:
        if node.type == 'BACKGROUND':
            color = tuple(int(c * 255) for c in node.inputs['Color'].default_value[:3])
            strength = node.inputs['Strength'].default_value
            return color, strength
    return (0, 0, 0), 0.0


# Créer un matériau par défaut
def create_default_material():
    # Vérifier si le matériau existe déjà
    if "DefaultMaterial" not in bpy.data.materials:
        # Créer un nouveau matériau par défaut
        default_mat = bpy.data.materials.new(name="DefaultMaterial")
        default_mat.use_nodes = True
        # Optionnellement, tu peux modifier les propriétés du matériau ici
        default_mat.node_tree.nodes["Principled BSDF"].inputs["Base Color"].default_value = (1, 1, 1, 1)  # Blanc
        default_mat.node_tree.nodes["Principled BSDF"].inputs["Roughness"].default_value = 0.5  # Rouillé
    else:
        default_mat = bpy.data.materials["DefaultMaterial"]
    
    return default_mat

# Attribuer le matériau par défaut aux objets sans matériau
def assign_default_material_to_objects():
    default_mat = create_default_material()

    for obj in bpy.context.scene.objects:
        if obj.type == 'MESH' and len(obj.data.materials) == 0:
            # Si l'objet n'a pas de matériaux, lui attribuer le matériau par défaut
            obj.data.materials.append(default_mat)

# Write to file
with open(output_file_path, 'w') as file:
    assign_default_material_to_objects()
    file.write("# MATERIALS\n")
    file.write("#\tname                          color             metallic       roughness      ior            transmission   ambient_occlusion   emission_strength     emission_color   scale      normal_map      normal_intensity\n")
    
    for mat in bpy.data.materials:
        if mat.name != 'Dots Stroke':
            info = get_material_info(mat)
            
            # Formattage de la ligne pour chaque matériau
            line = (
                f"mat\t{info['name']:<30}"  # 35 caractères pour le nom
                f"{', '.join(map(str, info['color']))[:13] if isinstance(info['color'], (tuple, list)) else info['color'][:13]:<18}"  # 18 caractères pour la couleur
                f"{info['metallic']:<15.6f}"  # Le metallic, formatté en float
                f"{info['roughness']:<15.6f}"  # Le roughness, formatté en float
                f"{info['ior']:<15.6f}"  # L'IOR, formatté en float
                f"{info['transmission']:<15.6f}"  # La transmission, formatté en float
                f"{info['ambient_occlusion']:<20.6f}"  # Ambient occlusion, formatté en float
                f"{info['emission_strength']:<22.6f}"  # Emission strength, formatté en float
                f"{', '.join(map(str, info['emission_color'])) if isinstance(info['emission_color'], (tuple, list)) else info['emission_color']:<14}"  # Emission color
            )

            # Ajout du scale si disponible
            if info['scale'] is not None:
                line += f"   {info['scale']:.6f}"
            
            # Si un normal map existe, on ajoute le nom et l'intensité
            if info['normal_map']:
                line += f"   {info['normal_map'][:13]}   {info['normal_intensity']:.6f} "
                
            # Affichage de la ligne formattée dans le fichier
            file.write(line + "\n")
            
    # Objects
    spheres, planes, cylinders, pointlights, cameras, custom = [], [], [], [], [], []

    for obj in bpy.context.scene.objects:
        if obj.hide_get(): continue

        if is_camera(obj):
            loc, fov, dir = get_camera_info(obj)
            cameras.append(f"C\t{loc[0]:>12.6f},{loc[1]:>12.6f},{loc[2]:>12.6f}      "
                           f"{dir[0]:>12.6f},{dir[1]:>12.6f},{dir[2]:>12.6f}             "
                           f"{int(fov):<15}\n")  # Utilisation de int() pour supprimer la virgule
        elif is_pointlight(obj):
            loc, power, color = get_pointlight_info(obj)
            pointlights.append(f"l\t{loc[0]:>12.6f},{loc[1]:>12.6f},{loc[2]:>12.6f}          "
                               f"{power:<15.6f}{color[0]:>3},{color[1]:>3},{color[2]:>3}\n")
        elif is_sphere(obj):
            loc, d, mat = get_sphere_info(obj)
            spheres.append(f"sp\t{loc[0]:>12.6f},{loc[1]:>12.6f},{loc[2]:>12.6f}          "
                           f"{d:<15.6f}{mat:<20}\n")
        elif is_plane(obj):
            loc, norm, mat = get_plane_info(obj)
            planes.append(f"pl\t{loc[0]:>12.6f},{loc[1]:>12.6f},{loc[2]:>12.6f}      "
                          f"{norm[0]:>12.6f},{norm[1]:>12.6f},{norm[2]:>12.6f}             "
                          f"{mat:<20}\n")
        elif is_cylinder(obj):
            loc, ori, dia, h, mat = get_cylinder_info(obj)
            cylinders.append(f"cy\t{loc[0]:>12.6f},{loc[1]:>12.6f},{loc[2]:>12.6f}      "
                             f"{ori[0]:>12.6f},{ori[1]:>12.6f},{ori[2]:>12.6f}             "
                             f"{dia:<15.6f}{h:<15.6f}{mat:<20}\n")
        elif is_custom(obj):
            loc, ori, scale_x, scale_y, scale_z, mat, name = get_custom_info(obj)
            custom.append(f"cu\t{loc[0]:>12.6f},{loc[1]:>12.6f},{loc[2]:>12.6f}      "
                             f"{ori[0]:>12.6f},{ori[1]:>12.6f},{ori[2]:>12.6f}         "
                             f"{scale_x:>12.6f},{scale_y:>12.6f},{scale_z:>12.6f}"
                             f"{name:>17}{mat:>19}\n")

    if spheres:
        file.write("\n# SPHERES\n#\t    origin                                      diameter       material\n")
        file.writelines(spheres)
    if cylinders:
        file.write("\n# CYLINDERS\n#\t    origin                                      orientation                                    diameter       height         material\n")
        file.writelines(cylinders)
    if planes:
        file.write("\n# PLANES\n#\t    origin                                      normal                                         material\n")
        file.writelines(planes)
    if pointlights:
        file.write("\n# LIGHTS\n#\t    origin                                      intensity          color\n")
        file.writelines(pointlights)
    if cameras:
        file.write("\n# CAMERA\n#\t    origin                                      orientation                                    field of view\n")
        file.writelines(cameras)
    if custom:
        file.write("\n# CUSTOM\n#\t    origin                                      orientation                                    scale                                     path                material\n")
        file.writelines(custom)

    ambient_color, ambient_strength = get_world_background_info()
    file.write("\n# AMBIENT\n#\tstrength             color\n")
    file.write(f"A\t{ambient_strength:<20.6f}{ambient_color[0]:>3},{ambient_color[1]:>3},{ambient_color[2]:>3}\n")
    
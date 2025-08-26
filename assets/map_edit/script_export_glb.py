import bpy
import math
import os
import mathutils

# --- CONFIGURATION ---
# Modifiez ces chemins pour qu'ils correspondent à votre configuration
output_file_path = bpy.path.abspath("/home/liam/Documents/dev/42-miniRT/scenes/overview/2chevaux.rt")
output_dir_obj = bpy.path.abspath("/home/liam/Documents/dev/42-miniRT/assets/obj/2chevaux/")
output_texture_dir = bpy.path.abspath("/home/liam/Documents/dev/42-miniRT/assets/textures/2chevaux/")

# --- FIN DE LA CONFIGURATION ---


short_names = {}
full_names = {}

def get_material_info(material):
    # Valeurs par défaut
    color = (204, 204, 204)
    metallic = 0.0
    roughness = 0.5
    ior = 1.45
    transmission = 0.0
    ambient_occlusion = 1.0
    emission_strength = 0.0
    emission_color = (0, 0, 0)
    scale = 1.0
    normal_map = None
    normal_intensity = None
    color_map = None
    metallic_map = None
    emission_strength_map = None
    emission_color_map = None
    transmission_map = None
    ao_map = None
    roughness_map = None

    if material.use_nodes and material.node_tree:
        # Correction: Utiliser la variable 'material' passée en argument, pas 'mat'
        if material.users == 0:
            return None # Ne pas traiter les matériaux non utilisés

        for node in material.node_tree.nodes:
            if node.type == 'BSDF_PRINCIPLED':
                # --- Base Color ---
                if node.inputs['Base Color'].is_linked:
                    color_map_node = node.inputs['Base Color'].links[0].from_node
                    if color_map_node.type == 'TEX_IMAGE' and color_map_node.image:
                        color_map = color_map_node.image.name
                        for short, full in full_names.items():
                            if os.path.splitext(full)[0] == os.path.splitext(color_map)[0]:
                                color_map = short
                                break
                else:
                    c = node.inputs['Base Color'].default_value
                    color = tuple(int(c[i] * 255) for i in range(3))

                # --- Metallic ---
                if node.inputs['Metallic'].is_linked:
                    metallic_map_node = node.inputs['Metallic'].links[0].from_node
                    if metallic_map_node.type == 'TEX_IMAGE' and metallic_map_node.image:
                        metallic_map = metallic_map_node.image.name
                        for short, full in full_names.items():
                            if os.path.splitext(full)[0] == os.path.splitext(metallic_map)[0]:
                                metallic_map = short
                                break
                else:
                    metallic = node.inputs['Metallic'].default_value

                # --- Roughness ---
                if node.inputs['Roughness'].is_linked:
                    roughness_map_node = node.inputs['Roughness'].links[0].from_node
                    if roughness_map_node.type == 'TEX_IMAGE' and roughness_map_node.image:
                        roughness_map = roughness_map_node.image.name
                        for short, full in full_names.items():
                            if os.path.splitext(full)[0] == os.path.splitext(roughness_map)[0]:
                                roughness_map = short
                                break
                else:
                    roughness = node.inputs['Roughness'].default_value

                # --- Transmission ---
#                if node.inputs['Transmission'].is_linked:
#                    transmission_map_node = node.inputs['Transmission'].links[0].from_node
#                    if transmission_map_node.type == 'TEX_IMAGE' and transmission_map_node.image:
#                        transmission_map = transmission_map_node.image.name
#                        for short, full in full_names.items():
#                            if os.path.splitext(full)[0] == os.path.splitext(transmission_map)[0]:
#                                transmission_map = short
#                                break
#                else:
#                transmission = node.inputs['Transmission'].default_value

                # --- Emission ---
                # Dans les versions récentes, 'Emission' est devenu 'Emission Color'
#                emission_socket_name = 'Emission Color' if 'Emission Color' in node.inputs else 'Emission'
#                if node.inputs[emission_socket_name].is_linked:
#                    emission_map_node = node.inputs[emission_socket_name].links[0].from_node
#                    if emission_map_node.type == 'TEX_IMAGE' and emission_map_node.image:
#                        emission_color_map = emission_map_node.image.name
#                        for short, full in full_names.items():
#                            if os.path.splitext(full)[0] == os.path.splitext(emission_color_map)[0]:
#                                emission_color_map = short
#                                break
#                else:
#                c = node.inputs[emission_socket_name].default_value
#                emission_color = tuple(int(c[i] * 255) for i in range(3))

#                if node.inputs['Emission Strength'].is_linked:
#                    emission_strength_map_node = node.inputs['Emission Strength'].links[0].from_node
#                    if emission_strength_map_node.type == 'TEX_IMAGE' and emission_strength_map_node.image:
#                        emission_strength_map = emission_strength_map_node.image.name
#                        for short, full in full_names.items():
#                            if os.path.splitext(full)[0] == os.path.splitext(emission_strength_map)[0]:
#                                emission_strength_map = short
#                                break
#                else:
#                emission_strength = node.inputs['Emission Strength'].default_value

            # --- Normal Map ---
            if node.type == 'NORMAL_MAP':
                if node.inputs['Color'].is_linked:
                    normal_map_node = node.inputs['Color'].links[0].from_node
                    if normal_map_node.type == 'TEX_IMAGE' and normal_map_node.image:
                        normal_map = normal_map_node.image.name
                        normal_intensity = node.inputs['Strength'].default_value
                        for short, full in full_names.items():
                            if os.path.splitext(full)[0] == os.path.splitext(normal_map)[0]:
                                normal_map = short
                                break
            
            # --- Mapping (Scale) ---
            if node.type == 'MAPPING' and node.inputs.get('Scale'):
                scale = node.inputs['Scale'].default_value[0]

    return {
        'name': material.name.replace(' ', '_'),
        'color': color_map if color_map else color,
        'metallic': metallic_map if metallic_map else metallic,
        'roughness': roughness_map if roughness_map else roughness,
        'ior': ior,
        'transmission': transmission_map if transmission_map else transmission,
        'ambient_occlusion': ao_map if ao_map else ambient_occlusion,
        'emission_strength': emission_strength_map if emission_strength_map else emission_strength,
        'emission_color': emission_color_map if emission_color_map else emission_color,
        'scale': scale,
        'normal_map': normal_map,
        'normal_intensity': normal_intensity,
    }


def format_value(val, width=15, float_precision=6):
    """
    Formate la valeur selon son type.
    """
    if val is None:
        return " " * width
    elif isinstance(val, (float, int)):
        return f"{val:<{width}.{float_precision}f}"
    elif isinstance(val, (tuple, list)):
        s = ", ".join(map(str, val))
        return f"{s[:width]:<{width}}"
    elif isinstance(val, str):
        return f"{val[:width]:<{width}}"
    else:
        return f"{str(val)[:width]:<{width}}"

def make_unique_short_name(name, max_len=10):
    image_base = name
    base = os.path.splitext(name)[0]
    base = ''.join(c for c in base if c.isalnum() or c == '_')[:max_len-3]
    candidate = base
    i = 1
    while candidate in short_names:
        candidate = f"{base[:max_len - len(str(i))]}{i}"
        i += 1
    short_names[candidate] = name
    full_names[candidate] = image_base
    return candidate

def is_sphere(obj): 
    return obj.type == 'MESH' and obj.name.lower().startswith("sp_")

def is_plane(obj): 
    return obj.type == 'MESH' and obj.name.lower().startswith("pl_")

def is_cylinder(obj): 
    return obj.type == 'MESH' and obj.name.lower().startswith("cy_")

def is_pointlight(obj): 
    return obj.type == 'LIGHT' and obj.data.type == 'POINT'

def is_camera(obj): 
    return obj.type == 'CAMERA'

def is_custom(obj): 
    return obj.type == 'MESH' and obj.name.lower().startswith("cu_")

def get_custom_info(obj):
    location = tuple(round(coord, 10) for coord in (obj.location.x, obj.location.z, -obj.location.y))
    orientation_vec = obj.matrix_world.to_3x3() @ mathutils.Vector((0.0, -1.0, 0.0))
    orientation_vec = orientation_vec.normalized()
    orientation = tuple(round(coord, 10) for coord in (orientation_vec.x, orientation_vec.z, -orientation_vec.y))
    scale_x = obj.scale.x
    scale_y = obj.scale.y
    scale_z = obj.scale.z
    material = obj.active_material.name.replace(' ', '_') if obj.active_material else "None"
    
    bpy.ops.object.select_all(action='DESELECT')
    obj.select_set(True)
    bpy.context.view_layer.objects.active = obj
    
    export_path = os.path.join(output_dir_obj, f"{obj.name}.obj")
    
    if not os.path.exists(output_dir_obj):
        os.makedirs(output_dir_obj)

    # CORRECTION: Utilisation de la nouvelle fonction d'export OBJ
    if not os.path.exists(export_path):
        bpy.ops.wm.obj_export(
            filepath=export_path,
            check_existing=False,
            export_selected_objects=True,  # Ce paramètre est correct pour cet opérateur
            forward_axis='NEGATIVE_Z',
            up_axis='Y'
        )
    name = export_path
    return location, orientation, scale_x, scale_y, scale_z, material, name

def get_sphere_info(obj):
    location = tuple(round(coord, 10) for coord in (obj.location.x, obj.location.z, -obj.location.y))
    diameter = max(obj.dimensions)
    material = obj.active_material.name.replace(' ', '_') if obj.active_material else "None"
    return location, diameter, material

def get_plane_info(obj):
    location = tuple(round(coord, 10) for coord in (obj.location.x, obj.location.z, -obj.location.y))
    normal = obj.matrix_world.to_3x3() @ mathutils.Vector((0.0, 0.0, 1.0))
    normal = tuple(round(n, 10) for n in (normal.x, normal.z, -normal.y))
    material = obj.active_material.name.replace(' ', '_') if obj.active_material else "None"
    return location, normal, material

def get_cylinder_info(obj):
    location = tuple(round(coord, 10) for coord in (obj.location.x, obj.location.z, -obj.location.y))
    orientation = obj.matrix_world.to_3x3() @ mathutils.Vector((0.0, 0.0, 1.0))
    orientation = tuple(round(n, 10) for n in (orientation.x, orientation.z, -orientation.y))
    diameter = obj.dimensions.x
    height = obj.dimensions.z
    material = obj.active_material.name.replace(' ', '_') if obj.active_material else "None"
    return location, orientation, diameter, height, material

def get_pointlight_info(obj):
    location = tuple(round(coord, 10) for coord in (obj.location.x, obj.location.z, -obj.location.y))
    power = min(obj.data.energy / 1000, 1)
    color = tuple(int(c * 255) for c in obj.data.color[:3])
    return location, power, color

def get_camera_info(obj):
    location = tuple(round(coord, 10) for coord in (obj.location.x, obj.location.z, -obj.location.y))
    direction = obj.matrix_world.to_3x3() @ mathutils.Vector((0.0, 0.0, -1.0))
    direction = tuple(round(d, 10) for d in (direction.x, direction.z, -direction.y))
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

def create_default_material():
    if "DefaultMaterial" not in bpy.data.materials:
        default_mat = bpy.data.materials.new(name="DefaultMaterial")
        default_mat.use_nodes = True
        bsdf = default_mat.node_tree.nodes.get("Principled BSDF")
        if bsdf:
            bsdf.inputs["Base Color"].default_value = (0.8, 0.8, 0.8, 1)
            bsdf.inputs["Roughness"].default_value = 0.5
    else:
        default_mat = bpy.data.materials["DefaultMaterial"]
    return default_mat

def assign_default_material_to_objects():
    default_mat = create_default_material()
    for obj in bpy.context.scene.objects:
        if obj.type == 'MESH' and len(obj.data.materials) == 0:
            obj.data.materials.append(default_mat)

with open(output_file_path, 'w') as file:
    assign_default_material_to_objects()
    bpy.ops.object.select_all(action='DESELECT')

    # Renaming logic
    for obj in bpy.data.objects:
        name = obj.name.lower()
        if not (name.startswith("cu_") or name.startswith("cy_") or name.startswith("sp_") or name.startswith("pl_")):
             if obj.type == 'MESH':
                obj.name = "cu_" + obj.name

    file.write("# TEXTURES\n")
    file.write("#\tn   name  type   path\n")

    if not os.path.exists(output_texture_dir):
        os.makedirs(output_texture_dir)

    for image in bpy.data.images:
        # On ignore les images générées ou vides
        if not image.has_data or image.size[0] == 0:
            continue

        short_name = make_unique_short_name(image.name)
        ext = ".png"  # on choisit PNG pour l’universalité, sinon image.file_format peut être utilisé
        dst_filename = f"{short_name}{ext}"
        dst_path = os.path.join(output_texture_dir, dst_filename)

        # Sauvegarde l’image si le fichier n’existe pas déjà
        if not os.path.exists(dst_path):
            try:
                image.filepath_raw = dst_path
                image.file_format = 'PNG'
                image.save()
            except Exception as e:
                print(f"[Erreur] Impossible de sauvegarder {image.name} vers {dst_path} : {e}")
                continue

        # Écrire le chemin relatif dans le fichier .rt
        relative_texture_path = os.path.relpath(dst_path, os.path.dirname(output_file_path))
        line = f"tex {short_name} image {relative_texture_path}"
        file.write(line + "\n")


    file.write("\n")
    file.write("# MATERIALS\n")
    file.write("#\tname                          color             metallic       roughness      ior            transmission   ambient_occlusion   emission_strength     emission_color   scale      normal_map      normal_intensity\n")
    for mat in bpy.data.materials:
        if mat.name != 'Dots Stroke':
            info = get_material_info(mat)
            if info is None:
                continue

            line = (
                f"mat\t{format_value(info.get('name', ''), width=30, float_precision=0)}"
                f"{format_value(info.get('color'), width=18)}"
                f"{format_value(info.get('metallic'), width=15)}"
                f"{format_value(info.get('roughness'), width=15)}"
                f"{format_value(info.get('ior'), width=15)}"
                f"{format_value(info.get('transmission'), width=15)}"
                f"{format_value(info.get('ambient_occlusion'), width=20)}"
                f"{format_value(info.get('emission_strength'), width=22)}"
                f"{format_value(info.get('emission_color'), width=14)}"
            )

            if info.get('scale') is not None:
                line += f"   {format_value(info['scale'], width=10)}"

            if info.get('normal_map'):
                normal_map_str = str(info['normal_map'])[:13]
                normal_intensity_str = format_value(info.get('normal_intensity'), width=10)
                line += f"   {normal_map_str:<13}   {normal_intensity_str}"

            file.write(line + "\n")

    spheres, planes, cylinders, pointlights, cameras, custom = [], [], [], [], [], []

    for obj in bpy.context.scene.objects:
        if obj.hide_get(): continue

        if is_camera(obj):
            loc, fov, dir = get_camera_info(obj)
            cameras.append(f"C\t{loc[0]:>12.6f},{loc[1]:>12.6f},{loc[2]:>12.6f}      "
                           f"{dir[0]:>12.6f},{dir[1]:>12.6f},{dir[2]:>12.6f}             "
                           f"{int(fov):<15}\n")
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
            custom.append(
                f"cu\t{loc[0]:>12.6f},{loc[1]:>12.6f},{loc[2]:>12.6f}      "
                f"{ori[0]:>12.6f},{ori[1]:>12.6f},{ori[2]:>12.6f}         "
                f"{scale_x:>12.6f},{scale_y:>12.6f},{scale_z:>12.6f}   "
                f"{name:<40}"
                f"{mat:>19}\n"
            )

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
    if custom:
        file.write("\n# CUSTOM\n#\t    origin                                      orientation                                    scale                                     path                material\n")
        file.writelines(custom)
    if cameras:
        file.write("\n# CAMERA\n#\t    origin                                      orientation                                    field of view\n")
        file.writelines(cameras)

    ambient_color, ambient_strength = get_world_background_info()
    file.write("\n# AMBIENT\n#\tstrength             color\n")
    file.write(f"A\t{ambient_strength:<20.6f}{ambient_color[0]:>3},{ambient_color[1]:>3},{ambient_color[2]:>3}\n")

print(f"Scène exportée avec succès dans {output_file_path}")
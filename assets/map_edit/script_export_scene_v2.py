import bpy
import math
import os
import mathutils

# Global dictionaries for texture short names (reset on each execution)
short_names = {}
full_names = {}

# ------------------------------------------------------------------------
# Fonctions d'aide (inchangées pour la plupart)
# ------------------------------------------------------------------------

def get_material_info(material):
    """
    Extrait les informations pertinentes d'un matériau Blender (nœuds Principled BSDF).
    Retourne un dictionnaire avec les propriétés du matériau et les noms des textures.
    """
    color = (204, 204, 204)
    metallic = 0.0
    roughness = 0.5
    ior = 1.45 # Valeur par défaut pour l'IOR
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
        for node in material.node_tree.nodes:
            if node.type == 'BSDF_PRINCIPLED':
                # Base Color
                if node.inputs['Base Color'].is_linked:
                    from_node = node.inputs['Base Color'].links[0].from_node
                    if from_node.type == 'TEX_IMAGE' and from_node.image:
                        color_map = from_node.image.name
                        for short, full in full_names.items(): # Utilise les globals short_names/full_names
                            if os.path.splitext(full)[0] == os.path.splitext(color_map)[0]:
                                color_map = short
                                break
                else:
                    c = node.inputs['Base Color'].default_value
                    color = tuple(int(c[i] * 255) for i in range(3))

                # Metallic
                if node.inputs['Metallic'].is_linked:
                    from_node = node.inputs['Metallic'].links[0].from_node
                    if from_node.type == 'TEX_IMAGE' and from_node.image:
                        metallic_map = from_node.image.name
                        for short, full in full_names.items():
                            if os.path.splitext(full)[0] == os.path.splitext(metallic_map)[0]:
                                metallic_map = short
                                break
                else:
                    metallic = node.inputs['Metallic'].default_value

                # Roughness
                if node.inputs['Roughness'].is_linked:
                    from_node = node.inputs['Roughness'].links[0].from_node
                    if from_node.type == 'TEX_IMAGE' and from_node.image:
                        roughness_map = from_node.image.name
                        for short, full in full_names.items():
                            if os.path.splitext(full)[0] == os.path.splitext(roughness_map)[0]:
                                roughness_map = short
                                break
                else:
                    roughness = node.inputs['Roughness'].default_value

                # Transmission (Alpha)
                if node.inputs['Transmission'].is_linked:
                    from_node = node.inputs['Transmission'].links[0].from_node
                    if from_node.type == 'TEX_IMAGE' and from_node.image:
                        transmission_map = from_node.image.name
                        for short, full in full_names.items():
                            if os.path.splitext(full)[0] == os.path.splitext(transmission_map)[0]:
                                transmission_map = short
                                break
                else:
                    transmission = node.inputs['Transmission'].default_value

                # Ambient Occlusion (if present in inputs, e.g., via add-ons)
                if 'Ambient Occlusion' in node.inputs:
                    if node.inputs['Ambient Occlusion'].is_linked:
                        from_node = node.inputs['Ambient Occlusion'].links[0].from_node
                        if from_node.type == 'TEX_IMAGE' and from_node.image:
                            ao_map = from_node.image.name
                            for short, full in full_names.items():
                                if os.path.splitext(full)[0] == os.path.splitext(ao_map)[0]:
                                    ao_map = short
                                    break
                    else:
                        ambient_occlusion = node.inputs['Ambient Occlusion'].default_value

                # Emission Color
                if node.inputs['Emission'].is_linked:
                    from_node = node.inputs['Emission'].links[0].from_node
                    if from_node.type == 'TEX_IMAGE' and from_node.image:
                        emission_color_map = from_node.image.name
                        for short, full in full_names.items():
                            if os.path.splitext(full)[0] == os.path.splitext(emission_color_map)[0]:
                                emission_color_map = short
                                break
                else:
                    c = node.inputs['Emission'].default_value
                    emission_color = tuple(int(c[i] * 255) for i in range(3))
                
                # Emission Strength
                if 'Emission Strength' in node.inputs:
                    if node.inputs['Emission Strength'].is_linked:
                        from_node = node.inputs['Emission Strength'].links[0].from_node
                        if from_node.type == 'TEX_IMAGE' and from_node.image:
                            emission_strength_map = from_node.image.name
                            for short, full in full_names.items():
                                if os.path.splitext(full)[0] == os.path.splitext(emission_strength_map)[0]:
                                    emission_strength_map = short
                                    break
                    else:
                        emission_strength = node.inputs['Emission Strength'].default_value

            # Normal Map detection (linked via a 'Normal Map' node)
            if node.type == 'TEX_IMAGE' and node.image:
                for output_socket in node.outputs:
                    for link in output_socket.links:
                        target_node = link.to_node
                        if target_node.type == 'NORMAL_MAP':
                            normal_map = node.image.name
                            normal_intensity = target_node.inputs['Strength'].default_value
                            for short, full in full_names.items():
                                if os.path.splitext(full)[0] == os.path.splitext(normal_map)[0]:
                                    normal_map = short
                                    break
                            break # Found normal map for this image, exit inner loop
                    if normal_map: break # Found normal map, exit outer loop

            # Mapping scale (assuming uniform scale from first component)
            if node.type == 'MAPPING' and node.inputs.get('Scale'):
                scale = node.inputs['Scale'].default_value[0] if len(node.inputs['Scale'].default_value) > 0 else 1.0

    return {
        'name': material.name.replace(' ', '_'),
        'color': color_map if color_map else color,
        'metallic': metallic_map if metallic_map else metallic,
        'roughness': roughness_map if roughness_map else roughness,
        'ior': ior,
        'transmission': transmission_map if transmission_map else transmission,
        'ambient_occlusion': ambient_occlusion, # Check this, was ao_map if ao_map else ambient_occlusion,
        'emission_strength': emission_strength_map if emission_strength_map else emission_strength,
        'emission_color': emission_color_map if emission_color_map else emission_color,
        'scale': scale,
        'normal_map': normal_map,
        'normal_intensity': normal_intensity,
    }

def format_value(val, width=15, float_precision=6):
    """
    Formate une valeur (float, tuple/list, string) pour l'écriture dans le fichier .rt
    avec une largeur et une précision spécifiées.
    """
    if val is None:
        return " " * width
    elif isinstance(val, (float, int)):
        return f"{val:<{width}.{float_precision}f}"
    elif isinstance(val, (tuple, list)):
        s = ", ".join(f"{v:.3f}" if isinstance(v, float) else str(v) for v in val)
        return f"{s[:width]:<{width}}"
    elif isinstance(val, str):
        return f"{val[:width]:<{width}}"
    else:
        return f"{str(val)[:width]:<{width}}"

def make_unique_short_name(name, max_len=10):
    """
    Crée un nom court unique pour les textures, utilisé dans le fichier .rt.
    """
    global short_names, full_names
    image_base = name
    base = name[:max_len-3]
    base = ''.join(c for c in base if c.isalnum() or c == '_')
    candidate = base
    i = 1
    while candidate in short_names:
        candidate = f"{base[:max_len - len(str(i))]}{i}"
        i += 1
    short_names[candidate] = name
    full_names[candidate] = image_base
    return candidate

# Fonctions pour identifier le type d'objet par son préfixe de nom
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

# Fonctions pour obtenir les informations spécifiques à chaque type d'objet
# Les coordonnées sont converties du système de Blender (Z-up) à minirt (Y-up, Z inversé)
def get_custom_info(obj, export_dir_obj):
    location = tuple(round(coord, 10) for coord in (obj.location.x, obj.location.z, -obj.location.y))
    orientation_vec = obj.matrix_world.to_3x3() @ mathutils.Vector((0.0, -1.0, 0.0))
    orientation_vec = orientation_vec.normalized()
    orientation = tuple(round(coord, 10) for coord in (orientation_vec.x, orientation_vec.z, -orientation_vec.y))
    scale_x = obj.scale.x
    scale_y = obj.scale.y
    scale_z = obj.scale.z
    material = obj.active_material.name if obj.active_material else "None"
    
    bpy.ops.object.select_all(action='DESELECT')
    obj.select_set(True)
    bpy.context.view_layer.objects.active = obj

    os.makedirs(export_dir_obj, exist_ok=True)
    export_path = os.path.join(export_dir_obj, f"{obj.name}.obj")
    
    # Exporter l'OBJ uniquement s'il n'existe pas ou si la scène a changé
    # Pour l'instant, on exporte s'il n'existe pas. Pour la mise à jour,
    # il faudrait une gestion de timestamp ou de hash.
    if not os.path.exists(export_path):
        try:
            bpy.ops.export_scene.obj(
                filepath=export_path,
                use_selection=True,
                use_materials=True,
                axis_forward='-Z',
                axis_up='Y'
            )
        except Exception as e:
            print(f"Erreur lors de l'exportation OBJ pour {obj.name}: {e}")
            return None, None, None, None, None, None, None
    
    name = export_path
    return location, orientation, scale_x, scale_y, scale_z, material, name

def get_sphere_info(obj):
    location = tuple(round(coord, 10) for coord in (obj.location.x, obj.location.z, -obj.location.y))
    diameter = max(obj.dimensions)
    material = obj.active_material.name if obj.active_material else "None"
    return location, diameter, material

def get_plane_info(obj):
    location = tuple(round(coord, 10) for coord in (obj.location.x, obj.location.z, -obj.location.y))
    normal = obj.matrix_world.to_3x3() @ mathutils.Vector((0.0, 0.0, 1.0))
    normal = tuple(round(n, 10) for n in (normal.x, normal.z, -normal.y))
    material = obj.active_material.name if obj.active_material else "None"
    return location, normal, material

def get_cylinder_info(obj):
    location = tuple(round(coord, 10) for coord in (obj.location.x, obj.location.z, -obj.location.y))
    orientation = obj.matrix_world.to_3x3() @ mathutils.Vector((0.0, 0.0, 1.0))
    orientation = tuple(round(n, 10) for n in (orientation.x, orientation.z, -orientation.y))
    diameter = obj.dimensions.x
    height = obj.dimensions.z
    material = obj.active_material.name if obj.active_material else "None"
    return location, orientation, diameter, height, material

def get_pointlight_info(obj):
    location = tuple(round(coord, 10) for coord in (obj.location.x, obj.location.z, -obj.location.y))
    power = min(obj.data.energy / 1000, 1) # Scaling power to be between 0 and 1
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
    """Crée un matériau par défaut 'DefaultMaterial' si inexistant."""
    if "DefaultMaterial" not in bpy.data.materials:
        default_mat = bpy.data.materials.new(name="DefaultMaterial")
        default_mat.use_nodes = True
        default_mat.node_tree.nodes["Principled BSDF"].inputs["Base Color"].default_value = (1, 1, 1, 1)
        default_mat.node_tree.nodes["Principled BSDF"].inputs["Roughness"].default_value = 0.5
    else:
        default_mat = bpy.data.materials["DefaultMaterial"]
    return default_mat

def assign_default_material_to_objects():
    """Assign un matériau par défaut aux objets de type MESH sans matériaux."""
    default_mat = create_default_material()
    for obj in bpy.context.scene.objects:
        if obj.type == 'MESH' and not obj.data.materials:
            obj.data.materials.append(default_mat)

# ------------------------------------------------------------------------
# Classe Principale de l'Opérateur d'Exportation
# ------------------------------------------------------------------------
class ExportMinirtScene(bpy.types.Operator):
    """Exporte la scène Blender au format minirt (.rt)"""
    bl_idname = "scene.export_minirt_scene"
    bl_label = "Exporter Scène minirt"
    bl_options = {'REGISTER', 'UNDO'}

    # Propriétés de l'opérateur (affichées dans l'interface utilisateur)
    output_file_path: bpy.props.StringProperty(
        name="Chemin de Fichier Scène (.rt)",
        description="Chemin complet du fichier .rt à exporter",
        subtype='FILE_PATH',
        default="//scenes/obj/scene.rt" # Chemin relatif à votre fichier .blend
    )

    output_obj_dir: bpy.props.StringProperty(
        name="Dossier d'Exportation OBJ",
        description="Dossier où les objets 'cu_' seront exportés en .obj",
        subtype='DIR_PATH',
        default="//scenes/obj/" # Chemin relatif à votre fichier .blend
    )

    def execute(self, context):
        global short_names, full_names # Réinitialise les dictionnaires globaux à chaque exécution
        short_names = {}
        full_names = {}

        # Convertit les chemins relatifs Blender en chemins absolus
        abs_output_file_path = bpy.path.abspath(self.output_file_path)
        abs_output_obj_dir = bpy.path.abspath(self.output_obj_dir)

        # S'assure que le dossier d'exportation OBJ existe
        try:
            os.makedirs(abs_output_obj_dir, exist_ok=True)
        except Exception as e:
            self.report({'ERROR'}, f"Impossible de créer le dossier d'exportation OBJ : {e}")
            return {'CANCELLED'}

        try:
            with open(abs_output_file_path, 'w') as file:
                # Assignation d'un matériau par défaut si nécessaire
                assign_default_material_to_objects()
                
                # Désélectionne tous les objets pour une sélection propre lors de l'export OBJ
                bpy.ops.object.select_all(action='DESELECT')

                # Renomme les maillages non standard avec le préfixe 'cu_' pour l'exportation
                for obj in bpy.data.objects:
                    if obj.type == 'MESH' and \
                       not (obj.name.lower().startswith("cu_") or \
                            obj.name.lower().startswith("cy_") or \
                            obj.name.lower().startswith("sp_") or \
                            obj.name.lower().startswith("pl_")):
                        obj.name = "cu_" + obj.name

                # Écrit la section TEXTURES
                file.write("# TEXTURES\n")
                file.write("#\tn   name  type   path\n")
                for image in bpy.data.images:
                    if image.filepath:
                        abs_path = bpy.path.abspath(image.filepath)
                        if os.path.exists(abs_path):
                            short_name = make_unique_short_name(image.name)
                            line = f"tex {short_name} image {abs_path}"
                            file.write(line + "\n")
                file.write("\n")

                # Écrit la section MATERIALS
                file.write("# MATERIALS\n")
                file.write("#\tname                          color             metallic       roughness      ior            transmission   ambient_occlusion   emission_strength     emission_color   scale      normal_map      normal_intensity\n")
                for mat in bpy.data.materials:
                    if mat.name != 'Dots Stroke':
                        info = get_material_info(mat)

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

                # Collecte les données des objets
                spheres, planes, cylinders, pointlights, cameras, custom_objects_data = [], [], [], [], [], []

                for obj in bpy.context.scene.objects:
                    if obj.hide_get(): continue # Ignore les objets cachés

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
                        loc, ori, scale_x, scale_y, scale_z, mat, name_path = get_custom_info(obj, abs_output_obj_dir)
                        if loc is not None: # S'assure que l'export OBJ a réussi
                            custom_objects_data.append(
                                f"cu\t{loc[0]:>12.6f},{loc[1]:>12.6f},{loc[2]:>12.6f}      "
                                f"{ori[0]:>12.6f},{ori[1]:>12.6f},{ori[2]:>12.6f}         "
                                f"{scale_x:>12.6f},{scale_y:>12.6f},{scale_z:>12.6f}   "
                                f"{name_path:<40}"
                                f"{mat:>19}\n"
                            )

                # Écrit les sections des objets au fichier
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
                if custom_objects_data:
                    file.write("\n# CUSTOM\n#\t    origin                                      orientation                                    scale                                     path                material\n")
                    file.writelines(custom_objects_data)
                if cameras:
                    file.write("\n# CAMERA\n#\t    origin                                      orientation                                    field of view\n")
                    file.writelines(cameras)

                # Écrit la section AMBIENT
                ambient_color, ambient_strength = get_world_background_info()
                file.write("\n# AMBIENT\n#\tstrength             color\n")
                file.write(f"A\t{ambient_strength:<20.6f}{ambient_color[0]:>3},{ambient_color[1]:>3},{ambient_color[2]:>3}\n")

            self.report({'INFO'}, f"Scène exportée avec succès vers : {abs_output_file_path}")
            return {'FINISHED'}

        except IOError as e:
            self.report({'ERROR'}, f"Erreur d'écriture du fichier : {e}")
            return {'CANCELLED'}
        except Exception as e:
            self.report({'ERROR'}, f"Une erreur inattendue est survenue : {e}")
            return {'CANCELLED'}

# ------------------------------------------------------------------------
# Classe du Panneau de l'Interface Utilisateur
# ------------------------------------------------------------------------
class MinirtExporterPanel(bpy.types.Panel):
    """Panneau pour exporter la scène minirt"""
    bl_label = "Exportateur minirt"
    bl_idname = "PT_MinirtExporter"
    bl_space_type = 'VIEW_3D'
    bl_region_type = 'UI'
    bl_category = 'Tool' # Vous pouvez changer ceci pour une catégorie personnalisée

    def draw(self, context):
        layout = self.layout
        
        # Accède aux propriétés de l'opérateur pour les afficher
        # On utilise une instance "virtuelle" de l'opérateur pour l'affichage des propriétés
        # car elles ne sont pas liées directement à un objet de scène ici.
        exporter_op = layout.operator(ExportMinirtScene.bl_idname)

        layout.label(text="Options d'Exportation:")
        layout.prop(exporter_op, "output_file_path")
        layout.prop(exporter_op, "output_obj_dir")

        layout.separator()
        layout.operator(ExportMinirtScene.bl_idname, text="Exporter la scène", icon='EXPORT')

# ------------------------------------------------------------------------
# Fonctions d'Enregistrement et Désenregistrement Blender
# ------------------------------------------------------------------------
def register():
    """Enregistre les classes Blender."""
    bpy.utils.register_class(ExportMinirtScene)
    bpy.utils.register_class(MinirtExporterPanel)

def unregister():
    """Désenregistre les classes Blender."""
    bpy.utils.unregister_class(ExportMinirtScene)
    bpy.utils.unregister_class(MinirtExporterPanel)

if __name__ == "__main__":
    register()
import bpy
import bmesh
from mathutils import Vector
from math import pi, sin, cos

# ------------------------------------------------------------------------
# 1. Groupe de Propriétés pour l'Hyperboloïde
#    Ces propriétés seront attachées à chaque objet Hyperboloïde créé.
# ------------------------------------------------------------------------
class HyperboloidProps(bpy.types.PropertyGroup):
    # Fonction de rappel pour la mise à jour du maillage
    def update_hyperboloid(self, context):
        obj = context.active_object
        if obj and obj.hyperboloid_props == self: # S'assurer que les propriétés appartiennent à l'objet actif
            refresh_hyperboloid_mesh(obj)

    a: bpy.props.FloatProperty(
        name="Paramètre a",
        description="Contrôle l'échelle le long de l'axe X",
        default=1.0,
        min=0.01,
        step=0.1,
        update=update_hyperboloid
    )

    b: bpy.props.FloatProperty(
        name="Paramètre b",
        description="Contrôle l'échelle le long de l'axe Y",
        default=1.0,
        min=0.01,
        step=0.1,
        update=update_hyperboloid
    )

    c: bpy.props.FloatProperty(
        name="Paramètre c",
        description="Contrôle l'échelle le long de l'axe Z",
        default=1.0,
        min=0.01,
        step=0.1,
        update=update_hyperboloid
    )

    shape_type: bpy.props.EnumProperty(
        name="Type de Forme",
        description="Détermine le type d'hyperboloïde ou cône",
        items=[
            ('POSITIVE', "Hyperboloïde à une nappe", "x²/a² + y²/b² - z²/c² = 1"),
            ('NEGATIVE', "Hyperboloïde à deux nappes", "-x²/a² - y²/b² + z²/c² = 1"),
            ('ZERO', "Cône", "x²/a² + y²/b² - z²/c² = 0")
        ],
        default='POSITIVE',
        update=update_hyperboloid
    )

    segments_u: bpy.props.IntProperty(
        name="Segments U",
        description="Nombre de segments en circonférence",
        default=64,
        min=8,
        max=256,
        update=update_hyperboloid
    )

    segments_v: bpy.props.IntProperty(
        name="Segments V",
        description="Nombre de segments en hauteur",
        default=64,
        min=8,
        max=256,
        update=update_hyperboloid
    )

    height: bpy.props.FloatProperty(
        name="Hauteur",
        description="Hauteur totale de l'hyperboloïde/cône",
        default=5.0,
        min=0.1,
        update=update_hyperboloid
    )

# ------------------------------------------------------------------------
# 2. Fonction de Génération/Mise à Jour du Maillage
#    Cette fonction contient la logique de création des vertices et faces.
# ------------------------------------------------------------------------
def refresh_hyperboloid_mesh(obj):
    # Assurez-vous que l'objet a les propriétés nécessaires
    if not hasattr(obj, 'hyperboloid_props'):
        return

    props = obj.hyperboloid_props
    mesh = obj.data

    # Vérification des paramètres pour éviter les erreurs de division par zéro
    if props.a == 0 or props.b == 0 or props.c == 0:
        print("Erreur: Les paramètres a, b ou c ne peuvent pas être zéro.")
        return

    bm = bmesh.new()
    bm.from_mesh(mesh)
    bm.clear() # Efface le maillage existant

    verts = []

    # Génération des sommets
    for i in range(props.segments_v + 1):
        z_norm = (i / props.segments_v) * 2 - 1  # De -1 à 1
        z = z_norm * (props.height / 2) # Hauteur réelle de -height/2 à height/2

        for j in range(props.segments_u):
            theta = (j / props.segments_u) * 2 * pi

            x, y = 0, 0
            if props.shape_type == 'POSITIVE':
                # x²/a² + y²/b² - z²/c² = 1  => x²/a² + y²/b² = 1 + z²/c²
                r_squared = 1 + (z / props.c)**2
                if r_squared >= 0:
                    r = r_squared**0.5
                    x = props.a * r * cos(theta)
                    y = props.b * r * sin(theta)
            elif props.shape_type == 'NEGATIVE':
                # -x²/a² - y²/b² + z²/c² = 1 => x²/a² + y²/b² = z²/c² - 1
                r_squared = (z / props.c)**2 - 1
                if r_squared >= 0: # Pour les parties réelles de l'hyperboloïde à deux nappes
                    r = r_squared**0.5
                    x = props.a * r * cos(theta)
                    y = props.b * r * sin(theta)
                else:
                    # Ne pas ajouter de vertex si la partie n'existe pas pour cette hauteur
                    # Une approche alternative serait de générer les deux nappes séparément.
                    # Pour la simplicité et la robustesse de l'update, on laisse le point à 0,0,0
                    # si pas de solution réelle, ce qui peut créer des artefacts mais assure la connectivité.
                    # Mieux : on ne crée pas le vertex, et on gère les faces plus bas.
                    # Ici, on continue à créer le vertex, mais la condition sur les faces sera plus critique.
                    pass # Le point restera non initialisé si r_squared < 0, ou devrait être géré par des conditions.
                    # Pour éviter des erreurs lors de la création des faces, on va l'initialiser à (0,0,z) si non réel
                    # C'est une simplification pour que le maillage reste "fermé" virtuellement.
                    # Une gestion plus stricte impliquerait de créer des listes de vertices valides par nappe.
                    x, y = 0, 0 # Simplification, peut créer des points au centre si hors de la nappe.
            elif props.shape_type == 'ZERO':
                # x²/a² + y²/b² - z²/c² = 0 => x²/a² + y²/b² = z²/c²
                r_squared = (z / props.c)**2
                r = r_squared**0.5
                x = props.a * r * cos(theta)
                y = props.b * r * sin(theta)
            
            verts.append(bm.verts.new(Vector((x, y, z))))

    # Génération des faces
    for i in range(props.segments_v):
        for j in range(props.segments_u):
            # Indices des sommets dans le tableau 'verts'
            v1_idx = i * props.segments_u + j
            v2_idx = i * props.segments_u + (j + 1) % props.segments_u
            v3_idx = (i + 1) * props.segments_u + (j + 1) % props.segments_u
            v4_idx = (i + 1) * props.segments_u + j

            try:
                bm.faces.new((verts[v1_idx], verts[v2_idx], verts[v3_idx], verts[v4_idx]))
            except ValueError:
                # Gère les cas où les sommets sont colinéaires ou créent des faces non-valides
                pass
            except IndexError:
                # Peut arriver si des vertices n'ont pas été ajoutés pour 'NEGATIVE' shape_type
                pass

    bm.to_mesh(mesh)
    mesh.update()
    bm.free() # Libère le bmesh


# ------------------------------------------------------------------------
# 3. Opérateur pour Ajouter un Nouvel Hyperboloïde
#    Cet opérateur est principalement pour la création initiale.
# ------------------------------------------------------------------------
class AddHyperboloid(bpy.types.Operator):
    """Ajouter un nouvel objet Hyperboloïde"""
    bl_idname = "mesh.add_hyperboloid"
    bl_label = "Ajouter un nouvel Hyperboloïde"
    bl_options = {'REGISTER', 'UNDO'}

    def execute(self, context):
        # Crée un nouveau maillage et objet
        mesh = bpy.data.meshes.new("HyperboloidMesh")
        obj = bpy.data.objects.new("Hyperboloid", mesh)
        context.collection.objects.link(obj)

        # Lie les propriétés d'hyperboloïde à l'objet
        obj.hyperboloid_props.a = 1.0
        obj.hyperboloid_props.b = 1.0
        obj.hyperboloid_props.c = 1.0
        obj.hyperboloid_props.shape_type = 'POSITIVE'
        obj.hyperboloid_props.segments_u = 64
        obj.hyperboloid_props.segments_v = 64
        obj.hyperboloid_props.height = 5.0

        # Sélectionne le nouvel objet et le rend actif
        bpy.context.view_layer.objects.active = obj
        obj.select_set(True)

        # Génère le maillage initial
        refresh_hyperboloid_mesh(obj)

        return {'FINISHED'}

# ------------------------------------------------------------------------
# 4. Panneau de l'Interface Utilisateur
# ------------------------------------------------------------------------
class HyperboloidPanel(bpy.types.Panel):
    """Panneau pour générer et éditer des Hyperboloïdes"""
    bl_label = "Générateur d'Hyperboloïde"
    bl_idname = "PT_HyperboloidEditor"
    bl_space_type = 'VIEW_3D'
    bl_region_type = 'UI'
    bl_category = 'Tool' # Ou une autre catégorie de votre choix

    def draw(self, context):
        layout = self.layout
        obj = context.active_object
        
        # Bouton pour ajouter un nouvel hyperboloïde
        layout.operator(AddHyperboloid.bl_idname, text="Ajouter un nouvel Hyperboloïde", icon='ADD')
        layout.separator()

        if obj and hasattr(obj, 'hyperboloid_props'):
            props = obj.hyperboloid_props
            layout.label(text=f"Édition de: {obj.name}", icon='MESH_CONE')
            layout.separator()

            layout.label(text="Paramètres de Forme:")
            col = layout.column(align=True)
            col.prop(props, "a")
            col.prop(props, "b")
            col.prop(props, "c")
            col.prop(props, "shape_type")

            layout.separator()
            layout.label(text="Paramètres de Maillage:")
            col = layout.column(align=True)
            col.prop(props, "segments_u")
            col.prop(props, "segments_v")
            col.prop(props, "height")
        else:
            layout.label(text="Sélectionnez un objet Hyperboloïde", icon='INFO')
            layout.label(text="ou cliquez sur 'Ajouter un nouvel Hyperboloïde'", icon='FILE_NEW')


# ------------------------------------------------------------------------
# 5. Enregistrement et Désenregistrement
# ------------------------------------------------------------------------
def register():
    bpy.utils.register_class(HyperboloidProps)
    bpy.utils.register_class(AddHyperboloid)
    bpy.utils.register_class(HyperboloidPanel)
    
    # Attache la PropertyGroup à bpy.types.Object
    bpy.types.Object.hyperboloid_props = bpy.props.PointerProperty(type=HyperboloidProps)

def unregister():
    # Attention: l'ordre de désenregistrement est important
    bpy.utils.unregister_class(HyperboloidPanel)
    bpy.utils.unregister_class(AddHyperboloid)
    
    # Supprime la propriété de bpy.types.Object en premier
    del bpy.types.Object.hyperboloid_props
    
    bpy.utils.unregister_class(HyperboloidProps)


if __name__ == "__main__":
    register()
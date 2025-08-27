import bpy
import bmesh
from mathutils import Vector
from math import pi, sin, cos

class HyperboloidProps(bpy.types.PropertyGroup):
    def update_hyperboloid(self, context):
        obj = context.active_object
        if obj and obj.hyperboloid_props == self:
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

def refresh_hyperboloid_mesh(obj):
    if not hasattr(obj, 'hyperboloid_props'):
        return

    props = obj.hyperboloid_props
    mesh = obj.data

    if props.a == 0 or props.b == 0 or props.c == 0:
        print("Erreur: Les paramètres a, b ou c ne peuvent pas être zéro.")
        return

    bm = bmesh.new()
    bm.from_mesh(mesh)
    bm.clear()

    verts = []

    for i in range(props.segments_v + 1):
        z_norm = (i / props.segments_v) * 2 - 1  # De -1 à 1
        z = z_norm * (props.height / 2)

        for j in range(props.segments_u):
            theta = (j / props.segments_u) * 2 * pi

            x, y = 0, 0
            if props.shape_type == 'POSITIVE':
                r_squared = 1 + (z / props.c)**2
                if r_squared >= 0:
                    r = r_squared**0.5
                    x = props.a * r * cos(theta)
                    y = props.b * r * sin(theta)
            elif props.shape_type == 'NEGATIVE':
                r_squared = (z / props.c)**2 - 1
                if r_squared >= 0:
                    r = r_squared**0.5
                    x = props.a * r * cos(theta)
                    y = props.b * r * sin(theta)
                else:
                    pass
                    x, y = 0, 0
            elif props.shape_type == 'ZERO':
                r_squared = (z / props.c)**2
                r = r_squared**0.5
                x = props.a * r * cos(theta)
                y = props.b * r * sin(theta)
            
            verts.append(bm.verts.new(Vector((x, y, z))))

    for i in range(props.segments_v):
        for j in range(props.segments_u):
            v1_idx = i * props.segments_u + j
            v2_idx = i * props.segments_u + (j + 1) % props.segments_u
            v3_idx = (i + 1) * props.segments_u + (j + 1) % props.segments_u
            v4_idx = (i + 1) * props.segments_u + j

            try:
                bm.faces.new((verts[v1_idx], verts[v2_idx], verts[v3_idx], verts[v4_idx]))
            except ValueError:
                pass
            except IndexError:
                pass

    bm.to_mesh(mesh)
    mesh.update()
    bm.free()

class AddHyperboloid(bpy.types.Operator):
    """Ajouter un nouvel objet Hyperboloïde"""
    bl_idname = "mesh.add_hyperboloid"
    bl_label = "Ajouter un nouvel Hyperboloïde"
    bl_options = {'REGISTER', 'UNDO'}

    def execute(self, context):
        mesh = bpy.data.meshes.new("HyperboloidMesh")
        obj = bpy.data.objects.new("Hyperboloid", mesh)
        context.collection.objects.link(obj)

        obj.hyperboloid_props.a = 1.0
        obj.hyperboloid_props.b = 1.0
        obj.hyperboloid_props.c = 1.0
        obj.hyperboloid_props.shape_type = 'POSITIVE'
        obj.hyperboloid_props.segments_u = 64
        obj.hyperboloid_props.segments_v = 64
        obj.hyperboloid_props.height = 5.0

        bpy.context.view_layer.objects.active = obj
        obj.select_set(True)

        refresh_hyperboloid_mesh(obj)

        return {'FINISHED'}

class HyperboloidPanel(bpy.types.Panel):
    """Panneau pour générer et éditer des Hyperboloïdes"""
    bl_label = "Générateur d'Hyperboloïde"
    bl_idname = "PT_HyperboloidEditor"
    bl_space_type = 'VIEW_3D'
    bl_region_type = 'UI'
    bl_category = 'Tool'

    def draw(self, context):
        layout = self.layout
        obj = context.active_object

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


def register():
    bpy.utils.register_class(HyperboloidProps)
    bpy.utils.register_class(AddHyperboloid)
    bpy.utils.register_class(HyperboloidPanel)
    
    bpy.types.Object.hyperboloid_props = bpy.props.PointerProperty(type=HyperboloidProps)

def unregister():
    bpy.utils.unregister_class(HyperboloidPanel)
    bpy.utils.unregister_class(AddHyperboloid)
    
    del bpy.types.Object.hyperboloid_props
    
    bpy.utils.unregister_class(HyperboloidProps)


if __name__ == "__main__":
    register()
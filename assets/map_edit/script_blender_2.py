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

import numpy as np
from plyfile import PlyData
import matplotlib.pyplot as plt

def ply_to_yml(ply_path, yml_path):
    # Read PLY file
    plydata = PlyData.read(ply_path)
    vertex = plydata['vertex']
    points = np.vstack([vertex['x'], vertex['y'], vertex['z']]).T  # Shape (N, 3)
    points = points * 1000  # Scale to millimeters

    # Keep only points where y == 0
    # If floating point precision is a concern, use np.isclose instead of ==
    mask = np.isclose(points[:, 1], 0.0, atol=1e-6)
    points = points[mask]

    # Visualize point cloud
    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')
    ax.scatter(points[:, 0], points[:, 1], points[:, 2], s=1, c='b', marker='.')
    ax.set_xlabel("X")
    ax.set_ylabel("Y")
    ax.set_zlabel("Z")
    plt.title("Filtered Point Cloud (y ≈ 0)")
    plt.show()

    # Convert to OpenCV YAML format
    with open(yml_path, 'w') as f:
        f.write("%YAML:1.0\n")
        f.write("---\n")
        f.write("profile: [ ")
        for i, p in enumerate(points):
            f.write("{:.8e}, {:.1f}, {:.8e}".format(p[0], p[1], p[2]))
            if i != len(points) - 1:
                f.write(", ")
                if (i + 1) % 6 == 0:  # Newline after every 6 triplets
                    f.write("\n    ")
        f.write(" ]\n")

if __name__ == "__main__":
    # Fixed paths (modify as needed)
    input_ply = r"./tools/plys/point_cloud_00002.ply"   # Path to the input PLY file
    output_yml = r"./tools/plys/p2.yml"  # Path to the output YAML file

    ply_to_yml(input_ply, output_yml)
    print(f"YAML file saved to {output_yml}")
